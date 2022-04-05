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

/*
 * Enumerates
 */

typedef enum{
    BUZZER_TYPE_PASSIVE,
    BUZZER_TYPE_ACTIVE
}buzzer_type_e;

/*
 * Functions typedefs
 */

typedef void (*pwmOutFx)(uint32_t freq);
typedef void (*gpioOutFx)(uint32_t val);


/*
 * Structs and Unions
 */

typedef struct{
    buzzer_type_e type;
    uint_fast8_t active;
    uint_fast16_t baseTimeMs;
    uint_fast16_t counting;
    struct{
        uint_fast16_t *pTimes;
        uint_fast16_t *pFreq;
        uint_fast16_t i;
        uint_fast16_t len;

        int_fast32_t time;
        uint_fast16_t freq;

        uint_fast8_t loop;
    }play_param;
    struct{
    	pwmOutFx pwmOut;
    	gpioOutFx gpioOut;
    }fnx;
}buzzer_t;

/*
 * Functions Prototypes
 */

void buzzer_init_params(buzzer_t *buzzer);

void buzzer_init(buzzer_t *buzzer);

void buzzer_stop(buzzer_t *buzzer);

void buzzer_turn_on(buzzer_t *buzzer, uint16_t freq);

void buzzer_start(buzzer_t *buzzer, uint16_t freq, uint16_t period, uint8_t loop);

void buzzer_start_array(buzzer_t *buzzer, uint16_t *pPeriod, uint16_t *pFreq, uint16_t len);

uint8_t buzzer_is_active(buzzer_t *buzzer);

// call this function in a periodic timing
void buzzer_interrupt(buzzer_t *buzzer);

// callback
void buzzer_end_callback(buzzer_t *buzzer);


#endif /* APPLICATION_BUZZER_H_ */
