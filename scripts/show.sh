#!/bin/sh

exec mplayer $1 -demuxer rawvideo -rawvideo h=720:w=1280:format=bgra:fps=59.94
