#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NUMPIXELS 60

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
int val;
int color = 0;
bool change = 1;

void setup() {
  pixels.begin();
  Serial.begin(115200);
}

void loop() {
  int analog = analogRead(A0);  //connect the Audio Source here, for best results use an OP amp front end.
  int scale = 700;   //this ultimately sets the sensitivity, basically the high point for the VUmeter
  int threshold = 600; //if the signal crosses this point, the VUmeter will change color
  int hysterisis = 500; //the hysterisis is necessary, since it determins the reset value before another color change
   
  val = map(analog, 0, scale, 1, NUMPIXELS); //this scales the Audio source to the amount of pixels in our strip

  if ((analog > threshold) && (change == 1)) { //this advances to the next color whenever it peaks over the threshold.
    color++;
    if (color > 4) {
      color = 0;
    }
    change = 0;
  }

  if (analog < hysterisis) {  //this allows the change of the color for the next peak
    change = 1;
  }
    
  Serial.print(hysterisis); //debugging stuff, best if used with Serial Plotter (CTRL + SHIFT + L)
  Serial.print(" ,");       //beware, that the serial stream runs at 115200 baud
  Serial.print(threshold);
  Serial.print(" ,");
  Serial.print(scale);
  Serial.print(" ,");
  Serial.println(analog);

  for (byte i = 0; i < NUMPIXELS; i++) { //this draws the palette for our strip depending on the color variable
    switch (color) {
      case 4:
        pixels.setPixelColor(i, map(i, 0, NUMPIXELS, 0, 255), map(i, 0, NUMPIXELS, 160, 0), 0);
        break;
      case 1:
        pixels.setPixelColor(i, map(i, 0, NUMPIXELS, 0, 255), 0,  map(i, 0, NUMPIXELS, 255, 0));
        break;
      case 2:
        pixels.setPixelColor(i, 0,  map(i, 0, NUMPIXELS, 0, 255), map(i, 0, NUMPIXELS, 255, 0));
        break;
      case 3:
        pixels.setPixelColor(i, map(i, 0, NUMPIXELS, 0, 255), map(i, 0, NUMPIXELS, 255, 0), map(i, 0, NUMPIXELS, 0, 255));
        break;
      case 0:
        pixels.setPixelColor(i, map(i, 0, NUMPIXELS, 0, 255),  map(i, 0, NUMPIXELS, 0, 255),  map(i, 0, NUMPIXELS, 255, 0));
        break;
    }
  }
  for (int i = NUMPIXELS; i > val; i--) {  //this then disables the pixels that don't need to be on
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show(); //here we draw the result onto the actual LED strip
}
