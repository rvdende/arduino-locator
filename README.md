arduino-locator
==============

Arduino based realtime locator - initial 0.1 beta

Project aims
===============

Have an arduino equiped with gyro, accel, magnetometer, gps, barometric sensors and combine all of these into as accurate and responsive positional/rotational data as possible. This data should be repeatable and thus reliable for VR/REALWORLD integration/matching.

Usage
=====

Arduino UNO + pololu IMU

Load firmware

Use serial data at 115200bps and JSON.parse it.

```
node index.js
```
