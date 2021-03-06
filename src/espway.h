#pragma once

extern "C" {
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <httpd/httpd.h>

#include "lib/pid.h"
#include "lib/imu_hal.h"
#include "lib/vector3d.h"
}

#include "espway_config.h"

#define SAMPLE_TIME IMU_SAMPLE_TIME

#define FALL_LOWER_BOUND FLT_TO_Q16(STABLE_ANGLE - FALL_LIMIT)
#define FALL_UPPER_BOUND FLT_TO_Q16(STABLE_ANGLE + FALL_LIMIT)
#define RECOVER_LOWER_BOUND FLT_TO_Q16(STABLE_ANGLE - RECOVER_LIMIT)
#define RECOVER_UPPER_BOUND FLT_TO_Q16(STABLE_ANGLE + RECOVER_LIMIT)
#define ROLL_LOWER_BOUND FLT_TO_Q16(-ROLL_LIMIT)
#define ROLL_UPPER_BOUND FLT_TO_Q16(ROLL_LIMIT)

enum ws_msg_t {
  STEERING = 0,

  REQ_GRAVITY = 1,
  RES_GRAVITY = 2,

  BATTERY = 3,
  BATTERY_CUTOFF = 4,

  REQ_SET_PID_PARAMS = 5,
  RES_SET_PID_PARAMS_ACK = 6,
  REQ_GET_PID_PARAMS = 7,
  RES_PID_PARAMS = 8,

  REQ_SET_GYRO_OFFSETS = 9,
  RES_SET_GYRO_OFFSETS_FAILURE = 10,
  RES_SET_GYRO_OFFSETS_SUCCESS = 11,

  REQ_SAVE_CONFIG = 12,
  RES_SAVE_CONFIG_FAILURE = 13,
  RES_SAVE_CONFIG_SUCCESS = 14,

  REQ_CLEAR_CONFIG = 15,
  RES_CLEAR_CONFIG_FAILURE = 16,
  RES_CLEAR_CONFIG_SUCCESS = 17,

  REQ_LOAD_FLASH_CONFIG = 18,
  RES_LOAD_FLASH_CONFIG_DONE = 19,

  REQ_ENABLE_MOTORS = 20,
  REQ_DISABLE_MOTORS = 21
};

struct espway_config {
  pid_coeffs pid_coeffs_arr[2];
  int16_t gyro_offsets[3];
};

enum pid_controller_index {
  ANGLE = 0,
  VEL
};

extern TaskHandle_t xSteeringWatcher;

extern espway_config my_config;

extern SemaphoreHandle_t pid_mutex;
extern pidsettings pid_settings_arr[2];

extern SemaphoreHandle_t orientation_mutex;
extern vector3d_fix gravity;

extern q16 target_speed;
extern q16 steering_bias;

void pretty_print_config();
void apply_config_params();
bool save_flash_config();
bool clear_flash_config();
void load_config();
void load_hardcoded_config();
void update_pid_controller(pid_controller_index idx, q16 p, q16 i, q16 d);

void httpd_task(void *pvParameters);

void battery_cutoff();
