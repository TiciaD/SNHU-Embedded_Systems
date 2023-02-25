# SNHU Embedded Systems
This repo will contains a few of the embedded systems projects I've worked on throughout the CS-350 Emerging Systems, Architecture and Technology class at Southern New Hampshire University using TI Code Composer Studio (CCS) to program the TC CC3220x LAUNCHXL to achieve the different objectives.

### Blinking LEDs
#### Project Overview
Using TI Code Composer Studio (CCS) this program on the TC CC3220x LAUNCHXL was implemented to control an LED from the serial port using a state machine. The state machine is designed to turn on an LED when a user types ON into the console and to turn off the LED when a user types OFF, using only one byte of RAM.  

Requirements:
  - Use only one byte of RAM for the serial buffer
  - Use only one byte of RAM for the state
  - Receive only one character at a time from the UART and it is not buffered
  - Turn on an LED when a user types ON into the console
  - Turn off the LED when a user types OFF into the console


### Morse Code
#### Project Overview
Using TI Code Composer Studio (CCS) this program on the TC CC3220x LAUNCHXL was implemented to send a Morse code message via the LED using a synchronous state machine. The pattern contains an SOS message in Morse code. When a button is pressed, the Morse code message of the blinking LEDs changes to OK.  

Requirements:
  - Call state machine every 500000 us.
  - Continuously blink SOS in Morse code on the green and red LEDs.
  - If a button is pushed, toggle the message between SOS and OK. 
  - Pushing the button in the middle of a message should NOT change the message until it is complete.

### Thermostat
#### Project Overview
Using TI Code Composer Studio (CCS) this program on the TC CC3220x LAUNCHXL was implemented to create a prototype of low-level thermostat functionality by reading room temperature, indicating the result via LED output, increasing or decreasing the set temperature, and simulating the data being sent to a server.  

Requirements:
  - Check the buttons every 200ms,
  - Check the temperature every 500ms
  - Update the LED and report to the server every second (via the UART) 
  - If a button is pressed, it increases or decreases the temperature set-point by 1 degree every 200ms 
  - If the temperature is greater than the set-point, the LED should turn off 
  - If the temperature is less than the set-point, the LED should turn on
  - The output to the server (via UART) should be formatted as <AA,BB,S,CCCC>
    - AA = ASCII decimal value of room temperature (00 - 99) degrees Celsius
    - BB = ASCII decimal value of set-point temperature (00-99) degrees Celsius
    - S = ‘0’ if heat is off, ‘1’ if heat is on
    - CCCC = decimal count of seconds since board has been reset

## Analysis
Each of these projects had different use cases and solved different problems, my final project (Thermostat) in particular could be used as a prototype to have a proof-of-concept for a more sophisticated thermostat embedded system. I think I did a good job emulating how a real thermostat would work with the way it adjusts based on the button presses and outputs the results regularly. I also feel I did a good job in keeping my code clean by avoiding magic numbers, leaving comments and using good naming conventions. To improve the project I would like to get more familiar with the Task Scheduler design so I can refine it more for other interrupts or tasks like adding a clock or Air Conditioning feature. The way the Task Scheduler is setup makes it easy to add additional tasks in the future should I come back to this project. I've found many resources to help develop my coding skills like Codecademy and Udemy, as well as practice sites to help me become more comfortable with C language. I feel like state management is something that's utilized in most, if not all, software applications and gaining a better understanding of it with these projects will be transferable to other software projects I work on regardless of language or the type of application. 
