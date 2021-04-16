#include "init.h"

/************************************************************************
* INITIALISATION SUB FUNCTIONS											*
*************************************************************************/

/************************************************************************
* InitOscillator														*
*************************************************************************/
void InitOsc(void)
{
	// Fast RC with Postcaler and PLL module selected by POR configuration bits
	OSCCONbits.SOSCEN = 0;				// Disables the secondary oscillator
	CLKDIVbits.DOZEN = 0;				// DOZE Mode currently disabled
	CLKDIVbits.RCDIV = 1;				// Take internal 8 MHz FRC divided by 2 = 4MHz
										// together with the x4 PLL this results in 16MHz
	OSCCONbits.CLKLOCK = 1;				// Clock and PLL selection locked
	while(OSCCONbits.LOCK != 1) {};		// Wait for PLL to lock
}


/************************************************************************
* InitPorts																*
*************************************************************************/
void InitPorts(void)
{
    RST_N = 0;
    RES_AMP_N = 0;
    DAC_MUTE_N = 0;
	TRISA = PORTA_INOUT;				// Configure in/outputs
	TRISB = PORTB_INOUT;
	AD1PCFG = ANIN_CFG;					// Set Digital or Analog
    /*Configure the input change notification interrupt*/
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_DISABLE | INT_PRI_6);
    EnableCN4; // dir9001 error
    EnableCN5; // fault1_n
    EnableCN6; // fault2_n
 }

/************************************************************************
* InitI2C																*
*************************************************************************/
void InitI2C(void)
{
   UINT config1 = 0;
   UINT config2 = 0;

   /* Turn off I2C modules */
   CloseI2C1();    //Disbale I2C1 mdolue if enabled previously


//************ I2C interrupt configuration ******************************************************
    ConfigIntI2C1(MI2C_INT_OFF);  //Disable I2C interrupt

//***************** I2C1 configuration **********************************************************
/**********************************************************************************************
*
*        I2C1 enabled
*        continue I2C module in Idle mode
*        IPMI mode not enabled
*        I2CADD is 7-bit address
*        Disable Slew Rate Control for 100KHz
*        Enable SM bus specification
*        Disable General call address
*        Baud @ 8MHz = 39 into I2CxBRG
**********************************************************************************************/
   config1 = (I2C_ON  | I2C_7BIT_ADD );
   config2 = 78;
   OpenI2C1(config1,config2);   //configure I2C1

}

/************************************************************************
* InitI2C																*
*************************************************************************/
void InitTimer(void)
{
//Configure Timer 2 for reEnable TimeOut/
    T2CON = 0; //good practice
    TMR2 = 0;
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_3); /*Enable Interrupt*/
    PR2 = TWO_SEC;
    T2CON = (T2_PS_1_256);
    EnableIntT2;
}
