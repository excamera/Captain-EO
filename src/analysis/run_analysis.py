import argparse
import os
import shutil
import subprocess
from rgb2y4m import RGB2Y4M


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

    def ingestLog(self):
        for line in self.log.readlines():
            idx, ul, lr, time = [int(item) for item in line.split()]
            if ul != lr:
                raise VideoBarcodeException
            if idx != ul:
                raise VideoFrameException
            self.records[ul] = time

class CaptureLogReader(LogReader):
    
    def __init__(self, logfile, height, width):
        super(CaptureLogReader, self).__init__(logfile, height, width)
        self.ulTimes = {}
        self.lrTimes = {}

    def ingestLog(self):
        for line in self.log.readlines():
            idx, ul, lr, time = [int(item) for item in line.split()]
            if ul == lr:
                if self.records.get(ul) is None:
                    self.records[ul] = time, idx
            else:
                if self.ulTimes.get(ul) is None:
                    self.ulTimes[ul] = time, idx
                if self.lrTimes.get(lr) is None:
                    self.lrTimes[lr] = time, idx


def main(args):
    playbackLogs = PlaybackLogReader(args.playback_log, args.height, args.width)
    playbackLogs.ingestLog()
    
    captureLogs = CaptureLogReader(args.capture_log, args.height, args.width)
    captureLogs.ingestLog()
    
    captureFrames = []
    playbackFrames = []
    results = []
    for frameno in sorted(playbackLogs.records.keys()):
        if captureLogs.records.get(frameno) is not None:
            results.append((frameno, playbackLogs.records[frameno],
                           captureLogs.records[frameno][0],
                           "FULL", captureLogs.records[frameno][1]))
            captureFrames.append((captureLogs.records[frameno][1], frameno))
            playbackFrames.append((frameno, frameno))
        elif captureLogs.ulTimes.get(frameno) is not None:
            results.append((frameno, playbackLogs.records[frameno],
                           captureLogs.ulTimes[frameno][0],
                           "UL", captureLogs.ulTimes[frameno][0]))
        elif captureLogs.lrTimes.get(frameno) is not None:
            results.append((frameno, playbackLogs.records[frameno],
                           captureLogs.lrTimes[frameno][0],
                           "RL", captureLogs.lrTimes[frameno][0]))
        else:
            results.append((frameno, playbackLogs.records[frameno], None, "DROPPED", None))

    captureConverter = RGB2Y4M(args.in_video, captureFrames, os.getcwd() + "/" + "capture-frames", args.width, args.height)
    captureConverter.convert()

    playbackConverter = RGB2Y4M(args.out_video, playbackFrames, os.getcwd() + "/" + "playback-frames", args.width, args.height)
    playbackConverter.convert()
    
 
    with open(os.devnull, 'w') as devnull:
        for frameidx, frameno in playbackFrames:
            ssim_output = subprocess.check_output(["%s/../../../third_party/daala_tools/dump_ssim" %os.getcwd(), 
                                                   "-r", "%s/playback-frames/%d.y4m" %(os.getcwd(), frameno),
                                                   "%s/capture-frames/%d.y4m" %(os.getcwd(), frameno)], stderr=devnull)
            args.output.write("Frame %d SSIM output: [%s]\n" %(frameno, ssim_output.split("\n")[1]))
    
    shutil.rmtree(os.getcwd() + "/" + "capture-frames")
    shutil.rmtree(os.getcwd() + "/" + "playback-frames")
    for frameLog in results:
        args.output.write("Frame %d (%d): " %(frameLog[0], frameLog[1]))
        args.output.write("%s%s\n" %(frameLog[3], 
                                     " %d (%d), %fms" %(frameLog[4], frameLog[2], (frameLog[2] - frameLog[1])/1000.0)
                                        if frameLog[3] != "DROPPED" else ""))
    

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
    parser.add_argument("-r", "--output", type=argparse.FileType('w'),
                        default="-", help="Output file path") 
    parser.add_argument("-h", "--height", type=int, default=720, help="Height of frames")
    parser.add_argument("-w", "--width", type=int, default=1280, help="Width of frames")
    
    args = parser.parse_args()
    print args
    main(args)