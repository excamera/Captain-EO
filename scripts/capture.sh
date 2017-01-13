#!/bin/bash

SCRIPT_FILEPATH=$(readlink -e ${BASH_SOURCE[0]})
SCRIPT_DIRPATH=$(dirname $SCRIPT_FILEPATH)
CAPTURE_BINARY=$SCRIPT_DIRPATH/../src/capture/capture

# check params
if [[ -z $1 ]]; then
    echo "Usage: $0 OUTPUT_VIDEO"
    echo
    exit -1
fi

# make sure capture binary exists
if [[ ! -f $CAPTURE_BINARY ]]; then
    echo "Error: 'capture' binary does not exist. Have you run make?"
    echo
    exit -1
fi

# create output file if it does exist already and get full path to it
touch $1
OUTPUT_FILEPATH=$(readlink -e $1)

$CAPTURE_BINARY -d 0 -m 15 -p 3 -v $OUTPUT_FILEPATH
