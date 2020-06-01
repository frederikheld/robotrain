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

## Configure the PiCam

Enable the camera follow [these instructions](https://www.emmaanuel.com/Use-Raspberry-Camera-Module-with). You might need to look into [this tutorial](https://www.picoreplayer.org/how_to_edit_config_txt.shtml) and follow the steps in the section 'Longhand method—using standard Linux commands' to understand how to edit the `config.txt` mentioned in the instructions.

On the RasPi, mount the boot partition:

```sh
$ mount /mnt/mmcblk0p1
```

Add the following lines at the end of `/mnt/mmcblk0p1/config.txt`:

```
gpu_mem=128
start_file=start_x.elf
start_fixup=fixup_x.dat
disable_camera_led=1
```

In the same file, change the following line:

```
dtparam=i2c=on,spi=on,i2s=on #before
dtparam=i2c=on,spi=on,i2s=on,i2s_vc=off #after
```

Install _raspivid_ through `tce-load`:

```sh
$ tce-load -wi rpi-vc.tcz
$ sudo chmod 777 /dev/vchiq
$ filetool.sh -b
```

After that, restart your RasPi with:
```sh
$ sudo reboot
```

#### Troubleshooting

* `/dev/vchiq` doesn't exist because the camera isn't connected properly
    * check if ribbon cable sits correctly in both connectors and isn't bend
    * check if the connector that connects the black camera unit with the board of the camera sits correctly
* `start-stream.sh` quits immediately. Check with `raspistill`. If you get `failed to open vchiq instance`:
    * You need to run raspistill/raspivid as `sudo`!
    * Alternative: add your user to the `video`group: `$ sudo usermod -a -G video $USER`
* `Camera is not enabled in this build. Try running "sudo raspi-config" and ensure that "camera" has been enabled`
    * See [this post](http://forum.tinycorelinux.net/index.php?topic=19743.0) for hints on how to solve the issue


### Streaming options

* [This page](https://wiki.marcluerssen.de/index.php?title=Raspberry_Pi/Camera_streaming) gives an overview over the different ways to stream video from the RasPi.

#### _raspivid_ and _netcat_ (latency-free)

First, on the receiving device, run:

> Requires _NetCat_ and _mplayer_

```sh
$ netcat -l -p 5000 | mplayer -fps 30 -cache 1024 -framedrop -
```

Then, on the RasPi, run:

> Requires _RaspiVid_ and _NetCat_

```sh
$ raspivid -hf -vf -t 0 -w 640 -h 480 -fl -o - | nc 192.168.178.51 5000
```

[Tutorial](https://dantheiotman.com/2017/08/23/using-raspivid-for-low-latency-pi-zero-w-video-streaming/)

##### Pros

* nearly latency-free
* works with simple standard packages in piCore

##### Cons
* Due to the use of netcat, the RasPi is sending the stream to one specific device. So it needs to know the IP of the device that wants to receive the stream. This could be handled via MQTT, but still each recipient would require a distinct stream that consumes ressources on the RasPi (if it is even possible).

## Setup MQTT Client

As there's no ready-to-use MQTT client in the piCore extension repository, I have to figure out a way to do this.

> This is still work in progress!

### Option A: Via self-made extension

There is no MQTT client available as TinyCore Extensions. So we need to build our own.

I'm still figuring this out. See [./tutorials/tiny-core-extensions.md](./tutorials/tiny-core-extensions.md)!

> I didn't continue this approach. See option B instead!


### Option B: via Python script

Install Python3.6 extension via TCE:

```sh
$ tce-load -wi python3.6.tcz
```

Now you can start Python scripts with 

```sh
$ python3.6 myscript.py
```

and install packages with

```sh
$ pip3.6 install my-package
```

Install `Paho MQTT`:

```sh
$ pip3.6 install paho-mqtt
```

> If you get a "permission denied" error, run it again as `sudo -H`.

pip packages will not be persisted in standard configuration of piCore. To perist it, you have to add the package install directory to `/opt/.filetool.lst`, which is a list of all directories backed up when running `filetool.sh -b`.

To find the install path, check the details of the recently installed `paho-mqtt` package:

```sh
$ pip3.6 show paho-mqtt
```

The output will contain a line beginning with `Location:` that most probably will have the value `/usr/local/lib/python3.6/site-packages`.

Add it as a new line at the end of `/opt/.filetool.lst` with

```sh
$ sudo echo 'usr/local/lib/python3.6/site-packages' >> /opt/.filetool.lst
```

Note that the leading slash is being omitted!

Then run

```sh
$ filetool.sh -b
```

## Development

All files that need to be copied to the RasPi are located in `./raspi`.

You can either copy those files to RasPi and the work via SSH (which can be slow), or work locally and sync changes to RasPi. Either way, the simplest and most efficient way is to use `rsync`.

### Sync files to RasPi

`rsync` needs to be installed on both sides. Most Linux distributions already come with it.

On RasPi, you can install the package `rsync.tcz` via

```sh
$ tce-load -wi rsync.tcz
$ filetool.sh -b
```

On your computer, navigate to `./raspi`, then run

```sh
$ rsync --delete -avz ./cockpit-cam tc@box:/home/tc
```

`rsync` will only transfer the files that have changed. It will also delete files in the target that have been deleted in the source.

### Run

On RasPi, start the MQTT client:

```sh
$ python3.6 mqtt-client.py
```

It will now wait for messages on topic _robotrain/cockpit-cam/request-stream_.

On any other device that can talk with the MQTT broker, start _mplayer_ to be ready to receive a stream. There's already a script that issues the neccessary comand:

```sh
$ sh start-stream.sh
```

Then use the [_mqtt-publisher_ tool](../../tools/mqtt-publisher/README.md) that comes with this repo to publish the IP of your device via the above topic:

```sh
$ node mqtt-publisher.js "robotrain/cockpit-cam/request-stream" <ip>
```

After a couple of seconds a window should open that shows the video stream.

You can close that window to end the stream. 

To start it again, run `$ sh start-stream.sh` again and then send the MQTT message again.