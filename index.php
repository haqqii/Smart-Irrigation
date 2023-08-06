<!DOCTYPE HTML>
<html>

<head>
    <title>Gardenly Monitoring</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }

        p {
            font-size: 1.2rem;
        }

        h4 {
            font-size: 0.8rem;
        }

        body {
            margin: 0;
        }

        .topnav {
            overflow: hidden;
            background-color: #0c6980;
            color: white;
            font-size: 1.2rem;
        }

        .content {
            padding: 5px;
        }

        .card {
            background-color: white;
            box-shadow: 0px 0px 10px 1px rgba(140, 140, 140, .5);
            border: 1px solid #0c6980;
            border-radius: 15px;

        }

        .card.header {
            background-color: #0c6980;
            color: white;
            border-bottom-right-radius: 0px;
            border-bottom-left-radius: 0px;
            border-top-right-radius: 12px;
            border-top-left-radius: 12px;
        }

        .cards {
            max-width: 700px;
            margin: 0 auto;
            display: grid;
            grid-gap: 2rem;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
        }

        .reading {
            font-size: 2.2rem;
        }

        .packet {
            color: #bebebe;
        }

        .temperatureColor {
            color: #fd7e14;
        }

        .humidityColor {
            color: #1b78e2;
        }

        .statusreadColor {
            color: #702963;
            font-size: 12px;
        }

        .LEDColor {
            color: #183153;
        }

        /* ----------------------------------- Toggle Switch */
        .switch {
            position: relative;
            display: inline-block;
            width: 70px;
            height: 34px;
        }

        .switch input {
            display: none;
        }

        .sliderTS {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #D3D3D3;
            -webkit-transition: .4s;
            transition: .4s;
            border-radius: 34px;
        }

        .sliderTS:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: #f7f7f7;
            -webkit-transition: .4s;
            transition: .4s;
            border-radius: 50%;
        }

        input:checked+.sliderTS {
            background-color: #17bebb;
        }

        input:focus+.sliderTS {
            box-shadow: 0 0 1px #2196F3;
        }

        input:checked+.sliderTS:before {
            -webkit-transform: translateX(26px);
            -ms-transform: translateX(26px);
            transform: translateX(36px);
        }

        .sliderTS:after {
            content: 'OFF';
            color: white;
            display: block;
            position: absolute;
            transform: translate(-50%, -50%);
            top: 50%;
            left: 70%;
            font-size: 10px;
            font-family: Verdana, sans-serif;
        }

        input:checked+.sliderTS:after {
            left: 25%;
            content: 'ON';
        }

        /* ----------------------------------- */
    </style>
</head>

<body>
    <div class="topnav">
        <h3>GARDENLY MONITORING</h3>
    </div>

    <br>

    <!-- __ DISPLAYS MONITORING AND CONTROLLING ____________________________________________________________________________________________ -->
    <div class="content">
        <div class="cards">
            <?php include('database.php') ?>

            <!-- == MONITORING BOARD 1 ======================================================================================== -->
            <div class="card">
                <div class="card header">
                    <h3 style="font-size: 1rem;">BOARD 1</h3>
                </div>

                <!-- Displays the humidity and temperature values received from ESP32. *** -->
                <h3 class="temperatureColor"><i class="fas fa-thermometer-half"></i> TEMPERATURE</h3>
                <p class="temperatureColor"><span class="reading"><span id="ESP32_01_Temp"><?= $temperature1 ?></span> &deg;C</span></p>
                <!-- <p class="statusreadColor"><span>Status Read Sensor DHT11 : </span><span id="st"></span>
                </p> -->
                <h3 class="humidityColor"><i class="fas fa-tint"></i> KELEMBAPAN TANAH</h3>
                <p class="humidityColor"><span class="reading"><span id="ESP32_01_Humd"><?= $moist1 ?></span> &percnt;</span></p>
                <!-- <p class="statusreadColor"><span>Status Read Sensor DHT11 : </span><span id="ss"></span>
                </p> -->
                <!-- *********************************************************************** -->

            </div>

            <!-- == MONITORING BOARD 2 ======================================================================================== -->
            <div class="card">
                <div class="card header">
                    <h3 style="font-size: 1rem;">BOARD 2</h3>
                </div>

                <!-- Displays the humidity and temperature values received from ESP32. *** -->
                <h3 class="temperatureColor"><i class="fas fa-thermometer-half"></i> TEMPERATURE</h3>
                <p class="temperatureColor"><span class="reading"><span id="ESP32_01_Temp"><?= $temperature2 ?></span> &deg;C</span></p>
                <p class="statusreadColor"><span> </span><span id="st2"></span>
                </p>
                <h3 class="humidityColor"><i class="fas fa-tint"></i> KELEMBAPAN TANAH</h3>
                <p class="humidityColor"><span class="reading"><span id="ESP32_01_Humd"><?= $moist2 ?></span> &percnt;</span></p>
                <p class="statusreadColor"><span> </span><span id="ss2"></span>
                </p>
                <!-- *********************************************************************** -->

            </div>

        </div>
    </div>

    <br>

    <div class="content">
        <div class="cards">
            <div class="card header" style="border-radius: 15px;">
                <h2 style="font-size: 0.7rem;">Rata-Rata Sensor dan Status Pompa
                </h2>
                <button onclick="window.open('record.php', '_blank');">Open Record Table</button>
                <h3 style="font-size: 0.7rem;"></h3>
            </div>
        </div>
    </div>
    <!-- ___________________________________________________________________________________________________________________________________ -->
    <script type="text/javascript">
        function reload() {
            console.log("reloading")
            location.reload();
        }

        setInterval(reload, 10000);
    </script>
</body>

</html>