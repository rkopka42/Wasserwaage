/* http.h  Webserver handler
 * 
 */

#include "grafics.h"
#include "grafic_funcs.h"

bool getvaluesFromParams_(conf_t cfg[]);
void paint_main(String &message);

//#ifdef SHOW_KOMPASS
// set up JS code for painting the canvas for compass
void paint_compass(String &message);
 
void handleCalib() 
{
  bool        do_calib     =false;
  static bool calib_changed=false;  // erst nach Taste abspeichern
  
  String calib = server.arg("Calib");
  // Button Wert verwenden
  if (calib == "Calib")
  {
    // do calib   
    DSerial.println("calib");
    confvalues.fcp=fp;  confvalues.fcr=fr;    
    do_calib=true;
    calib_changed=true; // war für getrenntes oder späteres Schreiben erst nach Loslassen der Taste
    // Flag und dann schreiben
    if (calib_changed)
    {
      // write  
      DSerial.println(" C Calib");
      String output;
      write_ini(CFGFILE_NAME , config, sizeof(config)/sizeof(conf_t), OUTPUT_FILE, output);
      DSerial.println("New ini:\n" + output);
      calib_changed=false;
    }
  }

  String werte = "Pitch: " + String(fp_corr, 2)+ "<br>Roll: " + String(fr_corr, 2);
  // add calib data
  
  String message =R"=====(
    <html> <head>   <TITLE>Wasserwaage-Calib</TITLE>
    </head>
    <body>  
  <form action="/">
    <br>
    <input class="button" type="submit" name="Back" value="Back" style="height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;" formaction="/">
    <br>
    <br>
    <br>
    <input class="button" type="submit" name="Calib" value="Calib" style="height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;" formaction="/calib.html" 
      onclick="return confirm('Are you sure you want to calibrate?')">
  </form>
    <br>
    <br>
    STELLVERTRETER
</body>
</html>
  )=====";

  message.replace("STELLVERTRETER", werte);
  server.send(200, "text/html", message);
  DSerial.println("Root 200");
}

//String option_string;
void make_options(String &s, int selected)
{
  s="";
  for (int n=-3;n<=3;n++)
  {
    if (n!=0) 
      s+= "<option value=\"" + String(n) + "\" " + (n==selected ? "selected" : "") + ">" + String(n) + "</option>\n";
  }
}
              
void handleConfig() 
{
  bool         do_config     =false;
  static bool  config_changed=false;  // erst nach Taste abspeichern
  bool         is_save=false;
  confvalues_t confvalues_old;
    
  String config_ = server.arg("Config");

  if (config_ == "SetDefault")
  {
    DSerial.print(" SetDefaultValues ");
    //write_logfile("Set Default Values");
    read_defaults(config, sizeof(config)/sizeof(conf_t));
  }

  confvalues_old = confvalues;    // alte Werte merken, falls was nicht stimmt
  
  // Button Wert verwenden
  if (config_ == "Save")
  {
    DSerial.println("config");

    is_save=getvaluesFromParams_(config); // sync server version

    // man könnte die c1... auch mit testen für is_save, aber das müßte vorher schon erfüllt sein
    // get values - these are not handled by cfg functions
    String c1 = server.arg("c1");  
    if (c1 != "")
    {
      int c1_num = c1.toInt();
      if (c1_num>=-3 and c1_num<=3)  // dürfte nicht anders sein
      {
        confvalues.achse0 = c1_num;
      }
    }

    String c2 = server.arg("c2");  
    if (c2 != "")
    {
      int c2_num = c2.toInt();
      if (c2_num>=-3 and c2_num<=3)  // dürfte nicht anders sein
      {
        confvalues.achse1 = c2_num;
      }
    }

    String c3 = server.arg("c3");  
    if (c3 != "")
    {
      int c3_num = c3.toInt();
      if (c3_num>=-3 and c3_num<=3)  // dürfte nicht anders sein
      {
        confvalues.achse2 = c3_num;
      }        
    } 
  }

  if (!is_save)
    confvalues = confvalues_old;    // alte Werte zurück
  else
  {
    // Differenz ?
    if (memcmp(&confvalues, &confvalues_old, sizeof(confvalues)))
    {
      // write  
      DSerial.println(" Differenz ");
      String output;
      write_ini(CFGFILE_NAME , config, sizeof(config)/sizeof(conf_t), OUTPUT_FILE, output);
      DSerial.println("New ini:\n" + output);
    }
  }
   
  String message =R"=====(
    <html> <head>   <TITLE>Wasserwaage-Config</TITLE>
    </head>
    <body>  
<form action="/">
    <input class="button" type="submit" name="Back" value="Back" style="height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;" formaction="/">     
    <input class="button" type="submit" name="Config" value="Save" style="height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;" formaction="/config.html" 
      onclick="return confirm('Are you sure you want to save new config ?')">  
    <input class="button" type="submit" name="Config" value="SetDefault" style="height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;" formaction="/config.html">     
    <br>
<table>
    <tr>
    <td>X-Achse</td><td><select name="c1">
              OPTIONS1
            </select></td>
    </tr>
    <tr>                  
    <td>Y-Achse</td><td><select name="c2">
              OPTIONS2
            </select><td>      
    </tr>
    <tr>
    <td>Z-Achse</td><td><select name="c3">
              OPTIONS3
            </select><td>      
    </tr>
    CONFIGVALUES_
   </table>
 </form>
    <br>
    <br>
    STELLVERTRETER
</body>
</html>
  )=====";

  String stemp;
  make_options(stemp, confvalues.achse0);
  message.replace("OPTIONS1", stemp);
  make_options(stemp, confvalues.achse1);
  message.replace("OPTIONS2", stemp);
  make_options(stemp, confvalues.achse2);
  message.replace("OPTIONS3", stemp);
  message.replace("STELLVERTRETER", "");  // empty at the moment

  String output;
  write_ini("" , config, sizeof(config)/sizeof(conf_t), OUTPUT_FORM, output);
  message.replace("CONFIGVALUES_", output);

  DSerial.println(" ConfigText:");
  DSerial.println(output);
  write_ini("" , config, sizeof(config)/sizeof(conf_t), OUTPUT_SERIAL, output);
    
  server.send(200, "text/html", message);
  DSerial.println("Root 200");
}

// return JS code for main canvas
void handleMain() 
{
  String message;  
  paint_main(message);   // insert JS function 
  server.send(200, "application/javascript", message);
}

void handleRoot() 
{
  DSerial.println(" handle root");

  //DSerial.println(server.client().remoteIP().toString()); // -> bestimmen, wer da was anfragt. server an Funktion übergeben
  
  do_calib=false;
   
  String cont = server.arg("Fast");
  // Button Wert verwenden
  if (cont == "Fast" and modeT != MODE_FAST)  // ohne DPLUS ein Umweg, der aber dann zum richtigen Mode führt
  {
    modeT = MODE_FAST;
    last_modeT_change = time(NULL);
    DSerial.print(" FAST ");
  }
  else if (cont == "Slow" and modeT == MODE_FAST)
  {
    modeT = MODE_SLOW;
    last_modeT_change = time(NULL);
    DSerial.print(" SLOW ");
  }
  
  String message1 =R"=====(
  <html> 
  <head><TITLE>Wasserwaage</TITLE></head>
  <body onload="loadJS();">  
  
   <div class="wrapper" style="margin: 0 auto; width: 700px; position: absolute;">

    <canvas id="myCanvas" align="left" width="700" height="390" style="border:1px solid #d3d3d3; position: absolute; top:0; left:0;"> Your browser does not support the canvas element.   </canvas>
   </div>
   <div class="controls" style="position: absolute; top:0; left:720;">

    <form style="height: 100px; width: 200px;" align="right" action="/">
    <br>
  )====="; 

  String button_text;
  String value_text;

  if (modeT == MODE_FAST)
    button_text = "<input class=\"button\" type=\"submit\" name=\"Fast\" id=\"Fast\" value=\"Slow\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\">";
  else  
    button_text = "<input class=\"button\" type=\"submit\" name=\"Fast\" id=\"Fast\" value=\"Fast\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\">";
    

  button_text +="<br>\n"  "<br>\n"
                  "<input class=\"button\" type=\"submit\" name=\"Config\" value=\"Config\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" formaction=\"/config.html\">\n";
  button_text +="&ensp;"
                  "<input class=\"button\" type=\"submit\" name=\"Calib\" value=\"Calib\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" formaction=\"/calib.html\">\n";
#ifdef SHOW_KOMPASS
  button_text += "<br>\n"  "<br>\n"
                 "<input class=\"button\" type=\"submit\" name=\"compass\" value=\"Compass\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" formaction=\"/compass.html\">\n";
#endif
  value_text = "dummy"; // Test 
  value_text = "<div id=\"value_text\"> " + value_text + "</div>\n";
  
  String message2 =R"=====(       
  </form>
  </div>
  <script>  

var reloadid;
var but;

// mit callback
"use strict";
// src: https://stackoverflow.com/a/28002292
const getScript1 = (source, callback) => {
    var script = document.createElement('script');
    var prior  = document.getElementsByTagName('script')[0];
    script.async = 1;
    script.onload = script.onreadystatechange = (_, isAbort) => {
        if (isAbort || !script.readyState || /loaded|complete/.test(script.readyState)) {
            script.onload = script.onreadystatechange = null;
            script = undefined;
            if (!isAbort) if (callback) callback();
        }
    };
    script.src = source;
    prior.parentNode.insertBefore(script, prior);
};

function loadJS()
{
var url = "/main.js";
getScript1(url, function(){ paintmain();});
}
  )====="; 

  if (modeT == MODE_FAST)
    message2+="reloadid = setInterval(loadJS,1000);\n";
  else
    message2+="reloadid = setInterval(loadJS,5000);\n";
    
  String message4 = " </script>\n";  
  String end_text ="</body>\n </html>";

  server.send(200, "text/html", message1 + button_text + value_text +   message2 + message4 + end_text);

          DSerial.println("Root 200");
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
}

#ifdef SHOW_KOMPASS

// return JS code for main canvas
void handleJSCompass() 
{
  String message;

  if (do_calib)
  {
    if (CmaxX < comp_x)  CmaxX = comp_x;
    if (CminX > comp_x)  CminX = comp_x;
    if (CmaxY < comp_y)  CmaxY = comp_y;
    if (CminY > comp_y)  CminY = comp_y;
  }
  
  paint_compass(message);   // insert JS function 

  server.send(200, "application/javascript", message);
}

void handleCompass() 
{
  bool config_changed=false; 
  
  String cont = server.arg("Fast");
  // Button Wert verwenden
  if (cont == "Fast" and modeT != MODE_FAST)
  {
    modeT = MODE_FAST;
    last_modeT_change = time(NULL);
  }
  else if (cont == "Slow" and modeT == MODE_FAST)
  {
    modeT = MODE_SLOW;
    last_modeT_change = time(NULL);
  }
    
  String calib = server.arg("Calib");
  // Button Wert verwenden
  //DSerial.print(calib);
  //DSerial.print(" - ");
  //DSerial.println(do_calib);
  if (calib == "Calib" and not do_calib)
  {
    // bool is_calib   if not do_calib -> Button ändern, Startwerte  do_calib=true
    // irgendwas um beim ersten Mal die Werte auf Min Max zu setzen
    do_calib  =true;
    modeT = MODE_FAST;
    CmaxX=0; CminX=16000; CmaxY=0; CminY=16000;
  }
  else if (calib == "SaveCalib" and do_calib)
  {
    do_calib  =false;
    modeT = MODE_SLOW;
    // save calib Werte - oder noch eine Box wie bei config !?

    DSerial.println("config");
    confvalues.maxX=CmaxX; confvalues.minX=CminX; confvalues.maxY=CmaxY; confvalues.minY=CminY;

    // veränderte Werte checken ? Aber das macht das EEPROM Schreiben normalerweise selber
    config_changed=true; // war für getrenntes oder späteres Schreiben erst nach Loslassen der Taste
    // Flag und dann schreiben
    if (config_changed)
    {
      // write  
      DSerial.println(" C Calib");
      String output;
      write_ini(CFGFILE_NAME , config,  sizeof(config)/sizeof(conf_t), OUTPUT_FILE, output);
      DSerial.println("New ini:\n" + output);
      config_changed=false;
    }    
  }
  else if (calib == "AbortCalib" and do_calib)
  {
    do_calib  =false;
    modeT = MODE_SLOW;
  }
  
  if (do_calib)
  {
    if (CmaxX < comp_x)  CmaxX = comp_x;
    if (CminX > comp_x)  CminX = comp_x;
    if (CmaxY < comp_y)  CmaxY = comp_y;
    if (CminY > comp_y)  CminY = comp_y;
  }
  
  String s1;
  
  String message1 =R"=====(
  <html> 
  <head><TITLE>Kompass</TITLE></head>
  <body onload="loadJS();">  
   <div class="wrapper" style="margin: 0 auto; width: 700px; position: absolute;">

    <canvas id="myCanvas" align="left" width="700" height="390" style="border:1px solid #d3d3d3; position: absolute; top:0; left:0;"> Your browser does not support the canvas element.   </canvas>
   </div>
   <div class="controls" style="position: absolute; top:0; left:720;">

    <form style="height: 100px; width: 200px;" align="right" action="/compass.html">
    <br>
  )====="; 

  String button_text;

  if (modeT == MODE_FAST)
    button_text = "<input class=\"button\" type=\"submit\" name=\"Fast\" id=\"Fast\" value=\"Slow\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\">";
  else  
    button_text = "<input class=\"button\" type=\"submit\" name=\"Fast\" id=\"Fast\" value=\"Fast\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\">";
     
  button_text += "<br><br><input class=\"button\" type=\"submit\" name=\"home\" value=\"Home\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" formaction=\"/\">\n";
  button_text +="<br>\n" "<br>\n";
  
  if (do_calib)
  {
    button_text += "<input class=\"button\" type=\"submit\" name=\"Calib\" value=\"SaveCalib\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" "
                   //"onclick=\"return confirm('Are you sure you want to save new config ?')\""
                   ">\n";
    button_text += "&ensp;<input class=\"button\" type=\"submit\" name=\"Calib\" value=\"AbortCalib\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" "
                   ">\n";
  }
  else
    button_text += "<input class=\"button\" type=\"submit\" name=\"Calib\" value=\"Calib\" style=\"height: 60px; width: 90px; border: 3px solid black; border-radius: 10px;\" formaction=\"/compass.html\">\n";

  String value_text = "dummy"; // Test
  
  value_text = "<div id=\"value_text\"> " + value_text + "</div>\n";
  
  String message2 =R"=====(       
  </form>
  </div>
  <script>  

  var reloadid;
  var but;
  
// mit callback
"use strict";
// src: https://stackoverflow.com/a/28002292
const getScript1 = (source, callback) => {
    var script = document.createElement('script');
    var prior = document.getElementsByTagName('script')[0];
    script.async = 1;
    script.onload = script.onreadystatechange = (_, isAbort) => {
        if (isAbort || !script.readyState || /loaded|complete/.test(script.readyState)) {
            script.onload = script.onreadystatechange = null;
            script = undefined;
            if (!isAbort) if (callback) callback();
        }
    };
    script.src = source;
    prior.parentNode.insertBefore(script, prior);
};

function loadJS()
{
var url = "/compass.js";
getScript1(url, function(){ paintcompass();});
}
  )====="; 

  if (modeT == MODE_FAST)
    message2+="reloadid = setInterval(loadJS,1000);\n";
  else
    message2+="reloadid = setInterval(loadJS,5000);\n";
    
  String message4 = " </script>\n";  
  String end_text ="</body>\n </html>";                    

  server.send(200, "text/html", message1 + button_text + value_text + message2 + message4 + end_text);
    
          DSerial.println("Root 200");
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
}
#endif


// hier für den normalen Server

// only the correct ones are checked, but all need to be there !?  Or just skip the hidden ? or flag for taking only what's there and then save is only true if one parameter came.
// Hidden values are not changed in the form and don't need to be set, so they are missing in parameters  -> is_save = irgendwas geändert !
// what about DONTUSE ??? 
bool getvaluesFromParams_(conf_t cfg[])   // server ist global
{
  //AsyncWebParameter* p ;                                // Points to parameter structure
  static String      argument ;                         // Next argument in command
  static String      value ;                            // Value of an argument
  int                n;
  //String             pre_text;
  int                conf_no = sizeof(cfg)/sizeof(conf_t);
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
        *(int*)(cfg[n].value) = value.toInt();
        is_save=true;
      }
      else if (cfg[n].typ == TYPE_STRING)
      {
        *(String*)(cfg[n].value) = value;
        is_save=true;
      }               
      else if (cfg[n].typ == TYPE_FLOAT)
      {
        *(float*)(cfg[n].value) = value.toFloat();
        //DSerial.print(argument + "=" + String(value) +"\n");
        is_save=true;
      }        
    }  
  } // for
  return is_save;
}

String farben[]= {
  "ctx.fillStyle = \"rgb(0,200,0)\";\n",
  "ctx.fillStyle = \"rgb(200,0,0)\";\n",
  "ctx.fillStyle = \"rgb(200,200,0)\";\n",
  "ctx.fillStyle = \"rgb(0,0,200)\";\n"
};


// Canvas 

//vertikales Rechteck nach oben
// xpos ypos gibt den unteren linken Punkt an
// bei line_value gibt es einen inversen Strich
//ctx.fillRect (30, 18, 30, 200);  // x y b h
void draw_value_v3(int xpos,int ypos, float value, float max_value, int max_height, int breite, float line_value, String &s)
{
    int16_t hoehe, strich;
    int color=0;
    bool too_much=false;
    
    if (value > max_value)  
    {
      value=max_value;  // evt. Kennung für Überlauf
      too_much=true;
    }
    
    hoehe  = value     /max_value * max_height;
    strich = line_value/max_value * max_height;

    s = "ctx.fillStyle = \"rgb(0,0,0)\";\n";
    s += "ctx.beginPath();\n";
    s += "ctx.moveTo(" + String(xpos-1+breite)              + " , " + String(ypos-strich+max_height)               + " );\n";
    s += "ctx.lineTo(" + String(xpos-1+DREIECK_KANTE+breite)+ " , " + String(ypos-strich-DREIECK_KANTE+max_height) + " );\n";
    s += "ctx.lineTo(" + String(xpos-1+DREIECK_KANTE+breite)+ " , " + String(ypos-strich+DREIECK_KANTE+max_height) + " );\n";
    s += "ctx.fill();\n";

    s+="ctx.fillRect (";
    s+= String(xpos) + ", ";
    s+= String(ypos+ max_height) + ", ";
    s+= String(breite) + ", ";
    s+= String(1) + "); \n";
    
    if (too_much)
    {
      s+="ctx.fillRect (";
      s+= String(xpos) + ", ";
      s+= String(ypos) + ", ";
      s+= String(breite) + ", ";
      s+= String(-10) + "); \n";
    
      s+="ctx.fillStyle = \"rgb(200,0,0)\";\n";
    }      
    else
    {    
      if (line_value>value)  s+="ctx.fillStyle = \"rgb(0,200,0)\";\n";
      else                   s+="ctx.fillStyle = \"rgb(200,200,0)\";\n";
    }

    s+="ctx.fillRect (";
    s+= String(xpos) + ", ";
    s+= String(ypos+max_height-hoehe) + ", ";
    s+= String(breite) + ", ";
    s+= String(hoehe) + "); \n";
    //DSerial.println(s);
}

// Erste Linie unsichtbar !!!
void draw_pic(String &s, xy *form, int punkte, int x, int y, float angle=0, float factor=1)
{ 
 int n;//=0;
 float lastx, lasty;
 float nextx, nexty;
 
 s="ctx.beginPath();\n";

 lastx = x + form[0].x; // Startpunkt + erster Vektor
 lasty = y + form[0].y;
 //s+= "ctx.moveTo(" + String(int(lastx)) +", "+ String(int(lasty)) +");\n";

 for (n=1; n<punkte; n++)
 {
    //DSerial.print("N="+String(n)+ " ");
    nextx = lastx + form[n].x*factor * cos(angle*PI/180) - form[n].y*factor * sin(angle*PI/180);
    nexty = lasty + form[n].x*factor * sin(angle*PI/180) + form[n].y*factor * cos(angle*PI/180);

    if (n<=1)
      s+= "ctx.moveTo(" + String(int(nextx)) +", "+ String(int(nexty)) +");\n";
    else
      s+= "ctx.lineTo(" + String(int(nextx)) +", "+ String(int(nexty)) +");\n";
    lastx = nextx;
    lasty = nexty;
 }
 s+="ctx.fill();\n";
}

#ifdef SHOW_KOMPASS
// Funktion mit x y radius, farbe (Rosette), winkel; zurück String  
String draw_compass(int16_t x, int16_t y, int16_t radius, String farbe, float winkel)
{
  String message5 = "var x=" + String(x) + "; var y=" + String(y) + "; var radius=" + String(radius) + "\n";
  message5 += "ctx.fillStyle = \"rgb(0,0,0)\";";
  message5 += "ctx.fillText(\"" + String(int(winkel*10)/10.0, 1)+ "\",x-38 ,y-180 );\n";  

  String message6="";
  for (int n=0;n<360;n+=20)
  {
    message6+= "ctx.beginPath();"
      "ctx.moveTo(x, y);"
      "ctx.arc(x, y, 120, ";
    message6+= String(n/180.0) + "* 3.1415, " + String((n+10)/180.0);
    message6+=   " * 3.1415, false); ctx.fillStyle =";
    message6+=  farbe + ";" ;
    message6+= "ctx.fill();\n";
  }

  message6+= "    ctx.beginPath();"
      "ctx.arc(x, y, radius, 0, 2 * 3.1415, false); "
      "ctx.lineWidth = 2;"
      "ctx.strokeStyle = '#003300';"
      "ctx.stroke();";

  message6 += "ctx.fillStyle = 'black';";   // 238, 250,
  message6 += "ctx.fillText('S',x-9,        y+radius+35);\n";
  message6 += "ctx.fillText('N',x-9,        y-radius-15);\n";
  message6 += "ctx.fillText('W',x-radius-40,y+10);\n";
  message6 += "ctx.fillText('O',x+radius+12, y+10);\n";
  
// Kreuz
  String message8  =
      "ctx.beginPath();"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x,y-radius-10);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x,y+radius+10);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x+radius+10,y);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x-radius-10,y);"
      "ctx.lineWidth = 1;"
      "ctx.strokeStyle = '#003300';"
      "ctx.stroke();"
      ;

  String bild;
  int farbe_ = BLAU;
  draw_pic(bild, pfeil, sizeof(pfeil)/sizeof(xy), x, y,  winkel, 1);  

  return (message5 + message6 + message8 + farben[farbe_] + bild);
}
#endif

// set up html code for values
void make_value_text(String &text)
{
  text ="<br>"  
                  "clients: " + String(nb) + "<br>";
#ifdef SHOW_KOMPASS                  
  text +=         "Grad: " + String(mygrad,0) + "<br>";
 if (do_calib)                  
 {
   text +=  "C: " + String(CmaxX) + ", " + String(CminX) + ", " + String(CmaxY) + ", " + String(CminY) + "<br>"
                    "C_: " + String(comp_x) + ", " + String(comp_y) +"<br>";
   return;
 }
#endif

#ifdef USE_TEMP                  
  text +=         "Temp: " + String(tempC,1) + "C<br>"
 #ifndef USE_M5STACK 
                  "Humid:" + String(humidity,1) + "%<br>"
 #endif
                  ;                  
#endif                             
  text +=          "Volt:"  + String(sensorValue/175.5, 2) + "V<br>";        

  if (modeT==MODE_NODPLUS or modeT==MODE_WLAN or modeT==MODE_NODPLUSFAST)
  {
    text +=  "<b>" + String( MODETIMEOUT - (time(NULL) - last_modeT_change) ) + "s bis Sleep</b>"; 
  }
}

// set up JS code for painting the canvas
void paint_main(String &message)
{
  DSerial.print(" paint_main ");
  
    message =R"=====(       
    function paintmain()
    {
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "30px Arial";
    ctx.fillStyle = "rgb(0,0,0)";
    ctx.clearRect(0, 0, canvas.width, canvas.height);    
  )====="; 

// ab hier flexibler Part der Canvas Befehle
  
// wie variabel soll man es machen ? Änderungen hier bedingen auch Änderungen bei den anderen Werten
// alles mit einem Faktor versehen faktorW faktorH ?

  message += "ctx.fillText(\"" + String(int(z1))+ "cm\",80 ,50 );\n";
  message +=       "ctx.fillText(\"" + String(int(z4))+ "cm\",540,50 );\n";
  message +=       "ctx.fillText(\"" + String(int(z2))+ "cm\",80 ,370);\n";
  message +=       "ctx.fillText(\"" + String(int(z3))+ "cm\",540,370);\n";
  
  if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK ) // test for OK
    message += "ctx.fillStyle = \"rgb(0,200,0)\";\nctx.fillText(\"OK\",335,30);\n";            

  String s1;
  // Balken zeichnen
  draw_value_v3(30 ,18 , z1, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(30 ,220 , z2, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(640 ,18 , z4, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(640 ,220 , z3, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;

// Winkel multiplizieren, aber auch begrenzen - gibts in der Realität nicht, aber trotzdem min max ?
  float fp_corr_;               // Wert für die Anzeige vergrößern und begrenzen
  if (int(fp_corr) >0)
  {
    fp_corr_ = min((float)int(fp_corr*5),(float)45.0);
  }
  else
  {
    fp_corr_ = max((float)int(fp_corr*5),(float)-45.0);
  }

  float fr_corr_;
  if (int(fr_corr) >0)
  {
    fr_corr_ = min((float)int(fr_corr*5),(float)45.0);
  }
  else
  {
    fr_corr_ = max((float)int(fr_corr*5),(float)-45.0);
  }
      
  message += "ctx.fillStyle = \"rgb(0,0,0)\";";
  message += "ctx.fillText(\"Pitch:" + String(int(fp_corr*10)/10.0, 1)+ "\",200 ,140 );\n";  
  message += "ctx.fillText(\"Roll:" + String(int( (fr_corr <-180 ? -1*(360+fr_corr) :-fr_corr) *10)/10.0, 1)+ "\",405 ,140 );\n";  
  //if (fr_corr <-180)
    //DSerial.print(-1*(360+fr_corr));

  String bild;
  int farbe = GRUEN;
  if      (abs(fp_corr) >=WINKEL_ROT)  farbe=ROT;
  else if (abs(fp_corr) >=WINKEL_GELB) farbe=GELB;
  draw_pic(bild, seite, sizeof(seite)/sizeof(xy), 238, 250,  (float)int(-fp_corr_), 1.3);
  message += farben[farbe] + bild ;

  farbe = GRUEN;
  if      (abs(fr_corr) >=WINKEL_ROT)  farbe=ROT;
  else if (abs(fr_corr) >=WINKEL_GELB) farbe=GELB;
  draw_pic(bild, back, sizeof(back)/sizeof(xy), 455, 243,  (float)int(-fr_corr_), 1.3);
  message += farben[farbe] + bild;  

  #ifdef USE_TEMP        
    getWeather(); // für WLAN Zugriff eigens aufrufen, ebenso für andere Zugriffe
  #endif

  String values;
  make_value_text(values);
  message += "document.getElementById(\"value_text\").innerHTML = \"" + values + "\";\n "; 

  message += reloadstring;  // Interval umstellen

  if (reloadstring!="")
  {
    if (modeT == MODE_FAST)
    {
      message += "but=document.getElementById(\"Fast\");\n but.value = \"Slow\";\n ";       
    }
    else
    {
      message += "but=document.getElementById(\"Fast\");\n but.value= \"Fast\";\n "; 
    } 
  }
  reloadstring="";
  message += "}\n";  
}

#ifdef SHOW_KOMPASS
// set up JS code for painting the canvas for compass
void paint_compass(String &message)
{
    message =R"=====(       
    function paintcompass()
    {
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "30px Arial";
    ctx.fillStyle = "rgb(0,0,0)";
    ctx.clearRect(0, 0, canvas.width, canvas.height);    
  )====="; 

  message += draw_compass(200, 220, 120, "'rgb(120,200,120)'", mygrad);

  String values;
  make_value_text(values);
  message += "document.getElementById(\"value_text\").innerHTML = \"" + values + "\";\n "; 
  
  message += reloadstring;  // Interval umstellen

  if (reloadstring!="")
  {
    if (modeT == MODE_FAST)
    {
      message += "but=document.getElementById(\"Fast\");\n but.value = \"Slow\";\n ";       
    }
    else
    {
      message += "but=document.getElementById(\"Fast\");\n but.value= \"Fast\";\n "; 
    } 
  }
  reloadstring="";
  
  message += "}\n";  
}
#endif


void handleGetInfo() 
{  
  String calib = server.arg("Calib");
  // Button Wert verwenden
  if (calib == "Calib")
  {
  }

  String message = "pitch=" + String(fp_corr, 2)+ "\nroll=" + String(fr_corr, 2)+ "\n";
  message += "clients=" + String(nb) + "\n";
#ifdef SHOW_KOMPASS                  
  message += "grad=" + String(mygrad,0) + "\n";
#endif

#ifdef USE_TEMP                  
  message += "temp=" + String(tempC,1) + "\n"
 #ifndef USE_M5STACK 
          "humid=" + String(humidity,1) + "\n"
 #endif
                  ;                  
#endif                             
  message += "volt="  + String(sensorValue/175.5, 2) + "\n";        
  message += "dplus=" + String(Dplus)+ "\n";
  if (modeT==MODE_NODPLUS or modeT==MODE_WLAN or modeT==MODE_NODPLUSFAST)
  {
    message +=  "tilsleep=" + String( MODETIMEOUT - (time(NULL) - last_modeT_change) ) + "\n"; 
  }
  
  server.send(200, "text/plain", message);
  DSerial.println("Root 200");
}
