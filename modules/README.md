# Modules and Common Infrastructure

_Robotrain_ features are split into modules that each can be used independently. All put together form the full-fledged _Robotrain_, but removing one won't render the others useles. So it's kind of an microservices architecture.

The common communication protocol between those modules is _MQTT_. Additional peer-to-peer connections on different protocols are possible, but the have to be announced and negotiated via _MQTT_.

> You need to set up a _MQTT_ broker that serves as the central communication hub. _robotrain_ comes with an out-of-the-box broker tha you'll find in [common-infrastructure/mqtt-broker](../common-infrastructure/mqtt-broker).
>
>You also need to configure each module to use the broker. You'll find the respective config keys in each module's `config.template.*` file.

## Topic model

_MQTT_ organizes communication in cascaded topics. 

> [Learn more about Topics](https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/)

The root topic is `robotrain`.

Modules can publish to or receive information from topics under their module root topic `robotrain/module-name`.

It's up to the module to structure data below it's root topic.

For receiving data, it depends on the use case if a module is subscribing a topic under it's own name to which others can publish information or if it subscribes to another module's topic.
