/*
  This sketch demostrates the use of the Industruino's RS485 tranceiver.
  For testing purpose, it uses a IME MFD4421 (http://bit.ly/1MOjBJo)
  
  After initialize, it send a reset counter command:
    0x2400 U_WORD Reset Hour Meter, Maximum Powers, Maximum Voltages, Maximum Currents, Minimum Voltages, Active Partial Energy, Reactive Partial Energy 

 where
    b0 = 1  =>  Reset Hour Meter         
    b1 = 1  =>  Reset Peak Maximum Demand         
    b2 = 1  =>  Reset Maximum Voltage values             
    b3 = 1  =>  Reset Maximum Current values         
    b4 = 1  =>  Reset Minimum Voltage values         
    b5 = 1  =>  Reset Active Partial Energy         
    b6 = 1  =>  Reset Reactive Partial Energy         
    b7 = 1  =>  Reset Counter Input 1 ( Sw > 1.02 )         
    b8 = 1  =>  Reset Counter Input 2 ( Sw > 1.02 )         
    b9 .. b15 = 0 
  NOTE: Every write operation must be preceded by a "Master Unlock Key"  command.    
    0x2700 : write word with value = 0x5AA5 (Master Unlock Key)
  
  Then, it queries registers:
    0x1710 UD_WORD  Partial+ Active Energy   - Low Wh 
    0x1712 UD_WORD  Partial+ Active Energy   - High MWh 
    0x1714 UD_WORD  Partial+ Reactive Energy - Low varh 
    0x1716 UD_WORD  Partial+ Reactive Energy - High Mvarh

UD_WORD means unsigned double word

*/

#define DEBUG

#include <Wire.h>
#include <EEPROM.h>
#include <UC1701.h>
#include <Indio.h>
#include <SimpleModbusMaster.h>

/* LCD */
static UC1701 lcd;
const short LCD_BACKLIGHT_PIN = 13;
const short LCD_BACKLIGHT = 4;

/* Slave information */
#define SLAVE_ADDR 16 // slave address
#define SLAVE_BAUD_RATE 38400 // slave speed
#define SLAVE_TIMEOUT 600 // the reply timeout
#define SLAVE_POLLING 1000 // the scan rate
#define SLAVE_RETRY_COUNT 10 // how many times master should retry to send a packet in case of failure
#define RS485_ENABLE_PIN 9 // used to toggle the receive/transmit pin on the driver

// The total amount of available memory on the master to store data
#define TOTAL_NO_OF_REGISTERS 11

enum
{
  PKT_SAFETY_KEY,
  PKT_RESET_COUNTERS,
  PKT_ENERGY_COUNTERS,
  TOTAL_NO_OF_PACKETS // automatically updated
};


Packet packets[TOTAL_NO_OF_PACKETS];
unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup()
{
  // Init LCD
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT); //set backlight pin to output
  analogWrite(LCD_BACKLIGHT_PIN, (map(LCD_BACKLIGHT, 0, 5, 255, 0))); //convert backlight intesity from a value of 0-5 to a value of 0-255 for PWM.
  lcd.begin();
  
  // Initialize each packet
  modbus_construct(&packets[PKT_SAFETY_KEY], SLAVE_ADDR, PRESET_MULTIPLE_REGISTERS, 0x2700, 1, 0);
  modbus_construct(&packets[PKT_RESET_COUNTERS], SLAVE_ADDR, PRESET_MULTIPLE_REGISTERS, 0x2400, 1, 1);
  modbus_construct(&packets[PKT_ENERGY_COUNTERS], SLAVE_ADDR, READ_HOLDING_REGISTERS, 0x1710, 8, 2);
  
  // Initialize the Modbus Finite State Machine
  modbus_configure(&Serial1, SLAVE_BAUD_RATE, SERIAL_8E1, SLAVE_TIMEOUT, SLAVE_POLLING, SLAVE_RETRY_COUNT, RS485_ENABLE_PIN, packets, TOTAL_NO_OF_PACKETS, regs);
  
  // prepare value to write
  regs[0] = 0x5AA5; // this is the value of the "Master Unlock Key".
  regs[1] = 0x01FF; // reset all counters
}

void loop()
{
    modbus_update();
    
    #ifdef DEBUG
    for (int i=0; i<TOTAL_NO_OF_PACKETS; i++) {
      Serial.print("Pkt#");
      Serial.print(i);
      Serial.print("\tReq: "); 
      Serial.print(packets[i].requests);
      Serial.print("\tSuc: "); 
      Serial.print(packets[i].successful_requests);
      Serial.print("\tFail: ");
      Serial.print(packets[i].failed_requests);
      Serial.print("\tEx: ");
      Serial.print(packets[i].exception_errors);
      Serial.print("\n");
    }
    
    for (int i=0; i<TOTAL_NO_OF_REGISTERS; i++) {
      Serial.print("Reg#");
      Serial.print(i);
      Serial.print("\t"); 
      Serial.print(regs[i]);
      Serial.print("\n");
    } 
    
    #endif 
    
    // safety key and reset counters packets should be sent only once
    if (packets[PKT_SAFETY_KEY].successful_requests == 1)
      packets[PKT_SAFETY_KEY].connection = 0;

    if (packets[PKT_RESET_COUNTERS].successful_requests == 1)
      packets[PKT_RESET_COUNTERS].connection = 0;
    
    UpdateValues();
    DisplayValues();
}

long active_power;
long reactive_power;

void UpdateValues() {
  unsigned long temp1 = (unsigned long)regs[2] << 16 | regs[3];
  unsigned long temp2 = (unsigned long)regs[4] << 16 | regs[5];
  active_power = (temp2 * 1000000) + temp1;
  
  temp1 = (unsigned long)regs[6] << 16 | regs[7];
  temp2 = (unsigned long)regs[8] << 16 | regs[9];
  reactive_power = (temp2 * 1000000) + temp1;
}

void DisplayValues() {
  lcd.setCursor(0, 1);
  lcd.print(F("Active: "));
  lcd.print(active_power); lcd.print(F(" Wh"));
  lcd.setCursor(0, 2);
  lcd.print(F("Reactive: "));
  lcd.print(reactive_power); lcd.print(F(" varh"));
}
