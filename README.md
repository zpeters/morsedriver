# Morse Driver
A simple "driver" for an ardunio (Teensy) to emit morse code.

Timeouts are configureable in the source

There are various modes that can be switched between (see below)

## Output
### Morse Characters (in MORSE mode (default))
- . (period) "dit"
- \- (dash) "dah"
- / (slash) "division between letters"
- " " (space) "divison between words

### Ascii Characters (in ASCII mode)
- Alpha - A-Z
- Numbers - 0-9

## Commands (in COMMAND mode)
- .- Move Cursor Up
- -. Move Cursor Down
- .. Move Cursor Left
- -- Move Cursor Right

## Switching Modes
The "prosign" -..--- (XM or "Change Mode") will switch between three modes
1. "Morse Mode" (Default)
2. "ASCII Mode"
3. "Command Mode"


## Resources
- http://www.unit-conversion.info/texttools/morse-code/
- https://en.wikipedia.org/wiki/Morse_code
- https://morsecode.scphillips.com/translator.html
- https://en.wikipedia.org/wiki/Prosigns_for_Morse_code
- https://www.pjrc.com/teensy/