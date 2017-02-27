#include "NokiaLCD.h" 
#include "ASCII.h"

NokiaLCD::NokiaLCD(uint SCK, uint MOSI, uint DC, uint RST, uint CS) { 
  // Configure control pins defining as OUTPUTS. 
  pinMode(CS, OUTPUT); 
  pinMode(RST, OUTPUT); 
  pinMode(DC, OUTPUT); 
  pinMode(MOSI, OUTPUT); 
  pinMode(SCK, OUTPUT); 
  
  // Save pin number to variables. 
  _SCK = SCK; 
  _MOSI = MOSI; 
  _DC = DC; 
  _RST = RST; 
  _CS = CS; 
} 
  
  
/* 
  Init LCD with the properly commands instruction set 
*/
  
void NokiaLCD::init() { 
  digitalWrite(_RST, LOW); // Send a reset signal to properly init the LCD
  digitalWrite(_RST, HIGH); 
    
  writeData(LCD_COMMAND, LCDEXTENDED);  // Use extended instruction set
  writeData(LCD_COMMAND, LCDCONTRAST);  // Set Vop
  writeData(LCD_COMMAND, LCDTEMPC);     // Set Temp coefficent 
  writeData(LCD_COMMAND, LCDMODE);      // Set bias system
  
  writeData(LCD_COMMAND, LCDBCMODE);    // Change from extendend instruction set to basic one 
  writeData(LCD_COMMAND, LCDCONTROL);   // Set display control
} 

// Send a byte through the bus

void NokiaLCD::writeData(byte data_or_command, byte data) { 
  digitalWrite(_DC, data_or_command);  
  digitalWrite(_CS, LOW); 
  shiftOut(_MOSI, _SCK, MSBFIRST, data); 
  digitalWrite(_CS, HIGH); 
} 
  
  
// Clear the LCD sending 0x00 to the RAM. 
  
void NokiaLCD::clear(void) { 
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++) 
   writeData(LCD_DATA, 0x00); 
   setCursor(0, 0);                     // After clear the display, return to home position 
} 
  
/* 
  Set the cursor to the corresponding position. 
  0x80 = Maximun column position number. 
  0x40 = Maximun row position number. 
*/
  
void NokiaLCD::setCursor(int x, int y) { 
  writeData(LCD_COMMAND, 0x80 | x); // Column. 
  writeData(LCD_COMMAND, 0x40 | y); // Row. 
} 

void NokiaLCD::character(char character) { 
  writeData(LCD_DATA, 0x00);
  for (int index = 0 ; index < 5 ; index++) { 
    writeData(LCD_DATA, ASCII[character - 0x20][index]); 
  } 
  writeData(LCD_DATA, 0x00); 
} 

void NokiaLCD::print(char *characters) { 
  while(*characters) { 
    character(*characters++); 
  } 
} 

// void NokiaLCD::bitmap(unsigned char bmp[]) { 
//   for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++) { 
//     writeData(LCD_DATA, bmp[index]); 
//   } 
// } 
  
// void NokiaLCD::sBitmap() { 
//   if (Serial.available()) { 
//       for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++) { 
//         while(!Serial.available()) { 
//           // Do nothing if no data is received
//         } 
//         char c = Serial.read(); 
//         img[i] = c; 
//       } 
//       bitmap(img); 
//   } 
// } 


