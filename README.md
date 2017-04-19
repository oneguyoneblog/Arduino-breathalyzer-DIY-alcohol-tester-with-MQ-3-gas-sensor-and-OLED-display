# Arduino-breathalyzer-DIY-alcohol-tester-with-MQ-3-gas-sensor-and-OLED-display
This is the code I use for a simple olcohol detector or "breathalYzer",  using an Arduino Uno, MQ-3 gas sensor, OLED display and a couple of LEDs.

Everything is connected like this:

MQ-3 -> Arduino:
- AD -> A0
- DD -> (not connected)
- GND -> GND
- VCC -> 5V

OLED -> Arduino:
- GND -> GND
- VCC -> 5V
- SCL -> D10
- SDA -> D9
- RES -> D13
- DC -> D11

LED -> Arduino
- 1 Green -> D6
- 2 Green -> D5
- 3 Yellow -> D4
- 4 Yellow -> D3
- 5 Red -> D2

See the blog post at https://oneguyoneblog.com/2017/04/12/arduino-breathalyzer-alcohol-tester-mq-3/
