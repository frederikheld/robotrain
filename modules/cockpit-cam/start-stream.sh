#!/bin/sh

# 1. start mplayer to receive stream from RasPi:
netcat -l -p 5000 | mplayer -fps 30 -cache 1024 -framedrop -

# 2. request stream from RasPi via MQTT
# todo: needs MQTT client on RasPi (PiCore)

# 3. receive confirmation from RasPi via MQTT
# todo