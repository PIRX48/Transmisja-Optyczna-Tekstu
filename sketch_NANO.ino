#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDR_PIN A2
#define THRESHOLD 100
#define PERIOD 20

bool previous_state;
bool current_state;
int i=0;
int j=0;

void setup() 
{
    delay(1000);
    lcd.begin(16, 2);
    lcd.print("Ready");
    j=2;
}

void loop() 
{
  current_state = get_ldr();
  if(!current_state && previous_state)
  {
    print_byte(get_byte());

  }
  previous_state = current_state;
}

bool get_ldr()
{
  int voltage = analogRead(LDR_PIN);
  return voltage > THRESHOLD ? false : true;
}

char get_byte()
{
  char ret = 0;
  delay(PERIOD*1.5);
  for(int k = 0; k < 8; k++)
  {
   ret = ret | get_ldr() << k; 
   delay(PERIOD);
  }
  return ret;
}

void clear_screen()
{
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reddy");
  j=2;
  lcd.setCursor(0, j);
}

void print_byte(char my_byte)
{
  char buff[2];
  sprintf(buff, "%c", my_byte);
  lcd.setCursor(i, j);
  lcd.print(buff);
  i++;
  if (i == 16)
  {
    i = 0;
    j++;
    if(i==15 && j==3)
    {
      clear_screen();
    }
  }
}