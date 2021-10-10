# Get start RFID with Arduino

## First

+ Arduino UNO v3
+ RFID RC522

## Connect

Pin layout should be as follows:

| RFID | Arduino |
|:-----|--------:|
| 3.3V | 3.3V    |
| RST  | 9       |
| GND  | GND     |
| MISO | 12      |
| MOSI | 11      |
| SCK  | 13      |
| SDA  | 10      |
| IRQ  | -       |

 **IRQ** Not connected

## Installing RFID Libraries

	cd ~/Documents/Arduino/libraries
	git clone https://github.com/song940/rfid.git RFID
	
reboot arduino .

## Example Code

```cpp
#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop()
{
  if (rfid.isCard()) {
    Serial.println("Find the card!");
    if (rfid.readCardSerial()) {
      Serial.print("The card's number is  : ");
      Serial.print(rfid.serNum[0],HEX);
      Serial.print(rfid.serNum[1],HEX);
      Serial.print(rfid.serNum[2],HEX);
      Serial.print(rfid.serNum[3],HEX);
      Serial.print(rfid.serNum[4],HEX);
      Serial.println(" ");
    }
    rfid.selectTag(rfid.serNum);
  }
  rfid.halt();
}
```
    
## License

[MIT](http://lsong.mit-license.org/)


