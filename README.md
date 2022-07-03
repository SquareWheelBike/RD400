# RD400

## Electronics Tray

In the [sketchup](sketchup/) folder, you can find the .stl files for 3d printing the electronics tray. The outlets for wires are modular, so we can swap them out for different types and volumes of wires.

The tray has a top-mounted window, on the lid, so the electronics can be seen inside.

The design for the PCB was crated in EasyEDA.

## The Arduino

The arduino is necessary, since the pushbuttons on the handlebar are momentary push buttons, rather than latching push buttons. Making a latching circuit may have been a trivial affair, but we wanted the ability to expand features in the future. An arduino allows for this kind of modularity.

Arduino code can be viewed in [src/main.ino](src/main.ino).

## NFC tags

| Tag | Description |
| ---- | ----------- |
| 7A B3 2D 68 | FOB 1 |
| BA 60 22 68 | FOB 2 |
| 95 AB A0 EE | CARD 1 |
