# mqtt-publisher

Simple script to publish a message to a MQTT topic.

## Usage

The script takes two command line parameters:

| position | parameter | description |
| - | - | - |
| 1 | topic | The MQTT topic to publish the message to |
| 2 | message | The message to publish |

### Run with Docker

Initially you have to build the image:

```sh
$ sudo docker build -t mqtt-publisher .
```

After the image was built, you can run the container as often as you need:

```sh
$ sudo docker run --network="host" --rm --name mqtt-publisher mqtt-publisher <topic> <message>
```

### Run with NodeJS & npm

Initially you have to install the dependencies:

```sh
$ npm install
```

After the dependencies were installed, you can run the script _npm_ as often as you want:

```sh
$ npm start <topic> <message>
```