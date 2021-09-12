/******************************************************************/
/* Author      : Khaled Sherif                                    */
/* Version     : V04                                              */
/* Date        : 9 SEP 2021                                       */
/******************************************************************/

#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H


/******************************************************************/
/*                             Macros                             */
/******************************************************************/
#define CAN_TST_DISABLE                 0
#define CAN_TST_ENABLE                  1

#define CAN_TST_MODE_LBACK              0
#define CAN_TST_MODE_SILENT             1
#define CAN_TST_MODE_LBACK_AND_SILENT   2
#define CAN_TST_MODE_BASIC              3

typedef enum
{
    NOT_EXTENDED,
    EXTENDED
}ID_Size;


/******************************************************************/
/*                       Function Prototypes                      */
/******************************************************************/
// Initialization Function
void CAN_voidInit(void);

// Transmit Functions
void CAN_voidTransmitConfig(u32 Copy_u32MSG_ID, u32 Copy_u32MASK, u8 Copy_u8DATA_Size_in_Bytes, u8 Copy_u8MSG_NUM, ID_Size Copy_ID_Size);
void CAN_voidTransmitData(u8* Copy_u8Data, u8 Copy_u8DataSize);

// Receive Functions
void CAN_voidReceiveConfig(u32 Copy_u32MSG_ID, u32 Copy_u32MASK, u8 Copy_u8DATA_Size_in_Bytes, u8 Copy_u8MSG_NUM, ID_Size Copy_ID_Size);
u8* CAN_u8Receive(void);

//Error Handling Functions
ErrorCountStatus CAN_voidCheckCountError(void);
ErrorStatus CAN_voidCheckError(void);
SuccessStatus CAN_voidCheckTX_RXSuccess(void);
u8 CAN_GetErrorTEC(void);
u8 CAN_GetErrorREC(void);

#endif