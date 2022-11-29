/*
 * buzzer.h
 *
 *  Created on: 9 de fev de 2022
 *      Author: pablo.jean
 */

#ifndef APPLICATION_BUZZER_H_
#define APPLICATION_BUZZER_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "notes.h"
#include "ringtones.h"

/*
 * Enumerates
 */

typedef enum{
    BUZZER_TYPE_PASSIVE,
    BUZZER_TYPE_ACTIVE
}buzzer_type_e;

typedef enum{
	BUZZER_LOOP_OFF,
	BUZZER_LOOP_ON
}buzzer_loop_e;

typedef enum{
	BUZZER_IS_NOT_ACTIVE,
	BUZZER_IS_ACTIVE
}buzzer_active_e;

typedef enum{
	BUZZER_ERR_OK,
	BUZZER_ERR_FAIL,
	BUZZER_ERR_PARAMS,

	BUZZER_ERR_UNKNOWN = 0xFF
}buzzer_err_e;

/*
 * Functions typedefs
 */

typedef void (*pwmOutFx)(uint32_t freq);
typedef void (*gpioOutFx)(uint32_t val);


/*
 * Structs and Unions
 */

typedef struct{
	uint8_t started;
    buzzer_type_e type;
    buzzer_active_e active;
    uint_fast16_t interruptMs;
    uint_fast16_t counting;
    struct{
        uint16_t *pTimes;
        uint16_t *pFreq;
        uint_fast16_t i;
        uint_fast16_t len;

        int_fast32_t time;
        uint_fast16_t freq;

        buzzer_loop_e loop;
    }play_param;
    struct{
    	pwmOutFx pwmOut;
    	gpioOutFx gpioOut;
    }fnx;
}buzzer_t;

/*
 * Functions Prototypes
 */

/**
 * Initialize the buzzer
 */
buzzer_err_e buzzer_init(buzzer_t *buzzer);

/**
 * Stop buzzer immediately
 */
void buzzer_stop(buzzer_t *buzzer);

/**
 * Turnon buzzer immediately
 */
void buzzer_turn_on(buzzer_t *buzzer, uint16_t freq);

/**
 * Start buzzer for a determined period
 * If loop is BUZZER_LOOP_ON the buzzer will turn on and turn off
 * with the period
 * buzzer_interrupt must be working
 */
void buzzer_start(buzzer_t *buzzer, uint16_t freq, uint16_t period, buzzer_loop_e loop);

/**
 * Start to play a array of period and frequencies, generally used to play ringtones
 * buzzer_interrupt must be working
 */
void buzzer_start_array(buzzer_t *buzzer, uint16_t *pPeriod, uint16_t *pFreq, uint16_t len);

/**
 * Return if Buzzer is active
 */
buzzer_active_e buzzer_is_active(buzzer_t *buzzer);

// call this function in a periodic timing, if you're using a RTOS
// we suggest you to use a task to handle this with a sleep
// the period of interrupt is configured on buzzer_t structure
void buzzer_interrupt(buzzer_t *buzzer);

// callback when an array has your execution finished
void buzzer_end_callback(buzzer_t *buzzer);


#endif /* APPLICATION_BUZZER_H_ */
