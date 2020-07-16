# Wasserwaage
Elektronische Wasserwaage für Womo / Electronic spirit level for camper van
mit WLAN/Browser Ausgabe
![Bildschirmfoto](http://kopka.at/j/IMG_1477sm.jpg)
---


Ein Gyrosensor wird ausgewertet. Danach wird die Lage des Fahrzeugs errechnet, incl. der nötigen Unterlegkeile für die momentane Lage.
Außer einem mit Arduino programmierten ESP32-Modul wird fast nichts benötigt. Ein DCDC-Wandler für die Stromversorgung. 2 Widerstände für den D+ Eingang. Über I2C ein GyroModul. Nach Wunsch ebenfalls über I2C ein Kompaß und ein Temperaturmeßmodul. Eine LED.
![Platine](http://kopka.at/j/IMG_1515sm2.jpg)

Die Ausgabe erfolgt über einen WLAN AP und eine Website, in der mit Javascript Canvas das Bild gezeichnet wird. Die Aktualisierung der Seite erfolgt mit Javascript, das ein neues Canvas nachlädt und anzeigt. Auf einer eigenen Seite kann man die Einstellungen ändern und die Sensoren kalibirieren. Die Anzeige ist für meine Android Geräte beginnend mit 800-1000 Pixel Breite ausgelegt.

Eine frühere Version hat die Ausgabe auf ein TFT gemacht. Das wurde jetzt für ein M5Stack Modul hinzugefügt. Ebenso eine Anpassung des M5Stack Gyro Sensors. Man hat dann sowohl eine Ausgabe auf dem M5Stack Modul, als auch parallel über WLAN. Die TFT Grafik benutzte am Anfang sehr simple Pixelgrafiken, die die Drehung nur über wenige unterschiedliche Bilder darstellen. Eine noch frühere Version hat die Ausgabe auf einem Scharzweiß Display mit noch weniger Auflösung gemacht. Damals noch mit einem Arduino Nano.
Die TFT Grafik wird jetzt aus den Canvas Vektoren ermittelt. Damit sind die Bilder schöner und es gibt mehr Winkelauflösung. Auch Änderungen an der Grafik sind einfacher. Für M5 wurde die Aufteilung etwas geändert, da das TFT breiter ist als das frühere Display. Für das Polyfill habe ich eine JAVA Routine aus dem Netz adaptiert, was in C++ nicht sehr elegant ist. Außerdem ist die Zeichengeschwindigkeit nicht berauschend aber ausreichend. Es gibt auch Aufrufe für eine Vektordarstellung ohne Füllung.

Die verschiedenen Optionen werden in defines.h eingestellt. Eine Version für eine einfache Hardware mit TFT und AVR Arduino wäre wohl möglich mit genug defines, aber derzeit nicht unterstützt. Auch könnte dann der Speicher knapp werden, da die ESPs reichlich haben und nicht gespart wurde.

Ein Temperatursensor wird ausgelesen und die Werte auf der Hauptseite angezeigt. Ein Kompaßmodul wird für eine Anzeige auf einer eigenen Seite verwendet.
Temperatur und Gyro haben eigene Kalibrierungsmodi. Beide Sensore benötigen einige Grundeinstellungen bzgl. Fahrzeug und eine Kalibrierung in der Ebene (Gyro), bzw. durch einen 360° Kreis (Kompaß). Die Orientierung des Sensors und eine evt. Verdrehung müssen manuell eingestellt werden.

Der M5 Kompaß ist kaum brauchbar, da durch die Magnete im Gehäuse beeinträchtigt. Ein Kompaßmodul ist nur eingeschränkt nutzbar, da die Anzeige recht einfach ist und das Tablet ohne Umstellungen den Bildschirm nach einiger Zeit abschaltet. Der Temperatursensor muß vom Prozessor entfernt sein (Wärme). Da das Gerät während der Fahrt und bei mir während der Aufbau eingeschaltet ist, ständig in Betrieb ist, wird es recht warm. Andererseits will man ja sofort darauf zugreifen können.

Temperatursensor und Kompaß werden an den I2C Bus angeschlossen. Das D+ Signal wird über ADC1 Channel6 (ESP32) eingelesen und für das Abschalten benutzt. Mit Start des Motor wird das Gerät aktiv und nach dem Stop noch einige Zeit weiter aktiv bleiben. Danach wird es im Sleep warten. Ohne Motor kann man es mit einem Taster aufwecken, z.B. um Einstellungen zu ändern. Weitere Spannungen könnte man auch noch einlesen, da es genug ADC Eingänge gibt.

Die SW ist mit/für die Arduino IDE geschrieben mit der ESP32 Erweiterung und folgenden Bibliotheken:

#include <math.h> // Sinus...

#include <Wire.h> // Must include Wire library for I2C

#include "FS.h"

#include "SPIFFS.h"

#include <WebServer.h>

#include <M5Stack.h>

#include "utility/MPU9250.h"

#include <SparkFun_MMA8452Q.h> // Accelometer library

#include "SparkFun_Si7021_Breakout_Library.h"

Einige davon sind schon im ESP32 System enthalten. Andere nicht erwähnte gehören zum Standard Arduino.

Ich habe das Gerät in einem kleinen Gehäuse an der Wand des Womos fixiert, wo ich mit einem Kabel für Updates leicht herankomme. An Verbindungen braucht man nur die Versorgungsspannung und evt. noch D+. Eine LED zeigt blinkend den Betriebszustand. D.h. WLAN aktiv und Client eingeloggt. Mein Tablet ist auf automatische Verbindung eingestellt. Ich brauche dann nur noch einen Browser mit der passenden IP aufrufen, bzw. den entsprechenden Tab anwählen.

Die serielle Schnittstelle (ESP32 Serial2) war ursprünglich für ein weiteres eigenes Gerät gedacht, dessen Daten angezeigt werden sollten. Jetzt vielleicht für die Schnittstelle des Victron Solarreglers.

Die WLAN Reichweite ist erheblich. Von einem Strand aus über Wasser hatte ich noch aus über 100m Kontakt zum Gerät im Wohnmobil (Aluwände).

Die Grafik zeigt ein Alkovenwohnmobil (wie ich es habe). Wer etwas anderes haben will, muß selber mit den Canvasanweisungen rumbasteln. 
Das Bild ganz oben stammt von einer frühen Version, wo die Winkel noch nicht korrekt waren. Die Neigung beim Womobild wird übertrieben dargestellt, damit man sie besser erkennen kann. 1° würde sonst in der Auflösung untergehen.

----

Weitere Pläne:
- Temperatursensor auf Abstand (keine SW). Verkabelt ist er. Jetzt brauche ich noch ein sehr kleines Gehäuse (wohl im 3D-Druck).
- Schnittstelle (Webadresse AP) zum Auslesen von Werten (D+,Temp,Kompaß...) durch andere WLAN Geräte im Womo, die diese Infos brauchen, aber nicht selber ermitteln können/wollen. Dann muß man aber evt. am Sleep etwas ändern. Umgekehrt wäre auch die Darstellung weiterer Werte auf der GUI möglich, wenn die Geräte sich als Client einloggen.
- mehr ein Gimmick - am Kompaß ein Pfeil zum Astra Satelliten zur Ausrichtung.


