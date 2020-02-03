 LAB 01
 Authors:
 Joseph Cochran - jmc361
 Cameron Knaus  - cdk53
 Dominic Polochak - dp121

 Changes to BlackDOS:
 - Created a script to create a blank disk image, add the bootloader to sector zero,
   and put the kernal into sector 259.
-  Added the printString functionality to the interrupt vector table, allowing
   for printing to the screen or printing to a printer.


Running The Lab:
 Compile and set up using the following command:
    chmod +x compileOS.sh

 Run the script using:
    ./compileOS.sh

 Test BlackDOS using Bochs using the following command:
    echo "c" | bochs -f bdos.txt
