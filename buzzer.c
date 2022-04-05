/*
 * buzzer.c
 *
 *  Created on: 9 de fev de 2022
 *      Author: pablo.jean
 */

#include "buzzer.h"

/**
 * Macros
 */

#define _HIGH	1
#define _LOW	0

/**
 * privates
 */



// aux functions

void __buzzer_stop_gpio(buzzer_t *buzzer){
	if (buzzer->fnx.gpioOut != NULL)
		buzzer->fnx.gpioOut(_LOW);
}

void __buzzer_stop_pwm(buzzer_t *buzzer){
	if (buzzer->fnx.pwmOut != NULL)
		buzzer->fnx.pwmOut(0);
}

void __buzzer_turn_on_gpio(buzzer_t *buzzer){
	if (buzzer->fnx.gpioOut != NULL)
		buzzer->fnx.gpioOut(_HIGH);
}

void __buzzer_turn_on_pwm(buzzer_t *buzzer, uint32_t freq){
	if (buzzer->fnx.pwmOut != NULL)
		buzzer->fnx.pwmOut(freq);
}

void __buzzer_start_gpio(buzzer_t *buzzer){
    __buzzer_turn_on_gpio(buzzer);
}

void __buzzer_start_pwm(buzzer_t *buzzer){
    __buzzer_turn_on_pwm(buzzer, buzzer->play_param.freq);
}


void __buzzer_start_array_gpio(buzzer_t *buzzer){
    __buzzer_turn_on_gpio(buzzer);
    buzzer->play_param.time = buzzer->play_param.pTimes[0];

}

void __buzzer_start_array_pwm(buzzer_t *buzzer){
    __buzzer_turn_on_pwm(buzzer, buzzer->play_param.pFreq[0]);
    buzzer->play_param.time = buzzer->play_param.pTimes[0];
}




/*
 * Publics
 */

// callback

void __attribute__((weak)) buzzer_end_callback(buzzer_t *buzzer){

}

// interrupts

void buzzer_interrupt(buzzer_t *buzzer){
	uint16_t i;

	buzzer->counting += buzzer->baseTimeMs;
	if (buzzer->active &&
			buzzer->play_param.len > 0 &&
			buzzer->counting > buzzer->play_param.time){
		i = buzzer->play_param.i;
		buzzer->counting = 0;
		buzzer->play_param.i++;
		if (buzzer->play_param.i < buzzer->play_param.len){
			if (buzzer->play_param.pTimes == NULL){
				if (buzzer->play_param.loop == 1){
					buzzer->play_param.i %= 2;
				}
				if (buzzer->type == BUZZER_TYPE_ACTIVE){
					if (buzzer->play_param.i){
						__buzzer_stop_gpio(buzzer);
					}
					else{
						__buzzer_turn_on_gpio(buzzer);
					}
				}
			}
			else{
				buzzer->play_param.time = buzzer->play_param.pTimes[i];
				buzzer->play_param.freq = buzzer->play_param.pFreq[i];
				__buzzer_turn_on_pwm(buzzer, buzzer->play_param.freq);
			}
		}
		else{
			if (buzzer->type == BUZZER_TYPE_ACTIVE){
				__buzzer_stop_gpio(buzzer);
			}
			else{
				__buzzer_stop_pwm(buzzer);
			}
			buzzer_end_callback(buzzer);
			buzzer->active = 0;
		}
	}
}

/*
 * Default values:
 * baseTimeMs = 1;
 * type = BUZZER_TYPE_PASSIVE;
 *
 * the rest is zero
 */
void buzzer_init_params(buzzer_t *buzzer){
	if (buzzer != NULL){
		memset(buzzer, 0, sizeof(buzzer_t));
		buzzer->baseTimeMs = 1;
		buzzer->type = BUZZER_TYPE_PASSIVE;
	}
}

void buzzer_init(buzzer_t *buzzer){
    if (buzzer != NULL){
    	// nothing to do here
    }
}

void buzzer_stop(buzzer_t *buzzer){
    if (buzzer != NULL){
        buzzer->active = 0;
        if (buzzer->type == BUZZER_TYPE_ACTIVE){
            __buzzer_stop_gpio(buzzer);
        }
        else if (buzzer->type == BUZZER_TYPE_PASSIVE){
            __buzzer_stop_pwm(buzzer);
        }
    }
}

void buzzer_turn_on(buzzer_t *buzzer, uint16_t freq){
    if (buzzer != NULL){
        buzzer->active = 1;
        buzzer->play_param.loop = 0;
        buzzer->play_param.len = 0;
        if (buzzer->type == BUZZER_TYPE_ACTIVE){
            __buzzer_turn_on_gpio(buzzer);
        }
        else if (buzzer->type == BUZZER_TYPE_PASSIVE){
        	buzzer->play_param.freq = freq;
            __buzzer_turn_on_pwm(buzzer, freq);
        }
    }
}

void buzzer_start(buzzer_t *buzzer, uint16_t freq, uint16_t period, uint8_t loop){
    if (buzzer != NULL){
        buzzer->play_param.i = 0;
        buzzer->play_param.time = period;
        buzzer->play_param.loop = loop;
        buzzer->play_param.pTimes = NULL;
        buzzer->play_param.pFreq = NULL;
        buzzer->active = 1;
        buzzer->play_param.len = 2;
        if (buzzer->type == BUZZER_TYPE_ACTIVE){
            __buzzer_start_gpio(buzzer);
        }
        else if (buzzer->type == BUZZER_TYPE_PASSIVE){
        	buzzer->play_param.freq = freq;
            __buzzer_start_pwm(buzzer);
        }
    }
}

void buzzer_start_array(buzzer_t *buzzer, uint16_t *pPeriod, uint16_t *pFreq, uint16_t len){
    if (buzzer != NULL && pPeriod != NULL){
        buzzer->play_param.len = len;
        buzzer->play_param.pTimes = pPeriod;
        buzzer->play_param.pFreq = pFreq;
        buzzer->play_param.loop = 0;
        buzzer->active = 1;
        if (buzzer->type == BUZZER_TYPE_ACTIVE){
            __buzzer_start_array_gpio(buzzer);
        }
        else if (buzzer->type == BUZZER_TYPE_PASSIVE){
            __buzzer_start_array_pwm(buzzer);
        }
    }
}

uint8_t buzzer_is_active(buzzer_t *buzzer){
    if (buzzer != NULL){
        return buzzer->active;
    }
    return 0;
}
