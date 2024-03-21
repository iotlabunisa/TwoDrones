#include <Arduino_LSM6DS3.h>
#include <Math.h>
#include <MadgwickAHRS.h>
#include <PID_v1.h>
#include <WiFiNINA.h>
#define LED_PIN 13

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

// Motor C connections
int enC = 6;
int in5 = 1;
int in6 = 2;

// Motor D connections
int enD = 10;
int in7 = 11;
int in8 = 12;

// Pitch and Roll
double rollSetpoint, rollInput, rollOutput;
double pitchSetpoint, pitchInput, pitchOutput;

//Define the aggressive and conservative Tuning Parameters
double consKp = 0.5, consKi = 0.05, consKd = 0.05;
int targetSpeed[4];
PID pitchPID(&rollInput, &rollOutput, &rollSetpoint, consKp, consKi, consKd, DIRECT);
PID rollPID(&pitchInput, &pitchOutput, &pitchSetpoint, consKp, consKi, consKd, DIRECT);
Madgwick filter;

// Connection section
const char* ssid = "WiFi-LabIoT";
const char* password = "s1jzsjkw5b";
const uint16_t port = 8000;
WiFiServer wifiServer(port);


void setup() {
  filter.begin(10);
  Serial.begin(9600);
  pitchInput = 0.0;
  rollInput = 0.0;  
  pitchSetpoint = 0.0;
  rollSetpoint = 0.0;
  pitchPID.SetMode(AUTOMATIC);
  rollPID.SetMode(AUTOMATIC);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid,password);

  if (WiFi.status() == WL_NO_MODULE){
    Serial.println("Communication failed!");
    while(true);
  }

  /*Serial.println("\n");
  Serial.println("Connected!");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  */
  
  wifiServer.begin();

  pitchPID.SetOutputLimits(-20, 20);
  rollPID.SetOutputLimits(-20, 20);
  for (int i = 0; i < 4; i++) {
    targetSpeed[i] = 0;
  } 

	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(enC, OUTPUT);
	pinMode(enD, OUTPUT);
 
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(in5, OUTPUT);
	pinMode(in6, OUTPUT);
	pinMode(in7, OUTPUT);
	pinMode(in8, OUTPUT);

	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
	digitalWrite(in5, LOW);
	digitalWrite(in6, LOW);
	digitalWrite(in7, LOW);
	digitalWrite(in8, LOW);

  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }
	
  /*
  Serial.print("Accelerometer reading...");
  Serial.print("Gyroscope reading...");
  Serial.print("Temperature reading in Celsius...");
  Serial.println();
  */
}

void loop() {
  char buffer[5];
  float x_acc, y_acc, z_acc;
  float x_gyro, y_gyro, z_gyro;
  float t;
  WiFiClient client = wifiServer.available();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
  digitalWrite(LED_PIN, HIGH);
  
  // Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
 
	digitalWrite(in5, LOW);
	digitalWrite(in6, HIGH);
	digitalWrite(in7, LOW);
	digitalWrite(in8, HIGH);
	
	for (int i = 0; i < 4; i++) {
      targetSpeed[i] = 25;
    }
	
	if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable()) {
    //Serial.println("Client remoto connesso");
    IMU.readGyroscope(x_gyro, y_gyro, z_gyro);
    IMU.readAcceleration(x_acc, y_acc, z_acc);
    filter.updateIMU(x_gyro, y_gyro, z_gyro, x_acc, y_acc, z_acc);
	  t = (IMU.readTemperature(t)/512.0)+23;
	
    String rollInputString = (dtostrf(filter.getRoll(), 4, 0, buffer));
    String pitchInputString = (dtostrf(filter.getPitch(), 4, 0, buffer));
	
    rollInput = atof(rollInputString.c_str());
    pitchInput = atof(pitchInputString.c_str());

    pitchPID.Compute();
    rollPID.Compute();
    int actSpeed[4];
    stabilise (targetSpeed, actSpeed, rollOutput, pitchOutput);
	
	  // Invio dei valori
	  wifiServer.print(String(x_acc)+";"+String(y_acc)+";"+String(z_acc)+";"
                   +String(x_gyro)+";"+String(y_gyro)+";"+String(z_gyro)+";"+String(t)+";"
				           +String(actSpeed[0])+";"+String(actSpeed[1])+";"+String(actSpeed[2])+";"+String(actSpeed[3]) + ";" + String(rollInput) + ";" + String(pitchInput) + "a");

    //wifiServer.print(String(rollInput) + ";" + String(pitchInput));
    
	}
	delay(50);
  
}

void stabilise (int* currSpeed, int* actSpeed, float rollDiff, float pitchDiff) {
  //actual Speed is calculated as follows +- half rollDiff +- half pitchDiff
  /*
  actSpeed[0] = (int) currSpeed[0] + (rollDiff / 2) - (pitchDiff / 2);
  actSpeed[1] = (int) currSpeed[1] + (rollDiff / 2) + (pitchDiff / 2);
  actSpeed[2] = (int) currSpeed[2] - (rollDiff / 2) + (pitchDiff / 2);
  actSpeed[3] = (int) currSpeed[3] - (rollDiff / 2) - (pitchDiff / 2);
  */

  actSpeed[0] = (int) currSpeed[0] - (rollDiff / 2) - (pitchDiff / 2);
  actSpeed[1] = (int) currSpeed[1] - (rollDiff / 2) + (pitchDiff / 2);
  actSpeed[2] = (int) currSpeed[2] + (rollDiff / 2) - (pitchDiff / 2);
  actSpeed[3] = (int) currSpeed[3] + (rollDiff / 2) + (pitchDiff / 2);
  
  analogWrite(enA, actSpeed[0]);
  analogWrite(enB, actSpeed[1]);
  analogWrite(enC, actSpeed[2]);
  analogWrite(enD, actSpeed[3]);
  
  for (int i = 0; i < 4; i ++) {
    if (actSpeed[i] < 0) 
      actSpeed[i] = 0;  
  }
}