/************************************************************************
 *    Project:        MK4 D-Class (TAS3251)                             *
 *    Author:         Patrick Isele                                     *
 *    Filename:       main.c                                            *
 *    Date:           15.09.20                                          *
 *    File Version:   0.1                                               *
 *    Supported dev:  PIC24F16KA101                                     *
 *                                                                      *
 *    Frequency:    16 MHz (internal FRSC with PLL) --> 8 MIPS          *
 *                  32 MHz / 16 MIPS would be the maximum               *
 *    Hardware:     DE PPT ADDON 5003-6968/00                           *
 *    IDE:          MPLABX® v5.4                                        *
 *    C-Compiler:   MPLAB® C30 v3.31                                    *
 *    Tab Settings: 1 Tab = 4 Spaces                                    *
 *                                                                      *
 *    Description:  PAT9102 Optical XY Sensor has SPI Interface (2MHz)  *
 *                  Two modes of operation:                             *
 *                  mode0(default): SRom Firmware is downloaded         *
 *                  and Motion registers are transfered to Hammer.      *
 *                  Update rate is ~750Hz                               *
 *                  mode1: Frame raw data (32x32 byte) are read         *
 *                  and transfered to Hammer. Update rate is ~8Hz       *
 *                                                                      *
 *    History:      Version Date        Name    Change description      *
 *                  0.1     24.03.21    IP      Initial version.        *
 *                                                                      *
 *                                                                      *
 *  --> please remember to always update the version in defines.h !     *
 ************************************************************************/


/*************** FILE INCLUSION *********************/
#include "general_defines.h"

/*************** CONSTANT DEFINITIONS ***************/
const uint8_t i2cAddr[2] = {ADR_MIDH, ADR_BASS};
const uint8_t chOn[4]  = {0x00, 0x80, 0x00, 0x00};
const uint8_t chOff[4] = {0x00, 0x00, 0x00, 0x00};

const uint8_t bass_bq_1[20] = {0x00, 0x0C, 0x4A, 0x50, 0x00, 0x0C, 0x4A, 0x50, 0x00, 0x0C, 0x4A, 0x50, 0x7C, 0x72, 0x52, 0x6A, 0x86, 0xEA, 0x31, 0xEC};
const uint8_t bass_bq_2[20] = {0x00, 0x0C, 0x4A, 0x50, 0x00, 0x0C, 0x4A, 0x50, 0x00, 0x0C, 0x4A, 0x50, 0x7C, 0x72, 0x52, 0x6A, 0x86, 0xEA, 0x31, 0xEC};
const uint8_t bass_bq_3[20] = {0x7F, 0xED, 0x72, 0xCB, 0x80, 0x5B, 0x74, 0xEA, 0x7F, 0x5E, 0x26, 0xD5, 0x7F, 0xA4, 0x8B, 0x16, 0x80, 0xB4, 0x66, 0x60};
const uint8_t bass_bq_4[20] = {0x7F, 0xCE, 0x63, 0xE6, 0x80, 0xFA, 0x3C, 0xA1, 0x7E, 0x4F, 0x31, 0x68, 0x7F, 0x05, 0xC3, 0x5F, 0x81, 0xE2, 0x6A, 0xB0};
const uint8_t bass_bq_5[20] = {0x7F, 0xFF, 0xFF, 0xFF, 0x82, 0x02, 0x84, 0xCE, 0x7C, 0x1D, 0x84, 0xD1, 0x7E, 0x8D, 0x0E, 0xFA, 0x82, 0xC3, 0x2C, 0x3E};

const uint8_t mid_bq_1[20] = {0x7C, 0x7E, 0x9C, 0xBA, 0x83, 0x81, 0x63, 0x46, 0x7C, 0x7E, 0x9C, 0xBA, 0x7C, 0x72, 0x52, 0x6A, 0x86, 0xEA, 0x31, 0xEC};
const uint8_t mid_bq_2[20] = {0x7C, 0x7E, 0x9C, 0xBA, 0x83, 0x81, 0x63, 0x46, 0x7C, 0x7E, 0x9C, 0xBA, 0x7C, 0x72, 0x52, 0x6A, 0x86, 0xEA, 0x31, 0xEC};
const uint8_t mid_bq_3[20] = {0x02, 0xF8, 0x4D, 0x62, 0x02, 0xF8, 0x4D, 0x62, 0x02, 0xF8, 0x4D, 0x62, 0x61, 0x9D, 0xED, 0x3C, 0xB0, 0xE2, 0xF0, 0x01};
const uint8_t mid_bq_4[20] = {0x02, 0xF8, 0x4D, 0x62, 0x02, 0xF8, 0x4D, 0x62, 0x02, 0xF8, 0x4D, 0x62, 0x61, 0x9D, 0xED, 0x3C, 0xB0, 0xE2, 0xF0, 0x01};
const uint8_t mid_bq_5[20] = {0x7F, 0x79, 0xB1, 0x75, 0x87, 0x84, 0x16, 0x44, 0x76, 0xE0, 0x46, 0x8D, 0x78, 0x7B, 0xE9, 0xBC, 0x89, 0xA6, 0x07, 0xFC};

const uint8_t high_bq_1[20] = {0x63, 0xF3, 0x42, 0x42, 0x9C, 0x0C, 0xBD, 0xBE, 0x63, 0xF3, 0x42, 0x42, 0x62, 0x37, 0xC5, 0x4F, 0xB4, 0xA2, 0x81, 0x92};
const uint8_t high_bq_2[20] = {0x63, 0xF3, 0x42, 0x42, 0x9C, 0x0C, 0xBD, 0xBE, 0x63, 0xF3, 0x42, 0x42, 0x62, 0x37, 0xC5, 0x4F, 0xB4, 0xA2, 0x81, 0x92};
const uint8_t high_bq_3[20] = {0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t high_bq_4[20] = {0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t high_bq_5[20] = {0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t clip1[4] = {0x04, 0x09, 0xC2, 0xB1};
const uint8_t clip2[4] = {0x07, 0xEC, 0xA9, 0xCD};

const uint8_t swap[4] = {0x00, 0x00, 0x00, 0x01};
const uint8_t zero[4] = {0x00, 0x00, 0x00, 0x00};


/************ DEVICE CONFIGURATION BITS *********************************/
// Overwrites MPLAB config: see Configure - Configuration Bits...
// Refer to datasheet DS39995C page 239

_FBS(               // FBS register at 0xF80000
    BSS_OFF &           // No boot program flash segment
    BWRP_OFF)           // Boot Segment Write Protect OFF
_FGS(               // FGS register at 0xF80004
    GCP_ON &           // General Code Protect ON
    GWRP_OFF)           // General Segment Write-Protect OFF
//#warning "GCP_OFF"
_FOSCSEL(           // FOSCSEL register at 0xF80006
    FNOSC_FRCPLL &      // Fast RC oscillator w/ divide and PLL
    IESO_OFF)           // Two-speed Oscillator Startup OFF
_FOSC(              // FOSC register at 0xF80008
    POSCMOD_NONE &      // Primary oscillator diabled
    OSCIOFNC_OFF &      // CLKO output disabled
    FCKSM_CSDCMD)       // Both Clock Switching and Fail-safe Clock Monitor are disabled
_FWDT(              // FWDT register at 0xF8000A
    FWDTEN_ON &         // WDT enabled in hardware
    WINDIS_OFF &        // Standard WDT selected(windowed WDT disabled)
    FWPSA_PR32 &        // WD prescaler 1:32 (31 kHz : 32 = 969 Hz)
    WDTPS_PS512)        // WD postscaler 1:512 (969 Hz : 512 = 1.9 Hz = 528ms)
_FPOR(              // FPOR register at 0xF8000C
    PWRTEN_ON &         // Power-up timer (PWRT) enabled, typ. 64ms / max. 90ms
    BORV_V27 &          // Brown-out Reset set at 2.7V
    BOREN_BOR3 &        // Brown-out Reset is enabled in hardware; SBOREN bit is disabled
    MCLRE_ON)           // RA5 input pin disabled, MCLR enabled
_FICD(              // FICD register at 0xF8000E
    ICS_PGx3)           // ICD Pin Placement Select bit on PGC3/PGD3
_FDS(               // FDS register at 0xF80010
    DSWDTPS_DSWDTPS0 &  // Deep Sleep Watchdog Timer Postscaler set to 1:2 (2.1 ms)
    DSWDTOSC_LPRC &     // DSWDT uses Low Power RC Oscillator (LPRC)
    DSBOREN_OFF &       // Deep Sleep BOR disabled in Deep Sleep
    DSWDTEN_OFF)        // Deep Sleep Watchdog Timer Enable OFF


/*********** START OF GLOBAL DEFINITIONS ************/

/************************************************************************
* USER INTERRUPTS                                                       *
*************************************************************************/

//*********** Interrupt sevice rotuine for I2C1 Master ******************
void __attribute__((interrupt,no_auto_psv)) _MI2C1Interrupt(void)
{
  MI2C1_Clear_Intr_Status_Bit;  //Clear Interrupt status of I2C1
}

//******************** Interrupt sevice rotuine for IO change   **********
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    InputChange_Clear_Intr_Status_Bit;
    if (SPI_ERROR == 1 || FAULT1_N == 0 || FAULT2_N == 0){ // Fail condition
        RES_AMP_N = 0;
        LED_GN = OFF;
        LED_YE = ON;
    } else if (SPI_ERROR == 0 && FAULT1_N == 1 && FAULT2_N == 1){ // ReOp
        RES_AMP_N = 1;
        LED_YE = OFF;
        LED_GN = ON;
    }
}

/************************************************************************
* Sub Routines                                                          *
*************************************************************************/
/************************************************************************
* DelayXms																*
*************************************************************************/
void DelayXms(unsigned int xMs)
{
	// 1ms = 8'000 Taktzyklen @ 16MHz Fosc or 8MHz Fcy
	unsigned int i;
	for (i = 2*xMs; i > 0; i--)
	{
		asm volatile("repeat #4000");
		ClrWdt();	// Clear watchdog timer
	}
}

// Write single Register
Write_1_Reg(uint8_t DevAddr, uint8_t Reg, uint8_t Data)
{
    uint8_t WrAddr;
    WrAddr = (DevAddr << 1) | 0; //Device Address + Write bit
   
    IdleI2C1();
    StartI2C1();
    while(I2C1CONbits.SEN );     //Wait till Start sequence is completed
    
    MasterWriteI2C1(WrAddr);     //Write Slave address and set master for transmission
    while(I2C1STATbits.TBF);     //Wait till address is transmitted
    IdleI2C1();

    MasterWriteI2C1(Reg);        //Write Slave register and set master for transmission
    while(I2C1STATbits.TBF);     //Wait till register is transmitted
    IdleI2C1();

    MasterWriteI2C1(Data);       //Write Data
    while(I2C1STATbits.TBF);     //Wait till data is transmitted
    IdleI2C1();
 
    StopI2C1();
    while(I2C1CONbits.PEN);  //Wait till stop sequence is completed
}

// Write multiple registers with auto inc
Write_M_Reg(uint8_t DevAddr, uint8_t Reg, int ByteCnt, const uint8_t *wrptr)
{
    int i;
    uint8_t WrAddr;
    WrAddr = (DevAddr << 1) | 0; //Device Address + Write bit
   
    IdleI2C1();
    StartI2C1();
    while(I2C1CONbits.SEN );     //Wait till Start sequence is completed
    
    MasterWriteI2C1(WrAddr);     //Write Slave address and set master for transmission
    while(I2C1STATbits.TBF);     //Wait till address is transmitted
    IdleI2C1();

    MasterWriteI2C1(Reg);        //Write Slave register and set master for transmission
    while(I2C1STATbits.TBF);     //Wait till registe is transmitted
    IdleI2C1();

   for(i=0;i<ByteCnt;i++)        //Data
    {
        MasterWriteI2C1(wrptr[i]);   //Write Data
        while(I2C1STATbits.TBF);     //Wait till data is transmitted
        IdleI2C1();
    }
    StopI2C1();
    while(I2C1CONbits.PEN);  //Wait till stop sequence is completed
}

SetBookPage(uint8_t DevAddr, uint8_t Book, uint8_t Page)
{
    Write_1_Reg(DevAddr, 0x00, 0x00);
    Write_1_Reg(DevAddr, 0x7F, Book);
    Write_1_Reg(DevAddr, 0x00, Page);
}

/************************************************************************
* Main Routine                                                          *
*************************************************************************/
int main(void)
{
    int i;
    
    InitOsc();
    InitPorts();
    InitI2C();
    LED_GN = OFF;
    LED_YE = OFF;
    RST_N = 1;
    while (SPI_ERROR == 1 || FAULT1_N == 0 || FAULT2_N == 0){ // not yet ready
        ClrWdt();
    }
    LED_GN = ON;
    DelayXms(1000);
    
    // start enabling DSP
    SetBookPage(ADR_BASS, 0x00, 0x00);
    Write_1_Reg(ADR_BASS, 0x02, 0x00); // enable DSP
    SetBookPage(ADR_MIDH, 0x00, 0x00);
    Write_1_Reg(ADR_MIDH, 0x02, 0x00); // enable DSP

    // start set Input channel, enable sub channel
    for (i = 0; i < 2; i++){
        SetBookPage(i2cAddr[i], 0x8C, 0x1D);
        // sub channels (crossover mode))
        Write_1_Reg(i2cAddr[i], 0x28, 0x00);
        Write_1_Reg(i2cAddr[i], 0x29, 0x00);
        Write_1_Reg(i2cAddr[i], 0x2A, 0x00);
        Write_1_Reg(i2cAddr[i], 0x2B, 0x00);
        // input channels
        if (LR == 0){
            Write_M_Reg(i2cAddr[i], 0x5C, 4, chOff);
            Write_M_Reg(i2cAddr[i], 0x60, 4, chOn);
            Write_M_Reg(i2cAddr[i], 0x64, 4, chOff);
            Write_M_Reg(i2cAddr[i], 0x68, 4, chOff);
        } else {
            Write_M_Reg(i2cAddr[i], 0x5C, 4, chOn);
            Write_M_Reg(i2cAddr[i], 0x60, 4, chOff);
            Write_M_Reg(i2cAddr[i], 0x64, 4, chOff);
            Write_M_Reg(i2cAddr[i], 0x68, 4, chOff);
        }
        SetBookPage(i2cAddr[i], 0x8C, 0x23);
        Write_M_Reg(i2cAddr[i], 0x14, 4, swap);
    } // end set Input channel

    // start set BiQuads
    // Bass 1
    SetBookPage(ADR_BASS, 0x8C, 0x12);
    Write_M_Reg(ADR_BASS, 0x44, 20, bass_bq_1);
    Write_M_Reg(ADR_BASS, 0x58, 20, bass_bq_2);
    Write_M_Reg(ADR_BASS, 0x6C, 20, bass_bq_3);
    SetBookPage(ADR_BASS, 0x8C, 0x13);
    Write_M_Reg(ADR_BASS, 0x08, 20, bass_bq_4);
    Write_M_Reg(ADR_BASS, 0x1C, 20, bass_bq_5);
    SetBookPage(ADR_BASS, 0x8C, 0x1A);
    Write_M_Reg(ADR_BASS, 0x68, 4, zero);
    SetBookPage(ADR_BASS, 0x8C, 0x23);
    Write_M_Reg(ADR_BASS, 0x14, 4, swap);
    // Bass 2
    SetBookPage(ADR_BASS, 0x8C, 0x11);
    Write_M_Reg(ADR_BASS, 0x58, 20, bass_bq_1);
    Write_M_Reg(ADR_BASS, 0x6C, 20, bass_bq_2);
    SetBookPage(ADR_BASS, 0x8C, 0x12);
    Write_M_Reg(ADR_BASS, 0x08, 20, bass_bq_3);
    Write_M_Reg(ADR_BASS, 0x1C, 20, bass_bq_4);
    Write_M_Reg(ADR_BASS, 0x30, 20, bass_bq_5);
    SetBookPage(ADR_BASS, 0x8C, 0x1A);
    Write_M_Reg(ADR_BASS, 0x60, 4, zero);
    SetBookPage(ADR_BASS, 0x8C, 0x23);
    Write_M_Reg(ADR_BASS, 0x14, 4, swap);
    // Mid
    SetBookPage(ADR_MIDH, 0x8C, 0x12);
    Write_M_Reg(ADR_MIDH, 0x44, 20, mid_bq_1);
    Write_M_Reg(ADR_MIDH, 0x58, 20, mid_bq_2);
    Write_M_Reg(ADR_MIDH, 0x6C, 20, mid_bq_3);
    SetBookPage(ADR_MIDH, 0x8C, 0x13);
    Write_M_Reg(ADR_MIDH, 0x08, 20, mid_bq_4);
    Write_M_Reg(ADR_MIDH, 0x1C, 20, mid_bq_5);
    SetBookPage(ADR_MIDH, 0x8C, 0x1A);
    Write_M_Reg(ADR_MIDH, 0x68, 4, zero);
    SetBookPage(ADR_MIDH, 0x8C, 0x23);
    Write_M_Reg(ADR_MIDH, 0x14, 4, swap);
    // High
    SetBookPage(ADR_MIDH, 0x8C, 0x11);
    Write_M_Reg(ADR_MIDH, 0x58, 20, high_bq_1);
    Write_M_Reg(ADR_MIDH, 0x6C, 20, high_bq_2);
    SetBookPage(ADR_MIDH, 0x8C, 0x12);
    Write_M_Reg(ADR_MIDH, 0x08, 20, high_bq_3);
    Write_M_Reg(ADR_MIDH, 0x1C, 20, high_bq_4);
    Write_M_Reg(ADR_MIDH, 0x30, 20, high_bq_5);
    SetBookPage(ADR_MIDH, 0x8C, 0x1A);
    Write_M_Reg(ADR_MIDH, 0x60, 4, zero);
    SetBookPage(ADR_MIDH, 0x8C, 0x23);
    Write_M_Reg(ADR_MIDH, 0x14, 4, swap);
    // end set BiQuads
    
    // start Output clipper
    for (i = 0; i < 2; i++){
        SetBookPage(i2cAddr[i], 0x8C, 0x1D);
        Write_M_Reg(i2cAddr[i], 0x3C, 4, clip1);
        Write_M_Reg(i2cAddr[i], 0x30, 4, clip2);
        SetBookPage(i2cAddr[i], 0x8C, 0x23);
        Write_M_Reg(i2cAddr[i], 0x14, 4, swap);
    } // end set Output clipper

    // start set Out Mux
    for (i = 0; i < 2; i++){
        SetBookPage(i2cAddr[i], 0x8C, 0x1E);
        Write_M_Reg(i2cAddr[i], 0x14, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x18, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x1C, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x20, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x24, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x28, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x2C, 4, chOn);
        Write_M_Reg(i2cAddr[i], 0x30, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x34, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x38, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x3C, 4, chOff);
        Write_M_Reg(i2cAddr[i], 0x40, 4, chOn);
        SetBookPage(i2cAddr[i], 0x8C, 0x23);
        Write_M_Reg(i2cAddr[i], 0x14, 4, swap);
    } // end set Input channel
    
    DAC_MUTE_N = 1;
    RES_AMP_N = 1;

    while(1){
        ClrWdt();
    }
}
