#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPIFFS.h>

WebServer server(80);

const char* ssid = "MQTT";
const char* password = "test0000";
const int LEDPIN = 2;
const int LEDRED = 4;
const int LEDGREEN = 15;
// Define the pins for the extra LEDs
const int ledPins[] = {13,12,14,27,26,25,32,33}; 


String htmlPage;

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleMeasurement() {
  // Extract the wall type from the form
  String wallType = server.arg("wallType");

  // Perform the measurement logic here
  const int numMeasurements = 10;
  int totalSignalStrength = 0;

  // Display the header and div for measurements
  String response = "<br>Measurement started for wall type: " + wallType;
  response += "<br><h3>Measurement Results:</h3><div id='measurementResults'>";



  for (int i = 0; i < numMeasurements; ++i) {
    // Get the RSSI value
    int signalStrength = WiFi.RSSI();

    // Update LEDs based on signal strength
    updateLEDs(signalStrength, ledPins);

    // Print individual measurements to Serial Monitor
    Serial.println("Measurement " + String(i + 1) + ": " + String(signalStrength) + " dBm");

    // Accumulate total signal strength for average calculation
    totalSignalStrength += signalStrength;

    // Add individual measurement to the response
    response += "Measurement " + String(i + 1) + ": " + String(signalStrength) + " dBm<br>";

    // Simulate a delay between measurements
    delay(1000);
  }

  // Calculate the average signal strength
  float averageSignalStrength = static_cast<float>(totalSignalStrength) / numMeasurements;

  // Display the average on the web page
  response += "<br>Measurement complete. Average Signal Strength: " + String(averageSignalStrength) + " dBm<br>";

  // Save the average signal strength to a file
  saveToSPIFFS(wallType, averageSignalStrength, numMeasurements);

  // Add a link to download the saved file
  response += "Results saved. <a href='/download?wallType=" + wallType + "'>Download</a><br>";

  // Finish the response
  response += "<a href='/'>Back to Home</a></div>";
  server.send(200, "text/html", response);

  // Turn off all LEDs at the end of the measurement
  updateLEDs(0, ledPins);
}

void updateLEDs(int signalStrength, const int ledPins[]) {
  // Determine the LED group based on signal strength
  int ledGroup = signalStrength / 10;  // Group every 10 dBm

  // Turn off all LEDs first
  for (int i = 0; i < 8; ++i) {
    digitalWrite(ledPins[i], LOW);
  }

  // Turn on LEDs in the current group
  for (int i = 0; i <= ledGroup && i < 8; ++i) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void saveToSPIFFS(const String& wallType, float averageSignalStrength, int numMeasurements) {
  File file = SPIFFS.open("/" + wallType + ".txt", "a");  // Open the file in append mode
  if (file) {
    file.print("Wall Type: " + wallType + "\n");
    file.print("Average Signal Strength: " + String(averageSignalStrength) + " dBm\n");

    // Append individual measurements to the file
    for (int i = 0; i < numMeasurements; ++i) {
      int signalStrength = WiFi.RSSI();
      file.print("Measurement " + String(i + 1) + ": " + String(signalStrength) + " dBm\n");

      // Simulate a delay between measurements
      delay(1000);
    }

    file.close();
    Serial.println("Results saved to file: " + wallType + ".txt");
  } else {
    Serial.println("Error opening file for writing");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    digitalWrite(LEDRED, HIGH);
    delay(250);
    digitalWrite(LEDRED, LOW);
    delay(250);
    digitalWrite(LEDRED, HIGH);
    delay(250);
    digitalWrite(LEDRED, LOW);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    
    return;
  }

  htmlPage = "<html lang=en-EN><head><meta charset='UTF-8'><title>WiFi Measurement</title></head>";
  htmlPage += "<body><h1>WiFi Measurement</h1>";
  htmlPage += "<form action='/start-measurement' method='POST'>";
  htmlPage += "<label for='wallType'>Wall Type:</label>";
  htmlPage += "<input type='text' id='wallType' name='wallType' required><br><br>";
  htmlPage += "<input type='submit' value='Start Measurement'>";
  htmlPage += "</form>";
  htmlPage += "</body></html>";

  server.on("/", handleRoot);
  server.on("/start-measurement", HTTP_POST, handleMeasurement);
  server.on("/download", HTTP_GET, handleDownload);
  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LEDPIN, HIGH);
}

void loop() {
  server.handleClient();
}

void handleDownload() {
  String wallType = server.arg("wallType");
  server.sendHeader("Content-Disposition", "attachment; filename=" + wallType + ".txt");
  server.send(200, "text/plain", getFileContent(wallType));
}

String getFileContent(const String& wallType) {
  String content = "";

  File file = SPIFFS.open("/" + wallType + ".txt", "r");
  if (file) {
    while (file.available()) {
      content += char(file.read());
    }
    file.close();
  } else {
    content = "Error opening file for reading";
  }

  return content;
}
