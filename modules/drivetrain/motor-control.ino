// -- import libraries

#include <ESP8266WiFi.h>  // WiFi
#include <PubSubClient.h> // MQTT


// -- config

#include "config.h"


// -- define global objects

// wifi client:
WiFiClient wifiClient;

// mqtt client:
PubSubClient mqttClient(wifiClient);


// -- define global variables:

// mqtt:
bool mqtt_message_was_received = false;
char* mqtt_received_message;
char* mqtt_received_topic;

// nominal values:
int speed_nominal = 0;
char* direction_nominal = "FWD";

// actual values:
int speed_actual = 0;
int speed_actual_before = 0;
char* direction_actual = "FWD";
char * direction_actual_before = "FWD";


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
  }

  // // subscibe to outgoing topic to be able
  // // to check if message was delivered:
  // mqttClient.subscribe(topic);

  // publish message on given topic:
  Serial.print("Publishing message '");
  Serial.print(message);
  Serial.print("' on topic '");
  Serial.println(topic);
  
  mqttClient.publish(topic, message, true);

  // // wait until message was rebounced:
  // // Note: This is not an implement of QoS 1 (which this library is lacking)
  // //       as there will be no attempt to send the message again if it failed.
  // //       This behavior could be added though.
  // //       Right now it just makes sure that the message is fully sent before
  // //       the board enters deep sleep mode. By waiting for the message to be
  // //       delivered back to this device we can make sure that it was delivered
  // //       to the broker.
  
  // int receive_retry_delay = MQTT_SEND_REBOUNCE_DELAY;
  // int receive_retry_timeout = MQTT_SEND_REBOUNCE_TIMEOUT;
  
  // while (!mqtt_message_was_received && receive_retry_timeout > 0) {
  //   Serial.print(".");
  //   mqttClient.loop();

  //   // reset mqtt_message_was_received if wrong message was received:
  //   if (mqtt_message_was_received && !strcmp(mqtt_received_message, message) == 0) {
  //     mqtt_message_was_received = false;
  //   }

  //   // prepare next loop:
  //   receive_retry_timeout -= receive_retry_delay;
  //   delay(receive_retry_delay);
  // }

  // if (receive_retry_timeout <= 0) {
  //   Serial.println(" Timed out.");
  //   return false;
  // }

  // Serial.println(" Delivered.");
  return true;
  
}

void mqttMessageReceivedCallback(char* topic, const byte* payload, const unsigned int length) {

    // convert byte* payload to char*:
    char* result = (char*) payload;

    // truncate to correct length:
    if (length <= strlen(result)) {
      result[length] = '\0';
    }

    // return value:
    mqtt_message_was_received = true;
    mqtt_received_topic = topic;
    mqtt_received_message = result;
}

void setMotorSpeedPercent(int speed_percent, char* direction = "FWD") {

  // calculate pwm value:
  int analog_value = (int)(1024.0 * (float)abs(speed_percent) / 100.0);
  // info: analogWrite accepts 0..1024, setMotorSpeedPercent accepts 0..100.
  // TODO: Arduino provides a map() function for this purpose!
  
  // set motor pins:
  if (strcmp(direction, "FWD") == 0) {
    analogWrite(PIN_MOTOR_1_A, analog_value);
    analogWrite(PIN_MOTOR_1_B, HIGH);
  } else if (strcmp(direction, "REV") == 0) {
    analogWrite(PIN_MOTOR_1_A, HIGH);
    analogWrite(PIN_MOTOR_1_B, analog_value);
  } else {
    Serial.print("ERROR: invalid direction passed to setMotorSpeedPercent(): ");
    Serial.println(direction);
  }

  // give feedback on serial:
  Serial.print("Set motor to ");
  Serial.print(analog_value);
  Serial.print(" ");
  Serial.println(direction);

  // store values:
  speed_actual = speed_percent;
  direction_actual = direction;
  
}

void setup() {
  
  // start serial:
  Serial.begin(9600);
  
  // init status led:
  pinMode(PIN_LED_MQTT_RECEIVED, OUTPUT);
  digitalWrite(PIN_LED_MQTT_RECEIVED, LOW);
  
  // init motors:
  pinMode(PIN_MOTOR_1_B, OUTPUT);
  digitalWrite(PIN_MOTOR_1_B, LOW);
  pinMode(PIN_MOTOR_1_A, OUTPUT);
  digitalWrite(PIN_MOTOR_1_A, LOW);

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

  mqttClient.subscribe(String(MQTT_TOPIC_SPEED_NOMINAL).c_str());
  Serial.print("Subscribed to topic '");
  Serial.print(MQTT_TOPIC_SPEED_NOMINAL);
  Serial.println("'.");

  mqttClient.subscribe(String(MQTT_TOPIC_DIRECTION_NOMINAL).c_str());
  Serial.print("Subscribed to topic '");
  Serial.print(MQTT_TOPIC_DIRECTION_NOMINAL);
  Serial.println("'.");  
  
}

void loop() {

  // check for new mqtt messages:
  mqttClient.loop();
  if (mqtt_message_was_received) {

    // read nominal direction:
    if (strcmp(mqtt_received_topic, MQTT_TOPIC_DIRECTION_NOMINAL) == 0) {
      if (strcmp(mqtt_received_message, "FWD") == 0) {
        direction_nominal = "FWD";
      } else if (strcmp(mqtt_received_message, "REV") == 0) {
        direction_nominal = "REV";
      } else {
        Serial.println("ERROR: unexpected message received!");
        Serial.print("    topic:  ");
        Serial.println(mqtt_received_topic);
        Serial.print("    message: ");
        Serial.println(mqtt_received_message);
      }
    }

    // read nominal speed:
    if (strcmp(mqtt_received_topic, MQTT_TOPIC_SPEED_NOMINAL) == 0) {
      if (
        String(mqtt_received_message).toInt() <= SPEED_NOMINAL_MAX &&
        String(mqtt_received_message).toInt() >= SPEED_NOMINAL_MIN
      ) {
        speed_nominal = String(mqtt_received_message).toInt();
      } else {
        Serial.println("ERROR: received value for 'speed_nominal' out of range!");
        Serial.print("    value: ");
        Serial.println(String(mqtt_received_message).toInt());
      }
    }

    // acknowledge message to see when next message is available:
    mqtt_message_was_received = false;

    // set motor speed:
    setMotorSpeedPercent(speed_nominal, direction_nominal);

    // send actual speed and direction (if it has changed) back to remotes:
    if (speed_actual != speed_actual_before) {
      mqttSendMessage(String(MQTT_TOPIC_SPEED_ACTUAL).c_str(), String(speed_actual).c_str());
      speed_actual_before = speed_actual;
    }
    if (direction_actual != direction_actual_before) {
      mqttSendMessage(String(MQTT_TOPIC_DIRECTION_ACTUAL).c_str(), String(direction_actual).c_str());
      direction_actual_before = direction_actual;
    }

  }

}
