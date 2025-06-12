# Card Badging Mechanical Automation

## Phases
+ **Design Phase** (5/26/25 to 5/30/25)
+ **Component Setup and Testing** (Estimated: 6/4/25 to 6/6/25; Actual: 6/2/25 to 6/20/25)
+ **Construction** (Estimated: 6/9/25 to 6/23/25)
+ **Automation** (Estimated: 6/16/25 to 6/20/25)
+ **Refinement + Adding Features**


## Goals
The primary goal of this project is to create an automation that badges card for a user in a certain order. This design must be scalable, quick, and easy to use. It should be flexible as well; my intention is to make this as easy for a team to use as possible.



## Design Phase

---

### Design 1 (5/26 onwards)

For my first prototype, I'm creating a solution that can badge 16 cards in quick succession. It consists of a drum rotated accurately by a stepper motor. The drum has 16 holes that fit card slots into them. These card slots are lifted by a cam (operated by a micro servo) into the range of a stationary reader. 

In order to scale this design up, an additional cam must be placed under each new reader. Since micro servos are inexpensive, this design is simultaneously cost effective. Ideally, the larger the drum, the more readers can be incorporated.

##### Onshape

This is my first time using Onshape. I spent about a day learning Onshape and how to create models accurate to scale. I then set out to create the models by sketching designs on faces and extruding them as necessary. Throughout the process, the idea behind the design remained relatively unchanged, but I made one significant revision: making the range of motion of the cams a little larger and moving the readers further away from the cards in order to prevent interference/reading multiple cards accidentally. I've also tried to use mates in my model. Revolute_1 and Revolute_2 in the Onshape document are fully functional and can be animated, but I don't have a way to test a whole workflow using Onshape.


##### Print

I've sent the `.stl` files to be printed by [robu.in](https://robu.in/product/3d-printing-service/), which I will receive within a week's time (~6/9/25). The print file is saved as `Assembly2_ver3.stl` in the `3D/` directory, and the final product will look like the contents of `Prototype_Assembly.stl`. Find the Onshape document [here](https://cad.onshape.com/documents/6fe5cb060711fa738a919116/w/fa28f828e5c3d9cb04d2cd20/e/b66991c36ce5bdb22e432e13?renderMode=0&uiState=683d6334979e864c1bc73d1c). 


**Updates**
6/4/25: The print has been delayed, but I will move on to testing components and creating libraries to make the final product easier to construct.

6/9/25: The print was cancelled and refunded because I provided parts that were too large for the printer. I've edited the stl files. The latest is `Assembly2_ver4.stl`. The prints will hopefully come in by next week (6/16 to 6/20).

---


## Component Testing

Component List (tentative): Arduino UNO, Tower Pro 9g Micro Servos, NEMA 17 Stepper motor, DRV8825 stepper motor driver, 2N7000 N-MOSFETs


### Makefile

Since I'm using a more baremetal approach with this project, I'm skipping the Arduino IDE and managing functionality by editing register contents in the ATmega328p IC on-board. 


### Servos 

I couldn't find any C libraries for the micro servos, unfortunately. However, I managed to create my own simple servo library that functions exactly the same. I've used a general purpose timer on the ATmega, 16-bit Timer 2, to generate a constant Fast PWM signal with a period of 20ms and a duty cycle that varies between 1ms and 2ms. The duty cycle of this PWM wave determines the angle the servo turns to. My function is constructed in such a way that it takes an input from 0 to 180 degrees, maps it to milliseconds, and adjusts the duty cycle accordingly. 

The next step is to create a way to control four servos with the same timer. This is possible, but it requires some tweaking of my library to include taking a servo number as input, as well as NMOSFETs in the circuit to redirect the PWM signal to these servos as required. I will also need to tweak the duty cycle depending on the servo that is being addressed.


### Stepper Motor

I'm currently waiting for the stepper motor driver to arrive (DRV8825). Until then, however, I will work on customizing a library for this driver. Find the library [here](https://github.com/adrielluiz/drv8825_c/tree/main). I need to provide implementations of setting pins to output and creating a `delay_us()` function. Fortunately, I can just use the `utils/delay.h` library to complete that part. 

I've included `drv8825.c` and its associated header in `lib/`, and I will modify it there. Surprisingly, the original author has left in a ton of misspellings of DRV8825, including DVR8825 and DRB8825, which I have fixed.

The library readme recommends using a certain code structure, which I'll copy into `main.c`. I've finished modifying the methods I needed. Fortunately for me, I need to connect 8 GPIO pins through the DRV8825, so I can use one full port on the ATmega. I'm going to use `PORTD`, as `PORTB` currently is being used (`PB1` is needed for USART). 

Once the DRV8825 comes in, I can test the stepper and the driver, and I can conclude this phase.

---

