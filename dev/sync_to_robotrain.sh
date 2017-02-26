#!/bin/sh

# Run this script on the dev machine from the root of "robo-train" repository
# as follows to sync this package to the robotrain:
$ ssh dev/sync_to_robotrain.sh

rsync -avze ssh --exclude=.git --exclude=node_modules . pi@robotrain:/home/pi/robotrain

echo "If you have installed new node packages (aka added them to package.json) don't forget to run 'npm install' in the target directory now!"
