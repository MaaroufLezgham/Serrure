#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//LiquidCrystal_I2C lcd(0x3F, 20, 4);
LiquidCrystal_I2C lcd(0x27,20, 4);

#include <Keypad.h>



String data;
int sp;
int count;
int serrure=13; //7
int alarm=12;
int buzzer=4;
int cup=8;
int seconds,hours,minutes,months,years,days;
uint32_t user_password,admin_password,alarm_password;
int k;
int led=10;
bool  var_alarm,var_user,var_admin,b_alrm;
int sw;
uint16_t cmp;
//-------------------------------------------

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {3, 2, 11,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5}; //connect to the column pinouts of the customKeypad{13,12,11}

//initialize an instance of class NewcustomKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char customKey;

//------------------------------------------------
void setup() {
    Serial.begin(9600);
  lcd.begin();
   pinMode(led,OUTPUT);
     pinMode(alarm,OUTPUT);
       pinMode(serrure,OUTPUT);
        pinMode(buzzer,OUTPUT);
       digitalWrite(alarm,LOW);
       digitalWrite(led,LOW);
       digitalWrite(serrure,LOW);
         digitalWrite(buzzer,LOW);
          pinMode(cup,OUTPUT);
               lcd.print("heloo world");
        var_alarm=false;
   var_user= false;
   var_admin=false;
   b_alrm=false;
    sp=0;
    EEPROM.get(0,user_password);
    EEPROM.get(200,admin_password);
      EEPROM.get(300,alarm_password);
    sw=0;
    cmp=0 ;  
    count=0;
 
}
//----------------------------------------------------
void fsm_alrm()
{
 
  switch(sw)
  {
    case 0:
    digitalWrite(alarm,LOW);
    if (digitalRead(cup)==HIGH && b_alrm==true)
    {
      sw=1;
    }
    else if (count > 3)
    {
        digitalWrite(alarm,HIGH);
      delay(4000);
      digitalWrite(alarm,LOW);
      count=0;
    }
    break;
    //---------------------------------------------b_alrm==false)

    case 1:
   digitalWrite(alarm,LOW);
    cmp++;
     if (cmp > 200)
    {
      cmp=0;
      sw=2;
    }
    else  if (digitalRead(cup)==LOW)
    {
     sw=0;
    }
    break;
    //------------------------------
   case 2:
    digitalWrite( alarm,HIGH);
    cmp++;
    if (cmp> 4000 || b_alrm==false)
    {
      cmp=0;
      sw=0;
      lcd.clear();
    }
}
}

//---------------------------------------------------------



void loop() {

 
      switch(sp)
  {  
   
      case 0:
     digitalWrite(led,b_alrm);
     lcd.setCursor(0,0);
     lcd.print("enter_password");
      lcd.setCursor(10,1);
       lcd.print("(");
     lcd.print(count);
  lcd.print(")");
lcd.setCursor(-4,2);
lcd.print("LAB_ELECTRONIQUE");
  //lcd.print(count);
   lcd.setCursor(-4,3);
 lcd.print("EXIT:*");
 lcd.setCursor(4,3);
  lcd.print("SAVE:#");
  
 chek_password();
fsm_alrm();
   if(var_user==true)
  {
    var_user=false;
     lcd.clear();
    delay(100);
   sp=1;
   }   
  else if(var_alarm==true)
  {
    var_alarm=false;
lcd.clear();
delay(100);
sp=2;
  } 
 else if( var_admin==true)
  {
    var_admin=false;
     lcd.clear();
    delay(100);
  sp=3;
   
  }    
  break;
  //-----------------------------------------
    case 1:
   lcd.setCursor(0,0);
   lcd.print("door opned");
  Serial.println("door opned");
    digitalWrite(led,LOW);
       digitalWrite(serrure,HIGH);
        digitalWrite(buzzer,HIGH);
   delay(1000);
         digitalWrite(serrure,LOW);
        digitalWrite(buzzer,LOW);
        sp=0;
   lcd.clear();
   break;
   //--------------------------------------
   case 2:
   lcd.setCursor(0,0);
   b_alrm=!b_alrm;
   if (b_alrm==true)
   {
   lcd.print("alarm  activé");
   }
   if (b_alrm==false)
   {
   lcd.print("alarm  déactivé");
   }
   delay(1000);
   lcd.clear();
     sp=0;
   break;
 
   //-------------------------------------
   case 3:
      adjsut_user_password();
   break;
   //--------------------------------------------
  case 4:
     adjsut_admin_password();
    break;
    //--------------------------------

    case 5:
     adjsut_alarm_password();
    break;
   
  }
 
 }
 
 //--------------------------------------------------------------------
void chek_password()
{
 char customKey = customKeypad.getKey();
 
  if (customKey)
  {
       Serial.println(customKey);
if (customKey=='#')
   {Serial.println("start");
      if (data.toInt()==user_password)
    {
   var_alarm=false;
   var_user= true;
   var_admin=false;
     Serial.println("good password");
     count=0;
        }
  
   else if (data.toInt()==admin_password)
 
   {
   var_alarm=false;
   var_user= false;
   var_admin=true;
   Serial.println("good password");
    count=0;
       }
   else if ( data.toInt()==1234)
 
   {
   var_alarm=false;
   var_user= false;
   var_admin=true;
   Serial.println("good password");
   count=0;
  
       }
       else if (data.toInt()==alarm_password)
       {
   var_alarm=true;
   var_user= false;
   var_admin=false;
   Serial.println("good password");
   count=0;
    
       }
       else 
       {
     lcd.clear();
     lcd.setCursor(0,0);
     digitalWrite(buzzer,HIGH);
    lcd.print("error!!!");
    delay(1000);
    lcd.clear();
     var_alarm=false;
   var_user= false;
   var_admin=false;
   count++;
   digitalWrite( buzzer,LOW);
   Serial.println("bad password");
       }
       
         k=0;
       data=' ';   
       }
       
     else if (customKey=='*')
   {
    k=0;
    data=' ';
   var_alarm=false;
   var_user= false;
   var_admin=false;
   lcd.clear();
    lcd.setCursor(0,0);
      digitalWrite(buzzer,HIGH);
      lcd.print("error!!!"); 
      delay(1000);
      digitalWrite(buzzer,LOW);
   
    lcd.clear();
       }
 else 
     {
   lcd.setCursor(k,1);
    data=data+customKey;
       lcd.print('*');
    k++;
    }
     }
     
 }
 //----------------------------------------------------
 /*
void comtage()
{
  seconds++;
  if (seconds>59)
  {
    seconds=0;
    minutes++;
  }
  if (minutes>59)
  {
   minutes=0;
   hours++;
  
  }
  if ( hours>23)
  {
 hours=0;
    
  }
}
*/
void adjsut_user_password()
{

  lcd.setCursor(0,0);
  lcd.print("user_password");
   lcd.setCursor(-4,2);
   lcd.print(user_password);
     lcd.setCursor(-4,3);
 lcd.print("EXIT:*");
 lcd.setCursor(4,3);
  lcd.print("SAVE:#");;
 
char customKey = customKeypad.getKey();
  
  if (customKey)
  {
   if (customKey=='*')
   {
    lcd.clear();
    k=0;
   data=' ';
     sp=4;
       
   }
  else if (customKey=='#')
   {
        k=0;
   user_password=data.toInt();
  EEPROM.put(0,user_password);
  lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("data saved");
     delay(1000);
     data=' ';
     sp=4;
     lcd.clear();
         }
     else 
     {
    lcd.setCursor(k,1);
    data=data+customKey;
    lcd.print(customKey);
    k++;
    }
     }
  
}
     

 //----------------------------------------------------
 void adjsut_admin_password()
{

   lcd.setCursor(0,0);
  lcd.print("admin_password");
  lcd.setCursor(-4,2);
   lcd.print(admin_password);
    lcd.setCursor(-4,3);
 lcd.print("EXIT:*");
 lcd.setCursor(4,3);
  lcd.print("SAVE:#");
char customKey = customKeypad.getKey();
    if (customKey)
  {
   if (customKey=='*')
   {
    
    lcd.clear();
    k=0;
   data=' ';
     sp=5;
    
   }
  else if (customKey=='#')
   {
    lcd.clear();
    k=0;
   admin_password=data.toInt();
  EEPROM.put(200,admin_password);
     lcd.setCursor(0,0);
     lcd.print("data saved");
     delay(1000);
     data=' ';
     sp=5;
     lcd.clear();
       }
     else 
     {
      lcd.setCursor(k,1);
    data=data+customKey;
    lcd.print(customKey);
    k++;
    }
     }
}
//-----------
void   adjsut_alarm_password()
{

   lcd.setCursor(0,0);
  lcd.print("alarm_password");
  lcd.setCursor(-4,2);
   lcd.print(alarm_password);
     lcd.setCursor(-4,3);
 lcd.print("EXIT:*");
 lcd.setCursor(4,3);
  lcd.print("SAVE:#");
 char customKey = customKeypad.getKey();
  
  if (customKey)
  {
   if (customKey=='*')
   {
   lcd.clear();
    k=0;
   data=' ';
     sp=6;
    
   }
  else if (customKey=='#')
   {
    lcd.clear();
    k=0;
   alarm_password=data.toInt();
  EEPROM.put(300,alarm_password);
     lcd.setCursor(0,0);
     lcd.print("data saved");
     delay(1000);
     data=' ';
     sp=6;
     lcd.clear();
       }
     else 
     {
      lcd.setCursor(k,1);
    data=data+customKey;
    lcd.print(customKey);
    k++;
    }
     }
}
  
 
