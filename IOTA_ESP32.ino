// this version is supported only on esp32 boards
#include <WiFiClientSecure.h>
WiFiClientSecure client;
#include <ArduinoJson.h>
#include <string.h>

// Let's Encrypt root cert
const char* le_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
     "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
     "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
     "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
     "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
     "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
     "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
     "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
     "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
     "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
     "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
     "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
     "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
     "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
     "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
     "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
     "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
     "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
     "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
     "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
     "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
     "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
     "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
     "-----END CERTIFICATE-----\n";

// WiFi Credentials - MODIFY MEEEE
const char* ssid = "vivo1718";
const char* password =  "c412ac1492ed";
const char* server = "nodes.devnet.thetangle.org";  // Server URL
const char* address = "LNE9SZAJANDOCBWLHDKJFITWARIUDPXUISLEXHUESDFXJBTCEXT9XOSVK9XCHKVZFABYGJSYI9XUUGNVX";
const char* addressChecksum = "OUEVSOUGKSTMEFUMVLIXNUTKNNRHGAD9CXVBNJEBGFZYJYOHSELJDNWFYLUJNKZRHZMJLECNGIPFQ9CYCDFTNUZN9A";
int paymentsAtAddress = 0;
int iterator = 0;

void setup() 
{
  /// Start Serial
  Serial.begin(115200);
  Serial.println();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  // Set Cert
  client.setCACert(le_root_ca);
  Serial.println("Setup done");

}

void loop() 
{
    // Intialise loop by re/setting counters
    fetchTransactions();
    iterator = paymentsAtAddress;    
    
    // Check for payment
    while (paymentsAtAddress == iterator) {
        // Wait 5 seconds before checking
       delay(5000);
       fetchTransactions();
    }

  // Dispense coffee
  takeCarDown();
}

void fetchTransactions()
{
    // Make a HTTP request:
  char* data1="{\"command\":\"findTransactions\",\"addresses\":[\"";
  char* data2="\"]}";
  char payload[129] = "";
  strcat(payload, data1);
  strcat(payload, address);
  strcat(payload, data2);
  
  Serial.println("\nFetching Transactions...");
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    client.println("POST / HTTP/1.1");
    client.println("Host: nodes.devnet.thetangle.org");
    client.println("X-IOTA-API-Version: 1");
    client.println("Content-Type: application/json");
    client.println("Content-Length: 129");
    client.println();
    client.println(payload);
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        // Headers Received
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      // Parse JSON object
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, client);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      paymentsAtAddress = doc["hashes"].size();

      String output = "";
      serializeJson(doc, output);
      Serial.println(output);
      client.stop();
    }
  }
}


void takeCarDown()
{
  Serial.println("Payment Received.");
  delay(1000);
  Serial.println('D'); 
}
