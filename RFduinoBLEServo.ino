#include <RFduinoBLE.h>
#include <Servo.h>

Servo servo1;

// interval between advertisement transmissions ms (range is 20ms to 10.24s) - default 20ms
int interval = 500;  // 675 ms between advertisement transmissions
int pos = 0;

void setup() {
  // led used to indicate that the RFduino is advertising
	servo1.attach(3);
	Serial.begin(9600);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "ledbtn";
  RFduinoBLE.advertisementInterval = interval;
  RFduinoBLE.deviceName = "JBLEServo";
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
  if (start) {
		Serial.println("RFduino BLE advertising started");
	} else {
		Serial.println("RFduino BLE advertising stopped");
	}
}

void RFduinoBLE_onConnect()
{
	Serial.println("RFduino BLE connection successful");
}

void RFduinoBLE_onDisconnect()
{
	Serial.println("RFduino BLE disconnected");
}

void RFduinoBLE_onReceive(char *data, int len)
{
	// If the first byte is 0x01 / on / true
	Serial.println("Received data over BLE");
	Serial.println(int(data[0]));
	if (data[0])
	{
		ToggleMotor(int(data[0]));
		Serial.println("Toggle servo");
	}
	else {
		Serial.println("Do Nothing");
	}
}

void ToggleMotor(int val) {
	Serial.println(val);
	if (val == 1) {
		for(pos = 45; pos < 135; pos += 1) {
			servo1.write(pos);           // sets the servo 1 position according to the scaled value 
			delay(10);                       // waits 15ms for the servo to reach the position 
		} 
	} else if (val == 2) {
		for(pos = 135; pos >= 45; pos -= 1) {
			servo1.write(pos);           // sets the servo 1 position according to the scaled value 
			delay(10);                       // waits 15ms for the servo to reach the position 
		} 
	}
}
