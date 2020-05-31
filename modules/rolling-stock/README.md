# Rolling Stock

This module contains the _LDraw_ CAD files for the LEGO model. Although the whole train is one single file, it is modularized into sub-models.

![Robotrain LDraw model](media/robotrain.png)


## How to work with LDraw files

Over the last decade, [_LDraw_](https://www.ldraw.org/) became the standard CAD data format for LEGO models. The community has built up a comprehensive database of all the official LEGO parts and is still very actively the software and this database.

Based on the LDraw data format, there are [several editors available for different operating systems](https://www.ldraw.org/downloads-2/third-party-software.html). The most popular one is [_LeoCAD_](https://www.leocad.org/), which is also part of the official LDraw installer. It is available for Windows, MacOS and Linux and the one I use for this project.

You can also render photo-realistic images from LDraw models. I haven't covered that part yet, but I'm planning to dive into that topic some day.


### Thoughs about the approach

I had to make some concessions regarding the modularization of the whole project and about how the LEGO model should represent the real tech of _Robotrain_.

#### Modularization of the LEGO model

I'm not too happy about having the whole LEGO model in a separate module instead of having the LEGO sub-modules combined with the electronics and software in the respective feature module.

On the other hand, the LEGO model is (except for the train motors) quite independent from the electronics and software.

#### Mock vs. real tech

The mock pantographs are combined with mock trafo units in the respective car to resemble the setup of a real train like the German [ICE 3](https://en.wikipedia.org/wiki/ICE_3). But all of that is just mocked electronics and the real electric power comes from the train motors in the first and last car of the train.

I've been thinking for a long time about putting the mock electronics in the same cars as the real electronics. But the train looks much cooler with the pantographs spread over the whole train, so I decided to keep it as it is.