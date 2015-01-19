#include <RFduinoBLE.h>
#include <Servo.h>

#define RED_LED_PIN   2
#define GREEN_LED_PIN 3
#define BLUE_LED_PIN  4

Servo servo1;

// interval between advertisement transmissions ms (range is 20ms to 10.24s) - default 20ms
int interval = 500;  // 675 ms between advertisement transmissions
int pos = 0;

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
	servo1.attach(1);

	Serial.begin(9600);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "ledbtn";
  RFduinoBLE.advertisementInterval = interval;
  RFduinoBLE.deviceName = "JBLEAdv";
  RFduinoBLE.txPowerLevel = -20;

  // change the advertisement interval
  RFduinoBLE.advertisementInterval = interval;

  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
	// switch to lower power mode
	RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
  // off if we stop advertisement
  
  if (start) {
		SetLEDToReadyMode();
  } else {
		SetLEDToOffMode();
	}
}

void RFduinoBLE_onConnect()
{
	Serial.println("RFduino BLE connection successful");
	SetLEDToConnectedMode();
}

void RFduinoBLE_onDisconnect()
{
	Serial.println("RFduino BLE disconnected");
	SetLEDToReadyMode();
}

void RFduinoBLE_onReceive(char *data, int len)
{
	// If the first byte is 0x01 / on / true
	Serial.println("Received data over BLE");
	if (data[0])
	{
		SetLEDToToggledMode();
		ToggleMotor(int(data[0]));
		Serial.println("Turn RFduino Blue LED On");
	}
	else {
		SetLEDToConnectedMode();
		Serial.println("Turn RFduino Blue LED Off");
	}
}

void SetLEDToConnectedMode() {
	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN_LED_PIN, HIGH);
	digitalWrite(BLUE_LED_PIN, LOW);
}

void SetLEDToReadyMode() {
	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN_LED_PIN, LOW);
	digitalWrite(BLUE_LED_PIN, HIGH);
}

void SetLEDToOffMode() {
	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN_LED_PIN, LOW);
	digitalWrite(BLUE_LED_PIN, LOW);
}

void SetLEDToToggledMode() {
	digitalWrite(RED_LED_PIN, HIGH);
	digitalWrite(GREEN_LED_PIN, LOW);
	digitalWrite(BLUE_LED_PIN, LOW);
}

void ToggleMotor(int val) {
	Serial.println(val);
	if (val == 1) {
		for(pos = 0; pos < 180; pos += 1) {
			servo1.write(pos);           // sets the servo 1 position according to the scaled value 
			delay(15);                       // waits 15ms for the servo to reach the position 
		} 
	} else if (val == 2) {
		for(pos = 180; pos >= 1; pos -= 1) {
			servo1.write(pos);           // sets the servo 1 position according to the scaled value 
			delay(15);                       // waits 15ms for the servo to reach the position 
		} 
	}
}
