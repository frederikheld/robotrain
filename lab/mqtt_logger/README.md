# MQTT Logger

Useful for debugging the communication on the robotrain MQTT topic.

## Configure

Set the IP or hostname in line 5 of `mqtt_logger.js`. If you use a non-standard port, you can change the port here as well.

After the configuration is done, you have two options to run the logger.

## Run with NodeJS

If you have _NodeJS_ and _npm_ installed on your computer, you can simply run the app with _NodeJS_:

```sh
$ npm install
$ npm start
```

## Run with Docker

If you don't want to install the _NodeJS_ environment, but _Docker_ is installed on your computer, you can build and run the _Docker_ container:

```sh
$ docker build -t robotrain/mqtt_logger .
$ docker run --name robotrain_mqtt_logger robotrain/mqtt_logger
```

## Stop

In both cases you can stop the logger with <kbd>Ctrl</kbd> + <kbd>C</kbd>.