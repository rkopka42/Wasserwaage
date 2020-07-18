/*  utils.h  Hilfsfunktionen für Wasserwaage
 * 
 */

// Zahlenausgabe für Höhenwerte
void get_height(char *buf_, int val, int max_val, bool left)
{
  if (val<=max_val)        
  {
    if (left)
      sprintf(buf_,"%02d ",val);
    else
      sprintf(buf_," %02d",val);
  }
  else
    sprintf(buf_,">%02d",max_val);
}

// Darstellung der Winkel - geht sicher simpler
/*  -0.2
 *  -10
 *   10
 *   0.2
 */  
void get_angle(char *buf, float value)
{
  String s;
  
  if (value < 0)  s=String("-");
  else            s=String(" ");
  
  s += String(abs(value), (abs(value) >=10? 0:1) );
  if (abs(value) >=10)  s += " ";
  strcpy(buf, s.c_str() );
}       

String get_angle(float value)
{
  static String s;
  
  if (value < 0)  s=String("-");
  else            s=String(" ");
  
  s += String(abs(value), (abs(value) >=10? 0:1) );
  if (abs(value) >=10)  s += " ";
  DSerial.println(s);
  return s;
}       

/*
void dumpClients()
{
  int nb = WiFi.softAPgetStationNum();
  DSerial.print("Stations connected = ");
  DSerial.println(WiFi.softAPgetStationNum());
}
*/

// Adresse des HDMMI01
#define  COMP_I2ADDR  0x30 
#define  TakeMeasure  0x01 

// Kompaßkalibrierung
int CmaxX=0, CminX=16000, CmaxY=0, CminY=16000; // von http benutzt

int maxX=2124, minX=1947, maxY=2133, minY=1944;
float mygrad=0;

int comp_x,comp_y; 

// Werte in globalen Variablen 
// return 0-OK -1 -2 -3 Fehler bei I2C
int get_compass()
{ 
  byte MsbX,LsbX,MsbY,LsbY; 
  int  x,y; 
  char n; 

#ifdef USE_M5KOMPASS
#if 0
  DSerial.print(" Comp: ");
  DSerial.print(comp_x);
  DSerial.print(", ");
  DSerial.println(comp_y);
#endif
// done outside
#else
// falls niemand Werte liefert
  comp_x=1000;  // dummy
  comp_y=1000; 
#endif

#ifdef USE_KOMPASS 
  Wire.beginTransmission(COMP_I2ADDR); // Pollin HDMM01 
  Wire.write(byte(0));   // Adressfeld ist hier nicht wichtig 
  Wire.write(byte(TakeMeasure)); 
  Wire.endTransmission(); 
  delay(20); 
  //delay(50); 

  Wire.beginTransmission(COMP_I2ADDR); // Pollin HDMM01 
  Wire.write(byte(0));   // Adressfeld ist hier nicht wichtig 
  Wire.write(byte(TakeMeasure)); 
  Wire.endTransmission(); 
  delay(20);
  //delay(50); 

  Wire.beginTransmission(COMP_I2ADDR); 
  n =Wire.write(byte(0x01)); 
  if (n != 1)
  {
    DSerial.print("n wrong ");
    DSerial.print(n);
    return -1;
  }
  n = Wire.endTransmission(false); // hold the I2C-bus
  if (! ((n == 0) || (n ==7)) )
  {
    DSerial.print("n2 wrong ");
    DSerial.print(n);
    return -2;
  }    

  Wire.requestFrom(COMP_I2ADDR, 4);
  n=20;   // Timeout  20*20 = 400ms
  while (Wire.available()<4 and n-- >0)        // Timeout !!!!!!!!!!!!!!!!!!!
  {
    //DSerial.print(Wire.available()); 
    //DSerial.print(" ");
    delay(20);
  }
  if (n<=0)  
  {
    DSerial.print("n3 wrong ");
    DSerial.print(n);    
    return -3;
  }
  DSerial.print(n);    
  
  MsbX  =Wire.read();          // obere  4 Bit X 
  LsbX  =Wire.read();          // untere 8 Bit X 
  MsbY  =Wire.read();          // obere  4 Bit Y 
  LsbY  =Wire.read();          // untere 8 Bit Y 
  Wire.endTransmission();         // stop transmitting 

  comp_x=((MsbX&0x0f)*256)+(LsbX); 
  comp_y=((MsbY&0x0f)*256)+(LsbY); 
#endif
  /*
  DSerial.print(x);
  DSerial.print(", ");
  DSerial.print(y);
  DSerial.print(",  (" + String(confvalues.minX) + "," + String(confvalues.maxX) + "," + String(confvalues.minY) + "," + String(confvalues.maxY) +  ")  -> ");
  */
  x = map(comp_x, confvalues.minX, confvalues.maxX, -180, 180); 
  y = map(comp_y, confvalues.minY, confvalues.maxY, -180, 180); 
  /*DSerial.print(x);
  DSerial.print(", ");
  DSerial.print(y);
  DSerial.print(",  --> ");
  */
  //x = map(x, 1900, 2188, -180, 180); 
  //y = map(y, 1910, 2193, -180, 180); 
  mygrad = atan2(x, y)*180/3.1415927410; 

  mygrad += confvalues.angle2;  
  if (mygrad < 0)    
    mygrad = mygrad +360;
  if (mygrad > 360)    
    mygrad = mygrad -360;
#if 0    
  DSerial.print(mygrad);
  DSerial.println("  Grad"); 
#endif  
  return 0;
}

#ifdef USE_TEMP
//---------------------------------------------------------------
void getWeather()
{
#ifndef USE_M5STACK
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempC = sensor.getTemp();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
#else
  int tt = IMU.readTempData();
  
  const float _tempScale = 333.87f;
  const float _tempOffset = 21.0f;
  tempC = ((((float) tt) - _tempOffset)/_tempScale) + _tempOffset;
  //DSerial.print("Temp=" + String(tt)+ "   ");
  //DSerial.println(tempC,6);
  humidity=0;
#endif
}
#endif

//unsigned long f_rec=-1, ivalue=-1;  // -1 geht nicht !!!!!!

// alles in char besser ?
unsigned long get_value(String &p, int &pos, int len)
{
  unsigned long value=0;            
  char c;

  if (p.length()-pos < len)  len=p.length()-pos;
  len += pos;   // von Länge zu Index
  for (int m=pos;m<len;m++)
  {
    c=p.charAt(m);
    if (c>='0' and c<='9')
    {
      value= value*10 + c-'0';
      pos++;
    }
    else
      break;
  }
  return value;
}
