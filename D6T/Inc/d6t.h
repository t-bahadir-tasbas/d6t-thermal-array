/*
 * d6t.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Admin
 */

#ifndef INC_D6T_H_
#define INC_D6T_H_

#include "stdint.h"

typedef struct D6T_S * D6T_TypeDef;

typedef enum
{
    D6T_ERROR, D6T_OK
} D6T_Status_TypeDef;

D6T_TypeDef D6T_New(void);
D6T_Status_TypeDef D6T_Is_Connected(D6T_TypeDef me);
D6T_Status_TypeDef D6T_Read_Temperature(D6T_TypeDef me);
int16_t D6T_Get_Temperature_Reference(D6T_TypeDef me);
int16_t D6T_Get_Temperature_Of_Pixel(D6T_TypeDef me, uint8_t Pixel_Number);
void D6T_Get_Temperature_All(D6T_TypeDef me, uint16_t *Temperature_All_Values);

#endif /* INC_D6T_H_ */


/*
 * Pixel Table
 *
 * P0   - P1    - P2    - P3
 * P4   - P5    - P6    - P7
 * P8   - P9    - P10   - P11
 * P12  - P13   - P14   - P15
 *
 * */
