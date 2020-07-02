/*  Wasserwaage Copyright by Reinhard Kopka
 *  
 *           18.01.2020 rKop  M5Stack, Sensor geht, Display (von TFT Version) integriert
 *  0.09.001 24.01.2020 rKop  Canvas in Funktion und als .JS nachgeladen, auch für Kompaß, M5 Kompaß ja, aber komisch
 *  0.09.002 25.01.2020 rKop  CFG umgestellt, Anzeigefehler weg      
 *  0.09.003 11.06.2020 rKop  offenes Github
 *           27.06.2020 rKop  compiliert auch für externes TFT (kein M5STACK)
 *           28.06.2020 rKop  MODE eingeführt - Spannung und Sleep
 *           
 *  TODO
 *  
 *  - TFT Grafiken an M5 angepaßt, etwas größer und mehr, evt. aus Vektor errechnet      
 *  
 *  Höhe 700x480 -> 700(?) x 390 -> paßt für Pumpkin, noch Platz rechts
 */
 
#include <math.h> // Sinus...
#include <Wire.h> // Must include Wire library for I2C

#include <driver/adc.h>
#include "esp_system.h"
#include "FS.h"
#include "SPIFFS.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "defines.h"
#include "grafics.h"    // my bitmaps

#ifdef USE_M5STACK
 #include <M5Stack.h>
 #include "utility/MPU9250.h"

 MPU9250 IMU;   // internen Gyro nutzen, dazu noch andere Sensoren
#else
 #include <SparkFun_MMA8452Q.h> // Accelometer library

// Begin using the library by creating an instance of the MMA8452Q
//  class. We'll call it "accel". That's what we'll reference from
//  here on out.
// evt. 2 Werte anlegen mit beiden Adressen, dann Test und den funktionierenden über Zeiger ansprechen
 MMA8452Q accel(0x1D);  //(default 0x1D) kann auch 0x1C sein (I2C)

 #ifdef USE_DISPLAY   // TFT aber nicht M5Stack
  #include <Adafruit_GFX.h>    // Core graphics library
  #include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
  #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

  Adafruit_ST7735 tft_ = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
 #endif

#endif

WebServer server(80); //Server on port 80

#ifdef USE_TEMP
 #include "SparkFun_Si7021_Breakout_Library.h" // Temp und Humidity Sensor

//Create Instance of HTU21D or SI7021 temp and humidity sensor (and MPL3115A2 barrometric sensor)
 Weather sensor;

 float humidity = 0;
 float tempC = 0;
#endif

// ADC
int sensorValue = 0;        // value read from D+

// für GYRO Berechnungen
float ff[3]={0,0,0},f[3]={0,0,0};
float fxrot=0,fyrot=0,fzrot=0;
float fp=0,fr=0;
int   print_=0;
float fp_corr,fr_corr;
float z1,z2,z3,z4;

// Kompaßkalibrierung
int CmaxX=0, CminX=16000, CmaxY=0, CminY=16000;

long last_key=0;    // Zeit in millis
bool pressed=false;
long last_keyT=0;    // Zeit in millis
bool pressedT=false;
int modeT=MODE_WLAN;
bool standby=false;
bool eeprom_ok=false;
//bool do_refresh=false;
bool do_calib=false;  // compass
int nb; // number of clients
bool Dplus = false;
time_t last_modeT_change = 0;
bool spiffs_fail = false;

// Unterfunktionen
#include "cfg.h"
#include "utils.h"
#include "http.h"

//===============================================================
//                  SETUP
//===============================================================
void setup(void)
{
  String output;

#ifdef USE_M5STACK
  M5.begin();
  pinMode(CAL_KEY, INPUT_PULLUP);
  dacWrite (25,0); // das macht den Lautsprecher leise, nur ein kurzer Knack beim Start
#endif

  Wire.begin();
     
  DSerial.begin(115200);
  DSerial.println(""); // wegen Boot Messages
  DSerial.println((String("Wasserwaage - ") + String(VERSIONSTRING)));

  pinMode(LICHT, OUTPUT);
  digitalWrite(LICHT, HIGH);
  pinMode(TASTER, INPUT);
 
#ifdef ESP32
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    DSerial.println("SPIFFS Mount Failed");
    spiffs_fail = true;
//    return; // dann nur default Werte -> evt. irgendwie anzeigen
  }
#endif 

  read_defaults(config);
  read_ini(CFGFILE_NAME , config);  // Ergebnis auswerten ?

#ifdef USE_DISPLAY
  // Start device display with ID of sensor
  tft_.setTextSize(2);
  tft_.setTextColor(WHITE,BLACK); 
  
  tft_.setCursor(0,0);
  tft_.println(F("Wasserwaage"));
  tft_.print(F("Laenge:"));
  tft_.println(confvalues.laenge);
  tft_.print(F("Breite1:"));
  tft_.println(confvalues.breite1);
  tft_.print(F("Breite2:"));
  tft_.println(confvalues.breite2);
#endif

// nur bei Bedarf einschalten mit MODE
// was braucht man für Server ?

  WiFi.mode(WIFI_AP);           //Only Access point  

  // for ESP32
  WiFi.softAP(confvalues.ssid.c_str(), confvalues.password.c_str());  //Start HOTspot removing password will disable security   Kanal ????

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  DSerial.print("HotSpot IP:");
  DSerial.println(myIP);

  DSerial.print("ssid:");
  DSerial.print(confvalues.ssid);
  DSerial.print("  Passwd:");
  DSerial.print(confvalues.password);
  DSerial.println("<");
  
  server.on("/", handleRoot);        
  server.on("/calib.html", handleCalib);
  server.on("/config.html", handleConfig);
  server.on("/main.js", handleMain);

#ifdef SHOW_KOMPASS
  server.on("/compass.html", handleCompass);
  server.on("/compass.js", handleJSCompass);
#endif  
  
  server.begin();                  //Start server
  DSerial.println("HTTP server started");

#ifndef USE_M5STACK
  // Choose your adventure! There are a few options when it comes
  // to initializing the MMA8452Q:
  //  1. Default init. This will set the accelerometer up
  //     with a full-scale range of +/-2g, and an output data rate
  //     of 800 Hz (fastest).
  accel.init();
  //  2. Initialize with FULL-SCALE setting. You can set the scale
  //     using either SCALE_2G, SCALE_4G, or SCALE_8G as the value.
  //     That'll set the scale to +/-2g, 4g, or 8g respectively.
  //accel.init(SCALE_4G); // Uncomment this out if you'd like
  //  3. Initialize with FULL-SCALE and DATA RATE setting. If you
  //     want control over how fast your accelerometer produces
  //     data use one of the following options in the second param:
  //     ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12,
  //     ODR_6, or ODR_1. 
  //     Sets to 800, 400, 200, 100, 50, 12.5, 6.25, or 1.56 Hz.
  //accel.init(SCALE_8G, ODR_6);

  DSerial.println("Accel init finished (8452)");
#else
// Read the WHO_AM_I register, this is a good test of communication
  byte c = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  DSerial.print("MPU9250 "); DSerial.print("I AM "); DSerial.print(c, HEX);
  DSerial.print(" I should be "); DSerial.println(0x71, HEX);

  // if (c == 0x71) // WHO_AM_I should always be 0x68
  {
    DSerial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    IMU.MPU9250SelfTest(IMU.SelfTest);
    DSerial.print("x-axis self test: acceleration trim within : ");
    DSerial.print(IMU.SelfTest[0],1); DSerial.println("% of factory value");
    DSerial.print("y-axis self test: acceleration trim within : ");
    DSerial.print(IMU.SelfTest[1],1); DSerial.println("% of factory value");
    DSerial.print("z-axis self test: acceleration trim within : ");
    DSerial.print(IMU.SelfTest[2],1); DSerial.println("% of factory value");
    DSerial.print("x-axis self test: gyration trim within : ");
    DSerial.print(IMU.SelfTest[3],1); DSerial.println("% of factory value");
    DSerial.print("y-axis self test: gyration trim within : ");
    DSerial.print(IMU.SelfTest[4],1); DSerial.println("% of factory value");
    DSerial.print("z-axis self test: gyration trim within : ");
    DSerial.print(IMU.SelfTest[5],1); DSerial.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);

    IMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    DSerial.println("MPU9250 initialized for active data mode....");

#ifdef USE_M5KOMPASS

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = IMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    DSerial.print("AK8963 "); DSerial.print("I AM "); DSerial.print(d, HEX);
    DSerial.print(" I should be "); DSerial.println(0x48, HEX);

    // Get magnetometer calibration from AK8963 ROM
    IMU.initAK8963(IMU.magCalibration);
    // Initialize device for active mode read of magnetometer
    DSerial.println("AK8963 initialized for active data mode....");
    //if (Serial)
    {
      //  Serial.println("Calibration values: ");
      DSerial.print("X-Axis sensitivity adjustment value ");
      DSerial.println(IMU.magCalibration[0], 2);
      DSerial.print("Y-Axis sensitivity adjustment value ");
      DSerial.println(IMU.magCalibration[1], 2);
      DSerial.print("Z-Axis sensitivity adjustment value ");
      DSerial.println(IMU.magCalibration[2], 2);
    }
#endif

  } // if (c == 0x71)
  // else
  // {
  //   DSerial.print("Could not connect to MPU9250: 0x");
  //   DSerial.println(c, HEX);
  //   while(1) ; // Loop forever if communication doesn't happen
  // }  
#endif

  // Spannungsmessung
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);  // D+ ? Pin 34
  //int val = adc1_get_raw(ADC1_CHANNEL_6);
  //adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_2_5); // Verstärkung 2,5dB 2,5V? 0dB =1,1 V Max  // GPIO36
  //adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_0); //  Verstärkung 0dB =1,1 V Max   // GPIO39
    
#ifdef USE_TEMP
  //Initialize the I2C sensors and ping them
  sensor.begin();
#endif

#ifdef USE_DISPLAY
  delay(3000);
  tft_.fillScreen(BLACK);
#endif  
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void)
{
  long        time_        =millis()-last_key; // Zeit gedrückt
  bool        press_short  =false, press_long_active=false, press_long=false;
  bool        do_calib     =false;  // eigentlich doppelt, aber press_long_active könnte noch was anderes bewirken
  static bool calib_changed=false;  // erst nach Taste abspeichern
  static unsigned long next_LED=0;
  static bool last_LED_on=false;

  long        time_T        =millis()-last_keyT; // Zeit gedrückt
  bool        press_shortT  =false, press_long_activeT=false, press_longT=false;
  
  server.handleClient();          //Handle client requests
  
  nb = WiFi.softAPgetStationNum();

// Taste für AP/WLAN Umschaltung

 // DSerial.print(" T=" + String(digitalRead(TASTER))+ " ");

  if ( !digitalRead(TASTER))
  {
    if (!pressedT)  // neuer Druck
    {
      last_keyT=millis();
      pressedT=true;
    }
    else  // schon länger gedrückt
    {
      if (time_T>KEYTIME_LONG)
      {
        press_long_activeT=true;
      }
    }
  }
  else    // not pressed
  {
    if (pressedT)  // es war vorher gedrückt
    {
      if (time_T>KEYTIME_MIN and time_T<KEYTIME_SHORT)
        press_shortT=true;
      else if (time_T>KEYTIME_LONG)
        press_longT=true;
    }
    else  // keine Taste auch vorher
    {    
    }
    pressedT=false;
  }

//  DSerial.print(" Dplus=" + String(Dplus) + " modeT=" + String(modeT) + " deltaT=" + String(time(NULL) - last_modeT_change) + "  "); 
  int modeT_old = modeT;
  bool is10min =  last_modeT_change + MODETIMEOUT < time(NULL);
  switch (modeT)
  {
    case MODE_NOCLIENT:  //  D+ aber kein Client, WLAN bleibt an, Berechnungen nicht nötig, außer Spannung...
      if (!Dplus)            modeT = MODE_NODPLUS;
      else if (nb>0)         modeT = MODE_SLOW; 
      break;       
      
    case MODE_SLOW:      //  D+ und Client
      if (!Dplus)            modeT = MODE_NODPLUS;
      else if (nb<=0)        modeT = MODE_NOCLIENT;
      break;       
      
    case MODE_FAST:      //  D+ und Client und Anwahl in der GUI (10min lang, dann SLOW)
      if (!Dplus)            modeT = MODE_NODPLUSFAST;
      else if (is10min)      modeT = MODE_SLOW;   // muß auch Seite neu laden !!!
      else if (nb<=0)        modeT = MODE_NOCLIENT;
      break;       
      
    case MODE_NODPLUS:   //  kein D+, SLOW ?, nach 10min in SLEEP 
      if      (Dplus and nb<=0) modeT = MODE_NOCLIENT;
      else if (Dplus and nb>0 ) modeT = MODE_SLOW;
      else if (nb<=0)           modeT = MODE_SLEEP;
      else if (is10min)         modeT = MODE_SLEEP;
      break;

    case MODE_NODPLUSFAST:   //  kein D+, aber vorher Client(FAST), SLOW, nach 10min in SLEEP 
      if      (Dplus and nb<=0) modeT = MODE_NOCLIENT;
      else if (Dplus and nb>0 ) modeT = MODE_FAST;
      else if (nb<=0)           modeT = MODE_SLEEP;  // ohne Client kann man gleich in SLEEP
      else if (is10min)         modeT = MODE_SLEEP;
      break;

    case MODE_WLAN:      //  kein D+, durch Taste (wie geht das mit längerem Sleep ? Zeiten für pressed_long anschauen)  AP ein für 10min oder bis wieder Taste. Dann SLEEP
      if (Dplus)             modeT = MODE_NOCLIENT;
      else if (press_shortT) modeT = MODE_SLEEP;
      else if (is10min)      modeT = MODE_SLEEP;
      break;       
      
    case MODE_SLEEP:     //  längerer Sleep, sonst nichts, auf Taste und D+ achten, dann NOCLIENT oder WLAN
      if (Dplus)             modeT = MODE_NOCLIENT;
      else if (press_shortT) modeT = MODE_WLAN;
      break;       
  
    default:  
      DSerial.println("Falscher Modus : " + String(modeT));
      modeT = MODE_SLEEP;
      break;       
  }
  if (modeT != modeT_old)
  {
    DSerial.println(" (2) modeT=" + String(modeT) + " deltaT=" + String(time(NULL) - last_modeT_change) + "  "); 
  
    last_modeT_change = time(NULL);
    //je nach Mode WLAN an aus
    if (modeT==MODE_SLEEP)
    {
        // AP aus
    }
    else if (modeT_old==MODE_SLEEP)
    {
        // AP an
    }
  }
    
  //DSerial.println(" (2) modeT=" + String(modeT) + " deltaT=" + String(time(NULL) - last_modeT_change) + "  "); 
  
#ifdef USE_M5STACK    // auch bei AVR Variante so machen
  if ( !digitalRead(CAL_KEY))
  {
    if (!pressed)  // neuer Druck
    {
      last_key=millis();
      pressed=true;
    }
    else  // schon länger gedrückt
    {
      if (time_>KEYTIME_LONG)
      {
        press_long_active=true;
      }
    }
  }
  else    // not pressed
  {
    if (pressed)  // es war vorher gedrückt
    {
      if (time_>KEYTIME_MIN and time_<KEYTIME_SHORT)
        press_short=true;
      else if (time_>KEYTIME_LONG)
        press_long=true;
    }
    else
    {    
      if (calib_changed)
      {      
        //write_eeprom();  für Tests
        calib_changed=false;
      }
    }
    pressed=false;
  }
  
  if (press_long_active)    // anders, bei Erreichen Flag setzen, das bearbeitet wird und dann gesperrt bis !pressed
  {
    DSerial.println("calib");
    confvalues.fcp=fp;  confvalues.fcr=fr;    
    do_calib=true;
    // Flag und dann EEPROM schreiben

    calib_changed=true; // war für getrenntes oder späteres Schreiben erst nach Loslassen der Taste
    // Flag und dann EEPROM schreiben
    if (calib_changed)
    {    
      {
        // write  
        DSerial.println(" Taste Calib");
        //pre_text += "Differenz<br>\n";
        String output;
        write_ini(CFGFILE_NAME , config, OUTPUT_FILE, output);
        DSerial.println("New ini:\n" + output);
      }
//      config_changed=false;    
      calib_changed=false;
    }    
  } 
#endif

#ifdef USE_DISPLAY
  if (1)
#else  
  //if (!standby)
  if (modeT==MODE_SLOW or modeT==MODE_FAST or modeT==MODE_NODPLUS or modeT==MODE_NODPLUSFAST)
#endif
  {
#ifndef USE_M5STACK    
    // Use the accel.available() function to wait for new data
    //  from the accelerometer.
    if (accel.available())
    {      
      // First, use accel.read() to read the new variables:
      accel.read();
      // accel.read() will update two sets of variables. 
      // * int's x, y, and z will store the signed 12-bit values 
      //   read out of the accelerometer.
      // * floats cx, cy, and cz will store the calculated 
      //   acceleration from those 12-bit values. These variables 
      //   are in units of g's.    

      // f is the sensor output x=0
      f[0] = (accel.cx /SUMCNT + f[0] *(SUMCNT-1)/SUMCNT);
      f[1] = (accel.cy /SUMCNT + f[1] *(SUMCNT-1)/SUMCNT);
      f[2] = (accel.cz /SUMCNT + f[2] *(SUMCNT-1)/SUMCNT);
#else
     // test for M5 acc    
// If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
    if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {  
      IMU.readAccelData(IMU.accelCount);  // Read the x/y/z adc values
      IMU.getAres();

      // Now we'll calculate the accleration value into actual g's
      // This depends on scale being set
      IMU.ax = (float)IMU.accelCount[0]*IMU.aRes; // - accelBias[0];
      IMU.ay = (float)IMU.accelCount[1]*IMU.aRes; // - accelBias[1];
      IMU.az = (float)IMU.accelCount[2]*IMU.aRes; // - accelBias[2];    
/*
      DSerial.print("x=" + String(IMU.accelCount[0])+ " ");
      DSerial.print("y=" + String(IMU.accelCount[1])+ " ");
      DSerial.print("z=" + String(IMU.accelCount[2])+ " ");
      DSerial.print("ax=" + String(IMU.ax)+ " ");
      DSerial.print("ay=" + String(IMU.ay)+ " ");
      DSerial.print("az=" + String(IMU.az)+ " ");
      DSerial.println();
*/

    IMU.readMagData(IMU.magCount);  // Read the x/y/z adc values
    IMU.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    IMU.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    IMU.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    IMU.magbias[2] = +125.;

// 1900-2200 ca. 
    comp_x=IMU.magCount[0]+3300+2000; 
    comp_y=IMU.magCount[1]; 
  /*
       // Print mag values in degree/sec
        DSerial.print(" X-mag field: "); DSerial.print(IMU.magCount[0]);
        DSerial.print(" Y-mag field: "); DSerial.print(IMU.magCount[1]);
        DSerial.print(" Z-mag field: "); DSerial.print(IMU.magCount[2]);
    */    
    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    IMU.mx = (float)IMU.magCount[0]*IMU.mRes*IMU.magCalibration[0] -
               IMU.magbias[0];
    IMU.my = (float)IMU.magCount[1]*IMU.mRes*IMU.magCalibration[1] -
               IMU.magbias[1];
    IMU.mz = (float)IMU.magCount[2]*IMU.mRes*IMU.magCalibration[2] -
               IMU.magbias[2];
/*
     // Print mag values in degree/sec
      DSerial.print("X-mag field: "); DSerial.print(IMU.mx);
      DSerial.print(" mG ");
      DSerial.print("Y-mag field: "); DSerial.print(IMU.my);
      DSerial.print(" mG ");
      DSerial.print("Z-mag field: "); DSerial.print(IMU.mz);
      DSerial.println(" mG");
  */                   
    // f is the sensor output x=0
      f[0] = (IMU.ax /SUMCNT + f[0] *(SUMCNT-1)/SUMCNT);
      f[1] = (IMU.ay /SUMCNT + f[1] *(SUMCNT-1)/SUMCNT);
      f[2] = (IMU.az /SUMCNT + f[2] *(SUMCNT-1)/SUMCNT);
#endif    
   
      // now exchange from array    -> ff[0] is x in real (z shows down, x in driving direction)
      for (int n=0;n<3;n++)
      {
        ff[0] = f[ (int)(abs(confvalues.achse0)-1) ] * (confvalues.achse0 > 0 ? 1 : -1) ;
        ff[1] = f[ (int)(abs(confvalues.achse1)-1) ] * (confvalues.achse1 > 0 ? 1 : -1) ;
        ff[2] = f[ (int)(abs(confvalues.achse2)-1) ] * (confvalues.achse2 > 0 ? 1 : -1) ;       
/*        
        DSerial.print("n=");
        DSerial.print(n);
        DSerial.print(" f=");
        DSerial.print(f[n]);
        DSerial.print(" ff=");
        DSerial.print(ff[n]);
        DSerial.print("  vorz.=");
        DSerial.print((confvalues.achse[n] > 0 ? 1 : -1));
        DSerial.print(" confvalues.achse=");
        DSerial.println(confvalues.achse[n]);
  */      
      }
      // ab hier sind die Achsen korrigiert und man geht mit räumlich korrekten Achsen weiter. Nur noch die Drehung, die sich jetzt nich auf die 
      // Sensor xy-Achsen sondern auf die flache xy Achsen bezieht
      fxrot = cos(confvalues.angle*PI/180)*ff[0] + sin(confvalues.angle*PI/180)*ff[1];
      fyrot = sin(confvalues.angle*PI/180)*ff[0] - cos(confvalues.angle*PI/180)*ff[1];

      // Ausgabe nur bei jedem xten Durchlauf, so ca. 4-5 Werte pro sek.
      if ((print_ % CALC_PER_DISPLAY ) ==0) // and !standby)
      { 
          // in Funktion auslagern, evt. auch mal ein IRQ/Thread          
          if ( ff[2] !=0)
          {
            fr  =  atan2 ( fyrot, ff[2] ) * 180/PI ;
            fp  =  atan2 (-fxrot, ff[2] ) * 180/PI ;
          }
          else  // Nullstelle
          {
            fr  =  90 ;
            if (fyrot<0) fr = -fr;
  
            fp  =  90 ;
            if (fxrot>=0) fp = -fp;
          }
          fp_corr = fp - confvalues.fcp;
          fr_corr = fr - confvalues.fcr;
  
          fp_corr = -fp_corr; // nur wegen Sensor und Tests

          // hier check auf Bereich +-180°

          if (fp_corr <= -180)  fp_corr+=360;
          if (fp_corr >=  180)  fp_corr-=360;
          if (fr_corr <= -180)  fr_corr+=360;
          if (fr_corr >=  180)  fr_corr-=360;
    
          z1 = 0;
          z2 =      sin(-fp_corr*PI/180) * confvalues.laenge;
          z4 =      sin(-fr_corr*PI/180) * confvalues.breite1;
          z3 = z2 + sin(-fr_corr*PI/180) * confvalues.breite2;

          float minz=z1;        
          if (z2<minz)  minz=z2;
          if (z3<minz)  minz=z3;
          if (z4<minz)  minz=z4;
  
          minz = -minz;   // negative value will be positive to add it
          z1 +=minz; z2 +=minz; z3 +=minz; z4 +=minz; 

#ifdef USE_DISPLAY
          show_display();
#endif
/*
          char buf[10];
          
          if (do_calib)
            strcpy(buf,"cal");
          else if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK ) // test for OK
            strcpy(buf,"OK ");
          else
            strcpy(buf,"   ");
// was passiert mit buf ??
  */          
  /*
          DSerial.print("fp_corr=");
          DSerial.print(fp_corr);
          DSerial.print(" fr_corr=");
          DSerial.print(fr_corr);
          DSerial.print(" z1=");
          DSerial.print(z1);
          DSerial.print(" z2=");
          DSerial.print(z2);
          DSerial.print(" z3=");
          DSerial.print(z3);
          DSerial.print(" z4=");
          DSerial.println(z4);
*/

#if defined USE_KOMPASS || defined USE_M5KOMPASS
        int ret = get_compass();  // hier auch eine Mittelung ?
        if (ret !=0)  DSerial.println("Error with compass: " + String(ret));
#endif       
      }
      print_++;
  
    }
    else  // keine Positionsinfo gebraucht
    {
        // Fehlermeldung ausgeben
    }   
  } // !standby      

#ifdef USE_TEMP        
  if (modeT!=MODE_SLEEP)
    getWeather(); // für WLAN Zugriff
#endif

  //sensorValue = analogRead(ANALOGINPIN);  // Arduinofunktion
  // ESP32
  sensorValue = adc1_get_raw(ADC1_CHANNEL_6);  // 14V 2450  18K 3,3K -> 0,155 -> 14V * 0,155  = 2,16V -> 2450 -> 9V bei 273.0 -> 175,5 oder 14V=2450 ~ 
  if (sensorValue/175.5 > 10)   // 10.3V
    Dplus = true;
  else 
    Dplus = false;
  /*
  DSerial.print(" Ch6=");
  DSerial.print(sensorValue);
  DSerial.print("  ");
*/
  int led_on_time,led_off_time;
  
  switch (modeT)
  {
    case MODE_NOCLIENT:  //  D+ aber kein Client, WLAN bleibt an, Berechnungen nicht nötig, außer Spannung...
      led_off_time = LED_OFF;   // 3sek kurze Blitzer
      led_on_time  = LED_ON;      
      break;       
      
    case MODE_SLOW:      //  D+ und Client
    case MODE_FAST:      //  D+ und Client und Anwahl in der GUI (10min lang, dann SLOW)
    case MODE_NODPLUS:   //  kein D+, SLOW ?, nach 10min in SLEEP 
    case MODE_NODPLUSFAST:   //  kein D+, aber vorher Client(FAST), SLOW, nach 10min in SLEEP 
      if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK )
      {
        led_off_time = LED_OFF_OK;   // schnelles Blinken
        led_on_time  = LED_ON_OK;              
      }
      else
      {
        led_off_time = 0;   // Dauer AN
        led_on_time  = 1000;   // (egal)    
      }
      break;       
      
    case MODE_WLAN:      //  kein D+, durch Taste (wie geht das mit längerem Sleep ? Zeiten für pressed_long anschauen)  AP ein für 10min oder bis wieder Taste. Dann SLEEP
      led_off_time = 1000;
      led_on_time  = 1000;      
      break;       
      
    case MODE_SLEEP:     //  längerer Sleep, sonst nichts, auf Taste und D+ achten, dann NOCLIENT oder WLAN
      led_off_time = 1000;   // (egal)
      led_on_time  = 0;      // Dauer AUS
      break;       
  
    default:  
      DSerial.println("Falscher Modus : " + String(modeT));
      modeT = MODE_SLEEP;
      led_off_time = 1000;   // (egal)
      led_on_time  = 0;      // Dauer AUS
      break;       
  }

  if (led_on_time == 0)
    digitalWrite(LICHT, LOW);
  else if (led_off_time == 0)
    digitalWrite(LICHT, HIGH);
  else if (next_LED < millis())
  {
    if (last_LED_on)
    {
      digitalWrite(LICHT, LOW);   // turn the LED on (HIGH is the voltage level)
      next_LED = millis() + led_on_time;
    }
    else
    {
      digitalWrite(LICHT, HIGH);    // turn the LED off by making the voltage LOW
      next_LED = millis() + led_on_time;
    }
    last_LED_on = !last_LED_on;   
  }
  /*
  if (nb<=0)  // Number of clients
  {
     // LED just for standby - no clients 
    if (next_LED < millis())
    {
      if (last_LED_on)
      {
        digitalWrite(LICHT, LOW);   // turn the LED on (HIGH is the voltage level)
        next_LED = millis() + LED_OFF;
      }
      else
      {
        digitalWrite(LICHT, HIGH);    // turn the LED off by making the voltage LOW
        next_LED = millis() + LED_ON;
      }
      last_LED_on = !last_LED_on;   
    }
  }
  else if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK )
  {   // OK -> schnell blinken
    if (next_LED < millis())
    {
      if (last_LED_on)
      {
        digitalWrite(LICHT, LOW);   // turn the LED on (HIGH is the voltage level)
        next_LED = millis() + LED_OFF_OK;
      }
      else
      {
        digitalWrite(LICHT, HIGH);    // turn the LED off by making the voltage LOW
        next_LED = millis() + LED_ON_OK;
      }
      last_LED_on = !last_LED_on;     
    }
  }
  else    // normaler Betrieb
  {
    digitalWrite(LICHT, HIGH);
  }  
*/

  if (modeT == MODE_SLEEP)
  {
    // Sleep für 500ms(?)
#ifdef DO_SLEEP
      int sleep_time_in_ms = 500;
      
      if (1)
      {     
        esp_sleep_enable_timer_wakeup(sleep_time_in_ms * 1000); //0.1 seconds scheint auch nicht mehr Strom zu brauchen, reagiert aber besser;
        int ret = esp_light_sleep_start();
    
        DSerial.print(" S ");   
      }
      else
        flag_nosleep=false;
#else
    //delay(100);        
#endif        
  }
}
