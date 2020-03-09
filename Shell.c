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

        // Find the first space after the command
        while(input[indexOfSpace] != ' ' && input[indexOfSpace] != '\0'){
            ++indexOfSpace;
        }

        handleCommand(input, indexOfSpace);
        indexOfSpace = 0;
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
    char arg1[512];
    char arg2[512];
    int length = 0;

    // General purpose index value
    int index = 0;

    // Hold the index positions of spaces
    int indexSpaceOne = -1;
    int indexSpaceTwo = -1;

    // Count the number of spaces parsed in a command to check for validity
    int spaceCount = 0;

    // Used as a boolean value
    int validBit = 1;

    while(input[length] != '\0'){
        ++length;
    }

    //Get the command.
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
        // Find the spaces of the copy command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else if (spaceCount == 1) {
                    indexSpaceTwo = index;
                    ++spaceCount;
                }
                else {
                    // More than two spaces have been counted, this is an error
                    interrupt(33,0, "Error in copy command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If only one space was found between arguments, format is invalid
        if(indexSpaceTwo < 0) {
            interrupt(33,0, "Error in copy command format",0,0);
            validBit = 0;
        }

        // Using the two found space indexes, create substrings of the filenames
        // If a valid command was given we can make substrings
        if(validBit) {
            // arg1 will hold the first file name
            // *Note we don't want to include spaces in the substrings, thus +-1
            subString(input, arg1, indexSpaceOne + 1, indexSpaceTwo - 1);

            // Note that the previously used index value now holds the null terminator location
            subString(input, arg2, indexSpaceTwo + 1, index - 1);

            interrupt(33,0, "Copy command has been run with the arguments: ",0,0);
            interrupt(33,0, arg1,0,0);
            interrupt(33,0, " ",0,0);
            interrupt(33,0, arg2,0,0);
            interrupt(33,0, "\n",0,0);
        }
    }
    else if(compareStrings(command, "ddir") == 1){
        if(input[4] == '\0') {
            interrupt(33,0, "ddir command has been run with no arguments",0,0);
        } else {
            interrupt(33,0, "Error in ddir command format",0,0);
        }
    }
    else if(compareStrings(command, "exec") == 1){
        // Find the first space in the command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else {
                    // More than one space has been counted, this is an error
                    interrupt(33,0, "Error in exec command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If we have valid input, get a substring of the filename
        if(validBit) {
            subString(input, arg1, indexSpaceOne + 1, index - 1);
            interrupt(33,0, "exec command has been run with the argument: ",0,0);
            interrupt(33,0, arg1,0,0);
        }
    }
    else if(compareStrings(command, "help") == 1){
        if(input[4] == '\0') {
            interrupt(33,0, "help command has been run with no arguments",0,0);
        } else {
            interrupt(33,0, "Error in help command format",0,0);
        }
    }
    else if(compareStrings(command, "prnt") == 1){
        // Find the first space in the command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else {
                    // More than one space has been counted, this is an error
                    interrupt(33,0, "Error in prnt command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If we have valid input, get a substring of the filename
        if(validBit) {
            subString(input, arg1, indexSpaceOne + 1, index - 1);
            interrupt(33,0, "prnt command has been run with the argument: ",0,0);
            interrupt(33,0, arg1,0,0);
        }
    }
    else if(compareStrings(command, "remv") == 1){
        // Find the first space in the command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else {
                    // More than one space has been counted, this is an error
                    interrupt(33,0, "Error in remv command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If we have valid input, get a substring of the filename
        if(validBit) {
            subString(input, arg1, indexSpaceOne + 1, index - 1);
            interrupt(33,0, "remv command has been run with the argument: ",0,0);
            interrupt(33,0, arg1,0,0);
        }
    }
    else if(compareStrings(command, "senv") == 1){
        if(input[4] == '\0') {
            interrupt(33,0, "senv command has been run with no arguments",0,0);
        } else {
            interrupt(33,0, "Error in senv command format",0,0);
        }
    }
    else if(compareStrings(command, "show") == 1){
        // Find the first space in the command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else {
                    // More than one space has been counted, this is an error
                    interrupt(33,0, "Error in show command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If we have valid input, get a substring of the filename
        if(validBit) {
            subString(input, arg1, indexSpaceOne + 1, index - 1);
            interrupt(33,0, "show command has been run with the argument: ",0,0);
            interrupt(33,0, arg1,0,0);
        }
    }
    else if(compareStrings(command, "twet") == 1){
        // Find the first space in the command
        index = 0;
        validBit = 1;
        while(input[index] != '\0' && validBit){
            // if a space is found
            if(input[index] == ' ') {
                if(spaceCount == 0) {
                    indexSpaceOne = index;
                    ++spaceCount;
                } else {
                    // More than one space has been counted, this is an error
                    interrupt(33,0, "twet in prnt command format",0,0);
                    validBit = 0;
                }
            }
            ++index;
        }

        // If we have valid input, get a substring of the filename
        if(validBit) {
            subString(input, arg1, indexSpaceOne + 1, index - 1);
            interrupt(33,0, "twet command has been run with the argument: ",0,0);
            interrupt(33,0, arg1,0,0);
        }
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

// Note that the substring will include the characters found at startpos and endpos
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
