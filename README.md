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

Sample data
===========

You should get something like this. AccelXYZ, MagnetometerXYZ and GyroXYZ

```
{ "ax" : "119.00","ay" : "-18.00","az" : "-977.00","mx" : "-520.00","my" : "-71.00","mz" : "-443.00","gx" : "-153.00","gy" : "-2.00","gz" : "73.00"}
{ "ax" : "119.00","ay" : "-20.00","az" : "-980.00","mx" : "-520.00","my" : "-73.00","mz" : "-440.00","gx" : "-151.00","gy" : "-1.00","gz" : "89.00"}
{ "ax" : "119.00","ay" : "-20.00","az" : "-980.00","mx" : "-519.00","my" : "-73.00","mz" : "-443.00","gx" : "-106.00","gy" : "1.00","gz" : "129.00"}

```
