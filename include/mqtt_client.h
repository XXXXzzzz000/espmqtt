/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 * Tuan PM <tuanpm at live dot com>
 */

#pragma once
/**
 * @file mqtt_client.h
 *
 * //TODO:添加头文件说明
 * This header file defines interface between PM lock functions (pm_locks.c)
 * and the chip-specific power management (DFS/light sleep) implementation.
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "mqtt_config.h"
#include "esp_err.h"

typedef struct esp_mqtt_client *esp_mqtt_client_handle_t;

/*
 * Enumeration for mqtt event type used in esp_mqtt_event_t.
 */
typedef enum {
    MQTT_EVENT_ERROR = 0,
    MQTT_EVENT_CONNECTED,
    MQTT_EVENT_DISCONNECTED,
    MQTT_EVENT_SUBSCRIBED,
    MQTT_EVENT_UNSUBSCRIBED,
    MQTT_EVENT_PUBLISHED,
    MQTT_EVENT_DATA,
} esp_mqtt_event_id_t;

/*
 * Enumeration for transport type used in esp_mqtt_client_config_t.
 */
typedef enum {
    MQTT_TRANSPORT_UNKNOWN = 0x0,
    MQTT_TRANSPORT_OVER_TCP,
    MQTT_TRANSPORT_OVER_SSL,
    MQTT_TRANSPORT_OVER_WS,
    MQTT_TRANSPORT_OVER_WSS
} esp_mqtt_transport_t;

/**
 * @brief mqtt event
 *
 * this struct will be used in function mqtt_event_callback_t
 */
typedef struct
{
    esp_mqtt_event_id_t event_id;    /*!< the event type */
    esp_mqtt_client_handle_t client; /*!< the client which received the event */
    void *user_context;              /*!< user context pointer */
    char *data;                      /*!< the event data(mqtt payload) */
    int data_len;                    /*!< the data length */
    int total_data_len;              /*!< if the data too long, it's save the total length */
    int current_data_offset;         /*!< the data index of all data */
    char *topic;                     /*!< mqtt topic pointer */
    int topic_len;                   /*!< mqtt topic length */
    int msg_id;                      /*!< mqtt message id */
} esp_mqtt_event_t;

typedef esp_mqtt_event_t *esp_mqtt_event_handle_t;

/**
  * @brief The mqtt event callback function used in handle the mqtt event.
  *
  * @param event  mqtt event.
  */
typedef esp_err_t (*mqtt_event_callback_t)(esp_mqtt_event_handle_t event);

/*
 * This structure is used to initialization the mqtt client.
 */
typedef struct
{
    mqtt_event_callback_t event_handle;   /*!< handle for MQTT events */
    char host[MQTT_MAX_HOST_LEN];         /*!< MQTT server domain (ipv4 as string) */
    char uri[MQTT_MAX_HOST_LEN];          /*!<  */
    uint32_t port;                        /*!< MQTT server port */
    char client_id[MQTT_MAX_CLIENT_LEN];  /*!< default client id is ESP32_%CHIPID% */
    char username[MQTT_MAX_USERNAME_LEN]; /*!< MQTT username */
    char password[MQTT_MAX_PASSWORD_LEN]; /*!< MQTT password */

    char lwt_topic[MQTT_MAX_LWT_TOPIC]; /*!< lwt_topic, lwt_msg, lwt_qos, lwt_retain,  */
    char lwt_msg[MQTT_MAX_LWT_MSG];     /*!< lwt_msg_len are mqtt lwt options, default NULL */
    int lwt_qos;
    int lwt_retain;
    int lwt_msg_len;
    int disable_clean_session;      /*!< mqtt clean session, default is true */
    int keepalive;                  /*!< (value in seconds) mqtt keepalive, default is 120 seconds */
    bool disable_auto_reconnect;    /*!< this mqtt client will reconnect to server (when errors/disconnect). Set true to disable */
    void *user_context;             /*!< pass user context to this option, then can receive that context in event->user_context */
    int task_prio;                  /*!< task_prio, task_stack */
    int task_stack;                 /*!< for MQTT task, default priority is 5, and task_stack = 6144 bytes (or default task stack can be set via make menucofig). */
    int buffer_size;                /*!< for MQTT send/receive buffer, default is 1024 */
    const char *cert_pem;           /*!< pointer to CERT file for server verify (with SSL), default is NULL, not required to verify the server */
    esp_mqtt_transport_t transport; /*!< override URI transport */
} esp_mqtt_client_config_t;

/**
 * @brief Initialization the mqtt client.
 *
 * @param config the esp_mqtt_client_config_t pointer
 * @return mqtt client handle
 */
esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t *config);
/**
 * @brief set client config (host and port ...) from uri
 *
 * @param client mqtt client handle
 * @param uri the uri string
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_set_uri(esp_mqtt_client_handle_t client, const char *uri);
/**
 * @brief start mqtt client
 *
 * @param client mqtt client handle
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client);
/**
 * @brief stop mqtt client
 *
 * @param client mqtt client handle
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_stop(esp_mqtt_client_handle_t client);
/**
 * @brief subscribe to a topic useing client
 *
 * @param client mqtt client handle
 * @param topic tpoic name
 * @param qos qos level (0-2)
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_subscribe(esp_mqtt_client_handle_t client, const char *topic, int qos);
/**
 * @brief unsubscribe to a topic
 *
 * @param client mqtt client handle
 * @param tpoic topic name
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_unsubscribe(esp_mqtt_client_handle_t client, const char *topic);
/**
 * @brief publish a topic to the broker
 *
 * @param client mqtt client handle
 * @param tpoic topic name
 * @param data the payload in mqtt message
 * @param len the length of data
 * @param qos qos level (0-2)
 * @param retain mqtt message retain flag
 *
 * @return message id on success , -1 on failed
 */
int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain);
/**
 * @brief destory a mqtt client
 * @param client mqtt client handle
 * @return ESP_OK on success
 */
esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client);
