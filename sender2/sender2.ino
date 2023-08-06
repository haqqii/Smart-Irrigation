// Library ESP-NOW
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

// Library Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Library MySQL Connector
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 2

// Digital pin connected to the sensor
#define DHTPIN 13
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int sensor_analog, moisture;
const int SOILPIN = 32;

// MAC Address of the receiver
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0x6A, 0x1D, 0x78};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    int id;
    float tempr;
    int mois;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0; // millis() returns an unsigned long.
const long interval = 10000;      // the time we need to wait

// Insert your SSID
const char *ssid = "hehe";
const char *password = "aksarajawa";

int32_t getWiFiChannel(const char *ssid)
{
    if (int32_t n = WiFi.scanNetworks())
    {
        for (uint8_t i = 0; i < n; i++)
        {
            if (!strcmp(ssid, WiFi.SSID(i).c_str()))
            {
                return WiFi.channel(i);
            }
        }
    }
    return 0;
}

float readTemperature()
{
    float t = dht.readTemperature();

    if (isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        return 0;
    }
    else
    {
        Serial.println(t);
        return t;
    }
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// MySQL Connection parameters
IPAddress serverIP(192, 168, 43, 116); // IP address of the MySQL server
char user[] = "new_user";             // MySQL username
char passwordDB[] = "localhost";      // MySQL password
char db[] = "irrigation";             // MySQL database name
unsigned int port = 3306;             // MySQL port (default: 3306)

WiFiClient wifiClient;
MySQL_Connection conn((Client *)&wifiClient);

void setup()
{
    // Init Serial Monitor
    Serial.begin(115200);

    dht.begin();
    WiFi.begin(ssid, password);

    // Set device as a Wi-Fi Station and set channel
    WiFi.mode(WIFI_STA);

    int32_t channel = getWiFiChannel(ssid);

    WiFi.printDiag(Serial); // Uncomment to verify channel number before
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    WiFi.printDiag(Serial); // Uncomment to verify channel change after

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Transmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    // Establish MySQL connection
    if (conn.connect(serverIP, 3306, user, passwordDB))
    {
        Serial.println("Connected to MySQL server successfully!");
    }
    else
    {
        Serial.println("Connection to MySQL server failed.");
        return;
    }
}

void loop()
{
    sensor_analog = analogRead(SOILPIN);
    moisture = (100 - ((sensor_analog / 4095.00) * 100));
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        // Save the last time a new reading was published
        previousMillis = currentMillis;
        // Set values to send
        myData.id = BOARD_ID;
        myData.tempr = readTemperature();
        myData.mois = moisture;

        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
        if (result == ESP_OK)
        {
            Serial.println(myData.mois);
            Serial.println("Sent with success");

            // Check if MySQL connection is active
            if (conn.connected())
            {
                // Create an SQL INSERT query
                char query[128];
                snprintf(query, sizeof(query), "INSERT INTO %s.data_sensor2 (id_board, temperature, moisture) VALUES (%d, %.2f, %d)", db, myData.id, myData.tempr, myData.mois);

                // Create a MySQL cursor object
                MySQL_Cursor *cursor = new MySQL_Cursor(&conn);

                // Execute the SQL query
                cursor->execute(query);

                // Clean up
                delete cursor;
                conn.close();
                Serial.println("Data saved to MySQL!");
            }
            else
            {
                Serial.println("MySQL connection lost. Reconnecting...");
                if (conn.connect(serverIP, port, user, passwordDB))
                {
                    Serial.println("Reconnected to MySQL server successfully!");
                }
                else
                {
                    Serial.println("Reconnection to MySQL server failed.");
                }
            }
        }
        else
        {
            Serial.println("Error sending the data");
        }
    }
}
