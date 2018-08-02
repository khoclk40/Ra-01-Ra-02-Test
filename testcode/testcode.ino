#include <ModbusMaster232.h>  //Modbus Master

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ESP8266TelegramBOT.h> //Telegram bot
/*
  #define BLYNK_PRINT Serial
  #include <BlynkSimpleEsp8266.h>
*/
#include <FS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <WiFiManager.h>  // Wifi manager
#include <Ticker.h>


#include <PubSubClient.h> //nodered


#define BOTtoken "539100765:AAEpZaxNELpTAAry0BEFDmZhPD-hd-SgAhI"  //token of TestBOT
#define BOTname "KeyworkreportBot"
#define BOTusername "KeyworkreportBot"
#define IDGroup "-277789608"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 1000;
long Bot_lasttime;


int M41 = 0, M42 = 0, M43 = 0, M44 = 0, M45 = 0, M46 = 0, M47 = 0, M48 = 0, M49 = 0, M50 = 0,
    M51 = 0, M52 = 0, M53 = 0, M54 = 0, M55 = 0, M56 = 0, M57 = 0, M58 = 0, M59 = 0, M60 = 0,
    M61 = 0, M62 = 0, M63 = 0, M64 = 0, M65 = 0, M66 = 0, M67 = 0, M68 = 0, M69 = 0, M70 = 0,
    M71 = 0, M72 = 0, M73 = 0, M74 = 0, M75 = 0, M76 = 0, M77 = 0, M78 = 0, M79 = 0, M80 = 0,
    M81 = 0, M82 = 0, M83 = 0, M84 = 0, M85 = 0, M86 = 0, M87 = 0, M88 = 0, M89 = 0, M90 = 0;
int Mdelay = 5, Mdelay10 = 10;


String  codeM41 = "Loi dung khan cap",
        codeM42 = "Loi dong co keo khuon",
        codeM43 = "Loi qua gioi han duoi dong co keo khuon",
        codeM44 = "Loi qua gioi han tren dong co keo khuon",
        codeM45 = "Loi dong co keo bo hut",
        codeM46 = "Loi qua gioi han duoi dong co keo bo hut",
        codeM47 = "Loi qua gioi han tren dong co keo bo hut",
        codeM48 = "Loi cam bien tren xilanh de hang",
        codeM49 = "Loi cam bien duoi xilanh de hang",
        codeM50 = "Loi cam bien tren xilanh de xuong",
        codeM51 = "Loi cam bien duoi xilanh de xuong",
        codeM52 = "Loi cam bien tren xilanh dot chi so 1",
        codeM53 = "Loi cam bien duoi xilanh dot chi so 1",
        codeM54 = "Loi cam bien tren xilanh nhuom bac ha",
        codeM55 = "Loi cam bien duoi xilanh nhuom bac ha",
        codeM56 = "Loi cam bien tren xilanh len xuong hut hang",
        codeM57 = "Loi cam bien duoi xilanh len xuong hut hang",
        codeM58 = "",
        codeM59 = "",
        codeM60 = "",
        codeM61 = "Loi cam bien tren xilanh dot chi so 2",
        codeM62 = "Loi cam bien duoi xilanh dot chi so 2",
        codeM63 = "Loi cam bien tat van hut hang",
        codeM64 = "",
        codeM65 = "Loi hut hang",
        codeM66 = "Doc san luong ca 1",
        codeM67 = "Doc san luong ca 2",
        codeM68 = "Doc san luong ca 3",
        codeM69 = "",
        codeM70 = "San luong ca 1 vuot muc yeu cau",
        codeM71 = "San luong ca 1 dat yeu cau",
        codeM72 = "San luong ca 1 khong dat yeu cau",
        codeM73 = "San luong ca 2 vuot muc yeu cau",
        codeM74 = "San luong ca 2 dat yeu cau",
        codeM75 = "San luong ca 2 khong dat yeu cau",
        codeM76 = "San luong ca 3 vuot muc yeu cau",
        codeM77 = "San luong ca 3 dat yeu cau",
        codeM78 = "San luong ca 3 khong dat yeu cau ",
        codeM79 = "Chay may",
        codeM80 = "Dung may",
        codeM81 = "Dang o che do bang tay",
        codeM82 = "Tu dong tat may do thoi gian dung qua lau",
        codeM83 = "",
        codeM84 = "",
        codeM85 = "",
        codeM86 = "",
        codeM87 = "",
        codeM88 = "",
        codeM89 = "",
        codeM90 = "";

String total = "", target = "", pGood = "", pBad = "";

ModbusMaster232 node(1);
WiFiManager wifiManager;
Ticker ticker;

const char* mqtt_server = "camtt.dvrcam.info";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int D700last = 0, D710last = 0;
IPAddress _ipZero = IPAddress(0, 0, 0, 0);
int countip = 0;
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void tick()
{
  //toggle state
  int state = digitalRead(16);  // get the current state of GPIO1 pin
  digitalWrite(16, !state);     // set pin to the opposite state
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}


void setup()
{
  Serial.begin(115200);
  node.begin(19200);  // Modbus RTU
  pinMode(14, INPUT);
  pinMode(16, OUTPUT);
  ticker.attach(0.6, tick);
  wifiManager.setBreakAfterConfig(true);
  wifiManager.setAPCallback(configModeCallback);


  if (!wifiManager.autoConnect("DataLogging WiFi", "DataLogging@123")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    ESP.reset();
    delay(5000);
  }
  ticker.detach();
  digitalWrite(16, LOW);
  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  bot.begin();
  attachInterrupt(14, resetWiFi, RISING);
  client.setServer(mqtt_server, 1883);

  wdt_enable(WDTO_15MS);
}
void resetWiFi()
{
  Serial.println("Reseting.....");
  wifiManager.resetSettings();
  ESP.reset();
  delay(5000);
}

void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  total = "Tong san luong ca ";
  target = "San luong yeu cau ca ";
  pGood = "San luong thanh pham ca ";
  pBad = "San luong bi loi ca ";

  node.readHoldingRegisters(700, 1);
  int D700 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(710, 1);
  int D710 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  if (D700 != D700last && D700 > 0 && D700 < 90)
  {
    switch (D700)
    {
      case 42:
        bot.sendMessage(IDGroup, codeM42, "");
        break;
      case 43:
        bot.sendMessage(IDGroup, codeM43, "");
        break;
      case 44:
        bot.sendMessage(IDGroup, codeM44, "");
        break;
      case 45:
        bot.sendMessage(IDGroup, codeM45, "");
        break;
      case 46:
        bot.sendMessage(IDGroup, codeM46, "");
        break;
      case 47:
        bot.sendMessage(IDGroup, codeM47, "");
        break;
      case 48:
        bot.sendMessage(IDGroup, codeM48, "");
        break;
      case 49:
        bot.sendMessage(IDGroup, codeM49, "");
        break;
      case 50:
        bot.sendMessage(IDGroup, codeM50, "");
        break;
      case 51:
        bot.sendMessage(IDGroup, codeM51, "");
        break;
      case 52:
        bot.sendMessage(IDGroup, codeM52, "");
        break;
      case 53:
        bot.sendMessage(IDGroup, codeM53, "");
        break;
      case 54:
        bot.sendMessage(IDGroup, codeM54, "");
        break;
      case 55:
        bot.sendMessage(IDGroup, codeM55, "");
        break;
      case 56:
        bot.sendMessage(IDGroup, codeM56, "");
        break;
      case 57:
        bot.sendMessage(IDGroup, codeM57, "");
        break;
      case 61:
        bot.sendMessage(IDGroup, codeM61, "");
        break;
      case 83:
        bot.sendMessage(IDGroup, codeM62, "");
        break;
      case 84:
        bot.sendMessage(IDGroup, codeM63, "");
        break;
      case 65:
        bot.sendMessage(IDGroup, codeM65, "");
        break;
      case 70:
        bot.sendMessage(IDGroup, codeM70, "");
        break;
      case 71:
        bot.sendMessage(IDGroup, codeM71, "");
        break;
      case 72:
        bot.sendMessage(IDGroup, codeM72, "");
        break;
      case 73:
        bot.sendMessage(IDGroup, codeM73, "");
        break;
      case 74:
        bot.sendMessage(IDGroup, codeM74, "");
        break;
      case 75:
        bot.sendMessage(IDGroup, codeM75, "");
        break;
      case 76:
        bot.sendMessage(IDGroup, codeM76, "");
        break;
      case 77:
        bot.sendMessage(IDGroup, codeM77, "");
        break;
      case 78:
        bot.sendMessage(IDGroup, codeM78, "");
        break;
      case 79:
        bot.sendMessage(IDGroup, codeM79, "");
        break;
      case 80:
        bot.sendMessage(IDGroup, codeM80, "");
        break;
      case 81:
        bot.sendMessage(IDGroup, codeM81, "");
        break;
      case 82:
        bot.sendMessage(IDGroup, codeM82, "");
        break;
      case 41:
        bot.sendMessage(IDGroup, codeM41, "");
        break;
    }
    delay(Mdelay);
    D700last = D700;
  }
  if (D710 != D710last && D710 > 0 && D710 < 90)
  {
    switch (D710)
    {
      case 1:
        {
          node.readHoldingRegisters(240, 1);
          int D240 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(242, 1);
          int D242 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(244, 1);
          int D244 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(246, 1);
          int D246 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          total = total + "1: " + D240;
          target = target + "1: " + D242;
          pGood = pGood + "1: " + D244;
          pBad = pBad + "1: " + D246;
          delay(Mdelay);
          bot.sendMessage(IDGroup, total, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, target, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pGood, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pBad, "");
          delay(Mdelay10);
          sprintf (msg, "%03i", D240);
          client.publish("total1", msg);
          sprintf (msg, "%03i", D242);
          client.publish("target1", msg);
          sprintf (msg, "%03i", D244);
          client.publish("pGood1", msg);
          sprintf (msg, "%03i", D246);
          client.publish("pBad1", msg);
          delay(Mdelay10);
          break;
        }
      case 2:
        {
          node.readHoldingRegisters(248, 1);
          int D248 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(250, 1);
          int D250 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(252, 1);
          int D252 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(254, 1);
          int D254 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          total = total + "2: " + D248;
          target = target + "2: " + D250;
          pGood = pGood + "2: " + D252;
          pBad = pBad + "2: " + D254;
          delay(Mdelay);
          bot.sendMessage(IDGroup, total, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, target, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pGood, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pBad, "");
          delay(Mdelay10);
          sprintf (msg, "%03i", D248);
          client.publish("total2", msg);
          sprintf (msg, "%03i", D250);
          client.publish("target2", msg);
          sprintf (msg, "%03i", D252);
          client.publish("pGood2", msg);
          sprintf (msg, "%03i", D254);
          client.publish("pBad2", msg);
          delay(Mdelay10);
          break;
        }
      case 3:
        {
          node.readHoldingRegisters(256, 1);
          int D256 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(258, 1);
          int D258 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(260, 1);
          int D260 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(262, 1);
          int D262 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          total = total + "3: " + D256;
          target = target + "3: " + D258;
          pGood = pGood + "3: " + D260;
          pBad = pBad + "3: " + D262;
          delay(Mdelay);
          bot.sendMessage(IDGroup, total, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, target, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pGood, "");
          delay(Mdelay10);
          bot.sendMessage(IDGroup, pBad, "");
          delay(Mdelay10);
          sprintf (msg, "%03i", D256);
          client.publish("total3", msg);
          sprintf (msg, "%03i", D258);
          client.publish("target3", msg);
          sprintf (msg, "%03i", D260);
          client.publish("pGood3", msg);
          sprintf (msg, "%03i", D262);
          client.publish("pBad3", msg);
          delay(Mdelay10);
          break;
        }
      case 4:
        {
          node.readHoldingRegisters(174, 1);
          int D174 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          node.readHoldingRegisters(175, 1);
          int D175 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          node.readHoldingRegisters(176, 1);
          delay(Mdelay);
          int D176 = node.getResponseBuffer(0);
          node.clearResponseBuffer();
          delay(Mdelay);
          String sanluong = " Tong san luong ngay ";
          sanluong = sanluong + D175 + "/" + D176 + ": " + D174;
          bot.sendMessage(IDGroup, sanluong, "");
          delay(Mdelay10);
          sprintf (msg, "%03i", D174);
          client.publish("sanluong", msg);
          break;
        }
    }
    D710last = D710;
  }
  sprintf (msg, "%03i", D700);
  client.publish("pGood1", msg);
  sprintf (msg, "%03i", D700last);
  client.publish("pBad1", msg);
  sprintf (msg, "%03i", D710);
  client.publish("pGood2", msg);
  sprintf (msg, "%03i", D710last);
  client.publish("pBad2", msg);
  
  wdt_reset();
}
