# MQTT Logger

Useful for debugging the communication on the _robotrain_ MQTT topic.

## Prerequisites

You need to have _NodeJS_ and _npm_ installed on your computer in order to run the logger.

You can find installation instructions for your operating system here: https://nodejs.org/en/download/.

## Install

Needs to be done once.

```sh
$ npm install
```

## Configure

Set the IP or hostname in line 5 of `mqtt_logger.js`. If you use a non-standard port, you can change the port here as well.

Usually this needs to be done once. If your MQTT broker changes, you need to adjust it before you run the logger again.

## Run

```sh
$ npm start
```

You can stop the logger with <kbd>Ctrl</kbd> + <kbd>C</kbd>.