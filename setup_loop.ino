/* TEKNOFEST 2022 Unmanned Underwater Systems.
   Tis file is part of Marill ROV Flight Control Software

   Only the Marill ROV team can access MARILLFLIGHT!!

   Marill ROV team was founded by the students of our school
   Cezeri Yeşil Teknoloji Vocational and Technical Anatolian
   High School to take part in to take part in Teknofest 2022 for the first time.

   For more details, visit our GitHub

   Web Site: <https://www.marill.tech>.
   GitHub:   <https://github.com/Marill-ROV>.
*/

void setup() {
  Serial.begin(115200);
  Wire.begin();
  initializeLED();
  initializeCanBus();
  initializeIMU();
  initializeMS5837();
  initializePWM();
  initializeMotor();
  colorWipe(0x00, 0xff, 0x00, 50);
  colorWipe(0x00, 0x00, 0x00, 50);
  initializeDHT();
  initializeMS5837();
  initializeBaseModule();
  Serial.println("SETUP'S COMPLETED");
  Serial.println("------- MARILL ROV STARTED ----------");
}

void loop() {
  struct CanBusCommands cmd = GetCanBusCommands();
  struct Orientation o = getIMUOrientation();
  struct DHT11 dht = GetDHT11Values();
  struct Press5837 ms = GetPress5837Values();
  struct BaseModule bm = GetBaseModuleValues();


  if (cmd.Error == true || cmd.emergencybtn == 1 || cmd.gear == 1) {
    stopMotor();
    cmd.surge = 0;
    cmd.sway = 0;
    cmd.yaw = 0;
    cmd.heave = 0;
    forwardright_val = 1500;
    forwardleft_val = 1500;
    backwardright_val = 1500;
    backwardleft_val = 1500;
    midright_val = 1500;
    midleft_val = 1500;
  }
  else {
    setXZThrusterSpeed(cmd.surge, cmd.sway, cmd.yaw);
    setYThrusterSpeed(cmd.heave);
    spinThruster();
  }

  if (o.Error == 1) {
    o.Yaw = 0;
    o.Pitch = 0;
    o.Roll = 0;
  }



  setServoAngle(cmd.servoangle);

  canSens.can_id  = 0x10;
  canSens.can_dlc = 8;
  canSens.data[0] = dht.h1temp;   //h1temp
  canSens.data[1] = dht.h2temp;   //h2temp
  canSens.data[2] = dht.h1hum;    //h1hum
  canSens.data[3] = dht.h2hum;    //h2hum
  canSens.data[4] = o.Yaw;        //yaw
  canSens.data[5] = o.Pitch;;     //pitch
  canSens.data[6] = o.Roll;       //roll
  canSens.data[7] = bm.capacity; //batterystate

  canMS.can_id  = 0x20;
  canMS.can_dlc = 4;
  canMS.data[0] = highByte(pressure); //pressure
  canMS.data[1] = lowByte(pressure);  //pressure
  canMS.data[2] = highByte(depth); //depth
  canMS.data[3] = lowByte(depth);  //depth

  canBase.can_id  = 0x30;
  canBase.can_dlc = 4;
  canBase.data[0] = bm.current;   //current
  canBase.data[1] = bm.power;     //power
  canBase.data[2] = bm.voltage;   //voltage
  canBase.data[3] = ms.temp; //watertemp

  if (millis() - sendMessage > 10) {
    sendMessage = millis();
    mcp2515.sendMessage(&canSens);
    mcp2515.sendMessage(&canMS);
    mcp2515.sendMessage(&canBase);
  }
  if (millis() - printMessage > 25) {
    printMessage = millis();

    Serial.print("ROV Messages: ");
    Serial.print(cmd.Error);
    Serial.print(" ");
    Serial.print(ms.Error);
    Serial.print(" ");
    Serial.print(dht.Error);
    Serial.print(" ");
    Serial.print(o.Error);
    Serial.print(" ");
    Serial.print(bm.Error);
    Serial.print(" ");
    Serial.print(cmd.emergencybtn);
    Serial.print("V");

    Serial.print(forwardright_val);
    Serial.print(" ");
    Serial.print(forwardleft_val);
    Serial.print(" ");
    Serial.print(backwardright_val);
    Serial.print(" ");
    Serial.print(backwardleft_val);
    Serial.print(" ");
    Serial.print(midright_val);
    Serial.print(" ");
    Serial.print(midleft_val);
    Serial.print("C");

    Serial.print(cmd.surge);
    Serial.print(" ");
    Serial.print(cmd.sway);
    Serial.print(" ");
    Serial.print(cmd.heave);
    Serial.print(" ");
    Serial.print(cmd.yaw);
    Serial.print(" ");
    Serial.print(cmd.servoangle);
    Serial.print(" ");
    Serial.print(cmd.gear);
    Serial.print(" ");
    Serial.print(cmd.task);
    Serial.print("M");

    Serial.print(ms.temp);
    Serial.print(" ");
    Serial.print(ms.pressure);
    Serial.print(" ");
    Serial.print(ms.depth);
    Serial.print("O");

    Serial.print(o.Yaw);
    Serial.print(" ");
    Serial.print(o.Pitch);
    Serial.print(" ");
    Serial.print(o.Roll);
    Serial.print(" ");

    Serial.print(dht.h1temp);
    Serial.print(" ");
    Serial.print(dht.h2temp);
    Serial.print(" ");
    Serial.print(dht.h1hum);
    Serial.print(" ");
    Serial.print(dht.h2hum);
    Serial.print(" ");

    Serial.print(bm.voltage);
    Serial.print(" ");
    Serial.print(bm.current);
    Serial.print(" ");
    Serial.print(bm.capacity);
    Serial.print(" ");
    Serial.print(bm.power);
    Serial.println(" ");
  }
}
