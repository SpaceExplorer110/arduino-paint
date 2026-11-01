# Arduino Paint

A simple drawing program for the Arduino UNO R4 WiFi, using its built-in 8×12 LED matrix.

With a few buttons, you can move a cursor around the LED matrix and create your own pixel art!

## Features

- Draw pixel art using the built-in LED matrix.
- Move the cursor in four directions.
- Toggle individual pixels on and off.
- Show or hide the cursor.
- Blinking cursor that indicates whether the current pixel is on or off.
- Optional start screen.
- Configurable button pins.
- Option to use internal or external pull-up resistors.

## Requirements

- Arduino UNO R4 WiFi
- At least 4 push buttons
- Jumper wires
- Breadboard (optional)

The program supports up to 6 buttons:

- Up button: pin 7
- Down button: pin 6
- Left button: pin 5
- Right button: pin 4
- Hide/Show button: pin 3
- Select button: pin 2

You can leave out one vertical direction button (up or down) and one horizontal direction button (left or right). The program will still work.

## Configuration

At the top of the code, you can find the Settings and Button Mapping sections.

### Settings

You can enable or disable:
- Internal pull-up resistors.
- The start screen.

### Button Mapping

You can change the pin numbers to match your own wiring.

## How to use

- Use the directional buttons to move the cursor.
- Press the select button to turn a pixel on or off.
- Use the hide/show button to toggle the cursor visibility.

Have fun creating pixel art!

## Important notes

- Drawings are currently stored in RAM. This means your drawing will be lost when the Arduino is reset or disconnected from power.
- This project is designed for the Arduino UNO R4 WiFi and its built-in LED matrix.

## Contributing

I'm not a professional programmer, and this project may not use the most efficient or elegant solutions.

Feel free to modify the code and share your own version on GitHub. Please give credit to the original author when doing so.

Have fun experimenting with Arduino Paint!
