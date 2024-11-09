#define NO_BIT_DEFINES
#include <pic16f628a.h>
#include <stdint.h>
#include "scan_codes_set2.h"

__code uint16_t __at(_CONFIG) __configword = _INTOSC_OSC_NOCLKOUT & _WDTE_OFF & _LVP_OFF;
#define HIGH_IMPEDANCE    1
#define OUTPUT            0;

#define CLOCK_TRIS        TRISBbits.TRISB0
#define CLOCK_READER_TRIS TRISBbits.TRISB1
#define DATA_TRIS         TRISBbits.TRISB2
#define DATA_READER_TRIS  TRISBbits.TRISB3
#define PEDAL_TRIS        TRISAbits.TRISA4

#define CLOCK             PORTBbits.RB0
#define CLOCK_READER      PORTBbits.RB1
#define DATA              PORTBbits.RB2
#define DATA_READER       PORTBbits.RB3
#define PEDAL             PORTAbits.RA4

#define SWITCH_CLOCK() (PORTB ^= 1 << 0)

void formatKeyCode(keyCode_t *code_p){
    *code_p <<= 1;             // ADD STARTBIT
    *code_p |= 0b110000000000; // ADD ENDBIT

    // ADD PARITY BIT (ODD PARITY)
    int onesCount = 0;
    for (int i = 0; i < 12; i++) {
        if ((*code_p >> i) & 1){
            onesCount++;
        }
    }

    if(onesCount % 2 == 0){
        *code_p |= 0b1000000000;
    }

    // INVERT EVERYTHING
    *code_p ^= 0b111111111111;

}

void dummyDelay(int cycles){
    for(int i=0; i<cycles; i++){
        __asm nop __endasm;
    }
}

void sendCode(keyCode_t keyCode, int bypassDelay){
    //WAIT UNTIL NEXT TIMER2 CYCLE
    DATA = 1;
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;

    for (int i = 0; i < 22; i++) {

        while (!PIR1bits.TMR2IF){
            if (!CLOCK && CLOCK_READER == 0) {
                // HOST IS INHIBITING COMMUNICATION
                CLOCK = 0;
                DATA  = 0;
                return;
            }
        }
        
        PIR1bits.TMR2IF = 0;
        SWITCH_CLOCK();
                
        if(!CLOCK){
            keyCode >>= 1;
            DATA = keyCode & 1;
        }
        
    }

    if(bypassDelay) return;
    dummyDelay(105);
    return;
}

uint8_t readCommand(){
    //WAIT UNTIL HOST RELEASE CLOCK
    uint8_t onesCount = 0;
    uint8_t command = 0;

    //WAIT TO HOST RELEASE CLOCK LINE
    while(CLOCK_READER == 0);
    dummyDelay(5);

    //WAIT UNTIL NEXT TIMER2 CYCLE
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;

    //8 BITS OF DATA
    for (int i = 0; i < 16; i++) {
        while (!PIR1bits.TMR2IF);
        PIR1bits.TMR2IF = 0;
        SWITCH_CLOCK();
        if(!CLOCK){
            command >>= 1;
            if(DATA_READER){
                onesCount ++;
                command |= 0b10000000;
            }
        }

    }

    //PARITY BIT
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    SWITCH_CLOCK();

    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    SWITCH_CLOCK();
    onesCount += DATA_READER;

    //STOP BIT
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    SWITCH_CLOCK();

    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    SWITCH_CLOCK();

    // ACK BIT
    DATA = 1;    
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    SWITCH_CLOCK();

    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    DATA = 0;
    SWITCH_CLOCK();


    if(onesCount % 2 != 0){
        return command;
    }

    return 0;
}

void main(){
#pragma region // ------------------------------ PIN'S INITIALIZATION ----------------------------- //
    DATA_READER_TRIS = CLOCK_READER_TRIS = PEDAL_TRIS = HIGH_IMPEDANCE;
    DATA_TRIS = CLOCK_TRIS = OUTPUT;
    CLOCK = DATA = 0;
#pragma endregion

#pragma region // --------------------------------- TIMER2_CONFIG --------------------------------- //
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
#pragma endregion

    keyCode_t stp        = SCAN_CODE_SELF_TEST;
    keyCode_t scs        = SCAN_CODE_SET_2;
    keyCode_t echo       = SCAN_CODE_ECHO;
    keyCode_t ack        = SCAN_CODE_ACK;
    keyCode_t idMsb      = SCAN_CODE_IDMSB;
    keyCode_t idLsb      = SCAN_CODE_IDLSB;
    keyCode_t break_     = SCAN_CODE_BREAK;

    keyCode_t stop_f4    = SCAN_CODE_F4;
    keyCode_t play_f9    = SCAN_CODE_F9;
    keyCode_t rewind_f7  = SCAN_CODE_F7;
    keyCode_t forward_f8 = SCAN_CODE_F8;

    formatKeyCode(&stp);
    formatKeyCode(&scs);
    formatKeyCode(&echo);
    formatKeyCode(&ack);
    formatKeyCode(&idMsb);
    formatKeyCode(&idLsb);

    formatKeyCode(&break_);
    formatKeyCode(&stop_f4);
    formatKeyCode(&play_f9);
    formatKeyCode(&rewind_f7);
    formatKeyCode(&forward_f8);

    sendCode(stp,0);
    uint8_t command = 0;
    uint8_t lastCommand = 0;
    keyCode_t* lastKeyCode = &stp;
    int playCycle = 0;

    uint16_t pedalHoldTime = 0;
    uint16_t pedalDobleClickTimer = 0;
    int pedalState = PEDAL_STATE_SINGLE_CLICK;

    while(1) {

        if(pedalDobleClickTimer > 1){
            pedalDobleClickTimer ++;
        }

        if(pedalDobleClickTimer >= 2000){
            pedalDobleClickTimer = 0;
            if(pedalState == PEDAL_STATE_SINGLE_CLICK){
                //------------------------------------------- SINGLE CLICK
                playCycle ^= 1;
                if(playCycle == 1){
                    sendCode(play_f9,0);
                    sendCode(break_,0);
                    sendCode(play_f9,0);
                }

                else{
                    sendCode(stop_f4,0);
                    sendCode(break_,0);
                    sendCode(stop_f4,0);
                }
                
            }
        }

        if (PEDAL == 1 && CLOCK_READER == 1) {

            if (pedalHoldTime > 5){
                if (pedalDobleClickTimer > 1){
                    //---------------------------------------- DOUBLE CLICK
                    pedalState = PEDAL_STATE_DOBLE_CLICK;
                    sendCode(forward_f8,0);
                    sendCode(break_,0);
                    sendCode(forward_f8,0);
                }
                
                if (pedalDobleClickTimer == 0){
                    pedalDobleClickTimer = 1;
                }
            }

            if(pedalHoldTime < 3000) pedalHoldTime ++;
            if (pedalHoldTime == 3000 && (pedalState == PEDAL_STATE_SINGLE_CLICK || pedalState == PEDAL_STATE_HOLD_CLICK)){
                //--------------------------------------------- HOLD CLICK
                pedalState = PEDAL_STATE_HOLD_CLICK;
                sendCode(rewind_f7,0);
                sendCode(break_,0);
                sendCode(rewind_f7,0);
            }

        }

        if (PEDAL == 0){
            
            if(pedalHoldTime > 5){
                switch (pedalState){
                    case PEDAL_STATE_SINGLE_CLICK:
                    if(pedalDobleClickTimer == 1){
                        pedalDobleClickTimer = 2;
                    }
                    break;

                    //---------------------------------------- HOLD CLICK RELEASE
                    case PEDAL_STATE_HOLD_CLICK:
                    sendCode(break_,0);
                    sendCode(rewind_f7,0);
                    break;

                    //--------------------------------------- DOUBLE CLICK RELEASE
                    case PEDAL_STATE_DOBLE_CLICK:
                    pedalDobleClickTimer = 0;
                    sendCode(break_,0);
                    sendCode(forward_f8,0);

                    break;
                }                
            }
            
            pedalHoldTime = 0;
            pedalState = PEDAL_STATE_SINGLE_CLICK;
        }

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
    }
}