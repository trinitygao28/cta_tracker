// #include <WiFi.h>

// const char* ssid = "161WKinzie Residents";
// const char* password = "AsianCharlotteLeo";

// void printWiFiStatus(int status) {
//   switch (status) {
//     case WL_NO_SHIELD:      Serial.println("🔌 No WiFi shield."); break;
//     case WL_IDLE_STATUS:    Serial.println("💤 Idle status."); break;
//     case WL_NO_SSID_AVAIL:  Serial.println("📡 No SSID available."); break;
//     case WL_SCAN_COMPLETED: Serial.println("🔍 Scan complete."); break;
//     case WL_CONNECTED:      Serial.println("✅ Connected."); break;
//     case WL_CONNECT_FAILED: Serial.println("❌ Connection failed."); break;
//     case WL_CONNECTION_LOST:Serial.println("🔌 Connection lost."); break;
//     case WL_DISCONNECTED:   Serial.println("🔌 Disconnected."); break;
//     default:                Serial.print("❓ Unknown status: ");
//                             Serial.println(status); break;
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("🔍 Scanning for networks...");
//   int n = WiFi.scanNetworks();
//   for (int i = 0; i < n; ++i) {
//     Serial.print(i + 1);
//     Serial.print(": ");
//     Serial.print(WiFi.SSID(i));
//     Serial.print(" (");
//     Serial.print(WiFi.RSSI(i));
//     Serial.println(" dBm)");
//     Serial.print(" dBm)  Channel: ");
//     Serial.println(WiFi.channel(i));
//   }

//   WiFi.begin(ssid, password);

//   unsigned long start = millis();
//   while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
//     delay(1000);
//     wl_status_t status = WiFi.status();
//     Serial.print("Status: ");
//     Serial.println((int)status);
//     printWiFiStatus(status);
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\n✅ WiFi connected!");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
//   } else {
//     Serial.println("\n❌ Failed to connect to WiFi.");
//   }
// }


// void loop() {}
