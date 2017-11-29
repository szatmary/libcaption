#!/usr/bin/env bash

KEY=$1

ffmpeg -re -f lavfi -i "testsrc=size=1280x720:rate=45"  -f lavfi -i "sine=frequency=1000" \
-c:v libx264 -pix_fmt yuv420p -b:v 2500k -preset ultrafast -g 90 \
-strict -2 -c:a aac -b:a 96k -ac 2 \
-f flv - | ./party - - | ./rtmpspit - rtmp://live.twitch.tv/app/${KEY}
