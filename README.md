arduino-locator
==============

Arduino based realtime locator - initial 0.1 beta

Project aims
===============

Have an arduino equiped with gyro, accel, magnetometer, gps, barometric sensors and combine all of these into as accurate and responsive positional/rotational data as possible. This data should be repeatable and thus reliable for VR/REALWORLD integration/matching.

Usage
=====

Arduino UNO + pololu MiniIMU-9 v2 #1268 + sparkfun BMP085

https://www.sparkfun.com/tutorials/253

Load firmware onto your arduino from `/firmware`

Install node from http://nodejs.org/

Go to `/nodeviewer` folder in terminal/command prompt.

In windows shift right click on the folder and choose Open command window here.

Install node modules

```
npm install socket.io
npm install serialport
```

On linux you'll need to `sudo apt-get install build-essentials`
On windows you'll need to install visual studio first. Follow instructions on https://github.com/voodootikigod/node-serialport

```
node viewer.js
```

Open `http://localhost/`

Sample data
===========

You should get something like this. 

ax, ay, az  - Accelerometer meters per second
mx, my, mz  - Magnetometer flux
gx, gy, gz 	- gyro in degrees per second
tc 			- temperature in 0.1 Celsius
pr  		- air presure in HPa
al    		- altitude in Meters

```
{ "ax" : "-99.00","ay" : "9.00","az" : "-978.00","mx" : "-432.00","my" : "-145.00","mz" : "-422.00","gx" : "-172.00","gy" : "26.00","gz" : "41.00","tc" : "27.7","pr" : "85315","al" : "1427.33"}
{ "ax" : "-101.00","ay" : "12.00","az" : "-976.00","mx" : "-432.00","my" : "-145.00","mz" : "-422.00","gx" : "-164.00","gy" : "-20.00","gz" : "40.00","tc" : "27.7","pr" : "85315","al" : "1427.33"}
{ "ax" : "-101.00","ay" : "12.00","az" : "-976.00","mx" : "-432.00","my" : "-145.00","mz" : "-422.00","gx" : "-136.00","gy" : "-24.00","gz" : "102.00","tc" : "27.7","pr" : "85315","al" : "1427.33"}

```
