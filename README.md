# RD400

## Electronics Tray

In the [sketchup](sketchup/) folder, you can find the .stl files for 3d printing the electronics tray. The outlets for wires are modular, so we can swap them out for different types and volumes of wires.

The tray has a top-mounted window, on the lid, so the electronics can be seen inside.

The design for the PCB was crated in EasyEDA.

## The Arduino

The arduino is necessary, since the pushbuttons on the handlebar are momentary push buttons, rather than latching push buttons. Making a latching circuit may have been a trivial affair, but we wanted the ability to expand features in the future. An arduino allows for this kind of modularity.

Arduino code can be viewed in [src/main.ino](src/main.ino).

## NFC

### Recognized Tags

| Tag         | Description |
| ----------- | ----------- |
| 7A B3 2D 68 | FOB 1       |
| BA 60 22 68 | FOB 2       |
| 95 AB A0 EE | CARD 1      |

### RJ45 PIN MAPPING

| RJ45 | Arduino | MFRNC | Description    |
| ---- | ------- | ----- | -------------- |
| 1    | 10      | SDA   | SPI SS         |
| 2    | 15      | SCK   | SPI SCK        |
| 3    | 16      | MOSI  | SPI MOSI       |
| 4    | 14      | MISO  | SPI MISO       |
| 5    | -       | RQ    | *Unused*       |
| 6    | -       | GND   | *Ground*       |
| 7    | 9       | RST   | RST            |
| 8    | -       | Vcc   | 2.9V regulated |

### 3.3V Power

The 3.3V for the NFC was actually 2.9V, since the MFRNC522 can run anywhere between 2.5V and 3.3V.

[3V regulation](res/fig1.png)

### Assembly

Electrical assembly notes are under the [res](res/) folder
