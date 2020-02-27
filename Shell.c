#include "blackdos.h"

int compareStrings(char*, char*);
void subString(char*, char*, int, int);

void main() {
    char input[100];
    char command[100];
    int indexOfSpace;
    while(1) {
        PRINTS("~(__^> \0");
        SCANS(input);
        PRINTS(input);

        // Find the first space after the command
        indexOfSpace = 0;
        while(input[indexOfSpace] != ' ' && input[indexOfSpace] != '\0') {
            ++indexOfSpace;
        }
    }
}


// Returns true if both strings are equal
int compareStrings(char* stringOne, char* stringTwo) {
    int index = 0;

    while(stringOne[index] != '\0' && stringTwo[index] != '\0') {
        if(stringOne[index] != stringTwo[index]) {
            return 0;
        }
        ++index;
    }
    if(stringOne[index] == '\0' && stringTwo[index] == '\0') {
        return 1;
    } else {
        return 0;
    }
}

void subString(char* givenString, char* theSubString, int startPos, int endPos) {
    int index = startPos;
    int subStringIndex = 0;

    if(startPos < 0) {
        interrupt(33,0,"ERROR: Substring out of bounds\r\n\0",0,0);
        return;
    }

    while(index <= endPos) {
        if(givenString[index] != '\0')
        {
            theSubString[subStringIndex] = givenString[index];
            ++index;
            ++subStringIndex;
        } else {
            interrupt(33,0,"ERROR: Substring out of bounds\r\n\0",0,0);
            return;
        }
    }
    theSubString[subStringIndex] = '\0';

}
