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
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

UART_Handle uart;
char input;

/* Set states enum */
enum states {LED_INIT, WAIT, INPUT_O, INPUT_OF, LED_ON, LED_OFF} state;

// initialize state
const char  echoOConfirmation[] = "Echoing we are in INPUT_O state:\r\n";
const char  echoWaitConfirmation[] = "Echoing we are in WAIT state:\r\n";
const char  echoONConfirmation[] = "Echoing we are in LED_ON state:\r\n";
const char  echoOFConfirmation[] = "Echoing we are in INPUT_OF state:\r\n";
const char  echoOFFConfirmation[] = "Echoing we are in LED_OFF state:\r\n";

void my_state_machine () {
    UART_read(uart, &input, 1);
    // State Transitions
    switch (state) {
        case LED_INIT:
            state = WAIT;
            break;
        case WAIT:
            if (input == 'O') {
                state = INPUT_O;
            }
            break;
        case INPUT_O:
            if (input == 'N') {
                state = LED_ON;
            } else if (input == 'F') {
                state = INPUT_OF;
            } else {
                state = WAIT;
            }
            break;
        case LED_ON:
            if (input == 'O') {
                state = INPUT_O;
            }  else {
                state = WAIT;
            }
            break;
        case INPUT_OF:
            if (input == 'F') {
                state = LED_OFF;
            }  else if (input == 'O') {
                state = INPUT_O;
            } else {
                state = WAIT;
            }
            break;
        case LED_OFF:
            if (input == 'O') {
                state = INPUT_O;
            } else {
                state = WAIT;
            }
            break;
        default:
            state = WAIT;
            break;
    }

    switch (state) {
        case LED_INIT:
            // Turn LED off when initialized
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
            break;
        case LED_ON:
            // Turn LED on when in LED_ON state
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
            break;
        case LED_OFF:
            // Turn LED off when in LED_OFF state
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
            break;
        default:
            break;
    }
    UART_write(uart, &input, 1);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{

    const char  echoPrompt[] = "Echoing characters:\r\n";

    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    /* Turn on user LED to indicate successful initialization */
//    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    UART_write(uart, echoPrompt, sizeof(echoPrompt));
    state = LED_INIT;

    /* Loop forever echoing */
    while (1) {
        my_state_machine();
    }
}
