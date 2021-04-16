#ifndef __GENERAL_DEFINES_H
#define __GENERAL_DEFINES_H

/**************************************************************************************
* Project:      MK4
* Filename:     general_defines.h
* Date:         26.03.21
* Version:      0.1
* Autor:        Patrick Isele
**************************************************************************************/

/******** switches & settings ********/
#define BiQuadActive (1)                    // Use BeQuads

/*********** FILE INCLUSION *********************************************/
#define USE_AND_OR /* To enable AND_OR mask setting */

#if defined(__PIC24F16KA101__)
    #include <p24F16KA101.h>
#else
    #error -- processor ID not specified
#endif
#include <stdint.h>
#include <i2c.h>
#include <ports.h>
#include <timer.h>
#include "init.h"

/*********** START OF GLOBAL DEFINITIONS ********************************/
/*! Frequency */
#define GLOBAL_CLOCK_FREQ 16000000
#define GLOBAL_BUS_CLK_FREQ (GLOBAL_CLOCK_FREQ/2)
#define GLOBAL_BUS_CLK_PERIOD (2.0/GLOBAL_CLOCK_FREQ)

/************ CONSTANTS: I/O DEFINITIONS ********************************/
// LED
#define     LED_GN            LATBbits.LATB14
#define     LED_YE            LATBbits.LATB15
#define     ON              1
#define     OFF             0

// DIR9001 Interface
#define     RST_N           LATAbits.LATA0
#define     SPI_ERROR       PORTBbits.RB0

// TAS3251 Interface
#define     RES_AMP_N       LATBbits.LATB13       
#define     DAC_MUTE_N      LATBbits.LATB12       
#define     FAULT1_N        PORTBbits.RB1       
#define     FAULT2_N        PORTBbits.RB2       

// Else
#define     LR              PORTAbits.RA1

/************ CONSTANTS: DEFAULT VALUES *********************************/
// TRISx I/O definition
#define     PORTA_INOUT     0b1111111111111110  // All inputs, except RA0 (RST_N 9001)
#define     PORTB_INOUT     0b0000111111111111  // All inputs, except RB7/12/13/14/15

// AD1PCFG Analog input configuration
#define     ANIN_CFG        0b1111111111111111  // All digital

/************ CONSTANTS: I2C  ************************************/
/*
 * TAS3251 opcode: 1 0 0 1 0 1 0 RW
 *                            \    \_ Read/Write bit: 0=write, 1=read
 *                             \_ Address 0x4A (Adr low) 0x4B (Adr high)
 */
#define ADR_MIDH  (0x4A)
#define ADR_BASS  (0x4B)

/****** Timer Settings ******/
#define clearTimer2             WriteTimer2(0x0000)
#define M_SEC                   0x061A          // .1 Sek
#define TWO_SEC                0xF420          // 2 Sek

/************ EOF *******************************************************/
#endif /* __GENERAL_DEFINES_H */
        
