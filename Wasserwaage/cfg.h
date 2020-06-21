/* cfg.h  Configuration and ini file
 * 
 */

#define TYPE_INT    1
#define TYPE_STRING 2
#define TYPE_FLOAT  3
#define TYPE_INT_HIDDEN    4
#define TYPE_STRING_HIDDEN 5
#define TYPE_FLOAT_HIDDEN  6
#define TYPE_DONT_USE      7  // Werte die im Feld stehen und abgespeichert werden, aber nicht verändert werden durch die Funktionen -> bei FORM weglassen, sonst schon anzeigen ???
#define TYPE_INT_OPTIONS   8   // dann sind die Optionen im addon -> noch unklar
#define TYPE_INT_DONT_USE  9  // nicht bei FORM, aber bei read defaults schon, daher Typ nötig

#define OUTPUT_SERIAL 0
#define OUTPUT_TEXT   1
#define OUTPUT_HTML   2
#define OUTPUT_FILE   3
#define OUTPUT_FORM   4

/*
typedef struct {
  int n;
  //String text;
  int value;
} option_t;
*/

typedef struct {
    void *value;    // Zeiger auf die Variable
    float min;      // float used as int for the other types
    float max;      // max string length / highest value
    String def;     // kann auch Zahlen enthalten
    int typ;        // type of value TYPE_...
    String name;    // für File
    String text;    // Hilfetext für Eingabe
    void *addon;    // Zeiger aufs Hilfsinformationen z.B. für Auswahlfelder
}  conf_t;

typedef struct {
  float angle;   // Rotation um Z Achse UZS  -> unklar ob wirklich alles abgedeckt, aber grundsätzlich sieht es gut aus
  float laenge;  // muß kein float sein, aber Berechnungen sind immer in float(==double)
  float breite1;
  float breite2; // Hinterachse hat mehr Spurbreite(?)
  float fcp;     // Kalibrierwerte -> Hidden ?
  float fcr;
  int    achse0;  // Index gibt Achsnutzung an xyz   Wert gibt Achse des Sensors an 1 ist die x Achse nach Platinenaufdruck
  int    achse1;
  int    achse2;
                      // Vorzeichen drehen die Richtung um, Index nicht ab 0, da dort kein Vorzeichen geht !
  int maxX;     // Grenzwerte der Kompaßwerte aus der Kalibrierung
  int minX;
  int maxY;
  int minY;                      
  float  angle2;   // Rotation um Z Achse UZS  -> unklar ob wirklich alles abgedeckt, aber grundsätzlich sieht es gut aus  
     
  String ssid;    // oder char[] ?
  String password;
  String host;    // unklar
}  confvalues_t;

confvalues_t confvalues;

conf_t config[] =
{
    {&confvalues.angle, -360, 360, "0",TYPE_FLOAT, "angle", "Gyro Rotation um Z-Achse in Grad",NULL},
    {&confvalues.laenge, 100, 500, "381",TYPE_FLOAT, "laenge", "Abstand der Achsen (cm)",NULL},    // INT würde reichen, aber gerechnet wird immer mit Float
    {&confvalues.breite1, 100, 300, "184",TYPE_FLOAT, "breite1", "Spurbreite Vorderachse (cm)",NULL},
    {&confvalues.breite2, 100, 300, "202",TYPE_FLOAT, "breite2", "Spurbreite Hinterachse (cm)",NULL},
    {&confvalues.fcp, -2000, 2000, "0",TYPE_FLOAT_HIDDEN, "fcp", "Kalibrierwert fcp fuer Gyro",NULL}, // Wertebereich ? Aber HIDDEN egal
    {&confvalues.fcr, -2000, 2000, "0",TYPE_FLOAT_HIDDEN, "fcr", "Kalibrierwert fcr fuer Gyro",NULL},

    // Gyro Ausrichtung in den Achsen -3 ... +3 (ohne 0) - jede verfügbare Achse in beiden Richtungen
    {&confvalues.achse0,  -3, 3, "-2", TYPE_INT_DONT_USE, "achse0",  "Ausrichtung Achse0",NULL},   // erstmal händisch machen, ist sonst zu speziell
    {&confvalues.achse1,  -3, 3, "1",  TYPE_INT_DONT_USE, "achse1",  "Ausrichtung Achse1",NULL},   // ginge auch einfach als INT
    {&confvalues.achse2,  -3, 3, "3",  TYPE_INT_DONT_USE, "achse2",  "Ausrichtung Achse2",NULL},   // 

    // Kompaß - Hidden = wird nur durch SW gesetzt
    {&confvalues.maxX,  -6000, 6000, "2124", TYPE_INT_HIDDEN, "maxX",  "Kalibrierwerte max.Wert X-Achse",NULL},   // Test  2124,1947,2133,1944,
    {&confvalues.minX,  -6000, 6000, "1947", TYPE_INT_HIDDEN, "minX",  "Kalibrierwerte min.Wert X-Achse",NULL},   
    {&confvalues.maxY,  -6000, 6000, "2133", TYPE_INT_HIDDEN, "maxY",  "Kalibrierwerte max.Wert Y-Achse",NULL},   
    {&confvalues.minY,  -6000, 6000, "1944", TYPE_INT_HIDDEN, "minY",  "Kalibrierwerte min.Wert Y-Achse",NULL},   
    {&confvalues.angle2, -360, 360, "0",TYPE_FLOAT, "angle2", "Kompass Drehung um die Z-Achse in Grad",NULL},

    {&confvalues.ssid,       8, 20,  "Wasserwaage", TYPE_STRING, "ssid", "Accesspoint SSID",NULL},       // min Length in der SW testen ?  Fehlermeldungen ? Oder egal, man muß wissen, was man tut ?
    {&confvalues.password,   8, 20,  "ww00aa11", TYPE_STRING, "password", "Accesspoint password",NULL},  // ändern ?
    {&confvalues.host,       8, 20,  "hostIP",   TYPE_STRING, "host", "host for Client Requests",NULL}
};

// vordere Spaces überspringen, bei /n oder # abbrechen
void trim(char *src)
{
    int i, len;
    len = strlen(src);

    for(i = 0; i < len; i++) 
    {
        if(src[i] == ' ') 
        {
            continue;
        }
        if(src[i] == '\n' || src[i] == '#') 
        {
            break;
        }
        //printf("%c", src[i]); // prints: host=1.2.3.4
    }
}

// nur eine Zeile (in buf)anschauen und name und val zurück, den Rest in der Schleife
// hier auch den Index suchen -> return
int parse_line(conf_t cf[], char *name, char *val, char *buf)
{
    if (cf == NULL || val == NULL || name==NULL)
        return -1;

    char varname[100];
    char value[100];
    //const char* sep = "=\n"; // get also rid of newlines
    const char* sep = "=\n\r"; // get also rid of newlines
    
    char *token;
    int n;
    
    token = strtok(buf, sep);
    strncpy(varname, token, sizeof(varname));
    varname[sizeof(varname) - 1] = 0; // making sure that varname is C-String
    trim(varname);
    token = strtok(NULL, sep);
    if (token == NULL)
    {
        // line not in format var=val
        return -1;
    }

    strncpy(value, token, sizeof value);
    value[sizeof(varname) - 1] = 0;

  // varname und value 
    trim(value);
    strcpy(name, varname);
    strcpy(val, value);
    
    //Serial.print(" sizeof(config)= " + String(sizeof(config)));
    
    for (n=0;n<sizeof(config)/sizeof(conf_t);n++)
    {
      if (strcmp(varname, cf[n].name.c_str()) == 0)
      {
        return n;
      }
    }
    return -1;
}

// aus Struct die Defaultwerte nach cfg suchen und in cv eintragen
// immer beim Start, falls ini File unvollständig
int read_defaults(conf_t cfg[])
{
  int n;
   for (n=0;n<sizeof(config)/sizeof(conf_t);n++)
    {
      if (cfg[n].value != NULL)
      {
        if (cfg[n].typ == TYPE_INT or cfg[n].typ == TYPE_INT_HIDDEN or cfg[n].typ==TYPE_INT_DONT_USE)
        {
          *(int*)(cfg[n].value) = cfg[n].def.toInt();    
        }
        else if (cfg[n].typ == TYPE_STRING or cfg[n].typ == TYPE_STRING_HIDDEN)
        {
          *(String*)(cfg[n].value) = cfg[n].def;
        }
        else if (cfg[n].typ == TYPE_FLOAT or cfg[n].typ == TYPE_FLOAT_HIDDEN)
        {
          *(float*)(cfg[n].value) = cfg[n].def.toFloat();
        }
      }
    }
  return 0;
}

// aus filename die Werte nach cfg suchen und in cv eintragen
int read_ini(String filename, conf_t cfg[])
{
#if defined(ESP8266) || defined(ESP32)
   int linecnt = 0;
    char *file_name=CFGFILE_NAME;
    
    File file = SPIFFS.open(file_name, FILE_READ);
DSerial.println(file.size());
    // check if file is NULL, etc..

    char buffer[BUFSIZ];
    char varname[40], val[40];
    char *line;
    int i,ret,n;

    while(file.position() < file.size())
    {
      String dataBuff = file.readStringUntil('\n');
      DSerial.println(dataBuff);
      dataBuff.toCharArray(buffer, BUFSIZ);
 
      linecnt++;
      trim(buffer);
      if(buffer[0] == '#')
          continue;

      ret=parse_line(cfg, varname, val , buffer); // Werte der Zeile holen
      if (ret<0)
      {
          DSerial.printf("Error on line %d, ignoring.\n", linecnt);
          continue;
      }
      else
      {
        n=ret;
        if (cfg[n].typ == TYPE_INT or cfg[n].typ == TYPE_INT_HIDDEN or cfg[n].typ==TYPE_INT_DONT_USE)
        {
          *(int*)(cfg[n].value) = atoi(val);     
        }
        else if (cfg[n].typ == TYPE_STRING or cfg[n].typ == TYPE_STRING_HIDDEN)
        {
          *(String*)(cfg[n].value) = String(val);
        }               
        else if (cfg[n].typ == TYPE_FLOAT or cfg[n].typ == TYPE_FLOAT_HIDDEN)
        {
          *(float*)(cfg[n].value) = String(val).toFloat();
        }
      }
   }
   file.close();
#endif   
  return 0;
}

// in filename die Werte aus cv nach cfg eintragen
int write_ini(String filename, conf_t cfg[], int outputtype, String &output)
{
#if defined(ESP8266) || defined(ESP32)
 int n;
 String line,line2;
 File file;
 
  output="";
   for (n=0;n<sizeof(config)/sizeof(conf_t);n++)
    {
      if (cfg[n].value != NULL)
      {
        line="";
        if (cfg[n].typ == TYPE_INT or cfg[n].typ == TYPE_INT_HIDDEN or cfg[n].typ==TYPE_INT_DONT_USE)
        {
          line = String(*(int*)(cfg[n].value));    
          line2 = "<tr><td>" + cfg[n].text + " (" + String(cfg[n].min) + "..." + String(cfg[n].max) + ") </td><td> <input " + (cfg[n].typ == TYPE_INT_HIDDEN ? " disabled ":" ") + "type=\"number\" name=\"input" + String(n) + "\"" + "min=\"" + String(cfg[n].min) + "\" max=\""  + String(cfg[n].max) + "\" value=\"" + line + "\"> </td></tr>";
        }
        else if (cfg[n].typ == TYPE_STRING or cfg[n].typ == TYPE_STRING_HIDDEN)
        {
          line = *(String*)(cfg[n].value);              
          line2 =  "<tr><td>" + cfg[n].text + " (len max:" + String(cfg[n].max) + ") </td><td>  <input " + (cfg[n].typ == TYPE_STRING_HIDDEN ? " disabled ":" ") + "type=\"text\" name=\"input" + String(n) + "\"" + "\" maxlength=\""  + String(cfg[n].max) + "\" value=\"" + line + "\"> </td></tr>";
        }
        else if (cfg[n].typ == TYPE_FLOAT or cfg[n].typ == TYPE_FLOAT_HIDDEN)
        {
          line = String(*(float*)(cfg[n].value));   
          line2 = "<tr><td>" + cfg[n].text + " (" + String(cfg[n].min) + "..." + String(cfg[n].max) + ") </td><td> <input " + (cfg[n].typ == TYPE_FLOAT_HIDDEN ? " disabled ":" ") + " step=\"any\" type=\"number\" name=\"input" + String(n) + "\"" + "min=\"" + String(cfg[n].min) + "\" max=\""  + String(cfg[n].max) + "\" value=\"" + line + "\"> </td></tr>";
        }
        //  pattern=\"[0-9]+([\\,|\\.][0-9]+)?\"  bringt nichts
        // -> <table lang="en-US">  geht, dann wird der . akzeptiert  

        if (outputtype==OUTPUT_SERIAL)
          Serial.println(cfg[n].name + "=" + line);  
        else if (outputtype==OUTPUT_TEXT or outputtype==OUTPUT_FILE)
          output += cfg[n].name + "=" + line + "\n";
        else if (outputtype==OUTPUT_HTML)
           output += cfg[n].text + "=" +line + "<br>\n";
        else if (outputtype==OUTPUT_FORM and cfg[n].typ!=TYPE_DONT_USE and cfg[n].typ!=TYPE_INT_DONT_USE)        // FORM noch mit Tabelle ! Maße der Tabelle außen definieren
          output += line2 + "\n"; //"<br>\n";
        } //if NULL
    } // for
    if (outputtype==OUTPUT_FILE)          
    {
      File file = SPIFFS.open(filename, FILE_WRITE);
      if(!file)
      {
        DSerial.println("- failed to open file for writing");
      }
      else
      {  
        DSerial.println(F("File opened ok"));
        file.print(output);     
        DSerial.println(F("sucessfully written on SPIFFS"));
        file.close();
      }          
    }
#endif    
  return 0;  
}

// hier für den normalen Server

// only the correct ones are checked, but all need to be there !?  Or just skip the hidden ? or flag for taking only what's there and then save is only true if one parameter came.
// Hidden values are not changed in the form and don't need to be set, so they are missing in parameters  -> is_save = irgendwas geändert !
// what about DONTUSE ??? 
bool getvaluesFromParams_(void)   // server ist global
{
  //AsyncWebParameter* p ;                                // Points to parameter structure
  static String      argument ;                         // Next argument in command
  static String      value ;                            // Value of an argument
  int                n;
  //String             pre_text;
  int                conf_no = sizeof(config)/sizeof(conf_t);
  bool               is_save=false;
  
  for (n=0;n<conf_no;n++)       // input Werte durchgehen
  {
    argument = "input"+String(n);
    value = server.arg(argument);
    if (value!="")
    {
      //p = request->getParam("input"+String(n));
      //value = p->value() ;                                  // Get the specified value
      //argument = p->name() ;
      //value = 
    //  pre_text += argument + " = " + value +"<br>\n";

      DSerial.print(argument + " = " + value +"\n");

      if (config[n].typ == TYPE_INT)
      {
        *(int*)(config[n].value) = value.toInt();
        is_save=true;
      }
      else if (config[n].typ == TYPE_STRING)
      {
        *(String*)(config[n].value) = value;
        is_save=true;
      }               
      else if (config[n].typ == TYPE_FLOAT)
      {
        *(float*)(config[n].value) = value.toFloat();
        //DSerial.print(argument + "=" + String(value) +"\n");
        is_save=true;
      }        
    }  
  } // for
  return is_save;
}

/*
// nur für Tests
void write_test_cfg(void)
{
  File file = SPIFFS.open(CFGFILE_NAME, FILE_WRITE);
    if(!file)
    {
      DSerial.println("- failed to open file for writing");
    }
    else
    {  
      DSerial.println(F("File opened ok"));

      file.print("host=1234\n");
      file.print("port=42\n");
      file.print("time_kurz=31\n");
      file.print("ssid=WASSERSTOP\n");
        
      DSerial.println(F("sucessfully written on SPIFFS"));
      file.close();
  }
}
 */
