#!/usr/bin/env python

import datetime
import os
import re
import shutil
import subprocess
import sys
import pickle
import multiprocessing as mp

WIDTH = 1280
HEIGHT = 720

PLAYBACK_VIDEO = None
PLAYBACK_LOG = None
CAPTURE_VIDEO = None
CAPTURE_LOG = None
RESULTS_LOG = None

class PlaybackLogEntry:

    def __init__(self, frame_index, 
                 upper_left_barcode, 
                 lower_right_barcode,
                 cpu_time_scheduled, 
                 cpu_time_completed,
                 decklink_hardwaretime_scheduled, 
                 decklink_hardwaretime_completed_callback,
                 decklink_frame_completed_reference_time):
        
        print frame_index
        assert(upper_left_barcode == lower_right_barcode) # playback frames are not allowed to be corrupted
        assert(cpu_time_scheduled < cpu_time_completed)
        assert(decklink_hardwaretime_scheduled < decklink_hardwaretime_completed_callback)

        self.frame_index = frame_index

        self.upper_left_barcode = upper_left_barcode
        self.lower_right_barcode = lower_right_barcode

        self.cpu_time_scheduled = cpu_time_scheduled
        self.cpu_time_completed = cpu_time_completed

        self.decklink_hardwaretime_scheduled = decklink_hardwaretime_scheduled
        self.decklink_hardwaretime_completed_callback = decklink_hardwaretime_completed_callback

        self.decklink_frame_completed_reference_time = decklink_frame_completed_reference_time
        

class CaptureLogEntry:
    
    def __init__(self, frame_index, 
                 upper_left_barcode, 
                 lower_right_barcode,
                 cpu_timestamp, 
                 decklink_hardwaretimestamp,
                 decklink_frame_hardware_reference_time,
                 decklink_frame_hardware_reference_duration):
        
        print frame_index
        assert (upper_left_barcode == lower_right_barcode) # corrupted capture barcode

        self.frame_index = frame_index
        
        self.upper_left_barcode = upper_left_barcode
        self.lower_right_barcode = lower_right_barcode

        self.cpu_timestamp = cpu_timestamp
        self.decklink_hardwaretimestamp = decklink_hardwaretimestamp

        self.decklink_frame_hardware_reference_time = decklink_frame_hardware_reference_time
        self.decklink_frame_hardware_reference_duration = decklink_frame_hardware_reference_duration

class RGB2Y4M:

    def __init__(self, videofile, frames, dirname, filename, width=1280, height=720, bytesPerPixel=4):
        """
        frames should be list of frame index in video
        """
        self.video = videofile
        self.filename = filename
        self.frames = frames
        self.dirname = dirname
        self.width = width
        self.height = height
        self.framesize = width * height * bytesPerPixel

    def convert(self):
        if not os.path.exists(self.dirname):
            os.mkdir(self.dirname)

        rawfilename = self.dirname + "/" + self.filename + ".raw"
        print "Appending frames to %s" %rawfilename
        with open(rawfilename, "wb") as rawfile:
            for frameidx in sorted(self.frames):
                self.video.seek(frameidx * self.framesize)
                frame = self.video.read(self.framesize)
                rawfile.write(frame)
        #This is bad, change to subprocess.check_call
        os.system("avconv -f rawvideo -video_size 1280x720 -framerate 60 -pixel_format bgra -i %s -f yuv4mpegpipe -pix_fmt yuv444p -s 1280x720 -r 60 -y %s" %(rawfilename, self.dirname + "/" + self.filename + ".y4m"))

def get_lines_from_log_file(log_filename):
    lines = open(log_filename, 'r').read().split('\n')
        
    # remove comment lines
    lines = filter(lambda line: re.match('^\s*#.*', line) is None, lines)

    # remove blank lines
    lines = filter(lambda line: re.match('^\s*$', line) is None, lines)
    
    return lines

################################################################################
# start the script below
################################################################################
if ( len(sys.argv) != 6 ):
    print ( 'usage: python %s PLAYBACK.raw PLAYBACK.log CAPTURE.raw CAPTURE.log RESULTS.csv\n' % sys.argv[0] )
    sys.exit(-1)

PLAYBACK_VIDEO = sys.argv[1]
PLAYBACK_LOG = sys.argv[2]
CAPTURE_VIDEO = sys.argv[3]
CAPTURE_LOG = sys.argv[4]
RESULTS_LOG = sys.argv[5]

################################################################################
# parse the log files
################################################################################

# parse playback log
print "parsing playback log"
playback_log = []
for line in get_lines_from_log_file ( PLAYBACK_LOG ):
    p_entry = PlaybackLogEntry ( *[int(item) for item in line.split(',')] )
    playback_log.append ( p_entry )

# parse capture log
print "parsing capture log"
capture_corrupted_barcodes = 0
capture_log = []
for line in get_lines_from_log_file ( CAPTURE_LOG ):

    # skip frames where  the barcodes did not match
    line_eles = line.split(',')
    if ( line_eles[1] != line_eles[2] ):
        print "barcode corrupted"
        capture_corrupted_barcodes += 1
        continue
        
    c_entry = CaptureLogEntry( *[int(item) for item in line_eles] )
    capture_log.append(c_entry)

print "There were", capture_corrupted_barcodes, "barcodes from the captured video that could not be recovered"
    
################################################################################
# create a correspondence between the sent and recieved frames
# NOTE: this assumes code assumes that all frames have a unique barcode. 
################################################################################
print "producing correspondence between playback and capture frames"
if ( not os.path.exists(os.getcwd() + '/frame_correspondence.pickle') ):
    playback_capture_frame_correspondence = []
    capture_index = -1
    for playback_frame in playback_log:

        # try to find a corresponding frame in the capture log
        ul = playback_frame.upper_left_barcode
        matches = filter ( lambda x: x.upper_left_barcode == ul, capture_log )

        if( len(matches) == 0 ):
            playback_capture_frame_correspondence.append( (playback_frame, None) ) # no frame with matching barcode found

        else:
            idx = 0 
            capture_frame = matches[0]
            while ( capture_index > capture_frame.frame_index and idx < len(matches) ):
                print len(matches)
                print capture_index
                print capture_frame.frame_index

                capture_frame = matches[idx]
                idx += 1

            if ( capture_index > capture_frame.frame_index ):
                print "Warning: capture frames came out of order. Skipping."
                playback_capture_frame_correspondence.append( (playback_frame, None) ) # skip frame if you would go backwards
                continue

            capture_index = capture_frame.frame_index
            playback_capture_frame_correspondence.append( (playback_frame, capture_frame) )

    pickle.dump(playback_capture_frame_correspondence, open(os.getcwd() + '/frame_correspondence.pickle', 'wb'))

playback_capture_frame_correspondence = pickle.load( open( os.getcwd() + '/frame_correspondence.pickle', "rb" ) )
assert( len(playback_log) == len(playback_capture_frame_correspondence) )
print 'frames received:', len ( filter(lambda x: x[1] is not None, playback_capture_frame_correspondence) )
print 'frames dropped:', len ( filter(lambda x: x[1] is None, playback_capture_frame_correspondence) )

################################################################################
# get ssims for the received frames
################################################################################

# convert video to a form where the ssim can be computed
print 'converting playback frames'
def playback_frames():
    if ( not os.path.exists(os.getcwd() + '/playback-frames/playback.y4m') ):
        playback_frames = [frame[0].frame_index for frame in filter(lambda x: x[1] is not None, playback_capture_frame_correspondence)]
        with open(PLAYBACK_VIDEO, 'r') as out_video:
            playbackConverter = RGB2Y4M(out_video, playback_frames, os.getcwd() + '/' + 'playback-frames', 'playback', WIDTH, HEIGHT)
            playbackConverter.convert()

print 'converting capture frames'
def capture_frames():
    if ( not os.path.exists(os.getcwd() + '/capture-frames/capture.y4m') ):
        capture_frames = [frame[1].frame_index for frame in filter(lambda x: x[1] is not None, playback_capture_frame_correspondence)]
        with open(CAPTURE_VIDEO, 'r') as in_video:
            captureConverter = RGB2Y4M(in_video, capture_frames, os.getcwd() + '/' + 'capture-frames', 'capture', WIDTH, HEIGHT)
            captureConverter.convert()

# run the functions
p1 = mp.Process(target=playback_frames)
p2 = mp.Process(target=capture_frames)
            
p1.start()
p2.start()

p1.join()
p2.join()

print 'performing ssim computations'
if ( not os.path.exists(os.getcwd() + '/.ssim.log') ):
    os.system('/home/captaineo/captain-eo/third_party/daala_tools/daala/dump_ssim -p 8 %s/playback-frames/playback.y4m %s/capture-frames/capture.y4m | tee .ssim.log' % (os.getcwd(), os.getcwd()))

ssim_lines = map( lambda x: x.split() , open(os.getcwd() + '/.ssim.log', 'r').read().strip().split('\n')[:-1] )

ssim_results = []
for line in ssim_lines:
    total, Y, Cb, Cr = line[1], line[3], line[5], line[7]
    ssim_results.append( (total, Y, Cb, Cr) )
                         
################################################################################
# print the output log file
################################################################################
with open(RESULTS_LOG, 'w') as logfile:
    print len( filter(lambda x: x[1] is not None, playback_capture_frame_correspondence) )
    print len(ssim_results)
    assert( len( filter(lambda x: x[1] is not None, playback_capture_frame_correspondence) ) == len(ssim_results) )

    # print csv header
    logfile.write('# playback: ' + PLAYBACK_VIDEO + ' capture: ' + CAPTURE_VIDEO + '\n')
    logfile.write('# Timestamp: ' + str(datetime.datetime.now()) + '\n')
    logfile.write('# frame_index,sent_cpu_timestamp,received_cpu_timestamp,delay_cpu_time,sent_decklink_timestamp,received_decklink_time,delay_decklink_time,ssim_total,ssim_Y,ssim_Cb,ssim_Cr\n')

    for frame, ssim in zip(filter(lambda x: x[1] is not None, playback_capture_frame_correspondence), ssim_results ):
        playback_frame = frame[0]
        capture_frame = frame[1]
        
        logfile.write(str(playback_frame.frame_index) + ',' + 
                      str(playback_frame.cpu_time_completed) + ',' +
                      str(capture_frame.cpu_timestamp) + ',' + 
                      str(capture_frame.cpu_timestamp - playback_frame.cpu_time_completed) + ',' +
                      str(playback_frame.decklink_frame_completed_reference_time) + ',' +
                      str(capture_frame.decklink_frame_hardware_reference_time) + ',' + 
                      str(capture_frame.decklink_frame_hardware_reference_time - playback_frame.decklink_frame_completed_reference_time) + ',' +
                      str(ssim[0]) + ',' + 
                      str(ssim[1]) + ',' + 
                      str(ssim[2]) + ',' + 
                      str(ssim[3])[:-1] + '\n')

# shutil.rmtree('.ssim.log')
# shutil.rmtree(os.getcwd() + '/' + 'capture-frames')
# shutil.rmtree(os.getcwd() + '/' + 'playback-frames')
