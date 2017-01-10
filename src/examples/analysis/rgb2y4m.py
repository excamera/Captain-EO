import os
import subprocess
import sys

class RGB2Y4M:

    def __init__(self, videofile, frames, dirname, width=1280, height=720, bytesPerPixel=4):
        """
        frames should be list of (frame index in video, barcoded frameno)
        """
        self.video = videofile
        self.frames = frames
        self.dirname = dirname
        self.width = width
        self.height = height
        self.framesize = width * height * bytesPerPixel

    def convert(self):
        if not os.path.exists(self.dirname):
            os.mkdir(self.dirname)
        
        for frameidx, frameno in sorted(self.frames):
            self.video.seek(frameidx * self.framesize)
            outfilename = self.dirname + "/" + str(frameno) + ".raw"
            with open(outfilename, "wb") as outfile:
                outfile.write(self.video.read(self.framesize))
            #This is bad, change to subprocess.check_call
            os.system("ffmpeg -f rawvideo -video_size 1280x720 -framerate 60 -pixel_format bgra -i %s -f yuv4mpegpipe -pix_fmt yuv444p -s 1280x720 -r 60 %s" %(outfilename, self.dirname + "/" + str(frameno) + ".y4m"))

            
            
            

