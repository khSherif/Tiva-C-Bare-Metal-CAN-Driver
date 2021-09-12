/******************************************************************/
/* Author      : Khaled Sherif                                    */
/* Version     : V04                                              */
/* Date        : 9 SEP 2021                                       */
/******************************************************************/


#ifndef ERROR_STATUS_H_
#define ERROR_STATUS_H_

typedef enum
{
	BUS_OFF,
	ERR_COUNT_96,
	ERR_PASSIVE,
	ERR_ACTIVE,
    REC_ERR_PASSIVE

}ErrorCountStatus;

typedef enum
{
	NO_ERR,
	STUFF_ERR,
    FORMAT_ERR,
    ACK_ERR,
    BIT1_ERR,
    BIT0_ERR,
    CRC_ERR,
    NO_EVENT_ERR

}ErrorStatus;

typedef enum
{
	TRANSMIT_SUCCESS,
    RECEIVE_SUCCESS
}SuccessStatus;

#endif /* ERROR_STATUS_H_ */