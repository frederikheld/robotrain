# MQTT Sniffer

This is a simple tool that logs the messages sent on the _robotrain_ MQTT topic on the command line. It can be useful when debugging communication issus and to keep track of the messages that are sent to and from your train.

## Prerequisites

You need to have _NodeJS_ and _npm_ installed on your computer in order to run the logger.

You can find installation instructions for your operating system at https://nodejs.org/en/download/.

## Install

Before you can start using _MQTT Sniffer_, you have to install dependencies via

```sh
$ npm install
```

## Configure

Duplicate the file `config.template.js` and rename it to `config.js`. Set the IP or hostname of your MQTT broker in `config.js`. If you use a non-standard port, you can change the port here as well.

Usually this needs to be done once. If your MQTT broker changes, you need to adjust those settings before you run the sniffer again.

## Run

```sh
$ npm start
```

You can stop the sniffer with <kbd>Ctrl</kbd> + <kbd>C</kbd>.