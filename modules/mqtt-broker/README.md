# MQTT broker

_robotrain_ uses _MQTT_ as the communication protocol between the train and connected devices like the [_remote-control_](../remote-control/README.md). MQTT uses a publish/subscribe model to relay messages between clients. Clients can publish messages on _topics_, other clients can subscribe to those _topics_ to receive the messages. The communication in the MQTT network is handled by the so-called _MQTT broker_, which manages the topics and relays the messages between clients that are publishing/subscribing to/from the same topic.

For a detailled description of this concept, please have a look into this [post at hivemq.com](https://www.hivemq.com/blog/mqtt-essentials-part-3-client-broker-connection-establishment/).

This module come swith a broker you can use out of the box. At the end of this document there's notes on alternatives to this option.

## Preparation

Make sure that the machine your broker will be running on is available in the same network your robotrain and remote controls are connected to.

## Run Docker container

This module provides a Dockerfile that fires up a [HiveMQ Community Edition](https://www.hivemq.com/developers/community/) instance.

Learn [how to install Docker on your system](https://docs.docker.com/get-docker/).

If Docker is installed on your machine, you can build and start the container it with

```sh
$ docker build -t robotrain-mqtt-broker .
$ docker run -d -p 1883:1883 --name robotrain-mqtt-broker robotrain-mqtt-broker
```

The MQTT broker will now be available at port 1883.

## Alternatives

Alternatively you can install the MQTT broker directly on your system, use a broker that's already running on your system or a publicly available one. I won't go in depth on that as every option has their pros and cons. _Robotrain_ is only tested with the option described above.

In any case make sure that you configure [_remote-control_](../remote-control/README.md) and [_motor-control_](../motor-control/README.md) correctly so they can talk to this broker.