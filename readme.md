# Monitoring Smart Irrigation Using Fuzzy Logic

Repository ini berisi tentang sistem monitoring irigasi dengan web menggunakan parameter kelembapan tanah dan temperature yang diklasifikasikan menggunakan algoritma fuzzy logic untuk mengatur keluar masuknya air pada pompa ke lahan pertanian

hal-hal yang perlu diperhatikan:
1. Pastikan sudah mendownload library yang digunakan

```
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
```

2. Pada file sender maupun sender2 sesuaikan dengan wifi dan mysql yang kamu gunakan
```
// Insert your SSID
const char *ssid = "YOUR NAME WIFI";
const char *password = "YOUR PASS WIFI";

// MySQL Connection parameters
IPAddress serverIP(192, 168, 43, 116); // IP address of the MySQL server
char user[] = "root";             // MySQL username
char passwordDB[] = "";      // MySQL password
char db[] = "irrigation";             // MySQL database name
unsigned int port = 3306;             // MySQL port (default: 3306)
```
3. Pada file database.php sesuaikan dengan nama database yang kamu gunakan

-----------------------------------------

This repository is Web-based irrigation monitoring system using soil moisture and temperature parameters that are classified using fuzzy logic algorithms to regulate the entry and exit of water in the pump to the farm.

notes:
1. Make sure you have downloaded the library used
```
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
```
2. In the sender and sender2 files adjust the wifi and mysql that you use

```
// Insert your SSID
const char *ssid = "YOUR NAME WIFI";
const char *password = "YOUR PASS WIFI";

// MySQL Connection parameters
IPAddress serverIP(192, 168, 43, 116); // IP address of the MySQL server
char user[] = "root";             // MySQL username
char passwordDB[] = "";      // MySQL password
char db[] = "irrigation";             // MySQL database name
unsigned int port = 3306;             // MySQL port (default: 3306)
``` 

3. In the database.php file adjust the database name you are using
