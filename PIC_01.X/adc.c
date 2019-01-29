#include "adc.h"
#include <stdio.h>
#include <p30Fxxxx.h> //picks the correct device automatically

static char flag_interrupt = 0;

void resetFlagInterrupt(){
    flag_interrupt = 0;
}
char getFlagInterrupt(){
    return flag_interrupt;
}

void adc_setup()
{
    ADCON1bits.ADON=0;
    ADPCFG=0b1111111000110001; // set as analo
    //we now need configure the "scan sequence", we set the whole register
    ADCSSL=0b0000000111001110; //select scanned channels: 0b 0000 0000 0000 0111
    ADCON2bits.CSCNA=1; //this enables the scanning
    //we will sample those channels in (close) sequence
    //and connect the negative input of the s&h to Vref-
    ADCHSbits.CH0NA=0;
    ADCHSbits.CH0SA=0b0000; //we scan through the other channels, so this does not matter

    ADCON1bits.SIMSAM=0; //no simultaneous sampling
    ADCON2bits.CHPS=0b00; //and we only sample and hold unit 0 (Ch0)
    ADCON2bits.SMPI=0b0000; // generate an interrupt after conversion of 3 channels (0b0001 + 1)

    //select the conversion clock
    ADCON2bits.ALTS=0; //always use multiplexer settings A, i.e. scanning through inputs
    ADCON2bits.BUFM=0; //the conversion buffer is selected to use all 16 words (not 2 x 8 words)
    ADCON2bits.VCFG=0b000; //this selects the AVdd and AVss as voltage reference
    ADCON1bits.FORM=0b00; //we use the (unsigned) integer format for the A2D value
    ADCON3bits.ADRC=0; //clock is derived from internal clock;

    //Tad needs to be selected and must be a minimum of 83.33 ns
    //we derive Tad from Tcyc ==> Tad=32xTcyc = 2us
    ADCON3bits.ADCS=0b111111; //sets the conversion period to 32 x Tcyc (slowest possible)
    ADCON3bits.SAMC=0b11111; //     sets the auto sample speed to 32xTad ==> slowest setting
    //we need to select the source to start a conversion
    ADCON1bits.SSRC=0b111; //conversion (not sampling) is started with A2D clock afer SAMC (see above) cycles
    ADCON1bits.SAMP=1; //we are ready for sampling
    ADCON1bits.ASAM=0; //we start the autosampling in the timer ISR
    //lets now enable the interrupt and set the prio
    IFS0bits.ADIF=0 ; //clear the interrupt flag (in case it was on)
    IPC2bits.ADIP=5 ; // lets set the A2D prio to 5;
    IEC0bits.ADIE=1; //enable a2d interrupts
    //finally, we switch on the A2D converter again
    ADCON1bits.ADON=1; //ready to convert!
}

void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void) {
    IFS0bits.ADIF=0; // reset the interrupt flag
    flag_interrupt = 1;
}