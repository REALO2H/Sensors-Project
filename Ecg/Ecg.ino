#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#define WIFI_DEBUG 1
#include <esp_wifi.h>
const char* serverUrl = "http://192.168.1.240:5000/api/ecg"; 
const char* ssid = "AmerHalabi";         
const char* password = "19011975"; 



#define ECG_PIN 16 // Analog output from AD8232
#define LO_PLUS 13 // LO+ pin connected to GPIO 13
#define LO_MINUS 12 // LO- pin connected to GPIO 12



// Sampling Rate (in milliseconds)
const int SAMPLING_DELAY = 600;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    attempt++;
    if (attempt >= 10) {
      Serial.println("\nFailed to connect to Wi-Fi. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendDataToServer(int ecgValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Construct JSON payload
    String payload = "{\"value\":" + String(ecgValue) + "}";
    Serial.print("Sending data to server: ");
    Serial.println(payload);

    // POST to server
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Server response: ");
      Serial.println(response);
    } else {
      Serial.print("HTTP POST failed with error code: ");
      Serial.println(httpResponseCode);
    }
    http.end(); // End connection
  } else {
    Serial.println("Wi-Fi not connected. Data not sent.");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize pins
  pinMode(ECG_PIN, INPUT);
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
}

void loop() {
  // Check lead-off detection
  int loPlus = digitalRead(LO_PLUS);
  int loMinus = digitalRead(LO_MINUS);

  if (loPlus == HIGH || loMinus == HIGH) {
    Serial.println("Leads are off! Please check electrode connections.");
  } else {
    // Read ECG signal
    int ecgValue = analogRead(ECG_PIN);
    Serial.print("ECG Value: ");
    Serial.println(ecgValue);

    // Send data to server
    sendDataToServer(ecgValue);
  }

  // Delay for sampling rate
  delay(SAMPLING_DELAY);
}














// void setup() {

//   // Wifi set up
//   Serial.begin(9600);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to Wi-Fi");
//   int attempt=0;
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//     attempt++;
//     if(attempt==10){
//        ESP.restart();
//     }
//   }
//   Serial.println("\nConnected to Wi-Fi!");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());

//   pinMode(ECG_PIN, INPUT);
//   pinMode(LO_PLUS, INPUT);
//   pinMode(LO_MINUS, INPUT);
// }

// void loop() {
//   // Check lead-off detection
//   int loPlus = digitalRead(LO_PLUS);
//   int loMinus = digitalRead(LO_MINUS);

//   if (loPlus == HIGH || loMinus == HIGH) {
//     Serial.println("Leads are off! Please check electrode connections.");
//   } else {
//     // Read ECG signal
//     int ecgValue = analogRead(ECG_PIN);
//     Serial.print("ECG Value: ");
//     Serial.println(ecgValue);

//     // Send data to server
//     if (WiFi.status() == WL_CONNECTED) {
//       HTTPClient http;
//       http.begin(serverUrl);
//       http.addHeader("Content-Type", "application/json");
      
//       // Construct JSON payload
//       String payload = "{\"value\":" + String(ecgValue) + "}";
      
//       // POST to server
//       int httpResponseCode = http.POST(payload);
//       if (httpResponseCode > 0) {
//         String response = http.getString();
//         Serial.print("Server response: ");
//         Serial.println(response);
//       } else {
//         Serial.print("Error on sending POST: ");
//         Serial.println(httpResponseCode);
//       }
//       http.end(); // End connection
//     }
//   delay(600); // Adjust sampling rate (5 samples/sec here)
// }



// }





// void setup() {
//   Serial.begin(9600);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid,password);

//   int attempt=0;

//   Serial.print("Connecting");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//     attempt++;
//     if(attempt==20){
//       ESP.restart();
//     }
//   }
//   Serial.println("\nWiFi connected!");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

