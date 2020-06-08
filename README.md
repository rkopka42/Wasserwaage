# Wasserwaage
Elektronische Wasserwaage für Womo / Electronic spirit level for camper van
mit WLAN/Browser Ausgabe

Ein Gyrosensor wird ausgewertet. Danach wird die Lage des Fahrzeugs errechnet, incl. der nötigen Unterlegkeile für die momentane Lage.

Die Ausgabe erfolgt über einen WLAN AP und eine Website, in der mit Javascript Canvas das Bild gezeichnet wird. Die Aktualisierung der Seite erfolgt mit Javascript, das ein neues Canvas nachlädt und anzeigt. Auf einer eigenen Seite kann man die Einstellungen ändern und die Sensoren kalibirieren. Die Anzeige ist für meine Android Geräte beginnend mit 800-1000 Pixel Breite ausgelegt.

Eine frühere Version hat die Ausgabe auf ein TFT gemacht. Das wurde jetzt für ein M5Stack Modul hinzugefügt. Ebenso eine Anpassung des M5Stack Gyro Sensors. Man hat dann sowohl eine Ausgabe auf dem M5Stack Modul, als auch über WLAN. Die TFT Grafik benutzt sehr simple Pixelgrafiken, die die Drehung nur über wenige unterschiedliche Bilder darstellen.

Die verschiedenen Optionen werden in defines.h eingestellt.

Ein Temperatursensor wird ausgelesen und die Werte auf der Hauptseite angezeigt. Ein Kompaßmodul wird für eine Anzeige auf einer eigenen Seite verwendet.
Temperatur und Gyro haben eigene Kalibrierungsmodi. Beide Sensore benötigen einige Grundeinstellungen bzgl. Fahrzeug und eine Kalibrierung in der Ebene (Gyro), bzw. durch einen 360° Kreis (Kompaß). Die Orientierung des Sensors und eine evt. Verdrehung müssen manuell eingestellt werden.

Der M5 Kompaß ist kaum brauchbar, da durch die Magnete im Gehäuse beeinträchtigt. Ein Kompaßmodul ist nur eingeschränkt nutzbar, da die Anzeige recht einfach ist und das Tablet ohne Umstellungen den Bildschirm nach einiger Zeit abschaltet. Der Temperatursensor muß vom Prozessor entfernt sein (Wärme). Da das Gerät während der Fahrt und bei mir während der Aufbau eingeschaltet ist, ständig in Betrieb ist, wird es recht warm. Andererseits will man ja sofort darauf zugreifen können.

Temperatursensor und Kompaß werden an den I2C Bus angeschlossen. Die Betriebsspannung wird über ADC1 Channel6 (ESP32) gelesen. Eigentlich sollte das D+ Signal eingelesen werden und für das Abschalten benutzt werden, ist aber noch nicht implementiert. Mit Start des Motor sollte das Gerät aktiv werden und nach dem Stop noch einige Zeit weiter aktiv bleiben. Danach würde es im Sleep warten.

Die SW ist mit/für die Arduino IDE geschrieben mit der ESP32 Erweiterung und folgenden Bibliotheken:
...

Ich habe das Gerät in einem kleinen Gehäuse an der Wand des Womos fixiert, wo ich mit einem Kabel für Updates leicht herankomme. An Verbindungen braucht man nur die Versorgungsspannung und evt. noch D+. Eine LED zeigt blinkend den Betriebszustand. D.h. WLAN aktiv und Client eingeloggt. Mein Tablet ist auf automatische Verbindung eingestellt. Ich brauche dann nur noch einen Browser mit der passenden IP aufrufen, bzw. den entsprechenden Tab anwählen.
