# iKonvert

Welcome to our iKonvert Developer's Guide (SDK) on GitHub. This guide is intended to help developers quickly get to grips with our iKonvert NMEA2000 Gateway and implement it within their Desktop applications or embedded hardware designs.
<br>
<img src="https://github.com/digitalyacht/iKonvert/blob/master/Images/iKonvert_USB_Hi_Res.jpg" width=70%>
<br>
The iKonvert NMEA2000 Gateway is designed to allow an Application Developer or Equipment Manufacturer to support bi-directional communication with an NMEA2000 network, through simple serial messages. Any individual or company that wishes to integrate iKonvert will need as a minimum the NMEA2000 Appendix A+B which is available from [http://nmea.org](http://nmea.org) for both NMEA members and non-members.

<img src="https://github.com/digitalyacht/iKonvert/blob/master/Images/iKonvert_Tool.gif" width=70%>

Included in this repository are:-

*1.  Windows executable of the iKonvert Command Reference App (V1.30) so you can start playing straight away*

*2.  iKonvert Command Reference App source code showing how to talk to the gateway*

*3.  STM Flash Tool for Firmware Updating (Windows only)*

*4.  The latest iKonvert firmware for STM32F103 based units (V2.65)*

*5.  The latest iKonvert firmware for STM32F476 based units (V3.08)*

*6.  The iKonvert Developers Guide V1.04 (pdf)*

*7.  Latest User Manuals for iKonvert ISO and USB units*

*8.  Latest iKonvert USB Drivers for Windows OS* 

No special libraries are required to integrate iKonvert and the USB version uses an FTDI chipset that has good driver support for all common operating systems. If you are using iKonvert with a MAC or LINUX based computer, the drivers should already be included in the OS kernel, but if you are using Windows please download the latest drivers and install either the 32bit or 64bit drivers as applicable to your computer.

Our iKonvert will automatically take care of the complex NMEA 2000 bus management; claiming an address, respondng to other devices, etc. which significantly simplifies NMEA 2000 implementation. For developers that want an even quicker implementation path, iKonvert is compatible with the open source CANBoat library (coded in C) that translates the binary PGN data in to a more manageable JSON format. More information on using iKonvert with CANBoat [is available here.](https://github.com/canboat/canboat/wiki/ikonvert-serial)  

A full developer's guide is provided in the [associated Wiki](https://github.com/digitalyacht/iKonvert/wiki).
