#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <FreqCount.h>
#include <math.h>

const int SCREEN_WIDTH=128; // OLED display width, in pixels
const int SCREEN_HEIGHT=64; // OLED display height, in pixels
const int first_string=12;  //first string on LCD
const int second_string=41;  //second string on LCD

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)



void setup() {
  
  pinMode(A2, INPUT);
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }


  display.setFont(&FreeSerif9pt7b);
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,first_string);             
  display.println("RUN HVK OS...");
  display.display();
  delay(1000);
  display.clearDisplay();
  
  FreqCount.begin(100);


  
}

void loop() {

  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    //long freq=(count-45750)*10;    //MY
    long freq=(count-45681)*10;     //HVK
    //long freq=count; //debug
    int v=(fmax(0,analogRead(A2)-300));

    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK); 
    display.setCursor(25,first_string);
    display.print(freq);
    display.print(" Hz");
    
    display.drawLine(0, second_string,0+SCREEN_WIDTH,second_string, WHITE);
    display.drawLine(SCREEN_WIDTH/2, second_string,SCREEN_WIDTH/2,second_string-15, WHITE);

    long freq_lim=freq;
    if(freq_lim>980)freq_lim=980;
    if(freq_lim<-980)freq_lim=-980;
    
    display.fillRect(int(SCREEN_WIDTH/2-2+freq_lim*64/1000),second_string-15,5,15,WHITE);

    display.fillRect(SCREEN_WIDTH/2-v/2,second_string+5,v,15,WHITE);

    
    display.display();   
    

    
    Serial.print(v);
    if(v>0){        
        Serial.print(":");
        Serial.print(freq);
    }
    Serial.println("");
      
      
  }
}
