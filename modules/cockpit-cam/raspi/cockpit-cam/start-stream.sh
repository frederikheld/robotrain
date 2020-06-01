#!/bin/sh

# usage:
#
# sh start-stream.sh <client-ip>

echo "Starting stream to client $1"

sudo raspivid -hf -vf -t 0 -w 640 -h 480 -fl -o - | nc "$1" 5000
