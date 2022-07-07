# RD Build-Off Notes

## Post-Build May 1, 2022

### Electronics Enclosure

- The arduino nano *has* to go. this thing keeps having problems. Either move to an embedded ESP for space, or a micro
- The left (output) side output needs to be shifted about an inch outwards
- The FETs need to be more stable. Active low needs to stick around thanks to the bars, but pulldowns and stuff should be prioritized
- On top of the buck converter, voltage protection is a MUST on the arduino. Even if it's just a zener diode stopping the vin from passing a threshold
- The threads need to be shrunk so that the arduino can stay down

### Wiring Harness

- 18awg is NOT enough for the main rail. move to at least 14
- I need a mounted fuse block and preferably a distribution block for power
- the lights and ignition should be fused separately, after the main fuse
- the motogadget M unit is probably the way to stay, NFC will be stinky difficult, unless I wanna put time and ESP resources into it

## May 2, 2022

### What actually failed

- As it turns out, logic-level MOSFETs do not have a very high V<sub>GS</sub>. As a reesult, I burned out the FET gate after extended exposure to 12V. Although V<sub>DS</sub> is 60V, we need to maintain a low V<sub>GS</sub>.
- Somehow, an 18awg wire burned, drawing 2.5 amps.
- The 18awg has been replaced by 12awg for the main battery wire
- The motogadget will not stay on, even with no load on the system. It MAY need a backing capacitor


keith z

## July 7, 2022

Final assembly day for Freedom Machine

### PCB notes

Suggestions for the next iteration of the PCB

- The buck converter +- leads are backwards
- The RT and LT inputs are touching for some reason, I think one of the traces is too close to a larger solder pad and they're bridging
- The RJ45 needs to have an actual port on the board, that works with SPI.
- An arduino micro might draw too much power with the NFC on it; next revision needs to see an ESP of some kind with a sleep function, and a hardware interrupt on the NFC reader, so a low power mode is available
- the FETs should have pulldown resistors on the gates; when the controller is uninitalized or missing the gate leaks
- use automotive fuses next time, and make sure the footprint actually matches the part being used
- Optocouplers are a must for inputs
- reverse polarity protection
- a secondary 3.3V rail for NFC

As a side, the NFC could mount to where the old regulator rectifier would have mounted
