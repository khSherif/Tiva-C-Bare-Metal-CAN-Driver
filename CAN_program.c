/******************************************************************/
/* Author      : Khaled Sherif                                    */
/* Version     : V04                                              */
/* Date        : 9 SEP 2021                                       */
/******************************************************************/


/******************************************************************/
/*                            Includes                            */
/******************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "CAN_interface.h"
#include "CAN_privite.h"
#include "CAN_config.h"
#include "CAN_ERROR_STATUS.h"

/******************************************************************/
/*                      Function Definitions                      */
/******************************************************************/
void CAN_voidInit(void)
{
    // enable RCC for CAN and GPIO
    SET_BIT(RCGC0, 24);
    SET_BIT(RCGC2, 1);

    // Configure GPIO Pins for CAN
    GPIOB_AFSEL |= 0x30;
    GPIOB_PCTL  |= 0x880000;

    // Enable INIT and CCE pins in CTL Register
    CAN0 -> CTL = 0x41;

    // Wait if CAN is busy
    while(GET_BIT(CAN0->IF1CRQ, 15));

    //Select Test Mode
    #if CAN_TST_STS == CAN_TST_ENABLE
        SET_BIT(CAN0->CTL, 7);
        #if CAN_TST_MODE == CAN_TST_MODE_LBACK
            SET_BIT(CAN0->TST, 4);
        #elif CAN_TST_MODE == CAN_TST_MODE_SILENT
            SET_BIT(CAN0->TST, 3);
        #elif CAN_TST_MODE == CAN_TST_MODE_LBACK_AND_SILENT
            SET_BIT(CAN0->TST, 4);
            SET_BIT(CAN0->TST, 3);
        #elif CAN_TST_MODE == CAN_TST_MODE_BASIC
            SET_BIT(CAN0->TST, 2);

        #endif
    #endif

    //SET BaudRate
    Bit_Time_Calc();

    //Disable Interrupts
    CAN0 -> CTL &= ~(0x3E);

    //Enable CAN (Clear Init bit)
    CLR_BIT(CAN0 -> CTL, 0);    
}

void CAN_voidTransmitConfig(u32 Copy_u32MSG_ID, u32 Copy_u32MASK, u8 Copy_u8DATA_Size_in_Bytes, u8 Copy_u8MSG_NUM, ID_Size Copy_ID_Size)
{
    // Wait if CAN is busy
    while(GET_BIT(CAN0->IF2CRQ, 15));
    
    // Set WRNRD, ARB, CONTROL Bits
    CAN0 -> IF1CMSK = 0xB0;

    //Check ID Extended or Not
    if(Copy_ID_Size == EXTENDED)
    {
        if(Copy_u32MASK != 0)
        {
            // Configure MASK
            SET_BIT(CAN0 -> IF1MCTL, 12);
            CAN0 -> IF1MSK1 = Copy_u32MASK;
            CAN0 -> IF1MSK2 |= Copy_u32MASK >> 16;
            SET_BIT(CAN0 -> IF1MSK2, 15);
        }
        
        // Set DIR and MSGVAL Bits, 29 bit ID
        CAN0 -> IF1ARB1 = (0x0000FFFF) & Copy_u32MSG_ID;
        CAN0 -> IF1ARB2 |= Copy_u32MSG_ID >> 16;
        CAN0 -> IF1ARB2 |= 0xE000;
    }
    else
    {
        if(Copy_u32MASK != 0)
        {
            // Configure MASK
            SET_BIT(CAN0 -> IF1MCTL, 12);
            CAN0 -> IF1MSK2 |= Copy_u32MASK << 2;
        }

        // Set DIR and MSGVAL Bits, 11 bit ID
        CAN0 -> IF1ARB2 |= Copy_u32MSG_ID << 2;
        CAN0 -> IF1ARB2 = 0xA000;
    }

    // Set EOB and RMTEN bit and data size in DLC
    CAN0 -> IF1MCTL |= 0x280;
    CAN0 -> IF1MCTL |= Copy_u8DATA_Size_in_Bytes;
    
    CAN0 -> IF1CRQ |= Copy_u8MSG_NUM;
}

void CAN_voidTransmitData(u8* Copy_u8Data, u8 Copy_u8DataSize)
{
    u8 i;

    // Write data to be transmitted
    while (Copy_u8DataSize != 0)
    {
        i = Copy_u8DataSize / 2;
        if(Copy_u8DataSize % 2 == 0)
        {
            CAN0 -> IF1D[i - 1] = (Copy_u8Data[Copy_u8DataSize - 1] << 8) + Copy_u8Data[Copy_u8DataSize - 2];
            Copy_u8DataSize -= 2;
        }
        else
        {
            CAN0 -> IF1D[i] =  Copy_u8Data[Copy_u8DataSize - 1];
            Copy_u8DataSize -= 1;
        }  
    }
    // Set TXRQST Bit
    SET_BIT(CAN0 -> IF1MCTL, 8);
}

void CAN_voidReceiveConfig(u32 Copy_u32MSG_ID, u32 Copy_u32MASK, u8 Copy_u8DATA_Size_in_Bytes, u8 Copy_u8MSG_NUM, ID_Size Copy_ID_Size)
{    
    // Wait if CAN is busy
    while(GET_BIT(CAN0->IF2CRQ, 15));
    
    // Set WRNRD, ARB, CONTROL Bits
    CAN0 -> IF2CMSK = 0xB0;

    //Check ID Extended or Not
    if(Copy_ID_Size == EXTENDED)
    {
        if(Copy_u32MASK != 0)
        {
            // Configure MASK
            SET_BIT(CAN0 -> IF2MCTL, 12);
            CAN0 -> IF2MSK1 = Copy_u32MASK;
            CAN0 -> IF2MSK2 |= Copy_u32MASK >> 16;
            SET_BIT(CAN0 -> IF2MSK2, 15);
        }
        
        // Set DIR and MSGVAL Bits, 29 bit ID
        CAN0 -> IF2ARB1 = (0x0000FFFF) & Copy_u32MSG_ID;
        CAN0 -> IF2ARB2 |= Copy_u32MSG_ID >> 16;
        CAN0 -> IF2ARB2 |= 0xC000;
    }
    else
    {
        if(Copy_u32MASK != 0)
        {
            // Configure MASK
            SET_BIT(CAN0 -> IF2MCTL, 12);
            CAN0 -> IF2MSK2 |= Copy_u32MASK << 2;
        }

        // Set DIR and MSGVAL Bits, 11 bit ID
        CAN0 -> IF2ARB2 |= Copy_u32MSG_ID << 2;
        CAN0 -> IF2ARB2 = 0x8000;
    }

    // Set EOB and RMTEN bit and data size in DLC
    CAN0 -> IF2MCTL |= 0x280;
    CAN0 -> IF2MCTL |= Copy_u8DATA_Size_in_Bytes;

    CAN0 -> IF2CRQ |= Copy_u8MSG_NUM;
}

u8* CAN_u8Receive(void)
{
    static u8 data[8];

    // Copy the received data 
    if (GET_BIT(CAN0 -> IF2MCTL, 15))
    {
        // Save received data
        data[0] = (0x00FF  & (CAN0 -> IF2D[0])      );
        data[1] = ((0xFF00 & (CAN0 -> IF2D[0])) >> 8);
        data[2] = (0x00FF  & (CAN0 -> IF2D[1])      );
        data[3] = ((0xFF00 & (CAN0 -> IF2D[1])) >> 8);
        data[4] = (0x00FF  & (CAN0 -> IF2D[2])      );
        data[5] = ((0xFF00 & (CAN0 -> IF2D[2])) >> 8);
        data[6] = (0x00FF  & (CAN0 -> IF2D[3])      );
        data[7] = ((0xFF00 & (CAN0 -> IF2D[3])) >> 8);
    }

    return data;
}


void Bit_Time_Calc(void)
{
    u8 tprop, tphase1, tphase2, SJW;
    u16 tq, BuadRate;

    /* Result in nano second */
    tq = (1000000 / CAN_BITRATE) / CAN_TimeQUANTUM_Num;

    /* Set Buad Rate Value */
    BuadRate = (tq * CAN_ClC) / 1000;

    /* Set Time Prop Value to compensate physical delay */
    if((CAN_NETWORK_DELAY % tq) == 0)
        tprop = (CAN_NETWORK_DELAY / tq);
    else
        tprop = (CAN_NETWORK_DELAY / tq) + 1;

    tphase1 = CAN_TimeQUANTUM_Num - 1 - tprop;

    /* Set tphase1 and tphase2 values */
    if ((tphase1 % 2) == 0)
    {
        tphase1 = tphase1 / 2;
        tphase2 = tphase1;
    }
    else
    {
        tphase1 = tphase1 / 2;
        tphase2 = tphase1 + 1;
    }
    
    /* Set SJW value */
    if (tphase1 >= 4)
        SJW = 4;
    else
        SJW = tphase1;
    
    /* Configure BIT register with Calculations */
    CAN0 -> BIT |= ((tphase2 - 1) << 12);
    CAN0 -> BIT |= ((tphase1 + tprop - 1) << 8);
    CAN0 -> BIT |= ((SJW - 1) << 6);
    
    BuadRate -= 1;
    if(BuadRate > 63)
    {
        CAN0 -> BRPE = (BuadRate >> 6);
        CAN0 -> BIT |= (BuadRate & (~(1111<<6)));
    }
    else
    {
        CAN0 -> BIT |= BuadRate;
    }
}

ErrorCountStatus CAN_voidCheckCountError(void)
{
    // Check BOFF Bit
    if(GET_BIT(CAN0 -> STS, 7))
        return BUS_OFF;

    // Check EWARN Bit
    if(GET_BIT(CAN0 -> STS, 6))
    {
        // Check EPASS Bit
        if(GET_BIT(CAN0 -> STS, 5))
            return ERR_PASSIVE;
        else
            return ERR_COUNT_96;
    }
    else
        return ERR_ACTIVE;
}

ErrorStatus CAN_voidCheckError(void)
{
    // Check LEC Field
    if((0x07 & (CAN0 -> STS)) == 0x1)
        return STUFF_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x2)
        return FORMAT_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x3)
        return ACK_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x4)
        return BIT1_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x5)
        return BIT0_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x6)
        return CRC_ERR;
    else if((0x07 & (CAN0 -> STS)) == 0x7)
        return NO_EVENT_ERR;
    else
        return NO_ERR;   
}

SuccessStatus CAN_voidCheckTX_RXSuccess(void)
{
    // Check RXOK Bit
    if(GET_BIT(CAN0 -> STS, 4))
    {
        // A message is received successfully
        CLR_BIT(CAN0 -> STS, 4);
        return RECEIVE_SUCCESS;
    }
    
    // Check TXOK Bit
    if(GET_BIT(CAN0 -> STS, 3))
    {
        // A message is transmitted successfully
        CLR_BIT(CAN0 -> STS, 3);
        return TRANSMIT_SUCCESS;
    }
}

u8 CAN_GetErrorTEC(void)
{
    return (0x00FF & (CAN0 -> ERR));
}

u8 CAN_GetErrorREC(void)
{
    return ((0xFF00 & (CAN0 -> ERR)) >> 8);
}
