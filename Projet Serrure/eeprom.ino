#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x3F, 16, 2);  // LCD_I2C
LiquidCrystal_I2C lcd(0x20, 20, 4);
int addr=0;
char n;
int k=4, z=0;
char code[4]= {'1', '2', '3', '4'};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Wire.begin();
  lcd.init(); // initialisation de l'afficheur
  //Ecriture dans la eeprom
 for( int j=0; j<4; j++)
  {
    EEPROM.write(addr,code[j]);
   addr++;
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Ecriture en cours");
    lcd.setCursor(7, 1);
    lcd.print("...");
  }
  addr=0;
  //delay(5000);
  lcd.clear();
}
 
void loop() {
  // put your main code here, to run repeatedly:
 
    while(z<4)
  {
    n = EEPROM.read(addr);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Lecture-Affichage");
    lcd.setCursor(k, 1);
    lcd.print(n);
    delay(2000);
    addr++;
    k=k+1;
    z++;
  }
  delay(6000);
  lcd.clear();
    lcd.backlight();
    lcd.setCursor(7, 0);
    lcd.print("FIN");
    lcd.setCursor(2, 1);
    lcd.print("TEST...");

}
