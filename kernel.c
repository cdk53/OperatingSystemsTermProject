/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed: Joseph Cochran, Cameron Knaus, Dominic Polochak Date: 02/19/20 */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.03, Fall 2019.               */

void handleInterrupt21(int,int,int,int);
void printLogo();
void runProgram(int,int,int);

void main() {
    char buffer[512];
    makeInterrupt21();
    interrupt(33,2,buffer,258,1);
    interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
    printLogo();
    runProgram(30, 10, 2);
    interrupt(33,0,"Bad or missing comand interpreter.\r\n\0",0,0);
    while (1);
}

void printString(char* c, int d) {
   /* fill this in */
   int index = 0;
   while(c[index] != '\0')
   {
     if(d == 1){
       interrupt(23, c[index], 0,0,0);
     }
     else{
       interrupt(16,14*256+c[index], 0,0,0);
     }
     ++index;
   }
   return;
}

void printLogo() {
   printString("       ___   `._   ____  _            _    _____   ____   _____ \r\n\0",0);
   printString("      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0",0);
   printString("     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0",0);
   printString("    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0",0);
   printString("   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0",0);
   printString("._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0",0);
   printString(" BlackDOS2020 v. 1.03, c. 2019. Based on a project by M. Black. \r\n\0",0);
   printString(" Author(s): Joseph Cochran, Cameron Knaus, Dominic Polochak\r\n\r\n\0",0);
}
/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */
/*
*Takes in char string of at least 80 elements, calling interrupt to print elements
*to string.
*/

/* Provide support for modulus */
int mod(int a, int b) {
    int x = a;
    while (x >= b) x = x - b;
    return x;
}

/* Provide support for integer division */
int div(int a, int b) {
    int q = 0;
    while (q * b <= a) q++;
    return (q - 1);
}

void readString(char* cString) {

    char input;
    int index = 0;
    do {
        input = interrupt(22,0,0,0,0);

        /* If Backspace */
        if(input == 8 && index > 0){
                --index;
                interrupt(16,14*256+input, 0,0,0);
        }
        else {
            /* If Enter */
            if(input == 13){
                interrupt(16,14*256+10, 0,0,0);
                cString[index] = '\0';
            }
            else {
                cString[index] = input;
            }
            interrupt(16,14*256+input, 0,0,0);
            ++index;
        }
    } while(input != 13);

    return;
}

/* Converts n to a string, then outputs to the screen */
void writeInt(int n, int cx) {
    int index = 0;
    int tempIndex = 0;
    char outputString[80];
    char tempString[80];

    /* Exception, n already equals 0 */
    if(n == 0) {
        outputString[0] = '0';
        outputString[1] = '\0';
        interrupt(33,0,outputString,cx,0);
        return;
    }

    /* Account for the sign if negative */
    if(n < 0) {
        outputString[index] = '-';
        n = n * -1;
        ++index;
    }

    /* Fill tempString */
    while(n != 0) {
        tempString[tempIndex] = '0' + mod(n, 10);
        n = div(n, 10);
        ++tempIndex;
    }
    --tempIndex;

    /* Fill output string with results */
    while(tempIndex >= 0) {
        outputString[index] = tempString[tempIndex];
        ++index;
        --tempIndex;
    }

    /* Add null terminator to outputString */
    outputString[index] = '\0';

    /* Run output to screen or printer */
    interrupt(33,0,outputString,cx,0);

    return;
}

/* Reads in a char string and converts the input to an integer and stores it in the given n*/
void readInt(int* n) {
    char inputString[80];
    int index = 0;
    int sign = 1;

    interrupt(33, 1, inputString, 0, 0);

    if(inputString[0] == '-') {
        sign *= -1;
        ++index;
    }

    /* Convert chars to integer digits */
    *n = 0;
    while(inputString[index] != '\0') {
        *n *= 10;
        *n += (int)(inputString[index] - 48);
        ++index;
    }

    *n *= sign;

    return;
}

/* Reads a specified sector from a floppy disk */
void readSector(char* buffer, int sector, int sectorCount) {
    /* Convert to interrupt ax, cx, and dx register values */
    int ax;
    int cx;
    int dx;
    int relativeSector;
    int headNumber;
    int trackNumber;

    /* Convert given absolute sector number to a relative sector number */
    relativeSector = mod(sector, 18) + 1;

    /* Retrieve head number from the given absolute sector number */
    headNumber = mod(div(sector, 18), 2);

    /* Retrieve track number from the given absolute sector number */
    trackNumber = div(sector, 36);

    /* ax's first 512 bits use 2 for read, 768 for write */
    ax = 512 + sectorCount;

    /* Upper 8 bits of cx are track number of starting point, lower 8 bits are
    relative sector number of starting point*/
    cx = (trackNumber * 256) + relativeSector;

    /* Upper 8 bits of dx are the head number starting point and the lower 8 bits
    are the device number for the floppy disk to use (Defaulting to 0 for this project)*/
    dx = headNumber * 256;

    interrupt(19, ax, buffer, cx, dx);

    return;
}

/* Writes to a specified sector from a floppy disk */
void writeSector(char* buffer, int sector, int sectorCount) {
    /* Convert to interrupt ax, cx, and dx register values */
    int ax;
    int cx;
    int dx;
    int relativeSector;
    int headNumber;
    int trackNumber;

    /* Convert given absolute sector number to a relative sector number */
    relativeSector = mod(sector, 18) + 1;

    /* Retrieve head number from the given absolute sector number */
    headNumber = mod(div(sector, 18), 2);

    /* Retrieve track number from the given absolute sector number */
    trackNumber = div(sector, 36);

    /* ax's first 512 bits use 2 for read, 768 for write */
    ax = 768 + sectorCount;

    /* Upper 8 bits of cx are track number of starting point, lower 8 bits are
    relative sector number of starting point*/
    cx = (trackNumber * 256) + relativeSector;

    /* Upper 8 bits of dx are the head number starting point and the lower 8 bits
    are the device number for the floppy disk to use (Defaulting to 0 for this project)*/
    dx = headNumber * 256;

    interrupt(19, ax, buffer, cx, dx);

    return;
}


/* Clears the screen with the given background and foreground colors */
void clearScreen(int background, int foreground) {
  int i = 0;

  /* Clear away the screen with 24 new lines and returns */
  for(i; i < 24; ++i) {
    interrupt(16, 14*256+'\r', 0,0,0);
    interrupt(16, 14*256+'\n', 0,0,0);
  }

  /* reposition the cursor in the upper left-hand corner */
  interrupt(16, 512, 0,0,0);

  /* Change the colors if the given parameters are valid */
  if((background - 1) <= 8 || (foreground - 1) <= 16) {
      /* If the parameters are not the standard */
      if(background > 0 && foreground > 0) {
          interrupt(16, 1536, 4096 * (background - 1) + 256 * (foreground - 1), 0, 6223);
      }
  }

  return;
}

/*
takes as parameters the starting sector of the program you want to run from the
disk, the number of sectors to read and the segment where you want it to run.
Loads and runs the program.
*/
void runProgram(int start, int size, int segment) {

    char localBuffer[13312];
    int baseLocation = 0;
    int maxProgramSize = 13312;
    int i;

    /* Read Sectors */
    interrupt(33, 2, localBuffer, start, size);

    /* Multiply Segment */
    baseLocation = segment * 4096;

    /* Transfer loaded file from the local buffer into the segment location */
    for(i = 0; i < maxProgramSize; ++i) {
        putInMemory(baseLocation, i, localBuffer[i]);
    }

    /* Call launchProgram */
    launchProgram(baseLocation);
}

/* Terminate Program System Call */
void stop() {
    while(1);
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx) {
/*   return; */
    switch(ax) {
       case 0:
            printString(bx, cx);
            break;
        case 1:
            readString(bx);
            break;
        case 2:
            readSector(bx, cx, dx);
            break;
        /*case 3: case 4: */
        case 5:
            stop();
            break;
        case 6:
            writeSector(bx, cx, dx);
            break;
        /* case 7: case 8: case 9: case 10: */
        /*      case 11: */
        case 12:
            clearScreen(bx, cx);
            break;
        case 13:
            writeInt(bx, cx);
            break;
        case 14:
            readInt(bx);
            break;
/*      case 15: */
      default: printString("General BlackDOS error.\r\n\0");
   }
}
