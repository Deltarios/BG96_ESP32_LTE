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

  while (BG96LTE.MQTTSubscribeTopic(comm_mqtt_index, 1, "/a1mQBMY2HI9/D896E0FF00012940/update", mqtt_qos) != 0)
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
  }
  DSerial.println("\r\nMQTT Subscribe Topic Success!");
}

void loop()
{

  if (BG96LTE.MQTTPublishMessages(comm_mqtt_index, 0, mqtt_qos, "/v1.6/devices/avi-device", false, "{\"hello\":{\"value\":10}}") == 0){
      DSerial.println("\r\nMQTT Publish Messages Success!");
      delay(2500);
  }
}