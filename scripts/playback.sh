#!/bin/bash -e

readonly NUM_BLACK_FRAME=3600 # 1 minute of black frames at 60Hz
#readonly NUM_BLACK_FRAME=0

SCRIPT_FILEPATH=$(readlink -e ${BASH_SOURCE[0]})
SCRIPT_DIRPATH=$(dirname $SCRIPT_FILEPATH)
PLAYBACK_BINARY=$SCRIPT_DIRPATH/../src/playback/playback

# check params
if [[ -z $1 || -z $2 || -z $3 || -z $4 ]]; then
    echo "Usage: $0 INPUT_VIDEO OUTPUT_LOGFILE NUM_BLACK_FRAMES UPLINK_TRACE DOWNLINK_TRACE"
    echo
    exit -1
fi

# make sure playback binary exists
if [[ ! -f $PLAYBACK_BINARY ]]; then
    echo "Error: 'playback' binary does not exist. Have you run make?"
    echo
    exit -1
fi

# check to make sure input file exists
if [[ ! -f $1 ]]; then
    echo "Error: $1 does not exist."
    echo 
    exit -1
fi
INPUT_VIDEO_FILEPATH=$(readlink -e $1)

# create output file if ot doesn't exist already and get full path to it
touch $2
OUTPUT_LOG_FILEPATH=$(readlink -e $2)

nice -n -15 $PLAYBACK_BINARY -d 0 -m 14 -p 3 -v $INPUT_VIDEO_FILEPATH -l $OUTPUT_LOG_FILEPATH \
       -u $3 \
       -n $4 \
       -k uplink.log \
       -j downlink.log \
       -b $NUM_BLACK_FRAME
