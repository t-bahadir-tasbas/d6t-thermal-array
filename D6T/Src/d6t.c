/*
 * d6t.c
 *
 *  Created on: Oct 20, 2020
 *      Author: Admin
 */

#include "d6t.h"
#include "stm32f0xx_hal.h"
#include "stdlib.h"

extern I2C_HandleTypeDef hi2c1;

#define I2C_PERIPHRAL &hi2c1
#define I2C_ADDR_READ 0x15
#define I2C_ADDR_WRITE 0x14
#define I2C_TIMEOUT_MS 100

struct D6T_S
{
    int16_t Temperature_Reference;
    int16_t Temperature_Pixel[16];
    uint8_t Error_Check_Code;
};

//PRIVATE VARIABLES
static uint8_t Read_Buffer[35];
static uint8_t I2C_COMMAND = 0x4c;

//PRIVATE FUNCTION PROTOTYPES
static void Convert_Buffer_To_Temperature_Value(D6T_TypeDef me);

/**
 * @brief  Create new D6T object
 * @retval pointer of new object
 */
D6T_TypeDef D6T_New(void)
{
    struct D6T_S Temp;

    return calloc(1u, sizeof(Temp));
}
/**
 * @brief  it look for is IC available
 * @param D6T_TypeDef object
 * @retval D6T_Status_TypeDef D6T_OK for connected, D6T_ERROR not connected
 */
D6T_Status_TypeDef D6T_Is_Connected(D6T_TypeDef me)
{
    uint8_t Connection_Status = D6T_ERROR;

    if (HAL_I2C_IsDeviceReady(I2C_PERIPHRAL, I2C_ADDR_WRITE, 10, I2C_TIMEOUT_MS) == HAL_OK)
    {
        Connection_Status = D6T_OK;
    }

    return Connection_Status;
}

/**
 * @brief  read temperature and update object values
 * @param D6T_TypeDef object
 * @retval D6T_Status_TypeDef D6T_OK or D6T_ERROR
 */
D6T_Status_TypeDef D6T_Read_Temperature(D6T_TypeDef me)
{
    uint8_t Communication_Status = D6T_ERROR;

    if (HAL_I2C_Master_Transmit(I2C_PERIPHRAL, I2C_ADDR_WRITE, &I2C_COMMAND, 1, I2C_TIMEOUT_MS) != HAL_OK)
    {
        Communication_Status = D6T_ERROR;
    }
    else if (HAL_I2C_Master_Receive(I2C_PERIPHRAL, I2C_ADDR_READ, Read_Buffer, 35, I2C_TIMEOUT_MS) != HAL_OK)
    {
        Communication_Status = D6T_ERROR;
    }
    else
    {
        Convert_Buffer_To_Temperature_Value(me);
        Communication_Status = D6T_OK;
    }

    return Communication_Status;
}

/**
 * @brief  merge two 8bit value tı one 16 bit value
 * @param D6T_TypeDef object
 */
static void Convert_Buffer_To_Temperature_Value(D6T_TypeDef me)
{
    me->Temperature_Reference = 256 * Read_Buffer[1] + Read_Buffer[0];
    me->Temperature_Pixel[0] = 256 * Read_Buffer[3] + Read_Buffer[2];
    me->Temperature_Pixel[1] = 256 * Read_Buffer[5] + Read_Buffer[4];
    me->Temperature_Pixel[2] = 256 * Read_Buffer[7] + Read_Buffer[6];
    me->Temperature_Pixel[3] = 256 * Read_Buffer[9] + Read_Buffer[8];
    me->Temperature_Pixel[4] = 256 * Read_Buffer[11] + Read_Buffer[10];
    me->Temperature_Pixel[5] = 256 * Read_Buffer[13] + Read_Buffer[12];
    me->Temperature_Pixel[6] = 256 * Read_Buffer[15] + Read_Buffer[14];
    me->Temperature_Pixel[7] = 256 * Read_Buffer[17] + Read_Buffer[16];
    me->Temperature_Pixel[8] = 256 * Read_Buffer[19] + Read_Buffer[18];
    me->Temperature_Pixel[9] = 256 * Read_Buffer[21] + Read_Buffer[20];
    me->Temperature_Pixel[10] = 256 * Read_Buffer[23] + Read_Buffer[22];
    me->Temperature_Pixel[11] = 256 * Read_Buffer[25] + Read_Buffer[24];
    me->Temperature_Pixel[12] = 256 * Read_Buffer[27] + Read_Buffer[26];
    me->Temperature_Pixel[13] = 256 * Read_Buffer[29] + Read_Buffer[28];
    me->Temperature_Pixel[14] = 256 * Read_Buffer[31] + Read_Buffer[30];
    me->Temperature_Pixel[15] = 256 * Read_Buffer[33] + Read_Buffer[32];
    me->Error_Check_Code = Read_Buffer[34];
}

/**
 * @brief  get temperature referance
 * @param D6T_TypeDef object
 * @retval temperature referance value(multiplied with 10)
 */
int16_t D6T_Get_Temperature_Reference(D6T_TypeDef me)
{
    return me->Temperature_Reference;
}
/**
 * @brief  get temperature of pixel
 * @param D6T_TypeDef object
 * @param uint8_t Pixel_Number which pixel
 * @retval temperature of pixel value(multiplied with 10)
 */
int16_t D6T_Get_Temperature_Of_Pixel(D6T_TypeDef me, uint8_t Pixel_Number)
{
    return me->Temperature_Pixel[Pixel_Number];
}

/**
 * @brief  get temperature all
 * @param D6T_TypeDef object
 * @param uint16_t *Temperature_All_Values array with 16 elements
 * @retval temperature of all array value(multiplied with 10)
 */
void D6T_Get_Temperature_All(D6T_TypeDef me, uint16_t *Temperature_All_Values)
{
    for (int i = 0; i <= 15; i++)
    {
        Temperature_All_Values[i] = me->Temperature_Pixel[i];
    }
}
