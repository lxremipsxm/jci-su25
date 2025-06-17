# Card Badging Mechanical Automation

## Phases
+ **Design Phase** (5/26/25 to 5/30/25)
+ **Component Setup and Testing** (Estimated: 6/4/25 to 6/6/25; Actual: 6/2/25 to 6/20/25)
+ **Construction** (Estimated: 6/9/25 to 6/23/25)
+ **Automation** (Estimated: 6/16/25 to 6/20/25)
+ **Refinement + Adding Features**


## Goals
The primary goal of this project is to create an automation that badges card for a user in a certain order. This design must be scalable, quick, and easy to use. It should be flexible as well; my intention is to make this as easy for a team to use as possible.

---

## Design Phase 

### Protoype Design (5/26)

For my first prototype, I'm creating a solution that can badge 16 cards in quick succession. It consists of a drum rotated accurately by a stepper motor. The drum has 16 holes that fit card slots into them, which are lifted by a cam (operated by a micro servo) into the range of a stationary reader. 

In order to scale this design up, an additional cam must be placed under each new reader. Since micro servos are inexpensive, this design is simultaneously cost effective. Ideally, the larger the drum, the more readers can be incorporated.

##### Onshape

This is my first time using Onshape. I spent about a day learning Onshape and how to create models accurate to scale. I then set out to create the models by sketching designs on faces and extruding them as necessary. Throughout the process, the idea behind the design remained relatively unchanged, but I made one significant revision: making the range of motion of the cams a little larger and moving the readers further away from the cards in order to prevent interference/reading multiple cards accidentally. I've also tried to use mates in my model. `Revolute_1` and `Revolute_2` in the Onshape document are fully functional and can be animated, but Onshape does not have a way for me to test a complete workflow.

##### Print (6/2)

I've sent the `.stl` files to be printed by [Robu.in's 3D printing service](https://robu.in/product/3d-printing-service/), which I will receive within a week's time (~6/9/25). The print file is saved as `Assembly2_ver3.stl` in the `3D/` directory, and the final product will look like the contents of `Prototype_Assembly.stl`. Find the Onshape document [here](https://cad.onshape.com/documents/6fe5cb060711fa738a919116/w/fa28f828e5c3d9cb04d2cd20/e/b66991c36ce5bdb22e432e13?renderMode=0&uiState=683d6334979e864c1bc73d1c). 


**Updates**
6/4/25: The print has been delayed, but I will move on to testing components and creating libraries to make the final product easier to construct.

6/9/25: The print was cancelled and refunded because I provided parts that were too large for the printer. The latest print file is stored in `Assembly2_ver4.stl`, in which I've reduced the height of the card slots and increased the size of the cam lift range instead. The prints will hopefully come in by next week (6/16 to 6/20).

---


## Component Testing 

Component List (tentative): Arduino UNO(1), Tower Pro 9g Micro Servos(4), NEMA 17 Stepper motor(1), DRV8825 stepper motor driver(1), 2N7000 N-MOSFETs(4)


### Makefile (6/2)

Since I'm using a more baremetal approach with this project, I'm skipping the Arduino IDE and managing functionality by editing register contents in the ATmega328p IC on-board. Additionally, I'll be using the `avr-gcc` toolchain to compile and flash my code onto the ATmega. For this, I've written a makefile that is heavily inspired by the makefiles I've had to use in college for my embedded systems course. Find the references [here](). <- Insert link to 231 repo

### Servos 

I couldn't find any C libraries for the micro servos, unfortunately. However, I managed to create my own simple servo library that functions exactly the same. I've used a general purpose timer on the ATmega, 16-bit Timer 2, to generate a constant Fast PWM signal with a period of 20ms, which is required by the servo according to documentation, and a duty cycle that varies between 1ms and 2ms. The duty cycle of this PWM wave determines the angle the servo turns to. My function is constructed in such a way that it takes an input from 0 to 180 degrees, maps it to milliseconds, and adjusts the duty cycle by changing the value the timer waits till before it sets the PWM to `LOW` again.

The next step is to create a way to control four servos with the same timer. This is possible, but it requires some tweaking of my library to include taking a servo number as input, as well as NMOSFETs in the circuit to redirect the PWM signal to these servos as required. I will also need to tweak *when* the signal is `HIGH` depending on the servo that is being addressed.


### Stepper Motor (6/9)

I'm currently waiting for the stepper motor driver to arrive (DRV8825). Until then, however, I will work on customizing a library for this driver. Find the library github repository [here](https://github.com/adrielluiz/drv8825_c/tree/main). According to the documentation, I need to provide implementations of setting pins to output and creating a `delay_us()` function. Fortunately, I can just use the `utils/delay.h` library to complete that part. 

I've included `drv8825.c` and its associated header in `lib/`, and I will modify it there. The original author has left in some misspellings of DRV8825 in method and type identifiers, which I have fixed.

The library readme recommends using a certain code structure, which I'll copy into `main.c`. 

Now, I'll modify `drv8825.c`, which contains the methods I need to customize to my chip architecture. Fortunately for me, I will need to connect 8 pins from DRV8825 into GPIO pins (the rest are for power/go into the motor directly), so I can use one full port on the ATmega, which is exactly 8 bits wide/8 pins wide. I'm going to use `PORTD`, as `PORTB` currently is being used for USART. This way, each pin on the DRV8825's right-hand side can be connected directly to PD0-PD7, making the definition of `stepper_pin_set()` incredibly easy to implement via simple bitwise operations. Since PD0-PD7 are organized perfectly in order on the board, the connections will also be satisfyingly clean.

Note that in `drv8825.h`, the `enum drv8825_drv_pins_e` typedef is ordered significantly differently than that of the original author in order to implement `stepper_pin_set()` as described.

Once the DRV8825 comes in, I can test the stepper and the driver simultaneously, and I can conclude this phase. 

Update(6/16/25): The driver component has come in. 


### NMOSFETs (6/16)

For the NMOSFETs selecting the correct servo, I will need to create a breakout board, since they are so small. I'll use this both to test them and for the final construction. Additionally, I want to make it flexible, so that I can operate 1,2,3, or 4 servos depending on the workflow needed.

I'm thinking of using 4 pins on `PORTB`. Depending on the servos necessary, I can set `PORTB` with bitwise operations; however, I do need to consider moving multiple servos to different angles in the same 20ms period. In order to do this, I need to partition the 20ms period into 4 parts (one for each servo), and update the timer to increase or decrease the pulse width according to the angle. This means I will need to heavily modify my `servo_set_pos()` method. In addition to this, I will need to ensure that despite 'partitioning' the timer signal into four parts, each servo still receives a signal with period 20ms, or the servo will exhibit unexpected behavior.

For testing, I'll send a constant `HIGH` through the drain terminal, read the output on the source terminal, and constantly vary the gate voltage between 5V and 0V (1 and 0). The output should be exactly the same as the gate input(`HIGH` or `LOW`). I may also have to use resistors on the terminals depending on their rating. 

---

