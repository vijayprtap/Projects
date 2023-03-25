#include <esp_task_wdt.h>
//3 seconds WDT
#define WDT_TIMEOUT 3
void setup() {
  Serial.begin(115200);
  Serial.println("Configuring WDT...");
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
int i = 0;
int j = millis();
void loop() {
  // resetting WDT every 2s,
  if (millis() - j >= 2000 ) {
      Serial.println("Resetting TIMER");
      esp_task_wdt_reset();
      j = millis();
      i++;
      if (i == 5) {
        Serial.println("watchdog Bites!! CPU should reboot in 3s");
      }
  }
}
