#include <MobaTools.h>

#define STEP_PIN1 19
#define DIR_PIN1 18

#define STEP_PIN2 5
#define DIR_PIN2 4

#define STEPS_PER_REV 200

MoToStepper stepperX(STEPS_PER_REV, STEPDIR);
MoToStepper stepperY(STEPS_PER_REV, STEPDIR);

void setup() {
  Serial.begin(115200);

  stepperX.attach(STEP_PIN1, DIR_PIN1);
  stepperY.attach(STEP_PIN2, DIR_PIN2);

  stepperX.setSpeed(200);
  stepperX.setRampLen(50);

  stepperY.setSpeed(200);
  stepperY.setRampLen(50);

  stepperX.setZero(); // Set reference point for X-axis
  stepperY.setZero(); // Set reference point for Y-axis

  Serial.println("Ready for G-code commands");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("G1")) {
      int xPos = command.indexOf('X');
      int yPos = command.indexOf('Y');
      int fPos = command.indexOf('F');

      long xTarget = 0, yTarget = 0;
      int feedRate = 200;

      if (xPos != -1) {
        xTarget = command.substring(xPos + 1, command.indexOf(' ', xPos)).toInt();
      }
      if (yPos != -1) {
        yTarget = command.substring(yPos + 1, command.indexOf(' ', yPos)).toInt();
      }
      if (fPos != -1) {
        feedRate = command.substring(fPos + 1).toInt();
      }

      stepperX.setSpeed(feedRate);
      stepperY.setSpeed(feedRate);

      stepperX.write(xTarget); // Move to X position
      stepperY.write(yTarget); // Move to Y position

      while (stepperX.moving() || stepperY.moving()) {
        delay(10); // Wait for both motors to finish moving
      }

      Serial.println("Movement complete");
    }
  }
}
