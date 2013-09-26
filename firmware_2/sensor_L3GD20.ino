// L3GD20
// MEMS motion sensor
// three-axis digital output gyroscope

#define L3GD20_ADDRESS 0x6B

float sengyrovecx = 0.0;
float sengyrovecy = 0.0;
float sengyrovecz = 0.0;


bool L3GD20_init() {
  Wire.beginTransmission(L3GD20_ADDRESS);
  Wire.write(0x0F);
  Wire.endTransmission();
  
  Wire.requestFrom(L3GD20_ADDRESS, 1);  
  while(!Wire.available());      
  unsigned char test = Wire.read();

  if (test == 0xD4) {    
    //ENABLE DEFAULTS   
    Wire.beginTransmission(L3GD20_ADDRESS);
    Wire.write(0x20);
    Wire.write(0xFF); //0F    FF fast?
    Wire.endTransmission();
    
    return true;
  } else {
    return false;
  }  

}

void L3GD20_read() {
  Wire.beginTransmission(L3GD20_ADDRESS);
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  Wire.write(0x28 | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(L3GD20_ADDRESS, 6);

  while (Wire.available() < 6);

  short xlg = Wire.read();
  short xhg = Wire.read();
  short ylg = Wire.read();
  short yhg = Wire.read();
  short zlg = Wire.read();
  short zhg = Wire.read();

   sengyrovecx = (int16_t)(xhg << 8 | xlg);
   sengyrovecy = (int16_t)(yhg << 8 | ylg);
   sengyrovecz = (int16_t)(zhg << 8 | zlg);
}

float getgyrox() { return sengyrovecx; }
float getgyroy() { return sengyrovecy; }
float getgyroz() { return sengyrovecz; }

