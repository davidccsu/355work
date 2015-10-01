/*
 * Replicates the cat unix command
 * Skyler Lehan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * fgets() returns the line with \n appended to it, this strips the \n
 */
void cleanLine(char *lineBuf) {
    // Remove the \n from the lineBuf
    size_t length = strlen(lineBuf);
    if (lineBuf[length - 1] == '\n') 
        lineBuf[length - 1] = '\0';
}

/*
 * Creates a line number with a series of spaces
 */
char *createLineNumberStr(char *line, int lineNum, int numSpaces) {
    size_t counter = 0;
    char *lineNumStr = (char *)malloc(sizeof(char) * 100); 
    sprintf(lineNumStr, "%d", lineNum);
    
    size_t numLen = strlen(lineNumStr);
    /* Create the spaces to be added after the line number */
    for (counter = 0; counter < (numSpaces - numLen); counter++) {
        strcat(lineNumStr, " ");
    }
    strcat(lineNumStr, line);
    return lineNumStr;
}

/*
 * Adds the line number plus a series of spaces depending on how
 * many lines there are to an array of lines
 */
void addLineNumbers(char **lines, int numLines) {
    int index = 0;
    for (index = 0; index < numLines-1; index++) {
        char *newStr = createLineNumberStr(lines[index], index+1, numLines); 
        lines[index] = newStr;
    }
}

void printFile(FILE *file) {
    char *lineBuf = (char *)malloc(sizeof(char) * 128);
    char **lines = (char **)malloc(sizeof(char) * 1024);
    int lineNum = 1;
    //int lineCounter = 0;

    /* Loop through each line in the file and store them in an array */
    while (fgets(lineBuf, 128, file)) {
        /* Print line */
        cleanLine(lineBuf);
        lines[lineNum - 1] = lineBuf;
        //printf("%d     %s$\n", lineNum, lineBuf);
        printf("%d   %s\n", lineNum, lines[lineNum-1]);
        lineNum++;
    }

    /* Add line numbers */
   /* for (lineCounter = 0; lineCounter <= lineNum; lineCounter++) {
        addLineNumbers*/
    addLineNumbers(lines, lineNum);

    for (int count = 0; count < lineNum-1; count++) {
        printf("%s\n", lines[count]);
    }
}

int main(int argc, const char **argv) {
    int fileIndex = 0;
    int numFiles = argc - 1; 
    FILE **files = (FILE **)malloc(numFiles * sizeof(FILE *));

    for (fileIndex = 1; fileIndex <= numFiles; fileIndex++) {
        /* Loop through each file and create an array of all of them */
        FILE *newFile = fopen(argv[fileIndex], "r");
        files[fileIndex-1] = newFile;
    }

    printFile(files[0]);
}
