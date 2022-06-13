#include "HX711.h"
#include <LiquidCrystal.h>
#define DT 47
#define SCK 46

const int rs = 12, en = 11, d4 = 37, d5 = 36, d6 = 35, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

HX711 scale;
float val = 0;

const PROGMEM  char sixty[60][3] = {
  {"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"},
  {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"},
  {"20"}, {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"},
  {"30"}, {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"},
  {"40"}, {"41"}, {"42"}, {"43"}, {"44"}, {"45"}, {"46"}, {"47"}, {"48"}, {"49"},
  {"50"}, {"51"}, {"52"}, {"53"}, {"54"}, {"55"}, {"56"}, {"57"}, {"58"}, {"59"}
};
void LCD_WriteStrPROGMEM(char *str, int n)
{ 
  for (int i = 0; i < n; i++)
    lcd.print( (char)pgm_read_byte( &(str[i]) ) );
}

struct Time
{
  unsigned char second, minute, hour;
};

boolean motor_is_active = false;
Time mainT = {0, 0, 0};
Time T1 = {0,0,0};
boolean t1_active = false;
int t1_value = 0;


ISR(TIMER3_COMPA_vect){
   if (++mainT.second == 60)
  {
    mainT.second = 0;
    if (++mainT.minute == 60)
    {
      mainT.minute = 0;
      if (++mainT.hour == 24){
        mainT.hour = 0;
      }
    }
  }
  lcd.setCursor(3, 0);
  LCD_WriteStrPROGMEM(sixty[mainT.hour], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[mainT.minute], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[mainT.second], 2);
  
  if(t1_active && mainT.hour == T1.hour && mainT.minute == T1.minute && mainT.second == 2){
    if(!motor_is_active){
      motor_is_active = true;
      digitalWrite(A8, HIGH);
    }
  }
  
}

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(5);
  
  pinMode(A8, OUTPUT);

  noInterrupts();
  TCCR3A = 0;
  TCCR3B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
  TIMSK3 = (1<<OCIE1A);
  OCR3A = 0x3D08;
  interrupts();
  
  scale.begin(DT,SCK);
  pinMode(SCK, OUTPUT);
  scale.set_scale(18029.57);
  scale.tare();
  val = scale.get_units()/2;
  lcd.begin(16, 2);
  
}

void loop() {
  
    delay(200);
    val = scale.get_units()/2;
    

    Serial.println(scale.get_units()/2);
    
    if(val*10 >= t1_value){
       digitalWrite(A8, LOW);
        motor_is_active=false;
    
  }

  if(Serial.available()){
    char str[30];
    int amount = Serial.readBytesUntil(';',str,30);
    str[amount] = NULL;

    int data[10];
    int count = 0;
    char* offset = str;

    while(true){
      data[count++] =atoi(offset);
      offset = strchr(offset,',');
      if(offset) offset++;
      else break;
      }
      
    switch (data[0]){
      
      case 1: // motor on
      digitalWrite(A8, HIGH);
      break;
      case 2: // set time
      mainT.hour = data[1];
      mainT.minute = data[2];
      break;
      case 21: // set alarm 1
        T1.hour = data[1];
        T1.minute = data[2];
        if(data[3] == 1){
          t1_active = true;
        }else{
          t1_active = false;
        }
        t1_value = data[4];
        break;
      }
   }
 

 }
