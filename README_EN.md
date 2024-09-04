This is an automatic translation, may be incorrect in some places. See sources and examples!

# Looper
Easy pseudo-test event-oriented framework for Arduino
- a more convenient organization of the program
- division into virtual flows
- Static and dynamic creation of tasks
- calling tasks with a given period
- Sending events and data between tasks
- General execution in one stream without need for mittexes
- Convenient API to create your tasks and integrate with other classes
- Easy to unleash the arduino nucleus (replace two functions)
- without dependencies on other libraries
- lightweight: the core occupies 1 KB flash drive and 30 B RAM (AVR)

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Documentation
Located in [DOCS folder] (https://github.com/gyverlibs/looper/tree/main/docs/0.main.md)

## Content
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="usage"> </a>

## Why do you need a looper
Classic Arduino project, not based on operating systems like Freertos, has an architecture of a single -flow super -cycle (*POOL BASED*): There is a main `loop` cycle, which is performed in a circle throughout the work of the program and in this cycle we interview buttons/sensors/communications/communication/Other.Such survey functions in the terminology of this library are called "tickers."To initialize some entities, the code is also placed in `setup` to execute once before launching the main cycle.

When developing a large multi -file project, you have to drag tickers of various blocks of the program from different files in the main LOOP, and the initialization functions for calling in Setup are also drawn with them, and global variables are created for interaction between files and blocks.For tasks that are called with a certain period (for example, once per second), software timers are written and also placed in LOOP for a constant test of time.
`` `CPP
VOID setup () {
Disp_init ();
Sensor_init ();
wifi_init ();
}

VOID loop () {
Sensors_Tick ();
buttons_tick ();
}
`` `

This library solves all these problems:
- Adding tickers to the common to LOOP from any end of the program without connecting to the main executable file
- Creation of additional "virtual" setup and loop, encapsulated in separate files
- dynamic creation and removal of tasks during the work of the program
- Sending and receiving events through the entire program
- built -in timer for a periodic or postponed call of tasks

> This allows you to divide the program into completely independent flows, as well as write code in an event-oriented style

Cons:
- Each task occupies 7 bytes in RAM
- the passage along the list of tasks and calling the handlers occupies in the region of 8us per task (AVR 16MHz), which is tantamount to two calls of DigitalRead

<a id="versions"> </a>

## versions
- V1.0

<a id="install"> </a>
## Installation
- The library can be found by the name ** looper ** and installed through the library manager in:
- Arduino ide
- Arduino ide v2
- Platformio
- [download library] (https://github.com/gyverlibs/looper/Archive/Refs/Heads/Main.zip) .Zip archive for manual installation:
- unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
- unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
- unpack and put in *documents/arduino/libraries/ *
- (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="feedback"> </a>

## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code