#include <esp_now.h>
#include <WiFi.h>

// Library MySQL Connector
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

const char *ssid = "hehe";
const char *password = "aksarajawa";

const int relayPin = 27;
const char *statusTemp;
const char *statusMois;
const char *ruleFuzzy;
const char *action;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
  int id;
  float tempr;
  int mois;
} struct_message;

struct_message incomingReadings;
// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

IPAddress serverIP(192, 168, 43, 116); // IP address of the MySQL server
char user[] = "new_user";              // MySQL username
char passwordDB[] = "localhost";       // MySQL password
char db[] = "irrigation";              // MySQL database name
unsigned int port = 3306;              // MySQL port (default: 3306)

WiFiClient wifiClient;
MySQL_Connection conn((Client *)&wifiClient);

void data_receive(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  char macStr[18];
  Serial.print("Sensor Readings received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

  boardsStruct[incomingReadings.id - 1].tempr = incomingReadings.tempr;
  boardsStruct[incomingReadings.id - 1].mois = incomingReadings.mois;

  Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  Serial.printf("t value: %4.2f \n", boardsStruct[incomingReadings.id - 1].tempr);
  Serial.printf("moisture value: %d ", boardsStruct[incomingReadings.id - 1].mois);

  // Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  // Serial.printf("Temperature value: %4.2f \n", incomingReadings.tempr);
  // Serial.printf("Moisture value: %d \n", incomingReadings.mois);
  Serial.println();
}

void setup()
{

  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  pinMode(relayPin, OUTPUT);
  // digitalWrite(relayPin, HIGH);

  WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1000);
  //   Serial.println("Setting as a Wi-Fi Station..");
  // }
  // Serial.print("Station IP Address: ");
  // Serial.println(WiFi.localIP());
  // Serial.print("Wi-Fi Channel: ");
  // Serial.println(WiFi.channel());

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(data_receive);

  if (conn.connect(serverIP, 3306, user, passwordDB))
  {
    Serial.println("Connected to MySQL server");
  }
  else
  {
    Serial.println("MySQL connection failed");
  }
}

void insertDataToMySQL(float avgTemp, float avgMois, String action)
{
  MySQL_Cursor *cursor = new MySQL_Cursor(&conn);

  char query[128];
  snprintf(query, sizeof(query), "INSERT INTO %s.average (avg_temp, avg_mois, defuzzy) VALUES (%f, %f, '%s')", db, avgTemp, avgMois, action.c_str());

  cursor->execute(query);

  delete cursor;
}

void loop()
{
  // Calculate average temperature and moisture
  float avgTemperature = averageTemp();
  float avgMoisture = averageMois();

  // Apply fuzzy logic to determine linguistic terms for temperature and moisture
  fuzzifyTemp();
  fuzzifyMois();

  // Determine the appropriate fuzzy rule
  fuzzyRule();

  // Get the action to be taken based on the fuzzy rule
  String action = defuzzy();

  // Insert data into the MySQL database
  insertDataToMySQL(avgTemperature, avgMoisture, action);

  Serial.printf("\nAverage temperature: %4.2f \n", avgTemperature);
  Serial.printf("Average moisture: %4.2f \n", avgMoisture);
  Serial.printf("Action: %s \n", action.c_str());

  delay(10000);
}

float averageTemp()
{
  float sumT = 0;
  for (int i = 0; i < 2; i++)
  {
    sumT += boardsStruct[i].tempr;
  }
  float avgT = sumT / 2;
  return avgT;
}

float averageMois()
{
  float sumM = 0;
  for (int i = 0; i < 2; i++)
  {
    sumM += boardsStruct[i].mois;
  }
  float avgM = sumM / 2;
  return avgM;
}

void fuzzifyTemp()
{
  float temperat = averageTemp();
  if (temperat >= 0 && temperat <= 24)
  {
    statusTemp = "Dingin";
  }
  else if (temperat >= 25 && temperat <= 32)
  {
    statusTemp = "Normal";
  }
  else
  {
    statusTemp = "Panas";
  }
}

void fuzzifyMois()
{
  float moistu = averageMois();
  if (moistu >= 0 && moistu <= 40)
  {
    statusMois = "Kering";
  }
  else if (moistu >= 40 && moistu <= 70)
  {
    statusMois = "Lembab";
  }
  else
  {
    statusMois = "Basah";
  }
}

void fuzzyRule()
{
  if (statusTemp == "Dingin" && statusMois == "Kering")
  {
    ruleFuzzy = "Rule00";
  }
  else if (statusTemp == "Dingin" && statusMois == "Lembab")
  {
    ruleFuzzy = "Rule01";
  }
  else if (statusTemp == "Dingin" && statusMois == "Basah")
  {
    ruleFuzzy = "Rule02";
  }
  else if (statusTemp == "Normal" && statusMois == "Kering")
  {
    ruleFuzzy = "Rule20";
  }
  else if (statusTemp == "Normal" && statusMois == "Lembab")
  {
    ruleFuzzy = "Rule21";
  }
  else if (statusTemp == "Normal" && statusMois == "Basah")
  {
    ruleFuzzy = "Rule22";
  }
  else if (statusTemp == "Panas" && statusMois == "Kering")
  {
    ruleFuzzy = "Rule40";
  }
  else if (statusTemp == "Panas" && statusMois == "Lembab")
  {
    ruleFuzzy = "Rule41";
  }
  else if (statusTemp == "Panas" && statusMois == "Basah")
  {
    ruleFuzzy = "Rule42";
  }
  // else
  // {
  //   ruleFuzzy = "DefaultRule";
  // }
}

String defuzzy()
{
  // fuzzyRule();
  // String action;
  if (ruleFuzzy == "Rule00")
  {
    digitalWrite(relayPin, LOW);
    action = "Pompa Menyala";
  }
  else if (ruleFuzzy == "Rule01")
  {
    digitalWrite(relayPin, LOW);
    action = "Pompa Menyala";
  }
  else if (ruleFuzzy == "Rule02")
  {
    digitalWrite(relayPin, HIGH);
    action = "Pompa Mati";
  }
  else if (ruleFuzzy == "Rule20")
  {
    digitalWrite(relayPin, LOW);
    action = "Pompa Menyala";
  }
  else if (ruleFuzzy == "Rule21")
  {
    digitalWrite(relayPin, LOW);
    action = "Pompa Menyala";
  }
  else if (ruleFuzzy == "Rule22")
  {
    digitalWrite(relayPin, LOW); // mati
    action = "Pompa Mati";
  }
  else if (ruleFuzzy == "Rule40")
  {
    digitalWrite(relayPin, HIGH); // mati
    action = "Pompa Mati";
  }
  else if (ruleFuzzy == "Rule41")
  {
    digitalWrite(relayPin, LOW);
    action = "Pompa Menyala";
  }
  else if (ruleFuzzy == "Rule42")
  {
    digitalWrite(relayPin, HIGH); // mati
    action = "Pompa Mati";
  }

  return action;
}