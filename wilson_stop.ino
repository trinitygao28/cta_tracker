#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "constants.h"

void printTimestamp(time_t t) {
  struct tm *timeinfo = localtime(&t);  // Convert to local time
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);
  Serial.println(buffer);
}

void printWiFiStatus(int status) {
  switch (status) {
    case WL_NO_SHIELD:      Serial.println("🔌 No WiFi shield."); break;
    case WL_IDLE_STATUS:    Serial.println("💤 Idle status."); break;
    case WL_NO_SSID_AVAIL:  Serial.println("📡 No SSID available."); break;
    case WL_SCAN_COMPLETED: Serial.println("🔍 Scan complete."); break;
    case WL_CONNECTED:      Serial.println("✅ Connected."); break;
    case WL_CONNECT_FAILED: Serial.println("❌ Connection failed."); break;
    case WL_CONNECTION_LOST:Serial.println("🔌 Connection lost."); break;
    case WL_DISCONNECTED:   Serial.println("🔌 Disconnected."); break;
    default:                Serial.print("❓ Unknown status: ");
                            Serial.println(status); break;
  }
}

void connectToWifi() {
  Serial.println("🔍 Scanning for networks...");
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
    Serial.print(" dBm)  Channel: ");
    Serial.println(WiFi.channel(i));
  }

  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(1000);
    wl_status_t status = WiFi.status();
    Serial.print("Status: ");
    Serial.println((int)status);
    printWiFiStatus(status);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Failed to connect to WiFi.");
  }
}

void synchronizeTime() {
  // Set time via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print("⏳ Syncing time...\n");
  }
  Serial.println("⏰ Time synchronized!");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWifi();

  synchronizeTime();

  for (int i = 0; i < NUM_BUS_TARGETS; i++) {
    int stpid = get_stop(bus_lines[i]);
    bus_lines[i].stpid = stpid;  // Modify actual struct in the array
}
  
}

void loop() {

  time_t now = time(nullptr); // Current time from NTP

  clear_dict();
  get_arrival_times(now); // get train times
  for (Line line : bus_lines) { // get bus times
    get_prediction(line, now);
  }

  Serial.print("Querying at ");
  printTimestamp(now);
  for (Line line : lines) {
    print_values(line);
  }
  for (Line line : bus_lines) {
    print_values(line);
  }
  Serial.println();
  delay(10000); // Wait 10 second
}
