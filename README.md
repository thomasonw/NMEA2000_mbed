# NMEA2000_mbed

This library provides support for MBED environments hosting ARM CPUs.  It allows access to the [NMEA2000](https://github.com/ttlappalainen/NMEA2000) library using MBED devices internal CAN controllers.

See https://github.com/thomasonw/NMEA2000_mbed
       https://github.com/ttlappalainen/NMEA2000
       
>***Note:  THIS IS A VERY EARLY RELEASE, IT HAS SEVERAL LIMITATIONS AND HARD CODING (SEE LIMITATIONS BELOW).  IT IS HOWEVER USEFUL FOR INITIAL DEVELOPMENT AND TESTING.***

Though any MBED with internal CAN controller should work, this has been tested primarily with the STM32F072RB CPU
 
 <br> <br>


## Usage

This library is in support of ttlappalainen's NMEA2000 library.  See the [NMEA2000](https://github.com/ttlappalainen/NMEA2000) for more examples.  Include this library in your build files. 

*Serial output is directed to the 'pc' (USB serial emulator).  Future releases of this lib will allow other selections.*

<br><br>
## Limitation

At present serial output is fixed to the 'pc' stream.

There is no checks to assure CAN packets are kept in sent-order when sending fast-packet messages.  Be cautions, this can cause unexpected errors.

Caution:  review carefully the underlying MBED CANlibraries, the quality of them are suspect with major errors being found even in 2017.


The CAN port is hard coded - the pins used.  Look for future release which allows this to be defined.



<br><br>

## Hardware

Refer to the MBED CAN documentation for hardware configuration.
