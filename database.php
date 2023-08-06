<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "irrigation";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sensor1 = $conn->query("SELECT temperature, moisture FROM data ORDER BY id DESC LIMIT 1");
$sensor2 = $conn->query("SELECT temperature, moisture FROM data_sensor2 ORDER BY id DESC LIMIT 1");

if ($sensor1->num_rows > 0) {
  // output data of each row
  while ($row = $sensor1->fetch_assoc()) {
    $temperature1 = $row["temperature"];
    $moist1 = $row["moisture"];
  }
} else {
  echo "sensor 1 : 0 results";
  $temperature1 = "NaN";
  $moist1 = "NaN";
}

if ($sensor2->num_rows > 0) {
  // output data of each row
  while ($row = $sensor2->fetch_assoc()) {
    $temperature2 = $row["temperature"];
    $moist2 = $row["moisture"];
  }
} else {
  echo "sensor 2 : 0 results";
  $temperature2 = "NaN";
  $moist2 = "NaN";
}

$record_table = $conn->query("SELECT * FROM average");

if ($record_table->num_rows > 0) {
  // output data of each row
  $data_record = $record_table;
} else {
  echo "data record : 0 results";
  $data_record = "No Data";
}

$conn->close();
