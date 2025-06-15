DynamicJsonDocument send_request(char* url) {
  HTTPClient http;
  http.begin(url);
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
    Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
    return NULL;
  }
}