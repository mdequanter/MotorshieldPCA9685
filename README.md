<h1>PCA9685 motorshield,  led, servo controller</h1>
<p>This code is used to make optimal use of the PCA9685 chip (16-channel, 12-bit PWM Fm+ I2C-bus LED controller) and its IC2 capabilities.  The PCA9685 is also used to drive a motorcontroller chip, for instance the DRV8847PWR. 
  Therefore a PCB was build that has a PCA9685 and also 2 DRV8847PWR (18-V, 2-A dual H-bridge motor driver with current regulation and independent half-bridge con). This way up to 6 servo's or LEDS can be controlled via PWM and also max 4 motors can be connected.
With the scipts it should be fairly easy to implement a complete motor/servo/LED control solution.</p>

![image](https://github.com/mdequanter/MotorshieldPCA9685/assets/74420584/766f8490-4b70-43dd-9f21-587acaa2c8e1)


<h2>Code Description</h2>

<h3>Key Components of the Code</h3>
<p><b>Importing Libraries:</b> Includes WiFi management, web server functionality, mDNS support, and custom libraries for hardware control.</p>
<p><b>WiFi Network Settings:</b> Defines the SSID and password for the WiFi connection.</p>
<p><b>WebServer and MotorShield Instances:</b> Creates instances for web communication and hardware control.</p>

<h3>Function <u>handleRoot()</u></h3>
<p>Processes HTTP requests with parameters for hardware control and sends HTML content back to the client.</p>

<h3>Setup Function</h3>
<p>Starts serial communication, initializes the MotorShield, connects to WiFi, and sets up webserver routes.</p>

<h3>Loop Function</h3>
<p>Manages incoming web requests for continuous interaction with users.</p>

<h3>Operation and Functionalities</h3>
<p><b>Motor Control:</b> Adjusting direction and speed through the web interface.</p>
<p><b>Servo Control:</b> Position adjustment by users.</p>
<p><b>LED Control:</b> Controlling LEDs for traffic light simulation.</p>
<p><b>Web Interface:</b> HTML interface for interacting with the hardware.</p>


<h2>Webpage that is running on the ESP32</h2>

![image](https://github.com/mdequanter/MotorshieldPCA9685/assets/74420584/fed0e65e-3fb8-4675-8889-a53a324c08a0)
