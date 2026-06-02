This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/Looper.svg?color=brightgreen)](https://github.com/GyverLibs/Looper/releases/latest/download/Looper.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/Looper.svg)](https://registry.platformio.org/libraries/gyverlibs/Looper)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Looper?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

![logo](images/logo.png)

# Looper
Lightweight pseudo-multi-threaded event-oriented framework for Arduino
- More convenient organization of the program
- Separation into virtual flows
- Full streams with parallel execution and asynchronous waiting based on[Duff cars](https://en.wikipedia.org/wiki/Duff%27s_device)
- Static and dynamic task creation
- Challenges with a given period
- Sending events and data between tasks
- Total execution in one thread without the need for mutex
- Convenient OOP API for creating tasks and integration with other classes
- Easily untied from the Arduino kernel (replace two functions)
- Without dependence on other libraries
- Lightweight: the core takes up 450B flash drives and 5B RAM (AVR, ID and events disabled)

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Documentation.
Located in[file](https://github.com/GyverLibs/Looper/tree/main/docs/0.main.md)

## Contents
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## What a Looper is for
The classic Arduino project, not based on operating systems such as FreeRTOS, has a single-threaded supercycle architecture (*pool based*): there is a basic cycle.`loop`, which runs in a circle throughout the program and in this cycle we poll buttons/sensors/communication/other. Such polling functions in the terminology of this library are called tickers. For initialization of some entities, the code is also placed in`setup`Perform once before starting the main cycle.

When developing a large multi-file project, you have to drag the tickers of various blocks of the program from different files to the main loop, along with them, the initialization functions for calling in the setup are drawn, and global variables for interaction between files and blocks are created. For tasks that are called with a certain period (for example, once per second), program timers are written and also placed in the loop for constant time checking.
```cpp
void setup() {
    disp_init();
    sensor_init();
    wifi_init();
}

void loop() {
    sensors_tick();
    buttons_tick();
}
```

This library solves all these problems:
- Add tickers to the common loop from either end of the program without connecting to the main executable
- Create additional “virtual” setup and loop encapsulated in separate files
- Dynamic creation and removal of tasks during the program
- Sending and receiving events throughout the program
- Built-in timer for periodic or delayed task calls

> This allows you to divide the program into completely independent threads, as well as write code in an event-oriented style.

Cons:
- Each task takes up 7 bytes in RAM (weight can be reduced by disabling ID and event support)
- Going through the task list and calling handlers takes around 8us per task (AVR 16MHz), which is equivalent to two digitalRead calls.

<a id="versions"></a>

## Versions
- v1.0
- v1.1.0 - added threads with parallel execution and asynchronous waiting
- v1.2.0 - major update, changed the logic of calling Setup and removing tasks

<a id="install"></a>

## Installation
- The library can be found by the name **Looper** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/Looper/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
