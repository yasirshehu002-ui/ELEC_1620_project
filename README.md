# Washing Machine Control Panel - Mbed OS Project

This project is an embedded systems simulation of a washing machine control panel developed using Mbed OS. The system uses switches, potentiometers, sensors, LEDs, a buzzer, and a 7-segment display to model the basic operation of a washing machine.

The project allows the user to power the machine on/off, detect whether the washing tub is loaded, select a washing mode, choose a temperature setting, confirm the cycle, and run a timed washing sequence.

## Project Overview

The washing machine control panel includes the following functions:

- Power ON/OFF control
- Load detection using a force-sensitive resistor
- Washing mode selection using a potentiometer
- Temperature selection using a second potentiometer
- Visual feedback using LEDs and a 7-segment display
- Warning and completion sounds using a buzzer
- Basic solar activity detection using an LDR sensor
- Washing cycle animation and countdown display

## Hardware Used

- Mbed-compatible microcontroller board
- Force-sensitive resistor (FSR)
- Light-dependent resistor (LDR)
- Two potentiometers
- Three SPDT switches/buttons
- RGB LED
- Three individual LEDs
- Buzzer
- 7-segment display
- USB serial connection for status messages

## Pin Connections

| Component | Pin |
|---|---|
| FSR sensor | PA_1 |
| LDR sensor | PC_2 |
| Mode potentiometer | PA_5 |
| Temperature potentiometer | PA_7 |
| Button 1 / Confirm | PC_10 |
| Button 2 / Reset/Continue | PC_11 |
| Button 3 / Power | PD_2 |
| Power LED | PC_0 |
| LED bus | PC_1, PB_0, PA_4 |
| Buzzer | PA_15 |
| RGB green LED | PB_5 |
| RGB red LED | PB_3 |
| RGB blue LED | PB_4 |
| 7-segment display | PA_11, PA_12, PB_1, PB_15, PB_14, PB_12, PB_11 |
| Decimal point | PB_2 |

## Washing Modes

The washing mode is selected using the first potentiometer.

| Mode Value | Washing Mode | Display |
|---|---|---|
| 1 | Cotton | 1 |
| 2 | Eco | 2 |
| 3 | Quick Wash | 3 |

## Temperature Settings

The temperature is selected using the second potentiometer.

| Temperature Value | Temperature | LED Indication |
|---|---|---|
| 1 | Cold | Blue LED |
| 2 | Hot | Red LED |

## Cycle Times

| Mode | Temperature | Displayed Time |
|---|---|---|
| Cotton | Cold | 60 minutes |
| Cotton | Hot | 60 minutes |
| Eco | Cold | 45 minutes |
| Eco | Hot | 45 minutes / 60 minutes depending on programmed behaviour |
| Quick Wash | Cold | 20 minutes |
| Quick Wash | Hot | 20 minutes |

## System Operation

1. The washing machine starts in the OFF state.
2. Pressing the power button toggles the machine ON.
3. When ON, the system checks the tub load using the FSR sensor.
4. If the load is too high, the red LED turns on and a warning sound plays.
5. If the load is acceptable, the green LED indicates that the tub is loaded.
6. The user selects a washing mode using the first potentiometer.
7. The user selects a temperature using the second potentiometer.
8. The selected mode and temperature are shown using LEDs and the 7-segment display.
9. The user confirms the selection using the confirm button.
10. A washing cycle animation is displayed.
11. The countdown/timer runs.
12. When the cycle is complete, the buzzer plays an end sound.
13. The system resets and allows the user to make a new selection.

## Features

### Load Detection

The FSR sensor is used to check the washing tub load.

- If the FSR value is above the overload threshold, the system displays a warning.
- If the load is within an acceptable range, the system allows the user to continue.

### Mode Selection

The first potentiometer controls the washing mode. Depending on the potentiometer value, the user can choose between:

- Cotton
- Eco
- Quick Wash

### Temperature Selection

The second potentiometer controls the temperature setting. The RGB LED provides feedback:

- Blue LED: Cold wash
- Red LED: Hot wash

### 7-Segment Display

The 7-segment display is used to show:

- Selected washing mode
- Estimated cycle time
- Countdown values
- Washing animation

### Buzzer Feedback

The buzzer provides audio feedback for:

- Overload warning
- End of washing cycle

### Solar/LDR Check

When the washing machine is powered off, the LDR sensor is checked to simulate whether solar input is active.

## Main Functions

| Function | Description |
|---|---|
| `init_leds()` | Turns off the LED bus |
| `init_buttons()` | Initialises the button inputs |
| `SegDis_init()` | Clears the 7-segment display |
| `init_multiled()` | Turns off the RGB LED |
| `load_check()` | Checks whether the washing tub is overloaded |
| `select_mode()` | Reads the mode potentiometer and selects the washing mode |
| `select_temp()` | Reads the temperature potentiometer and selects hot or cold |
| `confirm_selection_and_run()` | Displays the selected wash program |
| `timer()` | Runs the washing cycle countdown |
| `SegDis_animation()` | Displays a simple washing/running animation |
| `play_note()` | Plays buzzer tones |
| `solar_check()` | Checks LDR value when the machine is off |
| `power_off()` | Turns off outputs and resets the system |

## Serial Monitor Output

The program prints status messages through serial communication at 115200 baud. These messages include:

- Power status
- Tub load status
- Selected mode
- Selected temperature
- Cycle progress
- Completion message
- Solar/LDR status

Example output:

```text
power on, please load tub
tub loaded
please select mode and temperature
mode: quick wash, temperature: cold, time: 20 mins
cycle in progress
completed!!
make new selections
