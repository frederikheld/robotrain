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

bool mqtt_message_is_received = false;
char* mqtt_received_message;

int speed_nominal = 0;
int speed_actual = 0;
char* direction_nominal = "FWD";
char* direction_actual = "FWD";


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

void drawSpeedometer(int speed_actual_percent, int speed_nominal_percent) {

  Serial.println(speed_nominal_percent);

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
  uint8_t speed_actual_width = (int)(128.0 * ((float)abs(speed_actual_percent) / scaling_factor));
  uint8_t speed_nominal_pos = (int)(128.0 * ((float)abs(speed_nominal_percent) / scaling_factor));

  uint8_t limiter_pos = 0;
  if (direction_nominal == limiter_direction) {
    limiter_pos = (int)(128.0 * ((float)abs(limiter_percent) / scaling_factor));
  }

  Serial.println(speed_nominal_pos);

  // actual speed as bar on top of display:
  u8g2.drawBox(0, 0, speed_actual_width, 16);

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

  // print text if direction is reversed:
  u8g2.setFont(u8g2_font_unifont_tf);
  u8g2.drawStr(103, 26, direction_nominal);

}

void setSpeed(int speed_nominal_percent, int speed_actual_percent) {

  // update display:
  u8g2.clearBuffer();
  drawSpeedometer(speed_actual_percent, speed_nominal_percent);
  u8g2.sendBuffer();

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

void setup() {
  
  // start serial:
  Serial.begin(9600);
  
  // init buttons:
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);

  // init display:
  u8g2.begin();

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

  // DEBUG:
  randomSeed(analogRead(A0));

  // update display:
  u8g2.clearBuffer();
  drawSpeedometer(speed_actual, speed_nominal);
  u8g2.sendBuffer();
  
  
}

void loop() {

  if (digitalRead(PIN_BUTTON_DOWN) == HIGH) {

    // DEBUG:
    speed_actual = random(0, 100);

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
    setSpeed(speed_nominal, speed_actual);

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_DOWN) == HIGH) {
      delay(100);
    }

  }
  
  // read buttons:
  if (digitalRead(PIN_BUTTON_UP) == HIGH) {

    // DEBUG:
    speed_actual = random(0, 100);

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
    setSpeed(speed_nominal, speed_actual);

    // wait for falling edge:
    while (digitalRead(PIN_BUTTON_UP) == HIGH) {
      delay(100);
    }

  }

  delay(10);
  
}
