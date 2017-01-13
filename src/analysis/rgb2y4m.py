import os
import subprocess
import sys

class RGB2Y4M:

    def __init__(self, videofile, frames, dirname, filename, width=1280, height=720, bytesPerPixel=4):
        """
        frames should be list of (frame index in video, barcoded frameno)
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
            for frameidx, frameno in sorted(self.frames):
                self.video.seek(frameidx * self.framesize)
                frame = self.video.read(self.framesize)
                rawfile.write(frame)

        #This is bad, change to subprocess.check_call
        os.system("avconv -f rawvideo -video_size 1280x720 -framerate 60 -pixel_format bgra -i %s -f yuv4mpegpipe -pix_fmt yuv444p -s 1280x720 -r 60 -y %s" %(rawfilename, self.dirname + "/" + self.filename + ".y4m"))

            
            
            

