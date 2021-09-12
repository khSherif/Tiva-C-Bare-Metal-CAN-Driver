/******************************************************************/
/* Author      : Khaled Sherif                                    */
/* Version     : V04                                              */
/* Date        : 9 SEP 2021                                       */
/******************************************************************/

#ifndef CAN_PRIVITE_H
#define CAN_PRIVITE_H

/******************************************************************/
/*                           Registers                            */
/******************************************************************/
typedef struct 
{
    volatile u32 CTL;
    volatile u32 STS;
    volatile u32 ERR;
    volatile u32 BIT;
    volatile u32 INT;
    volatile u32 TST;
    volatile u32 BRPE;
    volatile u32 IF1CRQ;
    volatile u32 IF1CMSK;
    volatile u32 IF1MSK1;
    volatile u32 IF1MSK2;
    volatile u32 IF1ARB1;
    volatile u32 IF1ARB2;
    volatile u32 IF1MCTL;
    volatile u32 IF1D[4];
    volatile u32 IF2CRQ;
    volatile u32 IF2CMSK;
    volatile u32 IF2MSK1;
    volatile u32 IF2MSK2;
    volatile u32 IF2ARB1;
    volatile u32 IF2ARB2;
    volatile u32 IF2MCTL;
    volatile u32 IF2D[4];
    volatile u32 TXRQ1;
    volatile u32 TXRQ2;
    volatile u32 NWDA1;
    volatile u32 NWDA2;
    volatile u32 MSG1INT;
    volatile u32 MSG2INT;
    volatile u32 MSG1VAL;
    volatile u32 MSG2VAL;
}CAN_Reg;

#define CAN0 ((CAN_Reg *) 0x40040000)
#define CAN1 ((CAN_Reg *) 0x40041000)

// RCC and GPIO Registers
#define RCGC0       *((volatile u32*) 0x400FE100)
#define RCGC2       *((volatile u32*) 0x400FE108)
#define GPIOB       *((volatile u32*) 0x40005000)
#define GPIOB_AFSEL *((volatile u32*) 0x40005420)
#define GPIOB_PCTL  *((volatile u32*) 0x4000552C)


/******************************************************************/
/*                  Privite Function Prototypes                   */
/******************************************************************/
void Bit_Time_Calc(void);

#endif