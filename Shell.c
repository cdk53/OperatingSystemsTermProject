

int compareStrings(char*, char*);

void main() {
    char* input;

    while(1) {
        interrupt(33,0,"~(__^> \0",0 ,0);
        interrupt(33, 1, input, 0, 0);
        interrupt(33,0,input,0,0);
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
