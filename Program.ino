#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LCD5110_Basic.h>
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;

// lcd  modülün bağlantı pinleri
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12
//
LCD5110 myGLCD(8,9,10,11,12);

extern uint8_t SmallFont[];
static const int RXPin = 6, TXPin = 5;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++nesne oluşturma
TinyGPSPlus gps;

// GPS cihazın serial bağlantısı
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
   myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  

  ss.begin(GPSBaud);

  
 
  while (!compass.begin())
  {
   
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
      pusula();
 
}

void displayInfo()
{
 
  if (gps.location.isValid())
  {
    
String buf;
  buf += F("EN:");
  buf += String(gps.location.lat(), 6);

  String baf;
  baf += F("BOY:");
  baf += String(gps.location.lng(), 6);

  myGLCD.print("                     ", CENTER, 0);
  myGLCD.print("                    ", CENTER, 8);
  myGLCD.print("SIMDI ", CENTER, 0);
  myGLCD.print("BURADASINIZ", CENTER, 8);
  myGLCD.print(buf, CENTER, 16);
  myGLCD.print(baf, CENTER, 24);
 
  }

  
  else
  {
  myGLCD.print("                    ", CENTER, 0);
  myGLCD.print("                   ", CENTER, 8);
  myGLCD.print("                   ", CENTER, 16);
  myGLCD.print("                      ", CENTER, 24);
  myGLCD.print("KONUM ALINAMADI ", CENTER, 0);
  myGLCD.print("DISARI CIK", CENTER, 8);
  }

}

void pusula(){
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (55.0 + (33.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 

 String paf;
  paf += F("DERECE:");
  paf += String(headingDegrees);

if (headingDegrees>337) {
  myGLCD.print("                    ", CENTER, 44);
  myGLCD.print("KUZEY", CENTER, 44);
}
else if (headingDegrees<23) {
   myGLCD.print("                   ", CENTER, 44);
   myGLCD.print("KUZEY", CENTER, 44);
}
else if (headingDegrees<68) {
   myGLCD.print("                  ", CENTER, 44);
   myGLCD.print("KUZEYDOGU", CENTER, 44);
}
else if (headingDegrees<113) {
   myGLCD.print("                  ", CENTER, 44);
   myGLCD.print("DOGU", CENTER, 44);
}
else if (headingDegrees<158) {
  myGLCD.print("                ", CENTER, 44);
   myGLCD.print("GUNEYDOGU", CENTER, 44);
}
else if (headingDegrees<213) {
   myGLCD.print("                  ", CENTER, 44);
   myGLCD.print("GUNEY", CENTER, 44);
}
else if (headingDegrees<248) {
  myGLCD.print("                  ", CENTER, 44);
   myGLCD.print("GUNEYBATI", CENTER, 44);
}
else if (headingDegrees<293) {
   myGLCD.print("                 ", CENTER, 44);
   myGLCD.print("BATI", CENTER, 44);
}
else if (headingDegrees<338) {
   myGLCD.print("                 ", CENTER, 44);
   myGLCD.print("KUZEYBATI", CENTER, 44);
}

myGLCD.print("                    ", CENTER, 36);
myGLCD.print(paf, CENTER, 36);
  delay(400);
  }
