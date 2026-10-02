# Washing Machine Control Panel - Mbed OS Project

This project is an embedded washing machine control panel simulation developed using Mbed OS. It uses sensors, switches, potentiometers, LEDs, a buzzer, and a 7-segment display to simulate the basic user interface and operating logic of a washing machine.

The system allows the user to power the washing machine on, check the tub load, select a washing mode, choose a temperature setting, confirm the wash cycle, and run a timed washing sequence. It also includes a simple solar/LDR feature that allows the system to detect light availability.

## Project Overview

The washing machine control panel includes:

- Power ON/OFF control
- Solar/light-based activation using an LDR
- Tub load detection using a force-sensitive resistor
- Washing mode selection using a potentiometer
- Temperature selection using a second potentiometer
- LED and RGB LED feedback
- 7-segment display output
- Buzzer warning and completion sounds
- Washing cycle animation
- Countdown-style timer display

## Hardware Implementation
<img width="2846" height="1152" alt="IMG_1102" src="https://github.com/user-attachments/assets/0849cf3d-5f71-4fb2-b102-233f8b362c74" />

## Hardware Used

- Mbed-compatible microcontroller board
- Force-sensitive resistor, FSR
- Light-dependent resistor, LDR
- Two potentiometers
- Three SPDT switches/buttons
- Power LED
- Three individual LEDs
- RGB LED
- Piezo buzzer
- 7-segment display
- USB serial connection for debugging/status messages

## Pin Connections

| Component | Pin |
|---|---|
| FSR sensor | PA_1 |
| LDR sensor | PC_2 |
| Mode potentiometer | PA_5 |
| Temperature potentiometer | PA_7 |
| Button 1 / Confirm | PC_10 |
| Button 2 / Continue/Reset | PC_11 |
| Button 3 / Power | PD_2 |
| Power LED | PC_0 |
| LED bus | PC_1, PB_0, PA_4 |
| Buzzer | PA_15 |
| RGB green LED | PB_5 |
| RGB red LED | PB_3 |
| RGB blue LED | PB_4 |
| 7-segment display segments | PA_11, PA_12, PB_1, PB_15, PB_14, PB_12, PB_11 |
| Decimal point | PB_2 |

## Washing Modes

The washing mode is selected using the first potentiometer.

| Potentiometer Value | Mode | 7-Segment Display |
|---|---|---|
| 1 | Cotton | 1 |
| 2 | Eco | 2 |
| 3 | Quick Wash | 3 |
| 4 | Spin and Dry | 4 |

## Temperature Settings

The temperature is selected using the second potentiometer.

| Potentiometer Value | Temperature Setting | RGB LED Indication |
|---|---|---|
| 1 | Cold | Blue LED |
| 2 | Hot | Red LED |
| 3 | Automatic | Red, Green and Blue LEDs |

## Programmed Cycle Times

| Mode | Temperature | Displayed Time |
|---|---|---|
| Cotton | Cold | 60 minutes |
| Cotton | Hot | 60 minutes |
| Eco | Cold | 45 minutes |
| Eco | Hot | 60 minutes |
| Quick Wash | Cold | 20 minutes |
| Quick Wash | Hot | 20 minutes |
| Spin and Dry | Automatic | 80 minutes |

## System Operation

1. The system starts with all LEDs and displays switched off.
2. Button 3 toggles the washing machine power state.
3. The machine can also activate if the LDR detects light, simulating solar availability.
4. When powered on, the system checks the washing tub load using the FSR sensor.
5. If the tub is overloaded, the red LED turns on and a warning tone is played.
6. If the tub is loaded correctly, the green LED indicates that the load is acceptable.
7. If the tub load is too low, the serial monitor asks the user to load the tub.
8. The user selects the wash mode using potentiometer 1.
9. The user selects the temperature using potentiometer 2.
10. Button 1 confirms the selected mode and temperature.
11. The 7-segment display shows the selected cycle time.
12. The running animation starts.
13. Button 1 and Button 2 together start the timer cycle.
14. When the cycle finishes, the buzzer plays an end sound and the system resets for new selections.

## Main Features

### Load Detection

The FSR sensor is used to determine whether the washing tub is empty, correctly loaded, or overloaded.

- FSR value above 50: tub overloaded
- FSR value between 30 and 50: tub loaded correctly
- FSR value below 30: tub needs to be loaded

### Solar/LDR Check

The LDR is used to detect light. When light is detected, the program prints that the solar panel is active and allows the system to operate.

### Mode Selection

The first potentiometer allows the user to choose between four washing programs:

- Cotton
- Eco
- Quick Wash
- Spin and Dry

### Temperature Selection

The second potentiometer allows the user to choose:

- Cold wash
- Hot wash
- Automatic temperature

The automatic option is used with the Spin and Dry mode.

### 7-Segment Display

The 7-segment display is used to show:

- Selected mode number
- Cycle time
- Countdown values
- Running animation

### Buzzer Feedback

The buzzer provides sound feedback for:

- Overload warning
- End of cycle notification

## Main Functions

| Function | Description |
|---|---|
| `init_leds()` | Turns off the LED bus |
| `init_power_led()` | Turns off the power LED |
| `init_buttons()` | Initialises the button inputs |
| `init_multiled()` | Turns off the RGB LED |
| `SegDis_init()` | Clears the 7-segment display |
| `load_check()` | Checks the washing tub load using the FSR |
| `solar_check()` | Checks the LDR value and returns whether solar/light is active |
| `select_mode()` | Reads potentiometer 1 and selects the washing mode |
| `select_temp()` | Reads potentiometer 2 and selects the temperature |
| `confirm_selection_and_run()` | Displays the chosen mode, temperature and cycle time |
| `timer()` | Runs the cycle countdown and completion sound |
| `SegDis_animation()` | Runs a 7-segment display animation |
| `play_note()` | Plays a buzzer note using PWM |
| `power_off()` | Turns off outputs and resets the display |

## Serial Monitor Output

The program uses serial communication at 115200 baud to print status messages.

Example messages include:

```text
power off
solar panel active
please select mode and temperature
tub loaded
tub overloaded, please remove load!
mode: cotton ; temperature: hot , time: 1 hour
mode: spin and dry , temperature: automatic , time: 80 mins
cycle in progress
completed!!
make new selections
