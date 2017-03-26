/*
NMEA2000_mbed.cpp

2017 Copyright (c) Al Thomason   All rights reserved

Support the MBED targeted CPUs  (STM32F072, etc)
See: https://github.com/thomasonw/NMEA2000_mbed
     https://github.com/ttlappalainen/NMEA2000


Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

See also NMEA2000 library.
*/

#include <CAN.h>
#include "NMEA2000_mbed.h"

CAN can1(PB_8, PB_9);    //  (Rx,Tx)  pins

 // CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
//DigitalOut can1_SleepMode(PA_7);     // Use pin to control the sleep mode of can1
DigitalOut can1_SleepMode(PA_7);     // Use pin to control the sleep mode of can1

extern Serial pc;

//*****************************************************************************
tNMEA2000_mbed::tNMEA2000_mbed() : tNMEA2000()
{
}


//*****************************************************************************
bool tNMEA2000_mbed::CANOpen()
{

    can1.frequency(250000);

    can1_SleepMode = 0;         // Set the Sleep Mode to 0 or low or false... this makes sure the transceiver is on





    return true;
}





//*****************************************************************************
bool tNMEA2000_mbed::CANSendFrame(unsigned long id, unsigned char len, const unsigned char *buf, bool wait_sent)
{
    CANMessage output;
    bool result;


    output.format=CANExtended;
    output.type=CANData;
    output.id = id;
    output.len = len;
    for (unsigned char i=0; i<len && i<8; i++) output.data[i]=buf[i];


    result=can1.write(output);

    if (wait_sent){
	//---- Something must be done here to make sure packets are kept in as-sent order
	#warning  CANSendFrame() is not assuring packet sent order is retained!
	}

    return result;
}


//*****************************************************************************
bool tNMEA2000_mbed::CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf)
{
    bool HasFrame=false;
    CANMessage incoming;

    if ( can1.read(incoming)) {           // check if data coming
        id=incoming.id;
        len=incoming.len;
        for (int i=0; i<len && i<8; i++) buf[i]=incoming.data[i];
        HasFrame=true;

           pc.printf("\r\n\nGOT ONE!!!!\r\n");

    }

    return HasFrame;
}




/********************************************************************
*	Other 'Bridge' functions and classes
*
*
*
**********************************************************************/
Serial pcx(SERIAL_TX, SERIAL_RX, 115200);                        // Likely /dev/ttyACM0


int tmbedStream::read() {
  return pcx.getc();
}

size_t tmbedStream::write(const uint8_t* data, size_t size) {
  if (size > 0)
    for (int i=0; i<size; i++)
        pcx.putc(data[i]);
  return size;
}


void delay(const uint32_t ms)
{
    HAL_Delay(ms);
};


uint32_t millis(void)
{
    return(HAL_GetTick());
};
