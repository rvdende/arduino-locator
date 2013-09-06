#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>


LSM303 compass;
L3G gyro;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  compass.init();
  compass.enableDefault();

  gyro.init(2);
  gyro.enableDefault();
  
  
}

void loop() {
  delay(10);
  compass.read();
  gyro.read(); 
  Serial.print("{ \"ax\" : \"");
  Serial.print(compass.a.x);
  Serial.print("\",");
  
  Serial.print("\"ay\" : \"");
  Serial.print(compass.a.y);  
  Serial.print("\",");
  
  Serial.print("\"az\" : \"");
  Serial.print(compass.a.z);  
  Serial.print("\",");  
  
  Serial.print("\"mx\" : \"");
  Serial.print(compass.m.x);  
  Serial.print("\",");    

  Serial.print("\"my\" : \"");
  Serial.print(compass.m.y);  
  Serial.print("\",");    

  Serial.print("\"mz\" : \"");
  Serial.print(compass.m.z);  
  Serial.print("\",");    

  Serial.print("\"gx\" : \"");
  Serial.print(gyro.g.x);  
  Serial.print("\",");    

  Serial.print("\"gy\" : \"");
  Serial.print(gyro.g.y);  
  Serial.print("\",");    

  Serial.print("\"gz\" : \"");
  Serial.print(gyro.g.z);  
  Serial.println("\"}");
  delay(20);
}

