#ifndef scan_codes_set2
#define scan_codes_set2

typedef uint16_t keyCode_t;

#pragma region // --------------------------- SCAN CODES --------------------------- //

#define SCAN_CODE_IDMSB        0xAB
#define SCAN_CODE_IDLSB        0x83
#define SCAN_CODE_ECHO         0xEE
#define SCAN_CODE_SELF_TEST    0xAA
#define SCAN_CODE_ACK          0xFA
#define SCAN_CODE_BREAK        0xF0
#define SCAN_CODE_A            0X1C
#define SCAN_CODE_B            0x32
#define SCAN_CODE_D            0x23
#define SCAN_CODE_E            0x24
#define SCAN_CODE_F            0x2B
#define SCAN_CODE_G            0x34
#define SCAN_CODE_H            0x33
#define SCAN_CODE_I            0x43
#define SCAN_CODE_J            0x3B
#define SCAN_CODE_K            0x42
#define SCAN_CODE_L            0x4B
#define SCAN_CODE_M            0x3A
#define SCAN_CODE_N            0x31
#define SCAN_CODE_O            0x44
#define SCAN_CODE_P            0x4D
#define SCAN_CODE_Q            0x15
#define SCAN_CODE_R            0x2D
#define SCAN_CODE_S            0x1B
#define SCAN_CODE_T            0x2C
#define SCAN_CODE_U            0x3C
#define SCAN_CODE_V            0x2A
#define SCAN_CODE_W            0x1D
#define SCAN_CODE_X            0x22
#define SCAN_CODE_Y            0x35
#define SCAN_CODE_Z            0x1A
#define SCAN_CODE_0            0x45
#define SCAN_CODE_1            0x16
#define SCAN_CODE_2            0x1E
#define SCAN_CODE_3            0x26
#define SCAN_CODE_4            0x25
#define SCAN_CODE_5            0x2E
#define SCAN_CODE_6            0x36
#define SCAN_CODE_7            0x3D
#define SCAN_CODE_8            0x3E
#define SCAN_CODE_9            0x46
#define SCAN_CODE_BACKTICK     0x0E
#define SCAN_CODE_MINUS        0x4E
#define SCAN_CODE_EQUAL        0x55
#define SCAN_CODE_BACKSLASH    0x5D
#define SCAN_CODE_BKSP         0x66
#define SCAN_CODE_SPACE        0x29
#define SCAN_CODE_TAB          0x0D
#define SCAN_CODE_CAPS         0x58
#define SCAN_CODE_L_SHFT       0x12
#define SCAN_CODE_L_CTRL       0x14
#define SCAN_CODE_L_GUI0       0xE0
#define SCAN_CODE_L_GUI1       0x1F
#define SCAN_CODE_L_ALT        0x11
#define SCAN_CODE_R_SHFT       0x59
#define SCAN_CODE_R_CTRL0      0xE0
#define SCAN_CODE_R_CTRL1      0x14
#define SCAN_CODE_R_GUI0       0xED
#define SCAN_CODE_R_GUI1       0x27
#define SCAN_CODE_R_ALT0       0xE0
#define SCAN_CODE_R_ALT1       0x11
#define SCAN_CODE_APPS0        0xE0
#define SCAN_CODE_APPS1        0x2F
#define SCAN_CODE_ENTER        0x5A
#define SCAN_CODE_ESC          0x76
#define SCAN_CODE_F1           0x05
#define SCAN_CODE_F2           0x06
#define SCAN_CODE_F3           0x04
#define SCAN_CODE_F4           0x0C
#define SCAN_CODE_F5           0x03
#define SCAN_CODE_F6           0x0B
#define SCAN_CODE_F7           0x83
#define SCAN_CODE_F8           0x0A
#define SCAN_CODE_F9           0x01
#define SCAN_CODE_F10          0x09
#define SCAN_CODE_F11          0x78
#define SCAN_CODE_F12          0x07
#define SCAN_CODE_PRNT_SCRN0   0xE0
#define SCAN_CODE_PRNT_SCRN1   0x12
#define SCAN_CODE_PRNT_SCRN2   0xE0
#define SCAN_CODE_PRNT_SCRN3   0x7C
#define SCAN_CODE_SCROLL       0x7E
#define SCAN_CODE_PAUSE0       0xE1
#define SCAN_CODE_PAUSE1       0x14
#define SCAN_CODE_PAUSE2       0x77
#define SCAN_CODE_PAUSE3       0xE1
#define SCAN_CODE_PAUSE4       0xF0
#define SCAN_CODE_PAUSE5       0x14
#define SCAN_CODE_PAUSE6       0xF0
#define SCAN_CODE_PAUSE7       0x77
#define SCAN_CODE_BRKOPEN      0x54
#define SCAN_CODE_INSERT0      0xE0
#define SCAN_CODE_INSERT1      0x70
#define SCAN_CODE_HOME0        0xE0
#define SCAN_CODE_HOME1        0x6C
#define SCAN_CODE_PGUP0        0xE0
#define SCAN_CODE_PGUP1        0x7D
#define SCAN_CODE_DELETE0      0xE0
#define SCAN_CODE_DELETE1      0x71
#define SCAN_CODE_END0         0xE0
#define SCAN_CODE_END1         0x69
#define SCAN_CODE_PGDN0        0xE0
#define SCAN_CODE_PGDN1        0x7A
#define SCAN_CODE_U_ARROW0     0xE0
#define SCAN_CODE_U_ARROW1     0x75
#define SCAN_CODE_D_ARROW0     0xE0
#define SCAN_CODE_D_ARROW1     0x72
#define SCAN_CODE_R_ARROW0     0xE0
#define SCAN_CODE_R_ARROW1     0x74
#define SCAN_CODE_L_ARROW0     0xE0
#define SCAN_CODE_L_ARROW1     0x6B
#define SCAN_CODE_NUM          0x77
#define SCAN_CODE_KP_SLASH0    0xE0
#define SCAN_CODE_KP_SLASH1    0x4A
#define SCAN_CODE_KP_STAR      0x7C
#define SCAN_CODE_KP_MINUS     0x7B
#define SCAN_CODE_KP_PLUS      0x79
#define SCAN_CODE_KP_EN0       0xE0
#define SCAN_CODE_KP_EN1       0x5A
#define SCAN_CODE_KP_DOT       0x71
#define SCAN_CODE_KP_0         0x70
#define SCAN_CODE_KP_1         0x69
#define SCAN_CODE_KP_2         0x72
#define SCAN_CODE_KP_3         0x7A
#define SCAN_CODE_KP_4         0x6B
#define SCAN_CODE_KP_5         0x73
#define SCAN_CODE_KP_6         0x74
#define SCAN_CODE_KP_7         0x6C
#define SCAN_CODE_KP_8         0x75
#define SCAN_CODE_KP_9         0x7D
#define SCAN_CODE_BRKCLOSE     0x5B
#define SCAN_CODE_SEMICOLON    0x4C
#define SCAN_CODE_QUOTE        0x52       
#define SCAN_CODE_COMMA        0x41
#define SCAN_CODE_DOT          0x49
#define SCAN_CODE_SLASH        0x4A

#pragma endregion

#pragma region // ---------------------------- COMMANDS ---------------------------- //

#define COMMAND_ECHO            0xEE
#define COMMAND_READID          0xF2
#define COMMAND_SET_SCAN_CODE   0xF0
#define COMMAND_RESEND          0xFE
#define COMMAND_RESET           0xFF

#pragma endregion

#pragma region // ------------------------- SCAN CODES SETS ------------------------ //

#define SCAN_CODE_SET_1 0x43
#define SCAN_CODE_SET_2 0X41
#define SCAN_CODE_SET_3 0x3F

#pragma endregion

#pragma region // -------------------------- PEDAL STATES -------------------------- //

#define PEDAL_STATE_SINGLE_CLICK  0
#define PEDAL_STATE_HOLD_CLICK    1
#define PEDAL_STATE_DOBLE_CLICK   2

#pragma endregion

#endif