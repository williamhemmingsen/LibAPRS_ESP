#include <ESP8266WiFi.h>
#include <LibAPRSesp.h>
#include <limits.h>
#include <SimpleTimer.h>
#include <KISS.h>

const char* ssid     = "xxxxxxx";
const char* password = "xxxxxxx";


WiFiClient client;

SimpleTimer timer;

boolean KISS = false;

char * lf = "\r\n";
boolean gotPacket = false;
char * srccall;
char * dstcall;
char * datapayload;

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


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  if (!client.connect("rotate.aprs.net", 14580)) {
    Serial.println("connection failed");
    return;
  } else {
    Serial.println(F("Connected to APRS.IS"));
    delay(1000);
    client.print("user NOCALL pass YOURCODE vers YOUR VERSION \r\n");\\ Your CAll, APRS.IS code, and version comment
  }

  APRS_init();
  APRS_setCallsign("NOCALL", 1); ///Your call
  APRS_setPath1("WIDE1", 1);
  APRS_setPath2("WIDE2", 2);
  APRS_setPreamble(450);
  APRS_setTail(150);
}


void processPacket() {
  boolean tcpippacket = false;
  if (gotPacket) {
    gotPacket = false;
    if (KISS) {
      kiss_wrapper();
    }
    else {
      srccall = incomingPacket.src.call;
      dstcall = incomingPacket.dst.call;
      char * datastr = (char*)incomingPacket.info;
      datastr[incomingPacket.len] = '\0';
      //Fix for digirepeated TCPIP APRS.IS Packets
      if (datastr[0] == '}') {
        tcpippacket = true;
      }
      datapayload = datastr;
      char sssid[10];
      itoa (incomingPacket.src.ssid, sssid, 10);
      char dssid[10];
      itoa (incomingPacket.dst.ssid, dssid, 10);
      char packetchar[200];
      packetchar[0] = 0;
      strcat (packetchar, srccall);
      strcat (packetchar, "-");
      strcat (packetchar, sssid);
      strcat (packetchar, ">");
      strcat (packetchar, dstcall);
      strcat (packetchar, "-");
      strcat (packetchar, dssid);
      for (int i = 0; i < incomingPacket.rpt_count; i++) {
        strcat (packetchar, ",");
        char * rptcall = incomingPacket.rpt_list[i].call;
        strcat (packetchar, rptcall);
        strcat (packetchar, "-");
        char rssid[10];
        itoa (incomingPacket.rpt_list[i].ssid, rssid, 10);
        strcat (packetchar, rssid);
      }
      strcat (packetchar, ",qAR,NOCALL-1:"); /// Your call and SSID
      strcat (packetchar, datapayload);
      //strcat (packetchar, lf);

      Serial.println(packetchar);

      if (!tcpippacket) {
        //Don't IGATE TCPIP repeaded packets
        client.println(packetchar);
        client.println();
      }
    }
    free(packetData);

  }
}

boolean whichExample = false;
void loop() {


  if (Serial.available() && KISS) {      // If anything comes in Serial (USB),
    kiss_serial(Serial.read());
  }

  if (client.available()) {     // If anything comes from APRS.IS
    Serial.write(client.read());   // read it and send it out Serial (USB)
  }
  processPacket();
  ESP.wdtFeed();
}



