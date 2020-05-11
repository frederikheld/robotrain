// -- DEVICE

// identity:
#define DEVICE_ID "0" // has to be unique within the Pot Pourri network!

// pinning:
#define PIN_BUTTON_UP D0
#define PIN_BUTTON_DOWN D5

#define PIN_DISPLAY_SCL D1
#define PIN_DISPLAY_SDA D2


// -- CONECTIVITY

// wifi:
#define WIFI_SSID "<wifi-ssid>"
#define WIFI_SECRET "<wifi-secret>"
#define WIFI_CONNECT_RETRY_DELAY 500 // time to wait (in milliseconds) before retrying if connect to wifi failed
#define WIFI_CONNECT_RETRY_TIMEOUT 10000 // time (in milliseconds) until a failed attempt to connect to wifi will be aborted

// mqtt:
#define MQTT_SERVER "<mqtt-broker-ip-or-hostname>"
#define MQTT_PORT 1883
#define MQTT_CONNECT_RETRY_DELAY 500 // time to wait (in milliseconds) before retrying if connect to mqtt broker failed. The connection attempt itself will timeout after 1000 ms.
#define MQTT_CONNECT_RETRY_TIMEOUT 10000 // time (in milliseconds) until a failed attempt to connect to mqtt broker will be aborted. This includes 1000 ms per connection attempt as well as MQTT_CONNECT_RETRY_DELAY.
#define MQTT_SEND_REBOUNCE_DELAY 500 // time to wait (in milliseconds) before retrying to receive a rebounced message
#define MQTT_SEND_REBOUNCE_TIMEOUT 10000 // time to wait (in milliseconds) for an sent MQTT message to be rebounced


// -- MQTT TOPICS

#define MQTT_TOPIC_SPEED_NOMINAL "robotrain/motor-control/speed/nominal"
#define MQTT_TOPIC_SPEED_ACTUAL "robotrain/motor-control/speed/actual"


// -- VALUES

#define SPEED_NOMINAL_STEP 10 // steps to increas/decrease "speed nominal" with each click of a button
#define SPEED_NOMINAL_MIN 0 // min value of "speed nominal"
#define SPEED_NOMINAL_MAX 100 // max value of "speed nominal"
