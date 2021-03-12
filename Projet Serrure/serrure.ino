#include <EEPROM.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//----------------------------Keypad construction-----------
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  { '1', '2', '3','A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char code [4];
char codepress [4] ;
int addr = 0;
int i = 0;
char option;
int col = 2;
int Val_champ;
char key;
int valide=0;
char Newcode[4];
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keyp = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//---------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x20, 20, 4);
//LiquidCrystal_I2C lcd(0x3F, 16, 4);  // LCD_I2C
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  keyp.begin(makeKeymap(keys));
  lcd.init(); // initialisation de l'afficheur
  Wire.begin();
  pinMode(10, OUTPUT); // PinSerrure
  digitalWrite(10,LOW);
  /*
   * lecture en eeprom (Le code est stock dans la mémoire eeprom du PIC)
   */
  for(int i=0; i<4; i++)
     {
       code[i] = EEPROM.read(addr);
       addr++;
      delay(5);
     }
     addr=0;
     //fin lecture
} 

void Etat_clavier()
{
  //Recuperation du chiffre tape sur le clavier ( pour valider taper sur '*');
   key=keyp.getKey();
  if (  key != NO_KEY )
  {
    codepress [i] = key;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( codepress[i] == '*') //Validation du code tape '*'
    {
      lcd.backlight();
      lcd.setCursor(col, 1);
      lcd.print(key);
      //controler si le code taper est juste
      //-----------------------------------------------------------------------------------------------
      if (codepress[0]==code[0]&&codepress[1]==code[1]&&codepress[2]==code[2]&&codepress[3]==code[3])
           {
              lcd.clear();
            while(1)
            {
              lcd.backlight();
              lcd.setCursor(0, 0);
              lcd.print("1-Desactivation");
              lcd.setCursor(0, 1);
              lcd.print("2-Changer Code");
              key=keyp.getKey();
              if(key!=NO_KEY)
              {
                option=key;
                break;
              }
             }
            switch (option){
        //option desactivation de la serrure
    case '1':   
        //afficher alarme deactivé
        lcd.backlight();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("CODE");
        lcd.setCursor(3,1);
        lcd.print("CORRECT");
       //désactivation de le serrure
       digitalWrite(10 ,HIGH);
        delay(5000); // delai d'attente avant reactivation la serrure
       digitalWrite(10,LOW);
        lcd.clear();
        break; 
       //Option changement de code
    case '2': 
                          i=0; col=2;
                          lcd.clear();
                          while(1)
                          {
                            lcd.backlight();
                            lcd.setCursor(1, 0);
                            lcd.print("Tapez le code :");
                            key=keyp.getKey();
                                if (  key != NO_KEY )
                                   {
                                      lcd.backlight();
                                      lcd.setCursor(col, 1);
                                      lcd.print(key);
                                      Newcode[i]=key;
                                      i++;
                                        col=col+2;
                                      if(key=='*')
                                      {
                                        lcd.clear();
                                        lcd.backlight();
                                        lcd.setCursor(0, 0);
                                        lcd.print("New code");
                                        lcd.setCursor(4, 1);
                                        lcd.print("Valide...");
                                        for(int j=0;j<4;j++)
                                        {
                                          code[j]=Newcode[j];
                                        };
                                        i=0;col=2; break;
                                       }
                                         }
                                           }
                                             for( int j=0; j<4; j++)
                                                 {
                                                       EEPROM.write(addr,code[j]);
                                                       addr++;
                                                   }
                                                   addr=0;
                                                   delay(1000);
                                                   lcd.clear();
                                                   break;
  }
}
//mot de passe incorrecte
else{ 
        // mot de passe incorrect
        lcd.backlight();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erreur de code");
        delay(2000);
        lcd.clear();
      }
      //--------------------------------------------------------------------------------------------
      i = 0;
      col = 2;
    }
    else
    {
      //afficher le chiffre taper
      // Envoi du message
      lcd.backlight();
      lcd.setCursor(col, 1);
      lcd.print(key);
      i++;
      col=col+2;
    }
  }
      else
    {
      lcd.backlight();
      lcd.setCursor(2, 0);
      lcd.print("Tapez le code:");
    }
  
} //Etat_clavier
void loop() {
    // put your main code here, to run repeatedly:
    Etat_clavier();
}
