#include <SPI.h>
#include <mcp2515.h>

const int speedSensorPin = 3;
volatile int speedPulses = 0;

struct can_frame canMsg;
MCP2515 mcp2515(10);

const float distancePerPulse = 1.0367;

void setup() {
    Serial.begin(9600);
    pinMode(speedSensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedSensorPin), countPulses, RISING);
    SPI.begin();
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
}

void loop() {
    static unsigned long lastSendTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime > 100) {
        lastSendTime = currentTime;

        float speedCmPerS = speedPulses * distancePerPulse * 10;

        canMsg.can_id  = 0x100;
        canMsg.can_dlc = sizeof(speedCmPerS);
        memcpy(canMsg.data, &speedCmPerS, sizeof(speedCmPerS));

        mcp2515.sendMessage(&canMsg);
        Serial.print("Speed (cm/s): ");
        Serial.println(speedCmPerS, 2);

        speedPulses = 0;
    }
}

void countPulses() {
    speedPulses++;
}
