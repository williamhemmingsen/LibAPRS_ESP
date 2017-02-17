// Include LibAPRS
#include <LibAPRSesp.h>
#include <limits.h>
#include <SimpleTimer.h>
#include <KISS.h>

SimpleTimer timer;

boolean KISS = true;

boolean gotPacket = false;
AX25Msg incomingPacket;

uint8_t *packetData;
void aprs_msg_callback(struct AX25Msg *msg) {
  if (!gotPacket) {
    gotPacket = true;

    memcpy(&incomingPacket, msg, sizeof(AX25Msg));

    if (freeMemory() > msg->len) {
      packetData = (uint8_t*)malloc(msg->len);
      memcpy(packetData, msg->info, msg->len);
      incomingPacket.info = packetData;
    } else {
      gotPacket = false;
    }
  }
}


void setup() {
  // Set up serial port
  Serial.begin(115200);
  ESP.wdtEnable(15000);
  timer.setInterval(150000, messageExample);
  APRS_init();
  APRS_setCallsign("W4WHL", 4);
  APRS_setPath1("WIDE1", 1);
  APRS_setPath2("WIDE2", 2);
  APRS_setPreamble(450);
  APRS_setTail(150);
}

void processPacket() {
  if (gotPacket) {
    gotPacket = false;
    if (KISS) {
      kiss_wrapper(); // Process KISS Packets from radio and send to Serial
    }
    else {
      Serial.println("");
      Serial.print(F("Received APRS packet. SRC: "));
      Serial.print(incomingPacket.src.call);
      Serial.print(F("-"));
      Serial.print(incomingPacket.src.ssid);
      Serial.print(F(". DST: "));
      Serial.print(incomingPacket.dst.call);
      Serial.print(F("-"));
      Serial.print(incomingPacket.dst.ssid);
      Serial.print(F(". Data: "));

      for (int i = 0; i < incomingPacket.len; i++) {
        Serial.write(incomingPacket.info[i]);
      }
      Serial.println("");
    }
    free(packetData);

  }
}


void loop() {


  if (Serial.available() && KISS) {      // Process serial KISS messages from client and send to radio
    kiss_serial(Serial.read());
  }
  processPacket();
  timer.run();  // Send a test message every 150 sec
  ESP.wdtFeed();
}

void messageExample() {

  APRS_setMessageDestination("W4WHL", 2);

  char *message = "This is a test!!!";
  APRS_sendMsg(message, strlen(message));

}


