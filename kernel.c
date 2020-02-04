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
/* Signed: Joseph Cochran, Cameron Knaus, Dominic Polochak Date: 01/28/20 */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.03, Fall 2019.               */

void handleInterrupt21(int,int,int,int);
void printLogo();

void main()
{
    char cString[80];
    int n = 0;

    makeInterrupt21();
    printLogo();
    interrupt(33,0,"Hello world from Joseph, Cameron, and Dominic\r\n\0",1,0);

    /* Lab 2 get string and output string */

    interrupt(33,0,"Enter a string\r\n\0",0,0);
    interrupt(33, 1, cString, 0, 0);
    interrupt(33,0,cString,0,0);

    
    /* Test readint and writeint
    interrupt(33, 14, &n, 0, 0);
    interrupt(33, 13, n, 0, 0);
    */
    while(1);
}

void printString(char* c, int d)
{
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

void clearScreen()
{
  int i = 0;
  for(i; i < 24; ++i)
  {
    interrupt(16, 14*256+'\r', 0,0,0);
    interrupt(16, 14*256+'\n', 0,0,0);
  }
  return;
}

void printLogo()
{
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
void readString(char* cString){

    char input;
    int index = 0;
    do{
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
}

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
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
/*   return; */
    switch(ax) {
       case 0:
            printString(bx, cx);
            break;
        case 1:
            readString(bx);
            break;
/*      case 2: case 3: case 4: case 5: */
/*      case 6: case 7: case 8: case 9: case 10: */
/*      case 11: case 12: */
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
