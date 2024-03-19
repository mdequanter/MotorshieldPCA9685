const char* htmlContent = R"html(
<!DOCTYPE html>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Motor en Verkeerslicht Controller</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            padding: 20px;
        }

        .button {
            background-color: #007bff;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            margin: 10px 0;
        }

        .button:hover {
            background-color: #0056b3;
        }

        .light {
            height: 50px;
            width: 50px;
            border-radius: 50%;
            display: inline-block;
            margin: 5px;
        }

        #red, #orange, #green {
            background-color: grey;
        }

        @media (min-width: 600px) {
            .light {
                height: 70px;
                width: 70px;
            }
        }

        @media (min-width: 1000px) {
            .light {
                height: 90px;
                width: 90px;
            }
        }

        .controls {
            margin: 15px 0;
        }

        .range-slider {
            width: 100%;
            margin: 15px 0;
        }

        .range-value {
            display: block;
            text-align: center;
        }
    </style>
</head>
<body>

    <button class="button" onclick="submitState()">Send new settings</button>
    
    <h2>Motor control</h2>
    <div class="controls">
        <button class="button" onclick="setMotorDirection('links')">Links</button>
        <button class="button" onclick="setMotorDirection('vooruit')">Vooruit</button>
         <button class="button" onclick="setMotorDirection('stop')">STOP</button>
        <button class="button" onclick="setMotorDirection('achteruit')">Achteruit</button>
        <button class="button" onclick="setMotorDirection('rechts')">Rechts</button>
        <input type="range" id="motorSpeed" min="0" max="100" value="0" class="range-slider">
        <span id="speedValue" class="range-value">0%</span>
    </div>

    <h2>Servo control</h2>
    <div class="controls">
        <input type="range" id="servoPosition" min="0" max="180" value="90" class="range-slider">
        <span id="positionValue" class="range-value">90</span>
    </div>

    <h2>Lights</h2>
    <div class="controls">
        <button onclick="setTrafficLight('red')" id="red" class="light"></button>
        <button onclick="setTrafficLight('orange')" id="orange" class="light"></button>
        <button onclick="setTrafficLight('green')" id="green" class="light"></button>
    </div>
    <div class="controls">
    <h2>Probe temperature</h2>
    <p>Temperature :  xxxTEMPERATURExxx &deg; Celcius</p>
    <h2>Ultrasonic sensor</h2>
    <p>Distance :  xxxDISTANCExxx cm</p>
    <h2>Battery status</h2>
    <p>Voltage :  xxxINPUTVOLTAGExxx V</p>

    
    </div>

    <script>
        function setMotorDirection(direction) {
            var speed = document.getElementById("motorSpeed").value;
            console.log("Motor " + direction + " met snelheid " + speed + "%");
            updateURL('direction', direction);
            updateURL('speed', speed);
            submitState();

        }

        document.getElementById("motorSpeed").oninput = function() {
            document.getElementById("speedValue").innerHTML = this.value + "%";
            updateURL('speed', this.value);
        }

        document.getElementById("servoPosition").oninput = function() {
            document.getElementById("positionValue").innerHTML = this.value;
            updateURL('servoPosition', this.value);            
        }

        function setTrafficLight(color) {
            document.getElementById("red").style.backgroundColor = "grey";
            document.getElementById("orange").style.backgroundColor = "grey";
            document.getElementById("green").style.backgroundColor = "grey";
            document.getElementById(color).style.backgroundColor = color;            
            console.log("Verkeerslicht " + color);
            updateURL('trafficLight', color);
        }

        function updateURL(key, value) {
            let newUrl = new URL(window.location.href);
            newUrl.searchParams.set(key, value);
            window.history.pushState({path:newUrl.href}, '', newUrl.href);
        }

        function submitState() {
            window.location.reload();
        }

        function applyStateFromURL() {
            let params = new URLSearchParams(window.location.search);
            let speed = params.get('speed');
            let direction = params.get('direction');
            let servoPosition = params.get('servoPosition');
            let trafficLight = params.get('trafficLight');

            if(speed !== null) {
                document.getElementById("motorSpeed").value = speed;
                document.getElementById("speedValue").innerHTML = speed + "%";
            }

            if(servoPosition !== null) {
                document.getElementById("servoPosition").value = servoPosition;
                document.getElementById("positionValue").innerHTML = servoPosition;
            }

            if(trafficLight !== null) {
                setTrafficLight(trafficLight);
            }
        }

        window.onload = applyStateFromURL;
    </script>
</body>
</html>
)html";