// -- import libraries

#include <ESP8266WiFi.h>  // WiFi
#include <PubSubClient.h> // MQTT
#include <U8x8lib.h>      // OLED display


// -- config

#include "config.h"


// -- define global objects

// wifi client:
WiFiClient wifiClient;

// mqtt client:
PubSubClient mqttClient(wifiClient);

// OLED display:
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(PIN_DISPLAY_SCL, PIN_DISPLAY_SDA, U8X8_PIN_NONE);


// -- define global variables:

uint8_t button_up_last_value;
uint8_t button_down_last_value;

bool mqtt_message_is_received = false;
char* mqtt_received_message;

int counter = 0;


// -- functions

bool wifiConnect(const char* ssid, const char* password, const int wifi_connect_retry_delay = 500, const int wifi_connect_retry_timeout = 10000) {

  int retry_delay = wifi_connect_retry_delay;
  int retry_timeout = wifi_connect_retry_timeout;
  
  Serial.println();
  Serial.print("Attempting to connect to WiFi ");
  Serial.print(ssid);
  Serial.print(".");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && retry_timeout > 0) {
    Serial.print(".");

    // prepare next loop:
    retry_timeout -= retry_delay;
    delay(retry_delay);
  }

  if (retry_timeout <= 0) {

    Serial.println(" Timed out.");
    return false;

  }
  
  Serial.println(" Connected.");
  Serial.print("  Assigned IP is ");
  Serial.print(WiFi.localIP());
  Serial.println(".");
  return true;
  
}

bool mqttConnect(const char* device_id, PubSubClient mqttClient, const char* mqtt_server, const int mqtt_port, const int mqtt_connect_retry_delay = 500, const int mqtt_connect_retry_timeout = 10000) {
  
  int retry_delay = mqtt_connect_retry_delay;
  int retry_timeout = mqtt_connect_retry_timeout;
  
  Serial.print("Attempting to connect to MQTT broker at ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.print(mqtt_port);
  Serial.print(".");
      
  // create a mqtt client id out of the device_id:
  String mqttClientId = "robotrain-";
  mqttClientId += String(device_id);
    
  while (!mqttClient.connected() && retry_timeout > 0) {
    
    // attempt to connect:
    if (mqttClient.connect(mqttClientId.c_str())) {
      break;
    } else {
      Serial.print(".");
      
      // prepare next loop:
      retry_timeout -= (retry_delay + 1000); // see [1] below
      delay(retry_delay);
    }
  }

  // [1]: PubSubClient::connect() uses Arduinos client.connect() function
  //      which has a default timeout of 1000 ms. This adds to the timeout
  //      which is defined in config.h!

  if (retry_timeout <= 0) {
    
    Serial.print(" Timed out. Error code ");
    Serial.println(mqttClient.state());
    
    return false;
    
  }
    
  Serial.println(" Connected.");
    
  Serial.print("  Client ID is ");
  Serial.print(mqttClientId);
  Serial.println(".");

  return true;
  
}

bool mqttSendMessage(const char* topic, const char* message) {

  // reconnect, if connectinon was lost:
  if (!mqttClient.connected()) {
    mqttConnect(DEVICE_ID, mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CONNECT_RETRY_DELAY, MQTT_CONNECT_RETRY_TIMEOUT);
    // bool mqttConnect(const char* device_id, PubSubClient mqttClient, const char* mqtt_server, const int mqtt_port, const int mqtt_connect_retry_delay = 500, const int mqtt_connect_retry_timeout = 10000) {
  }

  // subscibe to outgoing topic to be able
  // to check if message was delivered:
  mqttClient.subscribe(topic);

  // publish message on given topic:
  Serial.print("Publishing message '");
  Serial.print(message);
  Serial.print("' on topic '");
  Serial.print(topic);
  Serial.print("'.");
  
  mqttClient.publish(topic, message, true);

  // wait until message was rebounced:
  // Note: This is not an implement of QoS 1 (which this library is lacking)
  //       as there will be no attempt to send the message again if it failed.
  //       This behavior could be added though.
  //       Right now it just makes sure that the message is fully sent before
  //       the board enters deep sleep mode. By waiting for the message to be
  //       delivered back to this device we can make sure that it was delivered
  //       to the broker.
  
  int receive_retry_delay = MQTT_SEND_REBOUNCE_DELAY;
  int receive_retry_timeout = MQTT_SEND_REBOUNCE_TIMEOUT;
  
  while (!mqtt_message_is_received && receive_retry_timeout > 0) {
    Serial.print(".");
    mqttClient.loop();

    // reset mqtt_message_is_received if wrong message was received:
    if (mqtt_message_is_received && !strcmp(mqtt_received_message, message) == 0) {
      mqtt_message_is_received = false;
    }

    // prepare next loop:
    receive_retry_timeout -= receive_retry_delay;
    delay(receive_retry_delay);
  }

  if (receive_retry_timeout <= 0) {
    Serial.println(" Timed out.");
    return false;
  }

  Serial.println(" Delivered.");
  return true;
  
}

void mqttMessageReceivedCallback(const char* topic, const byte* payload, const unsigned int length) {

    // convert byte* payload to char*:
    char* result = (char*) payload;

    // truncate to correct length:
    if (length <= strlen(result)) {
      result[length] = '\0';
    }

    // return value:
    mqtt_message_is_received = true;
    mqtt_received_message = result;
}

void setup() {
  
  // start serial:
  Serial.begin(9600);
  
  // init buttons:
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);

  // init display:
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_8x13_1x2_f);
  u8x8.drawString(0,2,"UP:");
  u8x8.drawString(0,4,"DOWN:");
  u8x8.drawString(6,2,"-");
  u8x8.drawString(6,4,"-");


  // connect to wifi:
  if (!wifiConnect(WIFI_SSID, WIFI_SECRET, WIFI_CONNECT_RETRY_DELAY, WIFI_CONNECT_RETRY_TIMEOUT)) {
    Serial.println("Could not connect to WiFi!");
  }

  // connect to mqtt broker:
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttMessageReceivedCallback);

  if(!mqttConnect(DEVICE_ID, mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CONNECT_RETRY_DELAY, MQTT_CONNECT_RETRY_TIMEOUT)) {
    Serial.println("Could not connect to MQTT broker");
  }
  
  
}

void loop() {

  if (digitalRead(PIN_BUTTON_DOWN) == HIGH) {

    // update counter:
    if (counter > SPEED_NOMINAL_MIN) {
      counter -= SPEED_NOMINAL_STEP;
    } else {
      counter = SPEED_NOMINAL_MIN;
    }

    // update display:
    u8x8.drawString(6, 4, "HIGH");
    u8x8.clearLine(0);
    u8x8.clearLine(1);
    u8x8.drawString(0, 0, String(counter).c_str());

    // send mqtt message:
    mqttSendMessage(String(MQTT_TOPIC_SPEED_NOMINAL).c_str(), String(counter).c_str());

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_DOWN) == HIGH) {
      delay(100);
    }

    // update display:
    u8x8.drawString(6, 4, "LOW ");
  }
  
  // read buttons:
  if (digitalRead(PIN_BUTTON_UP) == HIGH) {

    // update counter:
    if (counter < SPEED_NOMINAL_MAX) {
      counter += SPEED_NOMINAL_STEP;
    } else {
      counter = SPEED_NOMINAL_MAX;
    }

    // update display:
    u8x8.drawString(6, 2, "HIGH");
    u8x8.clearLine(0);
    u8x8.clearLine(1);
    u8x8.drawString(0, 0, String(counter).c_str());

    // send mqtt message:
    mqttSendMessage(String(MQTT_TOPIC_SPEED_NOMINAL).c_str(), String(counter).c_str());

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_UP) == HIGH) {
      delay(100);
    }

    // update display:
    u8x8.drawString(6, 2, "LOW ");
  }
  
}
