#include <WiFi.h>
#include <esp_now.h>

uint8_t myMAC[6];
bool shouldBroadcast = true;  // Flag to control broadcasting

// Function to format MAC address to string
void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength) {
    snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", 
             macAddr[0], macAddr[1], macAddr[2], 
             macAddr[3], macAddr[4], macAddr[5]);
}

// Callback for when a message is successfully sent
void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.print("Last Packet Sent to: ");
    Serial.println(macStr);
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback for when a message is received
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);

    // Ensure the data received is valid
    if (dataLen == 6) {  // Check if the length matches a MAC address
        uint8_t receivedMAC[6];
        memcpy(receivedMAC, data, dataLen);

        Serial.print("Received MAC Address: ");
        formatMacAddress(receivedMAC, macStr, sizeof(macStr));
        Serial.println(macStr);

        // Set the flag to stop broadcasting after receiving
        shouldBroadcast = false;  
    } else {
        Serial.println("Received data length does not match MAC address size.");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Set device to Station mode
    WiFi.mode(WIFI_STA);
    Serial.println("ESP-NOW Sender and Receiver Example");

    // Print the sender MAC address
    esp_read_mac(myMAC, ESP_MAC_WIFI_STA);
    Serial.print("Sender MAC Address: ");
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", myMAC[0], myMAC[1], myMAC[2], myMAC[3], myMAC[4], myMAC[5]);

    // Disconnect from any Wi-Fi network
    WiFi.disconnect();

    // Initialize ESP-NOW
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESP-NOW Initialized");

        // Define broadcast address
        esp_now_peer_info_t peerInfo;
        memset(&peerInfo, 0, sizeof(peerInfo));  // Clear peerInfo
        memcpy(peerInfo.peer_addr, (uint8_t *)"\xFF\xFF\xFF\xFF\xFF\xFF", 6);  // Broadcast address
        peerInfo.channel = 1;  // Set the same channel
        peerInfo.encrypt = false;  // No encryption

        // Add peer
        if (esp_now_add_peer(&peerInfo) == ESP_OK) {
            Serial.println("Peer added");
        } else {
            Serial.println("Failed to add peer");
        }

        // Register callbacks for sending and receiving data
        esp_now_register_send_cb(sentCallback);   // For sending data
        esp_now_register_recv_cb(receiveCallback);  // For receiving data
    } else {
        Serial.println("ESP-NOW Initialization Failed");
        delay(3000);
        ESP.restart();
    }

    // Set the same channel as the receiver
    WiFi.channel(1);
}

void loop() {
    // Check if broadcasting is allowed
    if (shouldBroadcast) {
        // Broadcast the sender's MAC address
        uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Broadcast address

        // Send MAC address as bytes
        esp_err_t result = esp_now_send(broadcastAddress, myMAC, sizeof(myMAC));
        if (result == ESP_OK) {
            Serial.print("Broadcasting MAC Address: ");
            formatMacAddress(myMAC, (char *)myMAC, sizeof(myMAC)); // Cast myMAC to char* for printing
            // Serial.println((char *)myMAC);  // Print the sender's MAC Address
        } else {
            Serial.println("Broadcast failed");
        }
    } else {
        Serial.println("Broadcasting stopped.");
    }

    delay(2000);  // Delay for broadcast interval
}
