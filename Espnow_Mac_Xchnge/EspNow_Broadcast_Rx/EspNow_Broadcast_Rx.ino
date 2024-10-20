#include <WiFi.h>
#include <esp_now.h>

uint8_t myMAC[6];
uint8_t broadcastAddress[6];  // Will hold the converted MAC address

// Structure for sending data
typedef struct struct_message {
    uint8_t mac[6];  // To store sender's MAC address
} struct_message;

struct_message outgoingMessage;

// Function to convert MAC address string to uint8_t array
void convertMacStringToUint8(const char *macStr, uint8_t *macArray) {
    int values[6];
    if (sscanf(macStr, "%x:%x:%x:%x:%x:%x", 
               &values[0], &values[1], &values[2], 
               &values[3], &values[4], &values[5]) == 6) {
        // Convert parsed values to uint8_t and store in array
        for (int i = 0; i < 6; ++i) {
            macArray[i] = (uint8_t)values[i];
        }
    } else {
        Serial.println("Invalid MAC address format!");
    }
}

// Function to format MAC address to string
void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength) {
    snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", 
             macAddr[0], macAddr[1], macAddr[2], 
             macAddr[3], macAddr[4], macAddr[5]);
}

// Callback for when a message is received
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    
    // Ensure the received data is valid
    char receivedData[dataLen + 1];
    memcpy(receivedData, data, dataLen);
    receivedData[dataLen] = '\0';  // Null terminate the string

    Serial.printf("Received MAC address from: %s - %s\n", macStr, receivedData);
    
    // Convert the received string MAC address to uint8_t array
    convertMacStringToUint8(receivedData, broadcastAddress);
    
    // Print the uint8_t array format for verification
    Serial.print("Converted MAC address: ");
    for (int i = 0; i < 6; ++i) {
        Serial.printf("0x%02X", broadcastAddress[i]);
        if (i < 5) Serial.print(", ");
    }
    Serial.println();
    
    // Check if the peer already exists
    if (esp_now_is_peer_exist(broadcastAddress)) {
        Serial.println("Peer already exists, removing...");
        esp_now_del_peer(broadcastAddress);
    }
    
    // Add the peer (receiver) dynamically after receiving the MAC address
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  // You can set this according to your WiFi settings
    peerInfo.encrypt = false;

    // Add the peer to send to
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    } else {
        Serial.println("Peer added successfully");
    }

    // Prepare to send your own MAC address
    esp_read_mac(myMAC, ESP_MAC_WIFI_STA);  // Fixed the argument here
    memcpy(outgoingMessage.mac, myMAC, 6);
    
    // Send the message containing your own MAC address
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingMessage, sizeof(outgoingMessage));
    
    if (result == ESP_OK) {
        Serial.println("Sent my MAC address!");
    } else {
        Serial.println("Failed to send my MAC address");
    }
}

// Callback when a message is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Setup function
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    Serial.println("ESP-NOW Receiver Example");
    WiFi.disconnect();  // Disconnect from any Wi-Fi network

    // Get the ESP32's own MAC address
    esp_read_mac(myMAC, ESP_MAC_WIFI_STA);  // Fixed the argument here
    Serial.print("My MAC Address: ");
    Serial.println(WiFi.macAddress());

    // Initialize ESP-NOW
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESP-NOW Initialized");
        esp_now_register_recv_cb(receiveCallback);  // Register callback for receiving data
        esp_now_register_send_cb(onDataSent);       // Register callback for sending data
    } else {
        Serial.println("ESP-NOW Initialization Failed");
        ESP.restart();
    }
}

void loop() {
    // Nothing to do here. Everything is handled in the receive callback.
    delay(2000);  // Optional delay for stability
}
