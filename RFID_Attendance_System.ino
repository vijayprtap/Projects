#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);// set the LCD address to 0x27 for a 16 chars and 2 line display

String host = "139.144.5.107";
int httpport = 80;
String url = "http://139.144.5.107/api/attendance";

WiFiClient client;
HTTPClient http;

#define RST_PIN  0     // Configurable, see typical pin layout above
#define SS_PIN   2   // Configurable, see typical pin layout above
#define BUZZER   15     // Configurable, see typical pin layout above
 

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;  
ESP8266WiFiMulti WiFiMulti;
MFRC522::StatusCode status;      


/* Be aware of Sector Trailer Blocks */
int blockNum = 2;  


/* Create another array to read data from Block */
/* Legthn of buffer should be 2 Bytes more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];

String data2;

void setup() 
{

  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  // Serial.setDebugOutput(true);
lcd.init();
lcd.backlight();

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  
  /* Put your WIFI Name and Password here */
    WiFi.begin("esp","naruto900");
while (WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
lcd.setCursor(0,0);
lcd.print("CONNECTING TO");
lcd.setCursor(0,1);
lcd.print("INTERNET");
 
  delay(1000);  
  }
 
lcd.clear();
    /* Set BUZZER as OUTPUT */
  pinMode(BUZZER, OUTPUT);


  
  /* Initialize SPI bus */
  SPI.begin();
}

void loop()
{
 
lcd.setCursor(0,0);
lcd.print(" IOT ATTENDANCE ");

  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  /* Read data from the same block */
  Serial.println();
  Serial.println(F("Reading last data from RFID..."));
  ReadDataFromBlock(blockNum, readBlockData);
  /* If you want to print the full memory dump, uncomment the next line */
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  /* Print the data read from block */
  Serial.println();
  Serial.print(F("Last data in RFID:"));
  Serial.print(blockNum);
  Serial.print(F(" --> "));
  for (int j=0 ; j<16 ; j++)
  {
    Serial.write(readBlockData[j]);
  }
  lcd.setCursor(0,1);
lcd.print("CARD DETECTED ");

  Serial.println();
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  delay(200);
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
 delay(200);  
    digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  delay(200);
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);

    //Http part
     data2 = String((char*)readBlockData);
    data2.trim();
String Rfiddata = data2 ;
    Serial.println("rfid card data:"+ (Rfiddata));
    
    HTTPClient https;
    http.begin(client,host,httpport,url);

 http.addHeader("Content-Type", "application/x-www-form-urlencoded");

 String requestbody ="?id=121&name="+ String(Rfiddata);
 
 requestbody = url+requestbody;
  int httpcode = http.POST(requestbody);
 Serial.println(requestbody);
 String payload = http.getString();
 Serial.println("payload: " + payload);
 Serial.println( "  Http Code : " + (String) httpcode);
 delay(3000);
 lcd.clear();
}


void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  /* Prepare the ksy for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
}