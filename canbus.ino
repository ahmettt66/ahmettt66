#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg; //for read
struct can_frame canSens; //for read
struct can_frame canMpu; //for mpu6050
struct can_frame canBase; //for basemodule
struct can_frame canMS; //for MS5837


union ArrayToInteger {
  byte array[2];
  int integer;
} converter;

struct CanBusCommands {
  int surge, sway, heave, yaw, Error, Emergency, servoangle, gear, task, emergencybtn;
};

int PrevSurge, PrevSway, PrevHeave, PrevYaw = 0; int PrevServoangle, PrevGear, PrevTask, PrevEmergencybtn, PrevError, depth, pressure= 0; //for save coming value and data
bool battarystate=false;
int watertemp=0;
unsigned long canbus_last_communication_time = millis();
unsigned long sendMessage=0;
unsigned long printMessage=0;
MCP2515 mcp2515(8);



void initializeCanBus() {
  mcp2515.reset();
  mcp2515.setBitrate(SETBITRATE);
  mcp2515.setNormalMode();
}

struct CanBusCommands GetCanBusCommands() {
  struct CanBusCommands cmd;
  cmd.surge = PrevSurge;
  cmd.sway = PrevSway;
  cmd.heave = PrevHeave;
  cmd.yaw = PrevYaw;
  cmd.servoangle = PrevServoangle;
  cmd.gear = PrevGear;
  cmd.task = PrevTask;
  cmd.emergencybtn = PrevEmergencybtn;

  cmd.Error = false;

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {


    if (canMsg.can_id == 0x1B) //ID Station software tarafinda belirlenir. Station software tarafinda high ve low byte olarak 2 byta siralanip gönderilmis degerler burda integer olarak donusturulur.
    {
      for (int i = 0; i < canMsg.can_dlc; i += 2)
      {
        converter.array[1] = canMsg.data[i];
        converter.array[0] = canMsg.data[i + 1];
        if (i == 0)
          cmd.heave = converter.integer;
        else if (i == 2)
          cmd.yaw = converter.integer;
        else if (i == 4)
          cmd.surge = converter.integer;
        else if (i == 6)
          cmd.sway = converter.integer;


        PrevSurge = cmd.surge;
        PrevSway = cmd.sway;
        PrevHeave = cmd.heave;
        PrevYaw = cmd.yaw;
      }
    }

    canbus_last_communication_time = millis();

  }


  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {


    if (canMsg.can_id == 0x20B)
    {
      cmd.servoangle = canMsg.data[0];
      cmd.gear = canMsg.data[1];
      cmd.task = canMsg.data[2];
      cmd.emergencybtn = canMsg.data[3];


      PrevServoangle = cmd.servoangle;
      PrevGear = cmd.gear;
      PrevTask = cmd.task;
      PrevEmergencybtn = cmd.emergencybtn;

    }
  }

  else if (millis() - CANBUS_COMMUNICATION_TIMEOUT > canbus_last_communication_time) {

    cmd.Error = true;
    PrevError = cmd.Error;
  }
  return cmd;
}
