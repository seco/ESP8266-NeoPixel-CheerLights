#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

// The time to wait between checking the color
#define refreshInterval 15000

const int NUM_LEDS = 12;

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(#LEDs, pin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, D1);

const char ssid[]     = "?????";
const char password[] = "?????";

const char host[] = "api.thingspeak.com";

// The brightness, 1-255
int brightness = 50;

uint32_t currentColor = strip.Color(0, 0, 0);

void setup() {
    Serial.begin(115200);
    Serial.flush();

    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    
    strip.begin();
    strip.show();

    // Set the brightness
    strip.setBrightness(brightness);

    while (WiFi.status() != WL_CONNECTED) {
        cycleLed(40);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/channels/1417/field/2/last.txt";

    Serial.println("Sending request");
    
    // This will send the request to the server
    client.print(String("GET ") + url +
            " HTTP/1.1\r\n" +
            "Host: " + host + "\r\n" +
            "Connection: close\r\n\r\n");

    Serial.println("Reading reply");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Client Timeout !");
        client.stop();
        return;
      }
    }

    // Read all the lines of the reply from server and scan for hex color
    while(client.available() > 0) {
        unsigned long color;
        String line = client.readStringUntil('\n');

        if ((line[0] == '#') && (line.length() == 8)) {
          Serial.println("Color Received: ");
          Serial.println(line);
    
          // Get rid of '#' and convert it to integer
          int number = (int) strtol(&line[1], NULL, 16);
          
          // Split the hex value up into r, g, b values
          int r = number >> 16;
          int g = number >> 8 & 0xFF;
          int b = number & 0xFF;
          
          uint32_t newColor = strip.Color(r,g,b);
          
          // Change the color
          fadeUpdate(currentColor, newColor, 15);

          currentColor = newColor;
      }
    }
    delay(refreshInterval);
}

