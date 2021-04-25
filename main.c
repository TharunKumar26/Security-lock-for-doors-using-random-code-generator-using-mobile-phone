#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include<Stdlib.h>
#include<String.h>
#include<Servo.h>
LiquidCrystal lcd(9,8,7,6,5,4);
char password[4];
char pass[4],pass1[4];
int i=0;
bool door=false;
int attempts;
char customKey=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4,A5,3,2};   //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int led = 12;
int leds = 13;
int buzzer = 10;
int m11;
int m12;
int r,a;
char k[4];
int emy = 9999;

Servo servo;
int pos=0;

void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  servo.attach(9);
  lcd.begin(16,2);
  pinMode(led, OUTPUT);
  pinMode(leds, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);
  lcd.print("DOOR LOCKED");
  lcd.setCursor(0,1);
  lcd.print(".......");
  delay(2000);
  lcd.clear();
  lcd.print("CHOOSE A WAY:");
  lcd.setCursor(0,1);
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);

    
  
}

void loop()
{
  digitalWrite(11, LOW);
  customKey = customKeypad.getKey();
  if(customKey=='C')
  {
    
    change();
  }
  else if(customKey=='A')
  {
    if(!door){
    otp();
    }
  }
  else if(customKey=='B')
  {
    if(!door){
    customKey=0;
    code();
    }
  }
  else if(customKey=='D')
  {
    customKey=0;
    ochange();
  }
   else if(customKey=='#')
  {
    customKey=0;
    door = false;
    closedoor();
      

  }
  
  
}
void closedoor(){

  delay(100);
  lcd.clear();
  lcd.print("Door Locked");
  delay(2000);
  for(pos = 180; pos >= 0; pos -=5)
        { 
          servo.write(pos); 
          delay(15);
        
      }
  }

bool sendalert(){

  if(attempts>2){
  lcd.clear();
 
  lcd.print("Exceeded Attempts");
  lcd.setCursor(0,1);
  Serial.print('Alert has sent to user');
  lcd.print("User Alerted");
  return false;
  }
  return true;
  }




void otp()
{
  if(sendalert()){
    
  
  char num[4];
String b = String(random(1000,9999));
  for(i=0;i<4;i++)
  {
    num[i] = b[i] ;
  }
  lcd.clear();
  lcd.print(num);
  delay(2000);
  lcd.clear();
 int j=0;
  lcd.clear();
  lcd.print("Enter OTP");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      ;
    }
    key=0;
  }
  delay(500);
      
 ;
    
    delay(1000);
    lcd.clear();
  if((strncmp(pass1, num, 4)))
  {
    attempts++;
    lcd.clear();
    lcd.print("WRONG ATTEMPT.");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);
  }
  else
  {
    attempts=0;
    door = true;
    lcd.print(" Success..");
    delay(3000);
    lcd.clear();
    lcd.print("DOOR UNLOCKED..");
    lcd.setCursor(0,1);
    for(pos = 0; pos <= 180; pos +=5)
      { 
      servo.write(pos); 
      delay(15);
    }
  }
  
  customKey=0;
  }
  
}

void code()
{
 
  char temp1[4];
  lcd.clear();
  String temp = String(emy);
    i=0;
   for(i=0;i<4;i++)
            {
                temp1[i] = temp[i] ;
            }
    i=0;
    delay(2000);
    lcd.print("EMERGENCY PWD");
    lcd.setCursor(0,1);
   
    while(i<4)
    {
        char key=customKeypad.getKey();
        if(key)
        {
            password[i++]=key;
            lcd.print(key);
            ;
        }
        key=0;
    }
    delay(1000);
   
    if(i==4){
         
        if(!(strncmp(password, temp1,4)))
        {
            lcd.clear();
            lcd.print(temp1);
            attempts=0;
            digitalWrite(led, HIGH);
            ;
            delay(2000);
            lcd.clear();
            lcd.print("Enter Password:");
            lcd.setCursor(0,1);
            delay(2000);
            i=0;
            while(i<4)
            {
                char key=customKeypad.getKey();
                if(key)
                {
                    password[i++]=key;
                    lcd.print(key);
                    ;
                }
                    key=0;
            }

  
            if(i==4)
            {
                delay(200);
                for(int j=0;j<4;j++)
                pass[j]=EEPROM.read(j);
                if(!(strncmp(password, pass,4)))
                  {
                  attempts=0;
                  digitalWrite(led, HIGH);
                  door = true;
                  
                  lcd.clear();
                  lcd.print("Accepted");
                  
                  digitalWrite(11, HIGH);
                  delay(1000);
                  lcd.clear();
                  lcd.print("WELCOME ");
                  lcd.setCursor(0,1);
                  i=0;
                  digitalWrite(led, LOW);
                  digitalWrite(leds, LOW);
                  for(pos = 0; pos <= 180; pos +=5)
                  { 
                  servo.write(pos); 
                  delay(15);
               
                }
                  }
                else
                {
                
                attempts++;
                digitalWrite(11, LOW);
                digitalWrite(buzzer, HIGH);
                digitalWrite(led, LOW);
                digitalWrite(leds, HIGH);
                lcd.clear();
                lcd.print("WRONG PASSWORD");
                lcd.setCursor(0,1);
                
                delay(2000);
                lcd.clear();
                lcd.print("CHOOSE A WAY:");
                lcd.setCursor(0,1);
                i=0;
                digitalWrite(buzzer, LOW);
                digitalWrite(led, LOW);
                digitalWrite(leds, LOW);
                }
      
      
        }

        }
    else
        {
      
            attempts++;
            digitalWrite(11, LOW);
            digitalWrite(buzzer, HIGH);
            digitalWrite(led, LOW);
            digitalWrite(leds, HIGH);
            lcd.clear();
            lcd.print("WRONG ATTEMPT");
            lcd.setCursor(0,1);
            
            delay(2000);
            lcd.clear();
            lcd.print("CHOOSE A WAY:");
            lcd.setCursor(0,1);
            i=0;
            digitalWrite(buzzer, LOW);
            digitalWrite(led, LOW);
            digitalWrite(leds, LOW);
        }
            
    
    }
}



void change()
{
   if(sendalert()){
    
  int j=0;
  lcd.clear();
  lcd.print("Enter Curr pass");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      ;
    }
    key=0;
  }

    delay(1000);
    lcd.clear();
  if((strncmp(pass1, pass, 4)))
  {
    attempts++;
    lcd.clear();
    lcd.print("WRONG PASSWORD...");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("Enter New Pass:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass[j]=key;
        lcd.print(key);
        EEPROM.write(j,key);
        j++;
        ;
      }
    }
    lcd.print(" Success..");
    delay(1000);
  }
  
}
}
void ochange()
{
  char num[4];
String b = String(random(1000,9999));
  for(i=0;i<4;i++)
  {
    num[i] = b[i] ;
  }
  lcd.clear();
  lcd.print(num);
  delay(2000);
  lcd.clear();
 int j=0;
  lcd.clear();
  lcd.print("Enter OTP");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
    
    }
    key=0;
  }
  delay(500);
      
    lcd.print(pass1);
    
    delay(1000);
    lcd.clear();
  if((strncmp(pass1, num, 4)))
  {
    attempts++;
    lcd.clear();
    lcd.print("WRONG PASSWORD...");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);
  }
  else
  {
    attempts=0;
    j=0;
    lcd.clear();
    lcd.print("Enter New Pass:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass[j]=key;
        lcd.print(key);
        EEPROM.write(j,key);
        j++;
       
      }
    } 
    lcd.print(" Success..");
    delay(3000);
  }
  lcd.clear();
  lcd.print("CHOOSE A WAY:");
  lcd.setCursor(0,1);
  customKey=0;
}
