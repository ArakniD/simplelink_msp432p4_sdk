=========================== Segger RTT Debug for CAT II =========================

The files within this folder build a real-time debug and console interface
which is activated only once a reader connects (Segger RTT or SysView), handy
for pulling logs and messages from running code with minimal impact to the 
running system.

The file SYSVIEW_FreeRTOS.txt needs to be copied to the folder:

 C:\Program Files (x86)\SEGGER\SystemView_V252d\Description\SYSVIEW_FreeRTOS.txt
 
 Note: Modify the version number file path to point to the correct SysView version
 
==== Adding new message codes

New mesage codes go in the ..._mat.h file and are generally offset by 1024 for user
defined numbers. The calls to SEGGER_SYSVIEW_Error ( <free text> ) do not require an ID
where calls to SEGGER_SYSVIEW_RecordU32( <event number>, <uint32_t> ) do.

==== Special address 

The RTT buffer is read by specifying (or auto searching) a control context block located
at address 0x2002D800 . This address is specified in the linker file as a section called
SEGGER_RTT and it contains the console and RTT buffers seperate to heap or other areas in
case of a buffer overflow or alike.

==== Helpful command;

"C:\Program Files (x86)\SEGGER\SystemView_V252d\SystemView.exe" 
  -rttcbaddr 0x2002D800 -usb -device MSP432P4011 -speed 6000 -if SWD -single -start
  
  If one specifies the speed, interface and RTTCBaddress then sysview can startup and just
  run. without having to enter special details. I havn;t found the auto find feature to work
  or telling the Jlink plugin to send a "monitor jlink_command setrttcbaddr 0x2002D800" actually
  set the address as documented.
  
==== Debugger Seting

The plugin for GDB SEGGER J-Link Debugger must be installed for this to work. 
Using the NXP FreeRTOS Aware plugin is also advisable. Copy available in ZIP file 

Important settings are;

SVD Path -> 
    SVD File = "C:\Sandbox\Hardware\Dev\MAT\TexasInstruments.MSP432P4xx_DFP.3.2.5\SVD\MSP432P4011.svd"
         Alternative = "${workspace_loc}\..\TexasInstruments.MSP432P4xx_DFP.3.2.5\SVD\MSP432P4011.svd"

Debugger -> 
   Other options = "-singlerun -strict -timeout 0 -nogui -rtos GDBServer/RTOSPlugin_FreeRTOS -nohalt"
   Device name = "MSP432P4011"
   Executable path = "C:\Program Files (x86)\SEGGER\JLink_V644\${jlink_gdbserver}"\
Startup -> 
   RAM application (reload after each reset/restart) = Checked
   Enable SWO Cpu Freq = 24000000
   SWO Freq = 6000000
   Port mask = 0xFFFF
	 