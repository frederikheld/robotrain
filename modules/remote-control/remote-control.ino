// -- import libraries

#include <ESP8266WiFi.h>  // WiFi
#include <PubSubClient.h> // MQTT
#include <U8g2lib.h>      // OLED display


// -- config

#include "config.h"


// -- define global objects

// wifi client:
WiFiClient wifiClient;

// mqtt client:
PubSubClient mqttClient(wifiClient);

// OLED display:
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PIN_DISPLAY_SCL, PIN_DISPLAY_SDA, U8X8_PIN_NONE);


// -- define global variables:

uint8_t button_up_last_value;
uint8_t button_down_last_value;

bool mqtt_message_was_received = false;
char* mqtt_received_message;
char* mqtt_received_topic;

int speed_nominal = 0;
int speed_actual = 0;
char* direction_nominal = "FWD";
char* direction_actual = "FWD";


// -- functions

bool wifiConnect(const char* ssid, const char* password, const int wifi_connect_retry_delay = 500, const int wifi_connect_retry_timeout = 10000) {

  int retry_delay = wifi_connect_retry_delay;
  int retry_timeout = wifi_connect_retry_timeout;
  
  if (LOG_TO_SERIAL) {
    Serial.println();
    Serial.print("Attempting to connect to WiFi ");
    Serial.print(ssid);
    Serial.print(".");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && retry_timeout > 0) {
    if (LOG_TO_SERIAL) {
      Serial.print(".");
    }

    // prepare next loop:
    retry_timeout -= retry_delay;
    delay(retry_delay);
  }

  if (retry_timeout <= 0) {
    if (LOG_TO_SERIAL) {
      Serial.println(" Timed out.");
    }
    return false;

  }
  
  if (LOG_TO_SERIAL) {
    Serial.println(" Connected.");
    Serial.print("  Assigned IP is ");
    Serial.print(WiFi.localIP());
    Serial.println(".");
  }

  return true;
  
}

bool mqttConnect(const char* device_id, PubSubClient mqttClient, const char* mqtt_server, const int mqtt_port, const int mqtt_connect_retry_delay = 500, const int mqtt_connect_retry_timeout = 10000) {
  
  int retry_delay = mqtt_connect_retry_delay;
  int retry_timeout = mqtt_connect_retry_timeout;
  
  if (LOG_TO_SERIAL) {
    Serial.print("Attempting to connect to MQTT broker at ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.print(mqtt_port);
    Serial.print(".");
  }
      
  // create a mqtt client id out of the device_id:
  String mqttClientId = "robotrain-";
  mqttClientId += String(device_id);
    
  while (!mqttClient.connected() && retry_timeout > 0) {
    
    // attempt to connect:
    if (mqttClient.connect(mqttClientId.c_str())) {
      break;
    } else {
      if (LOG_TO_SERIAL) {
        Serial.print(".");
      }
      
      // prepare next loop:
      retry_timeout -= (retry_delay + 1000); // see [1] below
      delay(retry_delay);
    }
  }

  // [1]: PubSubClient::connect() uses Arduinos client.connect() function
  //      which has a default timeout of 1000 ms. This adds to the timeout
  //      which is defined in config.h!

  if (retry_timeout <= 0) {
    
    if (LOG_TO_SERIAL) {
      Serial.print(" Timed out. Error code ");
      Serial.println(mqttClient.state());
    }
    
    return false;
    
  }
  
  if (LOG_TO_SERIAL) {
    Serial.println(" Connected.");
      
    Serial.print("  Client ID is ");
    Serial.print(mqttClientId);
    Serial.println(".");
  }

  return true;
  
}

bool mqttSendMessage(const char* topic, const char* message) {

  // reconnect, if connectinon was lost:
  if (!mqttClient.connected()) {
    mqttConnect(DEVICE_ID, mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CONNECT_RETRY_DELAY, MQTT_CONNECT_RETRY_TIMEOUT);
    // bool mqttConnect(const char* device_id, PubSubClient mqttClient, const char* mqtt_server, const int mqtt_port, const int mqtt_connect_retry_delay = 500, const int mqtt_connect_retry_timeout = 10000) {
  }

  // publish message on given topic:
  if (LOG_TO_SERIAL) {
    Serial.print("Publishing message '");
    Serial.print(message);
    Serial.print("' on topic '");
    Serial.print(topic);
    Serial.println("'.");
  }
  
  mqttClient.publish(topic, message, true);
  
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

void sendSpeedNominal(int speed_nominal_percent) {

  // send direction:
  mqttSendMessage(
    String(MQTT_TOPIC_DIRECTION_NOMINAL).c_str(),
    direction_nominal
  );

  // send speed:
  mqttSendMessage(
    String(MQTT_TOPIC_SPEED_NOMINAL).c_str(),
    String(speed_nominal_percent).c_str()
  );
}

void drawSpeedNominal(int speed_nominal_percent) {

  // calculate pixel values:
  int scaling_factor;
  int limiter_percent;
  char* limiter_direction;
  if (SPEED_NOMINAL_MAX > abs(SPEED_NOMINAL_MIN)) {
    scaling_factor = SPEED_NOMINAL_MAX;
    limiter_percent = abs(SPEED_NOMINAL_MIN);
    limiter_direction = "REV";
  } else {
    scaling_factor = abs(SPEED_NOMINAL_MIN);
    limiter_percent = SPEED_NOMINAL_MAX;
    limiter_direction = "FWD";
  }
  uint8_t speed_nominal_pos = (int)(128.0 * ((float)abs(speed_nominal_percent) / scaling_factor));

  uint8_t limiter_pos = 0;
  if (direction_nominal == limiter_direction) {
    limiter_pos = (int)(128.0 * ((float)abs(limiter_percent) / scaling_factor));
  }

  // limiter:
  if (limiter_direction == direction_nominal) {
    u8g2.setDrawColor(2);
    u8g2.drawLine(limiter_pos, 0, limiter_pos, 15);
    u8g2.drawLine(limiter_pos + 1, 8, 128, 8);
  }

  // nominal speed as two XOR triangles
  // as overlay on the actual speed bar:
  u8g2.setDrawColor(2);

  // top triangle:
  u8g2.drawTriangle(
    speed_nominal_pos, 5,
    speed_nominal_pos - 4, 0,
    speed_nominal_pos + 4, 0
  );

  // bottom triangle:
  u8g2.drawTriangle(
    speed_nominal_pos, 11,
    speed_nominal_pos + 4, 16,
    speed_nominal_pos - 4, 16
  );

  // print texts:
  u8g2.setFont(u8g2_font_unifont_tf);
  u8g2.drawStr(0, 26, direction_nominal);
  String speed_string = String(abs(speed_nominal)) + String("/");
  if (direction_nominal == "FWD") {
    speed_string += String(SPEED_NOMINAL_MAX);
  } else {
    speed_string += String(abs(SPEED_NOMINAL_MIN));
  }
  u8g2.drawStr(36, 26, speed_string.c_str());

}

void drawSpeedActual(int speed_actual_percent) {

  // calculate pixel values:
  int scaling_factor;
  if (SPEED_NOMINAL_MAX > abs(SPEED_NOMINAL_MIN)) {
    scaling_factor = SPEED_NOMINAL_MAX;
  } else {
    scaling_factor = abs(SPEED_NOMINAL_MIN);
  }
  uint8_t speed_actual_width = (int)(128.0 * ((float)abs(speed_actual_percent) / scaling_factor));

  // actual speed as bar on top of display:
  u8g2.drawBox(0, 0, speed_actual_width, 16);
}

/**
 * This function is intended to draw the contents
 * of the display once every loop.
 * 
 * It has no input parameters as it takes the required
 * data from global variables.
 */
void drawDisplay() {

  u8g2.clearBuffer();

  drawSpeedNominal(speed_nominal);
  drawSpeedActual(speed_actual);

  u8g2.sendBuffer();
}

void setup() {
  
  // start serial:
  Serial.begin(115200);
  
  // init buttons:
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);

  // init display:
  u8g2.begin();

  // connect to wifi:
  if (!wifiConnect(WIFI_SSID, WIFI_SECRET, WIFI_CONNECT_RETRY_DELAY, WIFI_CONNECT_RETRY_TIMEOUT)) {
    if (LOG_TO_SERIAL) {
      Serial.println("Could not connect to WiFi!");
    }
  }

  // connect to mqtt broker:
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttMessageReceivedCallback);

  if(!mqttConnect(DEVICE_ID, mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CONNECT_RETRY_DELAY, MQTT_CONNECT_RETRY_TIMEOUT)) {
    if (LOG_TO_SERIAL) {
      Serial.println("Could not connect to MQTT broker");
    }
  }

  mqttClient.subscribe(String(MQTT_TOPIC_SPEED_ACTUAL).c_str(), 0);
  if (LOG_TO_SERIAL) {
    Serial.print("Subscribed to topic '");
    Serial.print(MQTT_TOPIC_SPEED_ACTUAL);
    Serial.println("'.");
  }

  mqttClient.subscribe(String(MQTT_TOPIC_DIRECTION_ACTUAL).c_str(), 0);
  if (LOG_TO_SERIAL) {
    Serial.print("Subscribed to topic '");
    Serial.print(MQTT_TOPIC_DIRECTION_ACTUAL);
    Serial.println("'.");
  }

  // update display:
  drawDisplay();
  
}

void loop() {

  // -- read mqtt messages

  mqttClient.loop();
  if (mqtt_message_was_received) {

    // read actual direction:
    if (strcmp(mqtt_received_topic, MQTT_TOPIC_DIRECTION_ACTUAL) == 0) {
      if (strcmp(mqtt_received_message, "FWD") == 0) {
        direction_actual = "FWD";
      } else if (strcmp(mqtt_received_message, "REV") == 0) {
        direction_actual = "REV";
      } else {
        if (LOG_TO_SERIAL) {
          Serial.println("ERROR: unexpected message received!");
          Serial.print("    topic:  ");
          Serial.println(mqtt_received_topic);
          Serial.print("    message: ");
          Serial.println(mqtt_received_message);
        }
      }
    }

    // read actual speed:
    if (strcmp(mqtt_received_topic, MQTT_TOPIC_SPEED_ACTUAL) == 0) {
      if (
        String(mqtt_received_message).toInt() <= SPEED_NOMINAL_MAX &&
        String(mqtt_received_message).toInt() >= SPEED_NOMINAL_MIN
      ) {
        speed_actual = String(mqtt_received_message).toInt();
      } else {
        if (LOG_TO_SERIAL) {
          Serial.println("ERROR: received value for 'speed_nominal' out of range!");
          Serial.print("    value: ");
          Serial.println(String(mqtt_received_message).toInt());
        }
      }

      // update display:
      drawSpeedActual(speed_actual);
    }

    mqtt_message_was_received = false;
  }


  // -- read inputs

  if (digitalRead(PIN_BUTTON_DOWN) == HIGH) {

    // change direction if speed is zero:
    if (speed_nominal == 0) {
      if (direction_nominal == "FWD") {
        direction_nominal = "REV";
      } else {
        direction_nominal = "FWD";
      }
    }

    // change speed in forward mode:
    if (direction_nominal == "FWD") {
      if (speed_nominal - SPEED_NOMINAL_STEP > 0) {
        speed_nominal -= SPEED_NOMINAL_STEP;
      } else {
        speed_nominal = 0;
      }
    }

    // change speed in reversed mode:
    if (direction_nominal == "REV") {
      if (speed_nominal + SPEED_NOMINAL_STEP < 0) {
        speed_nominal += SPEED_NOMINAL_STEP;
      } else {
        speed_nominal = 0;
      }
    }

    // update display and send speed via mqtt:
    sendSpeedNominal(speed_nominal);

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_DOWN) == HIGH) {
      delay(100);
    }

  }
  
  // read buttons:
  if (digitalRead(PIN_BUTTON_UP) == HIGH) {

    // change speed in forward mode:
    if (direction_nominal == "FWD") {
      if (speed_nominal + SPEED_NOMINAL_STEP < SPEED_NOMINAL_MAX) {
        speed_nominal += SPEED_NOMINAL_STEP;
      } else {
        speed_nominal = SPEED_NOMINAL_MAX;
      }
    }

    // change speed in reversed mode:
    if (direction_nominal == "REV") {
      if (speed_nominal - SPEED_NOMINAL_STEP > SPEED_NOMINAL_MIN) {
        speed_nominal -= SPEED_NOMINAL_STEP;
      } else {
        speed_nominal = SPEED_NOMINAL_MIN;
      }
    }

    // update display and send speed via mqtt:
    sendSpeedNominal(speed_nominal);

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_UP) == HIGH) {
      delay(10);
    }

  }


  // -- update display

  drawDisplay();


  // -- delay next loop

  delay(10);
  
}
