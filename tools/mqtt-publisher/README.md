# mqtt-publisher

Simple script to publish a message to a MQTT topic.

## Usage

The script takes two command line parameters:

| position | parameter | description |
| - | - | - |
| 1 | topic | The MQTT topic to publish the message to |
| 2 | message | The message to publish |

### Use with NodeJS & npm

First you have to install the dependencies:

```sh
$ npm install
```

Then you can run it with _npm_:

```sh
$ npm start <topic> <message>
```