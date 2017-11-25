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

//*****************************************************************************
//  Driver dependencies:
extern CAN can1;                                                                // Low level CAN needs to be created for us as 'can1'


//*****************************************************************************
tNMEA2000_mbed::tNMEA2000_mbed() : tNMEA2000()
{
}



//*****************************************************************************
bool tNMEA2000_mbed::CANOpen()
{

    can1.frequency(250000);

    #if defined(TARGET_STM)
      //#error  Need to verify CAN packet order is kept,  comment out this Error once you are sure of your local environment.
      #error   Make sure file: targets/TARGET_STM/can_api.c   has been edited per  https://github.com/ARMmbed/mbed-os/pull/4121

  #else
      #error  Review code / drivers to assure sent CAN message packet order is kept
      //  fastPacker in NMEA2000 requires that messages order be kept, as there is no ability to
      //  recover from out-of-order messages.
      //  If you received this error message, it means that the specific MBED CPU has not had its drivers reviewed
      //  and there is a risk of out-of-order packets.  Please make changes there, or below in ::CANSendFrame
      //  and  push the revised edits back to github.
  #endif



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
	     //---- You can put something here to assure these special class of messages are indeed sent in FIFO order.
       //   See above in ::CANOpen()

	     }

    return result;
}


//*****************************************************************************
bool tNMEA2000_mbed::CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf)
{
    bool HasFrame=false;
    CANMessage incoming;

    if (can1.read(incoming)) {           // check if data coming
        id=incoming.id;
        len=incoming.len;
        for (int i=0; i<len && i<8; i++) buf[i]=incoming.data[i];
        HasFrame=true;
    }

    return HasFrame;
}




/********************************************************************
*	Other 'Bridge' functions and classes
*
*
*
**********************************************************************/

int tmbedStream::read() {

  if (!feof(stdin))
      return (getc(stdin));                                                 	// Something to read!  Go get it and return it.
  else
      return -1;                                                                // Nothing to read,
}


size_t tmbedStream::write(const uint8_t* data, size_t size) {
  int i;

  for (i=0; (i<size) && data[i];  i++)
     putc(data[i],stdout);

  return i;
}




void delay(const uint32_t ms)
{
    HAL_Delay(ms);
};


uint32_t millis(void)
{
    return HAL_GetTick();
};
