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

typedef struct esp_mqtt_client* esp_mqtt_client_handle_t;

/*
 * Enumeration for hash operations.
 * When WPA2 is connecting, this enum is used to
 * request a hash algorithm via crypto_hash_xxx functions.
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
 * Enumeration for hash operations.
 * When WPA2 is connecting, this enum is used to
 * request a hash algorithm via crypto_hash_xxx functions.
 */
typedef enum {
    MQTT_TRANSPORT_UNKNOWN = 0x0,
    MQTT_TRANSPORT_OVER_TCP,
    MQTT_TRANSPORT_OVER_SSL,
    MQTT_TRANSPORT_OVER_WS,
    MQTT_TRANSPORT_OVER_WSS
} esp_mqtt_transport_t;


typedef struct {
    esp_mqtt_event_id_t event_id;
    esp_mqtt_client_handle_t client;
    void *user_context;
    char *data;
    int data_len;
    int total_data_len;
    int current_data_offset;
    char *topic;
    int topic_len;
    int msg_id;
} esp_mqtt_event_t;

/*
 * This structure is about the algorithm when do crypto_hash operation, for detail,
 * please reference to the structure crypto_hash.
 */
typedef esp_mqtt_event_t* esp_mqtt_event_handle_t;

/**
  * @brief The crypto callback function used in wpa enterprise hash operation when connect.
  *        Initialize a esp_crypto_hash_t structure.
  *
  * @param alg  Hash algorithm.
  *
  */
typedef esp_err_t (* mqtt_event_callback_t)(esp_mqtt_event_handle_t event);

/*
 * This structure is about the algorithm when do crypto_cipher operation, for detail,
 * please reference to the structure crypto_cipher.
 */
typedef struct {
    mqtt_event_callback_t event_handle;
    char host[MQTT_MAX_HOST_LEN];
    char uri[MQTT_MAX_HOST_LEN];
    uint32_t port;
    char client_id[MQTT_MAX_CLIENT_LEN];
    char username[MQTT_MAX_USERNAME_LEN];
    char password[MQTT_MAX_PASSWORD_LEN];
    char lwt_topic[MQTT_MAX_LWT_TOPIC];
    char lwt_msg[MQTT_MAX_LWT_MSG];
    int lwt_qos;
    int lwt_retain;
    int lwt_msg_len;
    int disable_clean_session;
    int keepalive;
    bool disable_auto_reconnect;
    void *user_context;
    int task_prio;
    int task_stack;
    int buffer_size;
    const char *cert_pem;
    esp_mqtt_transport_t transport;
} esp_mqtt_client_config_t;

/**
 * @brief
 * @param
 * @return
 */
esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t *config);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_set_uri(esp_mqtt_client_handle_t client, const char *uri);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_stop(esp_mqtt_client_handle_t client);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_subscribe(esp_mqtt_client_handle_t client, const char *topic, int qos);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_unsubscribe(esp_mqtt_client_handle_t client, const char *topic);
/**
 * @brief
 * @param
 * @return
 */
int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain);
/**
 * @brief
 * @param
 * @return
 */
esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client);

#endif
