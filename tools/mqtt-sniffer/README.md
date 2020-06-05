# MQTT Sniffer

This is a simple tool that logs the messages sent on the _robotrain_ MQTT topic on the command line. It can be useful when debugging communication issus and to keep track of the messages that are sent to and from your train.

## Configure

Duplicate the file `config.template.js` and rename it to `config.js`. Set the IP or hostname of your MQTT broker in `config.js`. If you use a non-standard port, you can change the port here as well.

Usually this needs to be done once. If your MQTT broker changes, you need to adjust those settings before you run the sniffer again.

## Run with _NodeJS_

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

You can find installation instructions for your operating system at [https://docs.docker.com/get-docker](https://docs.docker.com/get-docker).

Before you run the container for the first time, you have to build it:

```sh
$ docker build -t mqtt-sniffer .
```

After that, you can run it with:

```sh
$ docker run --rm mqtt-sniffer "topic/to/sniff"
```

You can use the usual wildcards `+` and `#` for topic names. To learn more about topic names, have a look at [this blog post](https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/).

The `--rm` flag deletes the container after it was stopped. This is handy as each run creates a new randomly named container which otherwise you would have to clean up manually.

# Stop

In both cases you can stop the sniffer with <kbd>Ctrl</kbd> + <kbd>C</kbd>.
