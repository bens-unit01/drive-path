
#ifndef SWITCHBOT_H_
#define SWITCHBOT_H_
//#define DEBUG_MODE
#define  ESTOP                      0x65
#define  CLEAR_ESTOP                0x66
#define  DRIVE                      0x81
#define	 DP_GOTO_BEACON             0x40
#define  DP_STOP                    0x41
#define  NOTF_GET_NEXT_BEACON       0x42
#define  DP_REACH_BEACON            0x43
#define  DP_NORDIC_MB_TEST          0x44
#define	 NOTF_NORDIC_MB_TEST        0x45
#define	 NOTF_DP_TARGET_REACHED     0x46
#define  DP_GET_CLOSEST_BEACON      0x48
#define  NOTF_DP_CLOSEST_BEACON     0x47
#define  DP_CHANGE_RANGE            0x49
#define  UART0                      0x00
#define  UART1                      0x01

#define  SET_KP                     0x70
#define  SET_KI                     0x71
#define  SET_KD                     0x72
#define  CHANGE_SPEED               0x73


#endif /* SWITCHBOT_H_ */
