# NMEA2000_mbed

This library provides support for MBED environments hosting ARM CPUs.  It allows access to the [NMEA2000](https://github.com/ttlappalainen/NMEA2000) library using MBED devices internal CAN controllers.

See https://github.com/thomasonw/NMEA2000_mbed
       https://github.com/ttlappalainen/NMEA2000
       

Though any MBED with internal CAN controller should work, this has been tested primarily with the STM32F072RB CPU
 
 <br> <br>


## Usage

This library is in support of ttlappalainen's NMEA2000 library.  See the [NMEA2000](https://github.com/ttlappalainen/NMEA2000) for more examples.  Include this library in your build files. 

`can1` must be defined and initialized externaly to this library
`canSerial` must be defined and initialized externaly to this library


<br><br>
## Limitation

At present serial text I/O  is fixed to `canSerial` which must be defined and initialized before NMEA2000.Open() is called.

The CAN port is fixed to `can1`, which must be initialized before NMEA2000.Open() is called.  Make sure to also take care of any transceiver sleep pins, if your hardware is so designed.
  
  
MBED libraries are of varying quality.  Review carefully the underlying MBED CAN libraries, the quality of them are suspect with major errors being found even in 2017.
  
  FastPacket protocol in NMEA2000 requires CAN messages be kept in as-sent order.  At present this is not assured in the MBED libraries.  Refer to `NMEA2000_mbed.cpp` and `open()` to see detail on how to address this.  Do note that these corrections may well require you  to locally edit your MBED libraries, refer to the current status of pull requests to see if this is needed.
  
  
-   At present the STM32Fx  CPUs have a correction identified, but not accepted into MBED libs
  
  If you make an additional correction to a new CPU, please edit this readme file as well as the lib source code.



<br><br>

## Hardware

Refer to the MBED CAN documentation for hardware configuration.
