/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

const int TIME_UNIT = 500000;
const int DOT_DELAY_TIME = TIME_UNIT;
const int DASH_DELAY_TIME = 3 * TIME_UNIT;
const int BETWEEN_CHARACTER_DELAY_TIME = 3 * TIME_UNIT;
const int BETWEEN_WORD_DELAY_TIME = 7 * TIME_UNIT;
enum btnStates
{
    SOS, OK
} btnState;

void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
}

void initTimer(void)
{
    Timer_Handle timer0;
    Timer_Params params;

    Timer_init();
    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL)
    {
        /* Failed to initialize timer */
        while (1)
        {
        }
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        /* Failed to start timer */
        while (1)
        {
        }
    }
}

void outputS()
{
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
}

void outputO()
{
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
}

void outputK()
{
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    usleep(DOT_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
    usleep(DASH_DELAY_TIME);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
}

void delayBetweenWords()
{
    usleep(BETWEEN_WORD_DELAY_TIME);
}

void delayBetweenCharacters()
{
    usleep(BETWEEN_CHARACTER_DELAY_TIME);
}

void outputSOS()
{
    outputS();
    delayBetweenCharacters();
    outputO();
    delayBetweenCharacters();
    outputS();
}

void outputOK()
{
    outputO();
    delayBetweenCharacters();
    outputK();
}
/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    /* Toggle SOS and OK state */
    if (btnState == SOS)
    {
        btnState = OK;
    }
    else
    {
        btnState = SOS;
    }
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    /* Toggle SOS and OK state */
    if (btnState == SOS)
    {
        btnState = OK;
    }
    else
    {
        btnState = SOS;
    }
}

/* Set states enum */
enum states
{
    INIT, SOS_STATE, OK_STATE
} state;

void my_state_machine()
{
    // State Transitions
    switch (state)
    {
    case INIT:
        btnState = SOS;
        if (btnState == SOS)
        {
            state = SOS_STATE;
        }
        else
        {
            state = OK_STATE;
        }
        break;
    case SOS_STATE:
        if (btnState == SOS)
        {
            state = SOS_STATE;
        }
        else
        {
            state = OK_STATE;
        }
        break;
    case OK_STATE:
        if (btnState == SOS)
        {
            state = SOS_STATE;
        }
        else
        {
            state = OK_STATE;
        }
        break;
    default:
        state = SOS_STATE;
        break;
    }

    switch (state)
    {
    case INIT:
        outputSOS();
        delayBetweenWords();
        break;
    case SOS_STATE:
        outputSOS();
        delayBetweenWords();
        break;
    case OK_STATE:
        outputOK();
        delayBetweenWords();
        break;
    default:
        outputSOS();
        delayBetweenWords();
        break;
    }
}

// Problem:
// CS-350 Milestone Three
// Blink the green and red LEDs in the appropriate Morse code patterns: SOS and OK

// Solution:
// Create a loop that alternates between two different states for each Morse code pattern
// Allow for the state to be changed when one of the buttons are pressed, but the current state
// must finish its loop before switching to the next.
/*
 *  ======== mainThread ========
 */
void* mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    initTimer();

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0,
                   GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1)
    {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(CONFIG_GPIO_BUTTON_1,
                       GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
        GPIO_enableInt(CONFIG_GPIO_BUTTON_1);

        state = INIT;
        /* loop through state machine forever */
        while (1)
        {
            my_state_machine();
        }
    }

    return (NULL);
}
