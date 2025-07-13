#include <WiFiClientSecure.h>

DynamicJsonDocument send_request(char* url) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.print("Unable to establish connection to ");
    Serial.println(url);
  }
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      // Serial.println(payload);

      // Allocate enough memory for JSON parsing (adjust if needed)
      const size_t capacity = 12 * 1024;  // 12 KB buffer for JSON
      DynamicJsonDocument doc(capacity);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return -1;
      }
      return doc;
    }
  } else {
    Serial.printf("GET request failed, error: %s for %s\n", http.errorToString(httpCode).c_str(), url);
    return NULL;
  }
}