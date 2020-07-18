/* cfg.cpp  Configuration and ini file 
 *  allgemeiner Teil
 * 
 */

#include "cfg.h"

// vordere Spaces überspringen, bei /n oder # abbrechen
static void trim(char *src)
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
static int parse_line(conf_t cf[], int cfg_elements, char *name, char *val, char *buf)
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
  
  //DSerial.print(" sizeof(config)= " + String(sizeof(config)));
  
  for (n=0;n<cfg_elements;n++)
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
int read_defaults(conf_t cfg[], int cfg_elements)
{
  int n;
  for (n=0;n<cfg_elements;n++)
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
int read_ini(String filename, conf_t cfg[], int cfg_elements)
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

      ret=parse_line(cfg, cfg_elements, varname, val , buffer); // Werte der Zeile holen
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
int write_ini(String filename, conf_t cfg[], int cfg_elements, int outputtype, String &output)
{
#if defined(ESP8266) || defined(ESP32)

  int n;
  String line,line2;
  File file;
 
  output="";
  for (n=0;n<cfg_elements;n++)
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
      {
        DSerial.println(cfg[n].name + "=" + line);  
      }
      else if (outputtype==OUTPUT_TEXT or outputtype==OUTPUT_FILE)
      {
        output += cfg[n].name + "=" + line + "\n";
      }
      else if (outputtype==OUTPUT_HTML)
      {
         output += cfg[n].text + "=" +line + "<br>\n";
      }
      else if (outputtype==OUTPUT_FORM and cfg[n].typ!=TYPE_DONT_USE and cfg[n].typ!=TYPE_INT_DONT_USE)        // FORM noch mit Tabelle ! Maße der Tabelle außen definieren
      {
        output += line2 + "\n"; //"<br>\n";
      }
      else
      {
        //DSerial.println( "outputtype=" + String( outputtype) + "  not " + String(OUTPUT_FORM) );        
      }
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
