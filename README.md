# PIC16-PS/2-Pedal

This repository provides an implementation of the PS/2 keyboard protocol for the PIC16 microcontroller,
along with code and schematics for an [`Express Scribe`](https://www.nch.com.au/scribe/index.html)-compatible
transcription pedal. I used [`SDCC`](https://sourceforge.net/p/sdcc/wiki/Home/) as the compiler,
and an arduino nano with [`ZEPP`](https://github.com/battlecoder/zeppp/) as the programmer, so ensure both are 
properly installed on your system to use the provided Makefile 
(which, by the way, was borrowed from [here](https://github.com/diegoherranz/sdcc-examples)).

I also used [`PulseView`](https://sigrok.org/wiki/PulseView) extensively for debugging purposes with an arduino 
uno as a signal analyzer, thanks to [this](https://github.com/gillham/logic_analyzer/) project.
To create a proper development environment, I used the [`Embedded IDE`](https://em-ide.com/en/docs/getting-started/setup/)
vscode extension. So, for using or editing this repository or similar ones, it would be a good idea to check above links.

## Make instructions

1. Install `SDCC` and `ZEPP`.
2. Edit the Makefile:
    - Set `FAMILY` and `PROC` to match your PIC microcontroller.
    - Set `PORT` to the correct Arduino port (verify through the Arduino IDE).

3. Connect the Arduino to the PIC as per the `ZEPP` guide.
4. Run make upload in the directory where the Makefile is located.

## How to use it

The pedal has three shortcuts for working with `Express Scribe`. If it is pressed once, it triggers the `F9` key,
or the `F8` key, which are the default keys for play and pause in `Express Scribe`. If it is pressed and held, it
continuously triggers the `F7` key for rewind. Finally, if it is quickly double-pressed and then held, it 
triggers the `F8` key for fast forward. If you want to change those shortcuts just change the next lines for the
keys you want:
```
keyCode_t stop_f4    = SCAN_CODE_F4;
keyCode_t play_f9    = SCAN_CODE_F9;
keyCode_t rewind_f7  = SCAN_CODE_F7;
keyCode_t forward_f8 = SCAN_CODE_F8;
```
You can also completely change how the pedal work following the guide below.

The pedal does not require additional software to work with any PC or device, as it operates using the PS/2 
keyboard protocol. Therefore, if your PC has a PS/2 keyboard port, the pedal will function correctly. 
It can also work via USB if your PC supports PS/2 over USB with passive adapters; otherwise, 
you will need to purchase a proper adapter.

## How does it work
Very basically, the program starts with some configuration code. It sets up the `DATA` and `CLOCK` pins as outputs. 
If you want to change the pins, make sure to update the SWITCH_CLOCK() macro as well. Next, it configures the Timer2
registers; the only important value there is PR2. This code is designed to work with the PIC's 4MHz internal clock, 
so if you're using a faster oscillator, you should adjust this value to maintain a transmition frequency between 
10 and 16.7 kHz.
```
// --------- THE CODE RELATED TO PIN CONFIGURATION LOOKS LIKE THIS
#define CLOCK_TRIS        TRISBbits.TRISB0
#define CLOCK_READER_TRIS TRISBbits.TRISB1
#define DATA_TRIS         TRISBbits.TRISB2
#define DATA_READER_TRIS  TRISBbits.TRISB3

#define CLOCK             PORTBbits.RB0
#define CLOCK_READER      PORTBbits.RB1
#define DATA              PORTBbits.RB2
#define DATA_READER       PORTBbits.RB3
#define SWITCH_CLOCK() (PORTB ^= 1 << 0)

DATA_READER_TRIS = CLOCK_READER_TRIS = 1;
DATA_TRIS = CLOCK_TRIS = 0;
CLOCK = DATA = 0;

// --------- THE CODE RELATED TO TIMER2 CONFIGURATION LOOKS LIKE THIS
T2CONbits.TMR2ON = 1; // ENABLE TIMER2

// PRESCALER - POSTSCALER CONFIG
T2CONbits.T2CKPS0 = 1;
T2CONbits.T2CKPS1 = 0;
T2CONbits.TOUTPS0 = 0;
T2CONbits.TOUTPS1 = 0;
T2CONbits.TOUTPS2 = 0;
T2CONbits.TOUTPS3 = 0;

// SET START AND END POINT
TMR2 = 0;
PR2  = 9;
```



As you probably want to send key codes to the host as a keyboard, you can use the `sendCode()`
function with the key code you want to send as the first parameter. For the second parameter, 
use 0 if you plan to send more than one code; otherwise, use 1

You can find the complete list of key codes [here](https://web.archive.org/web/20060718084816if_/http://www.computer-engineering.org/ps2keyboard/scancodes2.html),
but it is already included in the [scan_codes_set2.h](./scan_codes_set2.h) file. 

Before sending a code, you first need to use the `formatKeyCode()` function over it to set it 
up for be transmitted first.

```
// THESE INITIAL CODES ARE NECESSARY FOR KEYBOARD INITIALIZATION
keyCode_t stp        = SCAN_CODE_SELF_TEST;
keyCode_t scs        = SCAN_CODE_SET_2;
keyCode_t echo       = SCAN_CODE_ECHO;
keyCode_t ack        = SCAN_CODE_ACK;
keyCode_t idMsb      = SCAN_CODE_IDMSB;
keyCode_t idLsb      = SCAN_CODE_IDLSB;
keyCode_t break_     = SCAN_CODE_BREAK;

// THEN YOUR CUSTOM CODES
keyCode_t space      = SCAN_CODE_SPACE;
formatKeyCode(&space); // <--- REPEAT THIS LINE FOR ALL THE ABOVE CODES

// FOR A KEY PRESS
sendCode(space,1);

// FOR A KEY RELEASE
sendCode(break_,0);
sendCode(space,1);
```




Therefore, the program should consist of a continuous loop that checks if the host wants to 
send a command. If so, it should include a block that reads and manages the response. 
For example:
```
uint8_t command = 0;
uint8_t lastCommand = 0;
keyCode_t* lastKeyCode = &stp;
sendCode(stp,1);

while(1){
    // CHECK IF THERE IS A COMMAND TO READ
    if(CLOCK_READER == 0 && DATA_READER == 0){
        lastCommand = command;
        command = readCommand();
        while(CLOCK_READER == 0); //WAITS UNTIL HOST RELEASE'S CLOCK LINE
        switch(command){

            case COMMAND_READID:
            sendCode(ack,0);
            sendCode(idMsb,0);
            sendCode(idLsb,1);
            break;

            case COMMAND_ECHO:
            sendCode(echo,1);
            break;

            case COMMAND_RESEND:
            sendCode(*lastKeyCode,1);
            break;

            case COMMAND_RESET:
            sendCode(ack,0);
            sendCode(stp,1);
            break;

            default:
            sendCode(ack,1);
            if(lastCommand == COMMAND_SET_SCAN_CODE && command == 0){
                dummyDelay(105);
                sendCode(scs,1);
            }
            break;
        }
    }

    // --------- YOUR CODE HERE
}
```



These are the basic commands that a keyboard should respond to. If you want to add more or modify the behavior of 
the program after responding to a particular command, feel free to do so, but make sure to leave this code functionality as
it is because it is defined by the protocol itself.

## External links and documentation
At last, here is the documentation that i based this project on:
- https://en.wikipedia.org/wiki/PS/2_port
- https://www.burtonsys.com/ps2_chapweske.htm
- https://web.archive.org/web/20060901070633if_/http://www.computer-engineering.org/ps2keyboard/
- https://wiki.osdev.org/PS/2_Keyboard
