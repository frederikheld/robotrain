# Modules and Common Infrastructure

_Robotrain_ features are split into modules that each can be used independently. All put together they form the full-fledged _Robotrain_, but removing one won't render the others useles.

The common communication infrastructure between those modules is _MQTT_, which orders communication in cascaded topics.

## Topic model

The root topic is `robotrain`.

Modules can publish to or receive information via topics under their module root topic `robotrain/module-name`.

It's up to the module to structure data below it's root topic.

For receiving data, it depends on the use case if a module is subscribing a topic under it's own name to which others can publish information or if it subscribes to another module's topic.
