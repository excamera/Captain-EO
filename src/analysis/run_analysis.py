import argparse
import os
import shutil
import subprocess
import re
from rgb2y4m import RGB2Y4M

def parse_lines_from_log_file(log_file):
    '''
    parse_lines_from_log_file: helper function to get rid of the extraneous
    information in the log files produced by the barcode binaries
    '''

    lines = log_file.read().split('\n')
        
    # remove comment lines
    lines = filter(lambda line: re.match('^\s*#.*', line) is None, lines)

    # remove blank lines
    lines = filter(lambda line: re.match('^\s*$', line) is None, lines)
    
    return lines

class VideoException(Exception):
    pass

class VideoBarcodeException(VideoException):
    pass

class VideoFrameException(VideoException):
    pass

class LogReader(object):
    
    def __init__(self, logfile, height, width):
        self.log = logfile
        self.h = height
        self.w = width
        self.records = {}

class PlaybackLogReader(LogReader):

    def ingestLog(self, barcodeLog):
        logFileLines = parse_lines_from_log_file(self.log)
        logFileLines = logFileLines[1:] # remove csv headers

        for line in logFileLines:
            idx, ul, lr, time = [int(item) for item in line.split(',')]
            if ul != lr:
                raise VideoBarcodeException
            if idx != barcodeLog[ul]:
                raise VideoFrameException
            self.records[ul] = time, idx

class CaptureLogReader(LogReader):
    
    def __init__(self, logfile, height, width):
        super(CaptureLogReader, self).__init__(logfile, height, width)
        self.ulTimes = {}
        self.lrTimes = {}

    def ingestLog(self, barcodeLog):
        logFileLines = parse_lines_from_log_file(self.log)
        logFileLines = logFileLines[1:] # remove csv headers

        for line in logFileLines:
            idx, ul, lr, time = [int(item) for item in line.split(',')]
            if ul == lr:
                if self.records.get(ul) is None:
                    self.records[ul] = time, idx
            else:
                if self.ulTimes.get(ul) is None:
                    self.ulTimes[ul] = time, idx
                if self.lrTimes.get(lr) is None:
                    self.lrTimes[lr] = time, idx

def ingestBarcodeLog(log):
    barcodeLog = {}

    logFileLines = parse_lines_from_log_file(log)
    logFileLines = logFileLines[1:] # remove csv headers
    for line in logFileLines:
        idx, barcode = [int(item) for item in line.split(',')]
        barcodeLog[barcode] = idx
    return barcodeLog

def main(args):
    barcodeLog = ingestBarcodeLog(args.barcode_log)

    playbackLogs = PlaybackLogReader(args.playback_log, args.height, args.width)
    playbackLogs.ingestLog(barcodeLog)
    
    captureLogs = CaptureLogReader(args.capture_log, args.height, args.width)
    captureLogs.ingestLog(barcodeLog)
    
    captureFrames = []
    playbackFrames = []
    results = []
    for frameno in sorted(playbackLogs.records.keys(), cmp=lambda x, y: cmp(barcodeLog[x], barcodeLog[y])):
        if captureLogs.records.get(frameno) is not None:
            results.append((
                barcodeLog[frameno],              # Frame index in video passed to playback
                playbackLogs.records[frameno][0], # CPU time (us) frame was played
                captureLogs.records[frameno][0],  # CPU time (us) frame was captured
                "FULL",                           # Frame captured with UL == RL
                frameno                           # Barcode number
            ))

            captureFrames.append((captureLogs.records[frameno][1], frameno))
            playbackFrames.append((playbackLogs.records[frameno][1], frameno))

        elif captureLogs.ulTimes.get(frameno) is not None:
            results.append((
                barcodeLog[frameno],              # Frame index in video passed to playback
                playbackLogs.records[frameno][0], # CPU time (us) frame was played
                captureLogs.ulTimes[frameno][0],  # CPU time (us) frame was captured
                "UL",                             # Frame barcode only appears in UL
                frameno                           # Barcode number
            ))
        elif captureLogs.lrTimes.get(frameno) is not None:
            results.append((
                barcodeLog[frameno],              # Frame index in video passed to playback
                playbackLogs.records[frameno][0], # CPU time (us) frame was played
                captureLogs.lrTimes[frameno][0],  # CPU time (us) frame was captured
                "LR",                             # Frame barcode only appears in LR
                frameno                           # Barcode number
            ))
        else:
            results.append((
                barcodeLog[frameno],              # Frame index in video passed to playback
                playbackLogs.records[frameno][0], # CPU time (us) frame was played
                None, 
                "DROPPED",                        # Frame was not captured
                None
            ))

    for frameLog in results:
        args.output.write("Frame %d (%d): " %(frameLog[0], frameLog[1]))
        args.output.write("%s%s\n" %(frameLog[3], 
                                     " %d (%d), %fms" %(frameLog[4], frameLog[2], (frameLog[2] - frameLog[1])/1000.0)
                                        if frameLog[3] != "DROPPED" else ""))

    captureConverter = RGB2Y4M(args.in_video, captureFrames, os.getcwd() + "/" + "capture-frames", "capture", args.width, args.height)
    captureConverter.convert()

    playbackConverter = RGB2Y4M(args.out_video, playbackFrames, os.getcwd() + "/" + "playback-frames", "playback", args.width, args.height)
    playbackConverter.convert()
 
    with open(os.devnull, 'w') as devnull:
        #for frameidx, frameno in playbackFrames:
        subprocess.check_call(["%s/../../third_party/daala_tools/daala/dump_ssim" %os.getcwd(), 
                                                   "-r", "%s/playback-frames/playback.y4m" %(os.getcwd()),
                                                   "%s/capture-frames/capture.y4m" %(os.getcwd())])
    
    shutil.rmtree(os.getcwd() + "/" + "capture-frames")
    shutil.rmtree(os.getcwd() + "/" + "playback-frames")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("--help", help="Print help", action="help")
    parser.add_argument("-o", "--out-video", type=argparse.FileType('rb'), 
                        help="Path to raw video played on output")
    parser.add_argument("-i", "--in-video", type=argparse.FileType('rb'),
                        help="Path to captured video")
    parser.add_argument("-p", "--playback-log", type=argparse.FileType('r'),
                        help="Path to playback log")
    parser.add_argument("-c", "--capture-log", type=argparse.FileType('r'),
                        help="Path to capture log")
    parser.add_argument("-b", "--barcode-log", type=argparse.FileType('r'),
                        help="Path to barcoder log")
    parser.add_argument("-r", "--output", type=argparse.FileType('w'),
                        default="-", help="Output file path") 
    parser.add_argument("-h", "--height", type=int, default=720, help="Height of frames")
    parser.add_argument("-w", "--width", type=int, default=1280, help="Width of frames")
    
    args = parser.parse_args()
    print args
    main(args)
