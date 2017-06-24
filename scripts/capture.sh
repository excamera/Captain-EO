#!/bin/bash -e

SCRIPT_FILEPATH=$(readlink -e ${BASH_SOURCE[0]})
SCRIPT_DIRPATH=$(dirname $SCRIPT_FILEPATH)
CAPTURE_BINARY=$SCRIPT_DIRPATH/../src/capture/capture

# check params
if [[ -z $1 || -z $2 ]]; then
    echo "Usage: $0 OUTPUT_VIDEO OUTPUT_LOGFILE"
    echo
    exit -1
fi

# make sure capture binary exists
if [[ ! -f $CAPTURE_BINARY ]]; then
    echo "Error: 'capture' binary does not exist. Have you run make?"
    echo
    exit -1
fi

# create output files if they don't exist already and get full path to them
touch $1
OUTPUT_VIDEO_FILEPATH=$(readlink -e $1)

touch $2
OUTPUT_LOG_FILEPATH=$(readlink -e $2)

nice -n -15 $CAPTURE_BINARY -d 0 -m 14 -p 3 -v $OUTPUT_VIDEO_FILEPATH -l $OUTPUT_LOG_FILEPATH
