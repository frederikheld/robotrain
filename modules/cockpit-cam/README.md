# Cockpit Cam

This module sends a live video stream from the cockpit of the _robotrain_ and lets you experience the ride from the driver's point of view.

It is using a _Raspberry Pi Zero W_ with an _RaspiCam_ module. The operating system is _PiCore Linux_ (a port of _TinyCore Linux_ for _Raspberry Pi_) which runs entirely in RAM to avoid failures to wear and tear on the SD card (which the RasPi is prone for).


## Setup RasPi

Follow the [instructions in this repository](https://github.com/frederikheld/treecam) to get your RasPi up and running.

## Prepare _PiCore Linux_

Download the latest stable release for _Raspberry Pi_ [from the _TinyCore_ website](http://tinycorelinux.net/ports.html). This tutorial is based on version 9.0.3.

The ZIP file contains a `README` file which explains the concept of _TinyCore Linux_ in more detail.

### Write the SD card

The ZIP file also contains an `.img` file which you need to write to an Micro SD card using your favourite image writing tool. On _Ubuntu Desktop_ you can use _Startup Disk Creator_ for example.

After you have written the image to the SD card, put it into your RasPi. You also have to connect a monitor and a keyboard to the RasPi for the setup, as WiFi and SSH need to be configured via the command line.

### First startup

The `README` file gives you instructions how to save your auto-generated ssh keys and how to expand the file system to use the whole size of the SD card at first startup via the command line.

Please note that your keyboard layout isn't configured yet. Keys might not be where you expect them!

### Configure WiFi

Turn off the RasPi (just unplug the power source), take the SD card and put it back into your computer.

We need to manually download a couple of TinyCore modules as the package manager that comes with TinyCore can't connect to the internet yet - obviously.

There's a very good [tutorial on how to do this here](https://www.novaspirit.com/2018/01/09/tiny-core-raspberry-pi-zero-w-install/). The tutorial suggests to copy the packages to the SD card before first boot in the RasPi, but as I tried it the partition was too small to hold all modules. Therefore we resize it first, then do this step.

### Configure the PiCam

Enable the camera follow [these instructions](https://www.emmaanuel.com/Use-Raspberry-Camera-Module-with). You might need to look into [this tutorial](https://www.picoreplayer.org/how_to_edit_config_txt.shtml) and follow the steps in the section 'Longhand method—using standard Linux commands' to understand how to edit the `config.txt` mentioned in the instructions.

### Streaming options

* [This page](https://wiki.marcluerssen.de/index.php?title=Raspberry_Pi/Camera_streaming) gives an overview over the different ways to stream video from the RasPi.

#### _raspivid_ and _netcat_ (latency-free)

First, on the receiving device, run:

> Requires _NetCat_ and _mplayer_

```sh
netcat -l -p 5000 | mplayer -fps 30 -cache 1024 -framedrop -
```

Then, on the RasPi, run:

> Requires _RaspiVid_ and _NetCat_

```sh
$ raspivid -hf -vf -t 0 -w 640 -h 480 -fl -o - | nc 192.168.178.51 500
0
```

[Tutorial](https://dantheiotman.com/2017/08/23/using-raspivid-for-low-latency-pi-zero-w-video-streaming/)

##### Pros

* nearly latency-free
* works with simple standard packages in piCore

##### Cons
* Due to the use of netcat, the RasPi is sending the stream to one specific device. So it needs to know the IP of the device that wants to receive the stream. This could be handled via MQTT, but still each recipient would require a distinct stream that consumes ressources on the RasPi (if it is even possible).

## TinyCore Extensions

_TinyCore_ is using a package system called _TCE_ (TinyCore Extensions) to install software. It is comparable to _apt_ on Ubuntu. Each TinyCore Extension is packed as a _TCZ_ file.

There's a [repository of officially available TCZ files specifically made for _PiCore_](http://tinycorelinux.net/9.x/armv6/tcz/) (the Tiny Core version adapted for Raspberry Pi).

### Create own TCZ package

You can also create your own TCZ files. Unfortunately the documentation expects a high level of understanding how C build systems work and is also partly unavailable.

The best source is [this forum thread](http://forum.tinycorelinux.net/index.php/topic,17585.msg105532.html). The [mentioned wiki page about creating extensions](http://wiki.tinycorelinux.net/wiki:creating_extensions) is not available as the whole wiki has availability issues. The [corebook.pdf](tinycorelinux.net/corebook.pdf) however is available and in chapter 15 it gives an example on how to create a TCZ package.