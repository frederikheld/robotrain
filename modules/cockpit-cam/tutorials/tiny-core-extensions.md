# TinyCore Extensions

_TinyCore_ is using a package system called _TCE_ (TinyCore Extensions) to install software. It is comparable to _apt_ on Ubuntu. Each TinyCore Extension is packed as a _TCZ_ file.

## Official repository

There's a [repository of officially available TCZ files specifically made for _PiCore_](http://tinycorelinux.net/9.x/armv6/tcz/). PiCore is the Tiny Core version adapted for Raspberry Pi.

## Create your own TCZ package

You can also create your own TCZ files. Unfortunately the documentation expects a high level of understanding how C build systems work and is also partly unavailable.

The best source is [this forum thread](http://forum.tinycorelinux.net/index.php/topic,17585.msg105532.html). The [mentioned wiki page about creating extensions](http://wiki.tinycorelinux.net/wiki:creating_extensions) is not available as the whole wiki has availability issues. The [corebook.pdf](tinycorelinux.net/corebook.pdf) however is available and in chapter 15 it gives an example on how to create a TCZ package.