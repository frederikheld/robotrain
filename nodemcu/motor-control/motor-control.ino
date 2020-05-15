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

// motor control:
int speed_nominal = 0;
char* direction_nominal = "FWD";


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
  Serial.print(topic);
  Serial.print("'.");
  
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

    // DEBUG:
    Serial.print("mMRC(): topic is ");
    Serial.println(topic);
    // Serial.println(MQTT_TOPIC_SPEED_NOMINAL);

    if (strcmp(topic, MQTT_TOPIC_SPEED_NOMINAL) == 0) {
      Serial.println("mMRC(): write to var speed_nominal");
    } else if (strcmp(topic, MQTT_TOPIC_DIRECTION_NOMINAL) == 0) {
      Serial.println("mMRC(): write to var direction_nominal");
    } else {
      Serial.println("mMRC(): topic not of interest");
    }

    // return value:
    mqtt_message_was_received = true;
    mqtt_received_topic = topic;
    mqtt_received_message = result;
}

void ledBlink(uint8_t pin) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
        delay(100);
      }
}


void motorSetSpeedPercent(int speed_percent, bool reversed = false) {

  // set pwm value:
  const int analog_value               = (int)(1024.0 * ((float)speed_percent / 100));
  // const int complementary_analog_value = (int)(1024.0 - analog_value);
  // info: analogWrite accepts 0..1024, setMotor accepts 0..100.
  // TODO: Arduino provides a map() function for this purpose!
  
  if (reversed) {
    analogWrite(PIN_MOTOR_1_DIRECTION, LOW);
    analogWrite(PIN_MOTOR_1_PWM, analog_value);
  } else {
    analogWrite(PIN_MOTOR_1_DIRECTION, HIGH);
    analogWrite(PIN_MOTOR_1_PWM, analog_value);
  }

  Serial.print("Set motor speed to ");
  Serial.print(analog_value);
  Serial.print(" (");
  Serial.print(speed_percent);
  Serial.print(")");
  
  if (reversed) {
    Serial.println(" reversed");
  } else {
    Serial.println();
  }
  
}

void setup() {
  
  // start serial:
  Serial.begin(9600);
  
  // init status led:
  pinMode(PIN_LED_MQTT_RECEIVED, OUTPUT);
  digitalWrite(PIN_LED_MQTT_RECEIVED, LOW);
  
  // init motors:
  pinMode(PIN_MOTOR_1_PWM, OUTPUT);
  digitalWrite(PIN_MOTOR_1_PWM, LOW);
  pinMode(PIN_MOTOR_1_DIRECTION, OUTPUT);
  digitalWrite(PIN_MOTOR_1_DIRECTION, LOW);

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
  
}

void loop() {

    // check for new mqtt messages:
    mqttClient.loop();
    if (mqtt_message_was_received) {

      Serial.print("loop(): message received on topic ");
      Serial.println(mqtt_received_topic);

      // read nominal direction:
      if (strcmp(mqtt_received_topic, MQTT_TOPIC_DIRECTION_NOMINAL) == 0) {
        Serial.println("loop(): message was received on topic direction/nominal");
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
        Serial.print("loop(): message was received on topic speed/nominal was ");
        Serial.println(mqtt_received_message);
        if (
          String(mqtt_received_message).toInt() <= SPEED_NOMINAL_MAX &&
          String(mqtt_received_message).toInt() >= SPEED_NOMINAL_MIN
        ) {
          speed_nominal == String(mqtt_received_message).toInt();
        } else {
          Serial.println("ERROR: received value for 'speed_nominal' out of range!");
          Serial.print("    value: ");
          Serial.println(String(mqtt_received_message).toInt());
        }
      }

      // acknowledge:
      mqtt_message_was_received = false;

      // Serial.println("Message received:");
      // Serial.print("    topic:  ");
      // Serial.println(mqtt_received_topic);
      // Serial.print("    message: ");
      // Serial.println(mqtt_received_message);

      //motorSetSpeedPercent(String(mqtt_received_message).toInt(), false);

    }

    // send mqtt message:
    // mqttSendMessage(String(MQTT_TOPIC_SPEED_NOMINAL).c_str(), String(counter).c_str());
  }
