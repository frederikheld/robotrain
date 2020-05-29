# Robotrain

_Robotrain_ is the [LEGO® 4558 Metroliner](https://www.bricklink.com/v2/catalog/catalogitem.page?S=4558-1#T=S&O={%22iconly%22:0}) model on steroids. I modified the original LEGO model and augmented it with Arduino, RaspberryPi and several electronic components to make it remote controllable.

![Robotrain in action](media/robotrain-in-action.gif)

The main features are:

* [Jacobs bogies](https://en.wikipedia.org/wiki/Jacobs_bogie) instead of standard bogies
* Train running on 12 V permanently powered tracks
* Train carries an [Uninterruptible Power Supply](https://en.wikipedia.org/wiki/Uninterruptible_power_supply) that allows it to run on non-powered tracks
* [NodeMCU Mini](https://en.wikipedia.org/wiki/NodeMCU) with motor driver as [motor control](modules/drivetrain)
* NodeMCU Mini as [remote control](modules/remote-control)
* [Raspberry Pi Zero W](https://en.wikipedia.org/wiki/Raspberry_Pi) with camera module as [cockpit cam](modules/cockpit-cam)
* [MQTT](https://en.wikipedia.org/wiki/MQTT) as [common communication interface](common-infrastructure/mqtt-broker)

Planned features are:
* Mobile app that resembles the cockpit of a real train with a view from the drivers seat

> This is work in progress. Please see the the sub-directories of [modules](./modules) for more information!

## Join Development

Feel free to fork this repository and use it for your own projects!

### Electronics and Software

The electronics and software work for all 9 V track-powered train motors and should also work for the battery powered version.

> You need to modify the motors to lead the current out of the chassis through one pair of cables and take the motor power in through another pair of cables. I will provide a tutorial on that later!

### Working with the Repository

This repository uses the Git LFS extension for binary files, mainly because _Fritzing_ diagrams are compressed bundles.

Please [activate Git LFS](https://git-lfs.github.com/) before you commit changes to any of the file types listed in [.gitattributes](.gitattrbiutes)!

### Contribution

If you want to contribute to the main project, please have a look into the [Issues](https://github.com/frederikheld/robo-train/issues) and see if you can submit a pull request that implements one of it. I'm happy to discuss details with you before you start your work.

You can also submit pull requests for fixes and features that are not alrady in the issues. If I like it, I will merge it ;-)