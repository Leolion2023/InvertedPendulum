## InvertedPendulum
### Konzeptidee
Ein kleiner Roboter, der sich selbst balancieren kann, sodass er trotz nur zwei Rädern aufrecht stehen bleibt.
Die Motivation dahinter ist ein Schulprojekt, dessen Hauptfokus auf selbstregelnden Regelkreisen liegt. Dieser Roboter war dabei unsere Idee.

## Sprache
- Primäre Dokumentation (Englisch): [README.md](README.md)
- Deutsche Version: [README.de.md](README.de.md)

## Inhalt

1. [Hardware](#hardware)
    1. [Anforderungen](#anforderungen)
    2. [Pinbelegung](#pinbelegung)
    3. [Schaltplan](#schaltplan)
    4. [Breadboard](#breadboard)
2. [Software](#software)
    1. [Code](#code)
    2. [Regelkreis-Diagramm](#regelkreis-diagramm)
    3. [Programmablaufplan](#programmablaufplan)
3. [Betriebshinweise](#betriebshinweise)

## To Do

- [x] Ein CAD-Modell des Roboters erstellen
- [x] Code schreiben, damit der Roboter sich aufrecht hält
- [ ] Code verbessern, damit die Kalibrierung per Smartphone möglich ist
    - Per BLE mit einer App verbinden ODER
    - Einen Webserver im eigenen WLAN öffnen

# Hardware
## Anforderungen

- 2 x Getriebemotoren ~600 RPM
    - 2 x M3-Schrauben 7,3 mm --> Senkkopf (Flachkopf)¹
- ESP32
- BNO055 9DoF Sensor²
- H-Bridge (z. B. HW-095 L298N)
- CAD-3D-Druckteile ([siehe hier](TechDraw.pdf))³

**Hinweise:**
1. Die Schrauben sind vom verwendeten Getriebemotor abhängig.
2. Wir haben dieses selbst erstellte [Wiki](https://github.com/Leolion2023/BNO055) genutzt, weil die Bosch-Dokumentation schwer verständlich ist.
3. Falls es Probleme bei der Anzeige der technischen Zeichnung gibt, lade die Datei bitte herunter und öffne sie in einem modernen PDF-Viewer.

## Pinbelegung

| ESP32 Pin | Funktion | Zusatz |
|---|---|---|
| GPIO16 | H-Bridge IN1 | Verwendet für Motor 1 |
| GPIO17 | H-Bridge IN2 | Verwendet für Motor 1 |
| GPIO23 | H-Bridge ENA | Verwendet für Motor 1 |
| GPIO18 | H-Bridge IN3 | Verwendet für Motor 2 |
| GPIO19 | H-Bridge IN4 | Verwendet für Motor 2 |
| GPIO33 | H-Bridge ENB | Verwendet für Motor 2 |
| GPIO21 | BNO055 SDA | |
| GPIO22 | BNO055 SCL | |
|  3.3V  | BNO055 VIN | |
|   GND  | BNO055 GND | |
|   GND  | BNO055 ADD | |
|  ---   | BNO055 INT | -nicht verwendet- |
|  ---   | BNO055 RST | -nicht verwendet- |
|  ---   | BNO055 BOOT | -nicht verwendet- |

## Schaltplan
![Image](assets/Schematic.png)

## Breadboard
![Image](assets/BreadboardView.png)

# Software

## Code
Der Hauptcode befindet sich in [src/src/main.cpp](src/src/main.cpp).
Zum Flashen des ESP32 haben wir PlatformIO genutzt. Passe es bei Bedarf an deine Anforderungen an.

## Regelkreis-Diagramm

![Image](assets/ControlLoopDiagram.png)

## Programmablaufplan
Derzeit leider nur auf Deutsch verfügbar.

![Image](assets/ProgramFlowchart.png)

**Beide Diagramme wurden mit [excalidraw.com](https://excalidraw.com) erstellt.**

# Betriebshinweise

### Elektrischer Start

Der Roboter benötigt zwei getrennte Spannungsversorgungen: eine Logikspannung und eine Motorspannung. Die Logikspannung kann über den USB-C-Anschluss des ESP32, den VIN-Pin mit 3.3V-5V oder den 3.3V-Pin bereitgestellt werden. Die Motorspannung muss direkt an der H-Bridge am 12V- und GND-Anschluss angeschlossen werden.
Da du die PID-Werte wahrscheinlich kalibrieren musst, ist es am einfachsten, den ESP32 mit einem Computer zu verbinden, auf dem entweder PlatformIO oder die Arduino IDE installiert ist.

### Code-Anpassungen

Es gibt einige einfache Anpassungen, die vorgenommen werden müssen. Ganz oben stehen die drei PID-Konstanten, die auf den Roboter abgestimmt werden müssen. Danach gilt: Das Constraint begrenzt den Maximalwert des PID-Ausgangs. Typischerweise ist 255.0 sinnvoll. Dadurch drehen die Motoren nicht zu schnell. Die Variable `inverted` steuert die Drehrichtung der Motoren. Falls der Roboter in die falsche Richtung regelt, ändere sie auf -1.0 oder 1.0.

### Hardware-Informationen

Zum Bau des Roboters kannst du einfach die technischen Zeichnungen verwenden (diese sind aktuell noch in Arbeit). Das Einzige, was getestet werden sollte, ist der Offset in den FreeCAD-Dateien. Damit wird der Versatz der Slider angepasst und er kann genutzt werden, wenn Verbindungen zu locker sind.
