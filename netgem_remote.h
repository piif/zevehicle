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

#endif // IR_REMOTE_NETGEM
