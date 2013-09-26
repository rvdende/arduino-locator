//Ultra compact high performance e-compass
//3D accelerometer and 3D magnetometer module

#define LSM303DLHC_ACCEL_ADDRESS 0x19
#define LSM303DLHC_MAGNET_ADDRESS 0x1E

float sencompassx = 10.0;
float sencompassy = 0.0;
float sencompassz = 0.0;
float senaccelx = 0.0;
float senaccely = 0.0;
float senaccelz = -90.0;

bool did_timeout = false;
unsigned int io_timeout = 0;
byte last_status;

bool LSM303DLHC_init() {
  Wire.beginTransmission(LSM303DLHC_ACCEL_ADDRESS);
  Wire.write(0x20);
  byte last_status = Wire.endTransmission();
  
  Wire.requestFrom(LSM303DLHC_ACCEL_ADDRESS, 1);
  if (Wire.available()) {
    byte out = Wire.read();
    if (out!=0) {
      // Enable Accelerometer      
      // Normal power mode, all axes enabled
      Wire.beginTransmission(LSM303DLHC_ACCEL_ADDRESS);
      Wire.write(0x20);
      Wire.write(0x57);   // 0x27 = 0b00100111
      Wire.endTransmission();   
      Wire.beginTransmission(LSM303DLHC_ACCEL_ADDRESS);
      Wire.write(0x23);
      Wire.write(0x08);   // DLHC: enable high resolution mode
      Wire.endTransmission();         
    
      // Enable Magnetometer    
      // Continuous conversion mode  
      Wire.beginTransmission(LSM303DLHC_MAGNET_ADDRESS);
      Wire.write(0x02);
      Wire.write(0x00);   // 0x00 = 0b00000000
      Wire.endTransmission();        
      return true;
    }
  }
  return false;
}



void LSM303DLHC_readAcc()
{
  Wire.beginTransmission(LSM303DLHC_ACCEL_ADDRESS);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  Wire.write(0x28 | (1 << 7));
  last_status = Wire.endTransmission();
  Wire.requestFrom(LSM303DLHC_ACCEL_ADDRESS, 6);

  unsigned int millis_start = millis();
  did_timeout = false;
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout) {
      did_timeout = true;
      return;
    }
  }

  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  // combine high and low bytes, then shift right to discard lowest 4 bits (which are meaningless)
  // GCC performs an arithmetic right shift for signed negative numbers, but this code will not work
  // if you port it to a compiler that does a logical right shift instead.
  senaccelx = ((int16_t)(xha << 8 | xla)) >> 4;
  senaccely = ((int16_t)(yha << 8 | yla)) >> 4;
  senaccelz = ((int16_t)(zha << 8 | zla)) >> 4;
}

void LSM303DLHC_readMag()
{
  Wire.beginTransmission(LSM303DLHC_MAGNET_ADDRESS);
  Wire.write(0x03);
  last_status = Wire.endTransmission();
  Wire.requestFrom(LSM303DLHC_MAGNET_ADDRESS, 6);

  unsigned int millis_start = millis();
  did_timeout = false;
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout) {
      did_timeout = true;
      return;
    }
  }

  byte xhm = Wire.read();
  byte xlm = Wire.read();
  byte zhm = Wire.read();
  byte zlm = Wire.read();
  byte yhm = Wire.read();
  byte ylm = Wire.read();

  // combine high and low bytes
  sencompassx = (int16_t)(xhm << 8 | xlm);
  sencompassy = (int16_t)(yhm << 8 | ylm);
  sencompassz = (int16_t)(zhm << 8 | zlm);
}

float getaccelx() { return senaccelx; }
float getaccely() { return senaccely; }
float getaccelz() { return senaccelz; }

float getcompassx() { return sencompassx; }
float getcompassy() { return sencompassy; }
float getcompassz() { return sencompassz; }
