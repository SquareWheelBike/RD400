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

keith z