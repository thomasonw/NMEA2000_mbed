#include <mbed.h>
#include <NMEA2000_CAN.h>                                       // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>

Serial pc(SERIAL_TX, SERIAL_RX, "pc", 115200);                        // Likely /dev/ttyACM0
//Serial pc(PB_10, PB_11, "pc", 115200);                                // Open 'serial' port in UART-3 

//---  Externial definitions needed by NMEA2000_mbed.cpp library
CAN    can1(PB_8, PB_9);                                        //  (Rx,Tx)  pins

//DigitalOut can1_SleepMode(PA_7);                                // Use pin to control the sleep mode of can1 transeiver (if hardware is so configured)



DigitalOut myled(LED1);

const tProductInformation BatteryMonitorProductInformation={
                                       1300,                        // N2kVersion
                                       100,                         // Manufacturer's product code
                                       "Simple battery monitor",    // Manufacturer's Model ID
                                       "1.0.0.13 (2016-09-19)",     // Manufacturer's Software version code
                                       "1.0.0.0 (2015-08-03)",      // Manufacturer's Model version
                                       "00000001",                  // Manufacturer's Model serial code
                                       0,                           // SertificationLevel
                                       1                            // LoadEquivalency
                                      };

const tNMEA2000::tProgmemConfigurationInformation BatteryMonitorConfigurationInformation={
                                       "John Doe, john.doe@unknown.com", // Manufacturer information
                                       "Just for sample", // Installation description1
                                       "No real information send to bus" // Installation description2
                                      };




#define BatUpdatePeriod 1000

void SendN2kBattery() {
  static unsigned long TempUpdated=millis();
  tN2kMsg N2kMsg;

  if ( TempUpdated+BatUpdatePeriod<millis() ) {
    TempUpdated=millis();
    SetN2kDCBatStatus(N2kMsg,1,13.87,5.12,35.12,1);
    NMEA2000.SendMsg(N2kMsg);
    SetN2kDCStatus(N2kMsg,1,1,N2kDCt_Battery,56,92,38500,0.012);
    NMEA2000.SendMsg(N2kMsg);
    SetN2kBatConf(N2kMsg,1,N2kDCbt_Gel,N2kDCES_Yes,N2kDCbnv_12v,N2kDCbc_LeadAcid,AhToCoulomb(420),53,1.251,75);
    NMEA2000.SendMsg(N2kMsg);
  }
}






int main()
{

    freopen("/pc", "r", stdin);                                                 // retarget stdout to '/pc' device.
    freopen("/pc", "w", stdout);                                                // retarget stdout to '/pc' device.
    setvbuf (stdout, NULL, _IONBF, 0);                                          // No buffering on stdout, just send chars as they come.



    pc.printf("\r\n\nAbout to start   AGAIN!!!!\r\n");

  //  can1_SleepMode = 0;         // Set the Sleep Mode to 0 or low or false... this makes sure the transceiver is on



    // Set Product information
    NMEA2000.SetProductInformation(&BatteryMonitorProductInformation );
    // Set Configuration information
    NMEA2000.SetProgmemConfigurationInformation(&BatteryMonitorConfigurationInformation );
    // Set device information
    NMEA2000.SetDeviceInformation(1,      // Unique number. Use e.g. Serial number.
                                  170,    // Device function=Battery. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                  35,     // Device class=Electrical Generation. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                  2046    // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                                 );



     // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
     NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,22);
     // NMEA2000.SetDebugMode(tNMEA2000::dm_ClearText);     // Uncomment this, so you can test code without CAN bus chips on Arduino Mega
     NMEA2000.EnableForward(false);                      // Disable all msg forwarding to USB (=Serial)


     NMEA2000.SetForwardStream(&serStream);                        // Send serial streams to the stdio port.
     //   NMEA2000.SetDebugMode(tNMEA2000::dm_ClearText);                             // Lets us see the debug messages on the terminal
    NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);                              // Show in clear text

     //  NMEA2000.SetN2kCANMsgBufSize(2);                    // For this simple example, limit buffer size to 2, since we are only sending data
     NMEA2000.Open();




    while(1) {

        SendN2kBattery();

        NMEA2000.ParseMessages();

        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec

    }
}
