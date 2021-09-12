/******************************************************************/
/* Author      : Khaled Sherif                                    */
/* Version     : V04                                              */
/* Date        : 9 SEP 2021                                       */
/******************************************************************/

#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

/******************************************************************/
/*                     Configration Constants                     */
/******************************************************************/

/*Options:
          CAN_TST_DISABLE
          CAN_TST_ENABLE */
#define CAN_TST_STS             CAN_TST_DISABLE

/* Optons:
          CAN_TST_MODE_LBACK           
          CAN_TST_MODE_SILENT          
          CAN_TST_MODE_LBACK_AND_SILENT
          CAN_TST_MODE_BASIC         */ 
#if CAN_TST_STS == CAN_TST_ENABLE
   #define CAN_TST_MODE         CAN_TST_MODE_LBACK
#endif

/* CAN Clock
   in Mhz Seconds */
#define CAN_ClC                 25

/* CAN Bit Rate
   in Kbps Seconds */
#define CAN_BITRATE             1000

/* The number of time quantum in bit time
   Choose from 4 to 25                 */
#define CAN_TimeQUANTUM_Num     5

/* CAN Physical Delay
   in nano Seconds */
#define CAN_NETWORK_DELAY       300

#endif