#ifndef __INIT_H
#define __INIT_H

/**************************************************************************************
* Project:      OPT XY SEN
* Filename:     general_defines.h
* Date:         15.09.20
* Version:      0.1
* Autor:        Patrick Isele
*
* History:      Version Date        Name    Change description
*      0.1      15.09.20            IP      Initial version
**************************************************************************************/

/*************** FILE INCLUSION *********************/
#include "general_defines.h"

/******************* FUNCTIONS **********************/
void InitOsc(void);
void InitPorts(void);
void InitI2C(void);
void InitTimer(void);

#endif /* __INIT_H */
