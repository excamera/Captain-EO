#!/bin/bash

SCRIPT_FILEPATH=$(readlink -e ${BASH_SOURCE[0]})
SCRIPT_DIRPATH=$(dirname $SCRIPT_FILEPATH)
PLAYBACK_BINARY=$SCRIPT_DIRPATH/../src/playback/playback

# check params
if [[ -z $1 || -z $2 ]]; then
    echo "Usage: $0 INPUT_VIDEO OUTPUT_LOGFILE"
    echo
    exit -1
fi

# make sure playback binary exists
if [[ ! -f $PLAYBACK_BINARY ]]; then
    echo "Error: 'playback' binary does not exist. Have you run make?"
    echo
    exit -1
fi

# create input files if they don't exist already and get full path to them
touch $1
INPUT_VIDEO_FILEPATH=$(readlink -e $1)

touch $2
OUTPUT_LOG_FILEPATH=$(readlink -e $2)

$PLAYBACK_BINARY -d 0 -m 15 -p 3 -v $INPUT_VIDEO_FILEPATH -l $OUTPUT_LOG_FILEPATH
