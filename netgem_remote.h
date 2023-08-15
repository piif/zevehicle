// protocole details for Netgem remote controler
#ifdef IR_REMOTE_NETGEM
#define IR_REMOTE_PROTO_DEFINED

#define IR_MAX_DELAY 120000
#define IR_PROTO_FRAME_LEN    32
// adapt to word if len<16 or long long i >32 ?
// but how to handle error codes in this case ??
typedef unsigned long remoteIR_keyCode;

/*
input is high by default, then each "top" is a switch to low then high, finally a low before going back to high
each top is decoded from low and high duration :
protocol is constituted of a header top (H), 16 prefix bits, 16 data bits, a trail (T) :

_________        ____  ___  ___  _  _...  ___ __________
         ________    __   __   __ __ ...__   _          
nothing  |     H     | 1  | 1  |0 |0 |..| 1  |T| nothing

*/

#define IR_PROTO_PREFIX_MASK 0xFFFF0000
#define IR_PROTO_PREFIX      0xC1CC0000
#define IR_PROTO_DATA_MASK   0x0000FFFF

// header
#define IR_PROTO_HLOW  9040
#define IR_PROTO_HHIGH 4425
// repeat
#define IR_PROTO_RLOW  9040
#define IR_PROTO_RHIGH 2200
// 1
#define IR_PROTO_1LOW   628
#define IR_PROTO_1HIGH 1614
// 0
#define IR_PROTO_0LOW   628
#define IR_PROTO_0HIGH  448
// trail
#define IR_PROTO_TRAIL  608

#define IR_PROTO_MARGIN 100

#define IR_KEYPAD_1           0x8679
#define IR_KEYPAD_2           0x46B9
#define IR_KEYPAD_3           0xC639
#define IR_KEYPAD_4           0x26D9
#define IR_KEYPAD_5           0xA659
#define IR_KEYPAD_6           0x6699
#define IR_KEYPAD_7           0xE619
#define IR_KEYPAD_8           0x16E9
#define IR_KEYPAD_9           0x9669
#define IR_CANCEL             0x0EF1
#define IR_KEYPAD_0           0x06F9
#define IR_LIST               0x8E71
#define IR_RED                0x629D
#define IR_GREEN              0xE21D
#define IR_YELLOW             0x12ED
#define IR_BLUE               0x926D
#define IR_DIRECTION_UP       0x42BD
#define IR_DIRECTION_DOWN     0xC23D
#define IR_DIRECTION_LEFT     0x22DD
#define IR_DIRECTION_RIGHT    0xA25D
#define IR_OK                 0x827D
#define IR_VOLUME_UP          0x6A95
#define IR_VOLUME_DOWN        0xEA15
#define IR_PROGRAM_UP         0x1AE5
#define IR_PROGRAM_DOWN       0x9A65
#define IR_FAST_FORWARD       0x8A75
#define IR_REWIND             0x0AF5
#define IR_TEXT               0x4AB5
#define IR_INFO               0xCA35
#define IR_MUTE               0x4EB1
#define IR_PICTURE_IN_PICTURE 0x2AD5
#define IR_MENU               0xAA55
#define IR_POWER              0xCE31

#endif // IR_REMOTE_NETGEM
