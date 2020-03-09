 LAB 04
 Authors:
 Joseph Cochran - jmc361
 Cameron Knaus  - cdk53
 Dominic Polochak - dp121

 Changes to BlackDOS:
 - Created a script to create a blank disk image, add the bootloader to sector zero,
   and put the kernal into sector 259.
 - Added the printString functionality to the interrupt vector table, allowing
   for printing to the screen or printing to a printer.
 Changes for Lab 3
 - Added a new color scheme to BlackDOS (Red foreground, gray background).
 - Added the ability to read and write from a floppy disk
 Changes for Lab 4
 - Added ability to load and run programs
 - For Lab 4, currently runs the fib program
 Changes for Lab 5
 - In the main of shell, wthin an infinite loop, a mouse is printed
   to the screen using interrupt 33 and the shell prompts for input.
 - The input redirects to a handleCommand function to parse the input,
   searching for commands and their following characters.
 - A string compare and and a substring function were made to find the command,
   and compare it to the actual commands to check if there was a hit.
 - If the command was boot, interrupt(25,0,0,0,0) was called. If clrs, interrupt(33,12) was called.
 - If echo was called, the shell would print the text after the echo using interrupt 33.
 - Three functions, handleEcho, handleSpaces, and clearBuffer were created because if
   there was more than one space/tab, we were to shrink it to one space.



Running The Lab:
 Compile and set up using the following command:
    chmod +x compileOS.sh

 Run the script using:
    ./compileOS.sh

 Test BlackDOS using Bochs using the following command:
    echo "c" | bochs -f bdos.txt
