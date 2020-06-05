# MQTT Sniffer

This is a simple tool that logs the messages sent on the _robotrain_ MQTT topic on the command line. It can be useful when debugging communication issus and to keep track of the messages that are sent to and from your train.

## Configure

Duplicate the file `config.template.js` and rename it to `config.js`. Set the IP or hostname of your MQTT broker in `config.js`. If you use a non-standard port, you can change the port here as well.

Usually this needs to be done once. If your MQTT broker changes, you need to adjust those settings before you run the sniffer again.

## Run with _NodeJS_/ _npm_

You need to have _NodeJS_ and _npm_ installed on your computer.

You can find installation instructions for your operating system at [https://nodejs.org/en/download](https://nodejs.org/en/download).

Before you can start using _MQTT Sniffer_, you have to install dependencies via

```sh
$ npm install
```

Then you can run the script with


```sh
$ npm start "topic/to/sniff"
```

## Run with _Docker_

You meed to have _Docker_ installed on your computer.

You can find installation instructions for your operating system at [https://docs.docker.com/get-docker/](https://docs.docker.com/get-docker/).

```sh
$ docker build -t mqtt-sniffer . # before first run
$ docker run --name mqtt-sniffer mqtt-sniffer "topic/to/sniff" # first run
$ docker run mqtt-sniffer "topic/to/sniff" # any other run
```

# Stop

In both cases you can stop the sniffer with <kbd>Ctrl</kbd> + <kbd>C</kbd>.
