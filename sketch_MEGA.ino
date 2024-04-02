#include <LiquidCrystal.h>
#include <PS2Keyboard.h>

LiquidCrystal lcd(12, 13, 8, 9, 10, 11);

PS2Keyboard keyboard;

#define LED_PIN A1
#define PERIOD 20
#define DATA_PIN 4
#define IRQ_PIN 3

const int SCREEN_WIDTH = 16;
const int SCREEN_HEIGHT = 2;
char key2;

char string[SCREEN_HEIGHT][SCREEN_WIDTH];

void setup() 
{
  delay(1000);
  keyboard.begin(DATA_PIN, IRQ_PIN);
  pinMode(LED_PIN, OUTPUT);
  lcd.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() 
{
  read_keyboard();
  send_message();
  clear_string();
  lcd.clear();
  delay(1000);
}

void send_byte(char my_byte)
{
  digitalWrite(LED_PIN, LOW);
  delay(PERIOD);
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(LED_PIN, (my_byte&(0x01 << i))!=0 );
    delay(PERIOD);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(PERIOD);
}

void read_keyboard()
{
  bool state = true;
  int i = 0;
  int j = 0;
  char key;
  
  while(state)
  {
    if (keyboard.available())
    {
      key = keyboard.read();
      if (key == PS2_DELETE)
      {
        if (i == 0 && j == 0)
        {
          lcd.setCursor(i, j);
          lcd.print(' ');
        }
        else if (i == 0)
        {
          i = 15;
          j--;
          lcd.setCursor(i, j);
          lcd.print(' ');
        }
        else
        {
          i--;
          lcd.setCursor(i, j);
          lcd.print(' ');
        }
      }
      else if (key == PS2_ENTER)
      {
        state = false;
      }
      else 
      {
        string[j][i] = key;
        lcd.setCursor(i, j);
        lcd.print(key);
        i++;
        if (i == SCREEN_WIDTH)
        {
          i = 0;
          j++;
          if (j == SCREEN_HEIGHT)
          {
            j = 0;
          }
        }
      }
    }
  }
}

void send_message()
{
  for(int j = 0; j < SCREEN_HEIGHT; j++)
  {
    for(int i = 0; i < SCREEN_WIDTH; i++)
    {
      send_byte(string[j][i]);
    }
  }
}

void clear_string()
{
  for(int j = 0; j < SCREEN_HEIGHT; j++)
  {
    for(int i = 0; i < SCREEN_WIDTH; i++)
    {
      string[j][i] = 0;
    }
  }
}
