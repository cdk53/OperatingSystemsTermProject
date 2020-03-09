#include "blackdos.h"

int compareStrings(char*, char*);
void subString(char*, char*, int, int);
void handleCommand(char*, int);
void clearBuffer(char*);

void main() {
    char input[512];
    int length = 0;
    int indexOfSpace = 0;
    while(1) {
        clearBuffer(input);
        interrupt(33,0, "\n\r~(__^>",0,0);
        interrupt(33,1,input,0,0);

        // Find the first space bootafter the command
        while(input[indexOfSpace] != ' ' && input[indexOfSpace] != '\0'){
            ++indexOfSpace;
        }

        handleCommand(input, indexOfSpace);
        indexOfSpace = 0;
        //interrupt(33,0,"\n\r",0,0);
    }
}

//Pull everything one back to accomodate for an extra space.
void handleSpaces(char* input, int index){
    int tempIndex = index;
    int tempFutureIndex = index + 1;

    //Loop through, pulling each char back by one position.
    while(input[tempFutureIndex] != '\0'){
        input[tempIndex] = input[tempFutureIndex];
        ++tempIndex;
        ++tempFutureIndex;
    }
    input[tempIndex] = input[tempFutureIndex];
}

//Handles the echo string, potentially calling handleSpaces().
void handleEcho(char* input, int index){
    int numOfSpaces = 0;
    int spaceIndex;

    //Parse input, checking for tabs or spaces.
    while(input[index] != '\0'){
        if(input[index] == ' ' || input[index] == '\t')
        {
            ++numOfSpaces;
            if(numOfSpaces == 2){
                //More than one space or tab
                handleSpaces(input,index);

                --numOfSpaces;
                --index;
                spaceIndex = index;
            }
        }
        else{
            //Make sure num Of Spaces does not go negative.
            if(numOfSpaces > 0) {
                --numOfSpaces;
            }
        }

        ++index;
    }
}

//Clear buffer
void clearBuffer(char* buffer){
    int index = 0;
    while(buffer[index] != '\0'){

        buffer[index] = ' ';
        ++index;
    }
}

//Handle Commands from Shell
void handleCommand(char* input, int firstSpace){
    char command[512];
    char output[512];
    int indexForOutput = 0;
    int indexForEcho = firstSpace + 1;
    int lenOfOutput = 0;

    subString(input, command, 0, firstSpace-1);

    //Compare commands, executing accordingly.
    if(compareStrings(command, "boot") == 1){
        interrupt(25,0,0,0,0);
    }
    else if(compareStrings(command, "clrs") == 1){
        interrupt(33,12);
    }
    else if(compareStrings(command, "echo") == 1){
        handleEcho(input, firstSpace);

        //Get new output without command.
        while(input[indexForEcho] != '\0'){
            output[indexForOutput] = input[indexForEcho];
            ++indexForEcho;
            ++indexForOutput;
        }
        //print then clear for future echos.
        interrupt(33,0,output,0,0);
        clearBuffer(output);
    }
    else if(compareStrings(command, "copy") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "ddir") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "exec") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "help") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "prnt") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "remv") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "senv") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "show") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else if(compareStrings(command, "twet") == 1){
        interrupt(33,0, "Input line has been parsed",0,0);
    }
    else{
        interrupt(33,0, "Bad command or file name",0,0);
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
