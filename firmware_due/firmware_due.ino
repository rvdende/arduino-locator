#include <Wire.h>

//vehicle status
float compassx = 10.0;
float compassy = 0.0;
float compassz = 0.0;
float accelx = 0.0;
float accely = 0.0;
float accelz = -90.0;
float gyrox = 0.0;
float gyroy = 0.0;
float gyroz = 0.0;

  float tempC = 0.0;
  float pressurekPa = 0.0;
  float altitudeMeters = 0.0;     
  
//system vars
long int timerns = micros();
int updaterate = 100; // updaterate/1000000 sec

void setup()
{
  Serial.begin(115200);
  //Serial.println("booting...");
  Wire.begin();

  if (BMP085_init()) {
    Serial.println("Altimeter: connected");
  } else {
    Serial.println("Altimeter: error!");
  }
  
  if (L3GD20_init()) {
    Serial.println("Gyro: connected");
  } else {
    Serial.println("Gyro: error!");
  }  
  
  if (LSM303DLHC_init()) {
    Serial.println("Accel/magnet: connected");
  } else {
    Serial.println("Accel/magnet: error!");
  }    
  
  LSM303DLHC_init();
}

int loopcounter = 0;


void loop()
{
  sensorUpdateL3GD20();
  
  if (loopcounter%50 == 0) {
    sensorUpdateBMP085();  //update alti, temp and pressure
    loopcounter = 0;
  }
  //send to computer
  Serial.print("{ \"ax\" : \"");
  Serial.print(accelx);
  Serial.print("\",");  
  Serial.print("\"ay\" : \"");
  Serial.print(accely);  
  Serial.print("\",");  
  Serial.print("\"az\" : \"");
  Serial.print(accelz);  
  Serial.print("\","); 
  
  Serial.print("\"mx\" : \"");
  Serial.print(compassx);  
  Serial.print("\",");    

  Serial.print("\"my\" : \"");
  Serial.print(compassy);  
  Serial.print("\",");    

  Serial.print("\"mz\" : \"");
  Serial.print(compassz);  
  Serial.print("\",");    

  Serial.print("\"gx\" : \"");
  Serial.print(gyrox);  
  Serial.print("\",");    

  Serial.print("\"gy\" : \"");
  Serial.print(gyroy);  
  Serial.print("\",");    

  Serial.print("\"gz\" : \"");
  Serial.print(gyroz);  
  Serial.print("\",");  
  
  Serial.print("\"time\" : \"");
  Serial.print(micros());  
  Serial.print("\",");    
///
  Serial.print("\"tc\" : \"");
  Serial.print(tempC);
  Serial.print("\",");  

  Serial.print("\"pr\" : \"");
  Serial.print(pressurekPa);
  Serial.print("\",");  
  
  Serial.print("\"al\" : \"");
  Serial.print(altitudeMeters, 2);
  Serial.println("\"}");
  
  if (abs(micros() - timerns) < updaterate) {
    //delayMicroseconds( updaterate - abs(millis()-timerns) );  
  }  
  timerns = micros();
  loopcounter++;
}

////////////////////////////////////////////////////////////////////
void sensorUpdateL3GD20() {
 L3GD20_read();
 LSM303DLHC_readAcc();
 LSM303DLHC_readMag();
 
 compassx = getcompassx();
 compassy = getcompassy();
 compassz = getcompassz();
 accelx = getaccelx();
 accely = getaccely();
 accelz = getaccelz();
 gyrox = getgyrox();
 gyroy = getgyroy();
 gyroz = getgyroz();
}

void sensorUpdateBMP085() {
   tempC = bmp085_get_temp();
   pressurekPa = bmp085_get_pressure();
   altitudeMeters = bmp085_get_altitude();  
}

