#include <Arduino.h>

#include <LTEBG96MQTT.h>

#define DSerial Serial
#define ATSerial Serial2

char APN[] = "modem.nexteldata.com.mx";
char mqtt_server[] = "industrial.api.ubidots.com";
unsigned int mqtt_port = 1883;
char mqtt_client_id[] = "mqtt_test";
unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
unsigned int comm_mqtt_index = 1; // The range is 0 ~ 5
Mqtt_Version_t version = MQTT_V3;
Mqtt_Qos_t mqtt_qos = AT_MOST_ONCE;
char device_name[] = "AVI_TEST";

LTEBG96MQTT BG96LTE(ATSerial, DSerial);

unsigned long lastMillis = 0;

void sendData();
int getData();

void setup()
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the BG96LTE Debug Serial!");
  delay(1000);
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);
  while (!BG96LTE.InitModule());

  BG96LTE.SetDevCommandEcho(false);

  char inf[64];
  if (BG96LTE.GetDevInformation(inf))
  {
    DSerial.println(inf);
  }

  char apn_error[64];
  while (!BG96LTE.InitAPN(comm_pdp_index, APN, "", "", apn_error))
  {
    DSerial.println(apn_error);
  }
  DSerial.println(apn_error);

  while (!BG96LTE.SetMQTTConfigureParameters(comm_mqtt_index, comm_pdp_index, version, 150, SERVER_STORE_SUBSCRIPTIONS))
  {
    DSerial.println("\r\nConfig the MQTT Parameter Fail!");
    int e_code;
    if (BG96LTE.returnErrorCode(e_code))
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nConfig the MQTT Parameter Success!");

  while (BG96LTE.OpenMQTTNetwork(comm_mqtt_index, mqtt_server, mqtt_port) != 0)
  {
    DSerial.println("\r\nSet the MQTT Service Address Fail!");
    int e_code;
    if (BG96LTE.returnErrorCode(e_code))
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nSet the MQTT Service Address Success!");

  while (BG96LTE.CreateMQTTClient(comm_mqtt_index, mqtt_client_id, "", "avi-energy") != 0)
  {
    DSerial.println("\r\nCreate a MQTT Client Fail!");
    int e_code;
    if (BG96LTE.returnErrorCode(e_code))
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nCreate a MQTT Client Success!");

  while (BG96LTE.MQTTSubscribeTopic(comm_mqtt_index, 0, "/v1.6/devices/avi-device/hello/lv", mqtt_qos) != 0)
  {
    DSerial.println("\r\nMQTT Subscribe Topic Fail!");
    int e_code;
    if (BG96LTE.returnErrorCode(e_code))
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
    delay(5000);
  }
  DSerial.println("\r\nMQTT Subscribe Topic Success!");
}

void loop()
{
  getData();
  if (millis() - lastMillis > 5000)
  {
    Serial.println("Sending payload");
    sendData();
    lastMillis = millis();
  }
}

void sendData() 
{
  if (BG96LTE.MQTTPublishMessages(comm_mqtt_index, 0, mqtt_qos, "/v1.6/devices/avi-device", false, "{\"hello2\":{\"value\":10}}") == 0)
  {
    DSerial.println("\r\nMQTT Publish Messages Success!");
  }
}

int getData()
{
  char mqtt_recv[128];
  char *sta_buf;
  Mqtt_URC_Event_t ret = BG96LTE.WaitCheckMQTTURCEvent(mqtt_recv, 2);
  switch (ret)
  {
  case MQTT_RECV_DATA_EVENT:
    sta_buf = strstr(mqtt_recv, "\",\"");
    DSerial.println("\r\nThe MQTT Recv Data");
    DSerial.println(sta_buf + 3);
    break;
  case MQTT_STATUS_EVENT:
    sta_buf = strchr(mqtt_recv, ',');
    if (atoi(sta_buf + 1) == 1)
    {
      if (BG96LTE.CloseMQTTClient(comm_mqtt_index))
      {
        DSerial.println("\r\nClose the MQTT Client Success!");
      }
    }
    else
    {
      DSerial.print("\r\nStatus cade is :");
      DSerial.println(atoi(sta_buf + 1));
      DSerial.println("Please check the documentation for error details.");
    }
    break;
  }
  return 1;
}