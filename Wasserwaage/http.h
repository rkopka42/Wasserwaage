/* http.h  Webserver handler
 * 
 */
 
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
      write_ini(CFGFILE_NAME , config, OUTPUT_FILE, output);
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
    read_defaults(config);
  }

  confvalues_old = confvalues;    // alte Werte merken, falls was nicht stimmt
  
  // Button Wert verwenden
  if (config_ == "Save")
  {
    DSerial.println("config");

    is_save=getvaluesFromParams_(); // sync server version

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
      write_ini(CFGFILE_NAME , config, OUTPUT_FILE, output);
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
  write_ini("" , config, OUTPUT_FORM, output);
  message.replace("CONFIGVALUES_", output);

  DSerial.println(" ConfigText:");
  DSerial.println(output);
  write_ini("" , config, OUTPUT_SERIAL, output);
    
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
      write_ini(CFGFILE_NAME , config, OUTPUT_FILE, output);
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
