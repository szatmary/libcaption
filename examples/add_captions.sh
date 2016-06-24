#!/usr/bin/env bash

if [  $# -lt 2 ]
then
    echo "Need at least 2 arguments."
    echo "$0 InputVideo InputSRT [OutputFilename]"
    exit 1
fi

VIDEO=$1
SRT=$2

if [ -z "$3" ]
then
    OUTFILE="out.mp4"
else
    OUTFILE="$3"
fi

echo "Video=$VIDEO"
echo "Captions=$SRT"
echo "Outfile=$OUTFILE"

ffmpeg -i $VIDEO -acodec copy -vcodec copy -f flv - | ./flv+srt - $SRT - | ffmpeg -i - -acodec copy -vcodec copy $OUTFILE
