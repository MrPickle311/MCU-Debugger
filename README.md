# MCU Debugger

# Overview

This device is used to provide preview of variable values
in the embedded application runtime.

There are many evaluation boards available on the market which 
have built-in UART_USB intermediate chips. These chips provide
possibility of device programming and debugging.

Examples of chips:

![image](https://user-images.githubusercontent.com/59508136/128632395-ba64516b-752e-4f85-ba9b-25f2538fa2c0.png)

![image](https://user-images.githubusercontent.com/59508136/128632397-f41da85d-0de2-489b-a5d6-754bfbeb6755.png)

## What about microcontrollers without any intermediate chip?

Example:

![image](https://user-images.githubusercontent.com/59508136/128632618-253c846c-4355-41d3-8352-3148bdeee27e.png)

These microcontrollers are target of this project. 
This device is a great solution for standalone chips.

# Construction of the device

## Components list:
- Atmega328PB 
- Waveshare 2.4 LCD ( SPI Interface )
- 32 KB of external FRAM memory
- 3.3 V step down converter
- Logic level converter ( 3.3V <-> 5V )
- OK , BACK , RIGHT(UP) , LEFT(DOWN) buttons
## Electrical circuit 

[Schematic_debugger.pdf](https://github.com/MrPickle311/MCU-Debugger/files/6950864/Schematic_debugger.pdf)


## Circuit on the contact plate 

![image](https://user-images.githubusercontent.com/59508136/128633333-bcf2587b-6af0-4d03-aa6d-63e663bcb362.png)

# How does it work ? 

Debugger connects with debugged chip via TWI ( I2C ) interface. 

Debugged device -> master
Debugger -> slave

##Warning!
Debugged device requires a library from this repository:
There is also an example project of using that library 
//LINK

Variables are previewed by putting them in to breakpoints as following:

``` BREAKPOINT( (var1) , (var2) , (var3) , (var4) );```

But debugger must know how many breakpoint can show at the same time. 

For example :

```START_DEBUG_SESSION(4)
// One breakpoint can store max 8 KB
```

##Warning!
Variables name are also data!
Number of breakpoints must be power of 2!

Working from the debugging device side:
1 - Program is working and encounters BREAKPOINT() macro
2 - MCU going into loop , where waiting for a TRIGGER signal to be sent
3 - MCU sends variables to debugger
4 - Program executes its code

Trigger signal is sent when the user requests to go to next breakpoint.

# Correct debugger and debugged chip connection
- Apply +5V to VCC wire
- Connect GND of these devices
- join SCL wires
- joint SDA wires
- to TRIGGER pin lead any pin which can handle external interrupts

# Example of usage

This project is debugged device example

You should now:
1 - Upload the program to debugged MCU
2 - Connect debugger to the MCU
3 - Restart MCU
4 - Turn on debugger

And you should see:
![image](https://user-images.githubusercontent.com/59508136/128635446-b17b1efb-0827-42ec-b9f3-8865cc7937ba.png)
![image](https://user-images.githubusercontent.com/59508136/128635454-37436592-501e-4c73-9631-4600619c2a72.png)

Click OK to start debug session. You should see

![image](https://user-images.githubusercontent.com/59508136/128635470-0f72690e-e3a4-4a02-93e7-ea719302f23e.png)

But we haven't downloaded any data yet. Click Browse data to check it
![image](https://user-images.githubusercontent.com/59508136/128635703-83ae758a-e073-4e36-8c70-3f0f0f076728.png)

Click Back button to go to previous menu.

![image](https://user-images.githubusercontent.com/59508136/128635747-8c21adbc-435f-4f6b-bd62-05f69938182c.png)

Choose and click 'Load next breakpoint' position to download the data.

![image](https://user-images.githubusercontent.com/59508136/128635858-c3056945-a2ae-4e7f-9bb8-7900525ca3e7.png)

![image](https://user-images.githubusercontent.com/59508136/128635864-40c1a21e-cef8-458d-9044-811a784e4295.png)

And you can see that data from first breakpoint have been downloaded.

![image](https://user-images.githubusercontent.com/59508136/128635959-2d1db5ae-132f-4bff-be9e-26845f83fa74.png)







