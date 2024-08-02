#include <SPI.h>
#include <mcp2515.h>

const int speedSensorPin = 3;
volatile int speedPulses = 0;

struct can_frame canMsg;
MCP2515 mcp2515(10);

const float distancePerPulse = 1.0367;
const int movingAverageSize = 10;

float speedBuffer[movingAverageSize];
int bufferIndex = 0;
float speedSum = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(speedSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(speedSensorPin), countPulses, RISING);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  for (int i = 0; i < movingAverageSize; i++) {a
    speedBuffer[i] = 0.0;
  }
}

void loop() {
  static unsigned long lastSendTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime > 100) {
    lastSendTime = currentTime;

    float speedCmPerS = speedPulses * distancePerPulse * 10;

    speedSum -= speedBuffer[bufferIndex];
    speedBuffer[bufferIndex] = speedCmPerS;
    speedSum += speedCmPerS;
    bufferIndex = (bufferIndex + 1) % movingAverageSize;

    float averageSpeedCmPerS = speedSum / movingAverageSize;

    canMsg.can_id  = 0x100;
    canMsg.can_dlc = sizeof(averageSpeedCmPerS);
    memcpy(canMsg.data, &averageSpeedCmPerS, sizeof(averageSpeedCmPerS));

    mcp2515.sendMessage(&canMsg);
    Serial.print("Speed (cm/s): ");
    Serial.println(averageSpeedCmPerS, 2);

    speedPulses = 0;
  }
}

void countPulses() {
  speedPulses++;
}
