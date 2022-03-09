#include <Arduino.h>
#include <U8g2lib.h>
#include <map>

//Constants
  const uint32_t interval = 100; //Display update interval
  const int32_t stepSizes[12] = {51076922,54112683,57330004,60740599,64352276,68178701,72231589,76528508,81077269,85899345, 91006452, 96418111};
  const String notes[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  volatile int32_t currentStepSize;
  String currentNote;
//Pin definitions
  //Row select and enable
  const int RA0_PIN = D3;
  const int RA1_PIN = D6;
  const int RA2_PIN = D12;
  const int REN_PIN = A5;

  //Matrix input and output
  const int C0_PIN = A2;
  const int C1_PIN = D9;
  const int C2_PIN = A6;
  const int C3_PIN = D1;
  const int OUT_PIN = D11;

  //Audio analogue out
  const int OUTL_PIN = A4;
  const int OUTR_PIN = A3;

  //Joystick analogue in
  const int JOYY_PIN = A0;
  const int JOYX_PIN = A1;

  //Output multiplexer bits
  const int DEN_BIT = 3;
  const int DRST_BIT = 4;
  const int HKOW_BIT = 5;
  const int HKOE_BIT = 6;

void setRow(uint8_t rowIdx) {
  digitalWrite(REN_PIN, LOW);
  int bit0 = bitRead(rowIdx, 0);
  int bit1 = bitRead(rowIdx, 1);
  int bit2 = bitRead(rowIdx, 2);
  digitalWrite(RA2_PIN, bit2);
  digitalWrite(RA1_PIN, bit1);
  digitalWrite(RA0_PIN, bit0);
  digitalWrite(REN_PIN, HIGH);
}


uint8_t readCols() {
  //RA0 RA1 RA2 low, REN high
  //R0 low, use digitalRead to get the outputs
  //DigitalWrite sets the output
  int c0 = digitalRead(C0_PIN);
  int c1 = digitalRead(C1_PIN);
  int c2 = digitalRead(C2_PIN);
  int c3 = digitalRead(C3_PIN);
  //return (x3 << 2) + (x2 << 1) + (x1);
  return (c0 << 3) + (c1 << 2) + (c2 << 1) + c3; 
  //return (c3 << 3) + (c2 << 2) + (c1 << 1) + c0;
}
//Display driver object
U8G2_SSD1305_128X32_NONAME_F_HW_I2C u8g2(U8G2_R0);

int findSetBit(uint8_t n) {
   for(int i = 0; i < 4; i++) {
     if(!bitRead(n, i)) {
       return i;
     }
   }
   return bitRead(n, 1);
}

//Function to set outputs using key matrix
void setOutMuxBit(const uint8_t bitIdx, const bool value) {
      digitalWrite(REN_PIN,LOW);
      digitalWrite(RA0_PIN, bitIdx & 0x01);
      digitalWrite(RA1_PIN, bitIdx & 0x02);
      digitalWrite(RA2_PIN, bitIdx & 0x04);
      digitalWrite(OUT_PIN,value);
      digitalWrite(REN_PIN,HIGH);
      delayMicroseconds(2);
      digitalWrite(REN_PIN,LOW);
}

void sampleISR() {
  static int32_t phaseAcc = 0;
  phaseAcc += currentStepSize;
  int32_t Vout = phaseAcc >> 24;
  analogWrite(OUTR_PIN, Vout + 128);
}

void setup() {
  // put your setup code here, to run once:

  //Set pin directions
  pinMode(RA0_PIN, OUTPUT);
  pinMode(RA1_PIN, OUTPUT);
  pinMode(RA2_PIN, OUTPUT);
  pinMode(REN_PIN, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(OUTL_PIN, OUTPUT);
  pinMode(OUTR_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(C0_PIN, INPUT);
  pinMode(C1_PIN, INPUT);
  pinMode(C2_PIN, INPUT);
  pinMode(C3_PIN, INPUT);
  pinMode(JOYX_PIN, INPUT);
  pinMode(JOYY_PIN, INPUT);

  //Initialise display
  setOutMuxBit(DRST_BIT, LOW);  //Assert display logic reset
  delayMicroseconds(2);
  setOutMuxBit(DRST_BIT, HIGH);  //Release display logic reset
  u8g2.begin();
  setOutMuxBit(DEN_BIT, HIGH);  //Enable display power supply

  //Initialise UART
  Serial.begin(9600);
  Serial.println("Hello World");

  TIM_TypeDef *Instance = TIM1;
  HardwareTimer *sampleTimer = new HardwareTimer(Instance);
  sampleTimer->setOverflow(22000, HERTZ_FORMAT);
  sampleTimer->attachInterrupt(sampleISR);
  sampleTimer->resume();
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t next = millis();
  static uint32_t count = 0;
  uint32_t localCurrentStepSize;
  uint8_t keyarray[7];

  if (millis() > next) {
    next += interval;

    //Update display
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    
    for(int i = 0; i <= 2; i++) {
      setRow(i);
      delayMicroseconds(3);
      uint8_t keys = readCols();
      keyarray[i] = keys;
      if(keyarray[0] == 15 && keyarray[1] == 15 && keyarray[2] == 15) {
        localCurrentStepSize = 0;
      }
      for(int j = 0; j < 4; j++) {
        if(bitRead(keyarray[i], j) == 0) {
          int position = (4* (i)) + (3 - j);
          localCurrentStepSize = stepSizes[position];
          currentNote = notes[position];
        }
      }
    __atomic_store_n(&currentStepSize, localCurrentStepSize, __ATOMIC_RELAXED);
      //currentStepSize = 0;
      //currentStepSize = positions.at(keyarray[i]);
    }
    u8g2.setCursor(2,10);
    u8g2.print("Piano!");
    u8g2.setCursor(2,20);
    u8g2.print(keyarray[0], HEX);
    u8g2.print(keyarray[1], HEX);
    u8g2.print(keyarray[2], HEX);
    u8g2.setCursor(2,30);
    u8g2.print(currentStepSize);
    u8g2.sendBuffer();          // transfer internal memory to the display

    //Toggle LED
    digitalToggle(LED_BUILTIN);
  }
}