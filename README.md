# Introduction

A tiny and simple buzzer library, implements a simple turn on, turn off, and the capability to play loops and ringtones.
All functions work in NON-BLOCKING MODE, so, your application will never stop the execution while handling the device, and, inclusively, implementing a `callback` when an operation end.

Supports any microcontroller and Passive or Active buzzer.

### Requirements

- A GPIO (active) or PWM (passive) output;
- A Task or timer interrupt;

### Features
- Easy to use;
- Start and stop manually;
- Start for a defined time;
- "Blinks" with a fixed period;
- Play ringtones;
- Non-blocking functions;
- Callback to tell you that an operation is finished.

# How to Use

First, declares your ```buzzer_t``` with, at least, the ```pwmOut``` or ```gpioOut``` (just pick one! the other fxn must be `NULL`). About the functions:

- `void pwmOut(uint32_t freq)` : Is used for PASSIVE buzzers, this function returns `void` and receives only the `freq` argument. When implementing, the freq=0, the device MUST turn off PWM.
- `void gpioOut(uint32_t val)` : Is used for the ACTIVE buzzer, the value `val` needs to be turn on when is `1`, and `0` turnoff the Buzzer. These buzzers can't set frequency values.

The lib will automatically identify the Buzzer type on `buzzer_init()`, by checking the defined fxn.
Another parameter that needs to be implemented, if the user wants to user `buzzer_start` and `buzzer_start_array`, is the `interruptMs`, which only indicates how often you will call the `buzzer_interrupt` in a timer/task routine, in milliseconds.

This parameter serves to chronometer the buzzer, turning it on and off, for loops, ringtones, etc.

## Configuring for PASSIVE Buzzer

Bellow, in this example, we are configuring a buzzer in a generic chipset, with a 50ms timer interrupt and a PWM function.

```C
void __pwm_buzzer_chipset(uint32_t freq);
// Buzzer Handler
buzzer_t Buzzer = {  
  .pwmOut = __pwm_chipset,  
  .interruptMs = 50
}

// Functions
void __pwm_buzzer_chipset(uint32_t freq){
  if (freq == 0){    
    chipset_pwm_turnoff();  
  }  
  else{    
    chipset_pwm_turnon();    
    chipset_pwm_set_freq(freq);  
  }
}
  
// Interrupts
void __tim_interrupt_50ms(){  
  buzzer_interrupt(&Buzzer);
}

// Main
void main(){
  buzzer_init(&Buzzer);
}
```

## Configuring for ACTIVE Buzzer

In this example, an active buzzer is configured and implemented, with a 100ms timer interrupt and GPIO to control the Buzzer.
```C
void __gpio_pwm_chipset(uint32_t val);
// Buzzer Handler
buzzer_t Buzzer = {  
  .pwmOut = __pwm_chipset,  
  .interruptMs = 100
}
  
// Functions
void __gpio_pwm_chipset(uint32_t val){  
  chipset_gpio(BUZZER_GPIO_Port, BUZZER_Pin, val);
}

// Interrupts
void __tim_interrupt_100ms(){  
  buzzer_interrupt(&Buzzer);
}

// Main
void main(){  
  buzzer_init(&Buzzer);
}
```

# Examples

For the examples, consider a Passive Buzzer, configured with a timer and everything :) For Active buzzers, all freq parameters can be `0` or `NULL`.

## Turnon buzzer and Turnoff manually

```C
void main(){  
  ...  
  // turn on the buzzer with a 1500Hz frequency  
  buzzer_turn_on(&Buzzer, 1500);    
  
  // delay for 500ms  
  chipset_delay_ms(500);    
  
  // turnoff buzzer  
  buzzer_stop(&Buzzer);
}
```

## Turnon buzzer for 500ms

```C
void main(){  
  ...  
  // turnon buzzer with a 1500Hz frequency for 500ms  
  buzzer_start(&Buzzer, 1500, 500, BUZZER_LOOP_OFF);
}
```

## "Blink" buzzer for a period of 500ms

```C
void main(){  
  ...  
  // turn on the buzzer with a 2500Hz frequency  
  buzzer_start(&Buzzer, 2500, 500, BUZZER_LOOP_ON);    
  
  // The BUZZER_LOOP_ON parameter indicates that the Buzzer will be turnon for 500ms,
  // turnoff for the same period, and repeat the process
}
```

## Play Super Mario Ringtone and turn on a LED after finish

```C
// this callback is called when a buzzer_start or buzzer_start_array finish
// the solicited operation. In this case, when the Mario Ringtone is
// finished, the callback will be called
void buzzer_end_callback(buzzer_t *buzzer){  
  led_red(TRUE);
}

void main(){  
  ...  
  // play the Mario Theme ringtone, that's included in the library :D  
  buzzer_start_array(&Buzzer, mario_theme_time, mario_theme_melody, mario_theme_len);
}
```

# Doubts

Any doubts, or issues, just post an issue. We have too an example implemented on an STM32F411 (Black Pill).
Best regards folks.
