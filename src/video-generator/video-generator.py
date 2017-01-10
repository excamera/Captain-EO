import argparse
import sys

class VideoGenerator:
    def __init__(self, frame, numFrames, height=720, width=1280, filename="video.raw"):
        self.frame = frame
        self.height = height
        self.width = width
        self.nFrames = numFrames
        self.filename = filename

    def generate(self):
        with open(self.filename, 'wb') as f:
            for i in xrange(self.nFrames):
                f.write(self.frame)

def generateColorBars(height=720, width=1280):
    colors = [bytearray([0x00, 0x7F, 0x7F, 0xFF]), bytearray([0x00, 0xFF, 0xFF, 0x00]), 
              bytearray([0x00, 0x00, 0xFF, 0xFF]), bytearray([0x00, 0x00, 0xFF, 0x00]), 
              bytearray([0x00, 0xFF, 0x00, 0xFF]), bytearray([0x00, 0x00, 0x00, 0xFF]), 
              bytearray([0x00, 0xFF, 0x00, 0x00]), bytearray([0x00, 0x80, 0x80, 0x80])]
    frame = bytearray()
    for y in range(height):
        for x in range(width):
            frame += bytearray(reversed(colors[(x * 8) / width]))
    return frame

if __name__ == "__main__":
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("--help",  help="Print help", action="help")
    parser.add_argument("-n", "--num-frames", type=int, help="Number of frames to generate",
                            required=True, action="store")
    parser.add_argument("-h", "--height", type=int, default=720, help="Height of frames",
                            action="store")
    parser.add_argument("-w", "--width", type=int, default=1280, help="Width of frames",
                            action="store")
    args = parser.parse_args()
    generator = VideoGenerator(generateColorBars(), args.num_frames, args.height, args.width)
    generator.generate()
