/*
 * Replicates the cat unix command
 * Skyler Lehan
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Constants */
#define BUFF_SIZE 512
#define true 1
#define false 0
/* Structs */
struct Arguments {
    int nonPrint;
    int lineNumbers;
    int squeeze;
};


/*
 * fgets() returns the line with \n appended to it, this strips the \n
 */
void cleanLine(char *lineBuf) {
    // Remove the \n from the lineBuf
    size_t length = strlen(lineBuf);
    if (lineBuf[length - 1] == '\n' || lineBuf[length - 1] == '\r')
        lineBuf[length - 1] = '\0';
}

/*
 * Count Digits in a number
 */
int countDigits(int num) {
    int count = 1;
    while (num /= 10) {
        count++;
    }
    return count;
}

/*
 * Checks if a specific string is empty
 */
int isEmptyStr(const char *str) {
    // Loop through every character in the string
    while (*str != '\0') {
        // If the character isn't a space, $ or a number, return false
        if (!isspace(*str) && *str != '$' && !isdigit(*str))
            return false;
        str++;
    }
    // Only spaces and $s found, return that it is empty
    return true;
}
/*
 * Creates a line number with a series of spaces
 */
char *createLineNumberStr(char *line, int lineNum, int numLines) {
    int counter = 0;                                                 // Counter for the loop
    char *lineNumStr = (char *)malloc(sizeof(char) * BUFF_SIZE);           // New string to hold the line num + the string
    sprintf(lineNumStr, "%d", lineNum);                              // Allocate a new string
    
    size_t numLen = countDigits(numLines);                           // Get how long the line number should be
    /* Create the spaces to be added after the line number */
    for (counter = 1; strlen(lineNumStr) < numLen+1; counter++) {
        // Add a space at the end
        strcat(lineNumStr, " ");
    }
    
    // Add the line to the end of the line number and return it
    strcat(lineNumStr, line);
    return lineNumStr;
}

/*
 * Puts the line numbered string into the array
 */
void processLineNumber(char *line, int lineNum, int numLines) {
    // Create a line number string
    char *newStr = createLineNumberStr(line, lineNum, numLines);
    // Copy that new string into the array to replace the one currently in that spot
    strcpy(line, newStr);
}

/*
 * Adds the line number plus a series of spaces depending on how
 * many lines there are to an array of lines
 */
void addLineNumbers(char *lines[], int numLines, int squeeze) {
    int index = 0;                                                      // Array indexer
    int lastLineBlank = false;                                         // Flag that is set when the last line is blank
    
    // Loop through each string in the array and add a line number
    for (index = 0; index < numLines-1; index++) {
        // Check if we're squeezing the output
        if (squeeze) {
            // If the string isn't empty, create the line number string as normal and change the string at index
            if (!isEmptyStr(lines[index])) {
                processLineNumber(lines[index], index+1, numLines);
                lastLineBlank = false;
            }
            else { // If the string is empty, check if the last one was as well, if it was, restart the loop
                if (lastLineBlank)
                    continue;
                // Otherwise, create a line number string as usual
                else {
                    processLineNumber(lines[index], index+1, numLines);
                    lastLineBlank = true;
                }
            }
        }
        // If we're not squeezing, don't worry about the above
        else {
            processLineNumber(lines[index], index+1, numLines);
        }
    }
}

/*
 * Adds the $ to the end of the lines in the array
 */
void addDollarSign(char *lines[], int numLines) {
    // Start a counter/index
    int index = 0;
    
    // Loop through each string in the list and append a $ to the end of each one
    for (index = 0; index < numLines - 1; index++) {
        // Clean the line of \n and \r
        cleanLine(lines[index]);
        // Add the $ to the end of each string and insert it back into the array
        lines[index] = strcat(lines[index], "$");
    }
}

/*
 * Prints the lines in the line array
 */
void printLines(char **lines, int numLines, int squeeze) {
    int lastLineBlank = false;                                     // Flag for if the last line was blank
    // if it was, then any blank line after
    // must not be printed
    for (int index = 0; index < numLines-1; index++) {
        if (squeeze)
        {
            // If the string is empty
            if (isEmptyStr(lines[index]))
            {
                // and the last line was blank/empty as well, reiterate
                if (lastLineBlank)
                    continue;
                // Otherwise, print the line like normal
                else 
                    lastLineBlank = true; 
            }
            // If its not empty, print it
            else 
                lastLineBlank = false;

            printf("%s\n", lines[index]);
        }
        // If we're not squeezing, don't worry about the above
        else {
            cleanLine(lines[index]);
            printf("%s\n", lines[index]);
        }
    }
}

void printFile(int file, struct Arguments args) {
    const int fileLength = 1024;                                    // Hold the 1024 lines the file can have
    size_t lineLength = BUFF_SIZE;                                  // Hold the length of each line
    char *lineBuf = (char *)malloc(sizeof(char) * lineLength);      // Buffer to hold the line read in
    char *workingStr = (char *)malloc(sizeof(char) * lineLength);   // Buffer to hold overflow from line processing
    char **lines = (char **)malloc(sizeof(char *) * fileLength);    // Array for all of the lines
    int workingIndex = 0;                                           // Counts the number of lines in the file
    int linesIndex = 0;                                             // Current place in the lines[] array
    int lastCharNewLine = false;                                   // Signifies if the last line was \n
    
    /* Loop through each line in the file and store them in an array */
    while (read(file, lineBuf, sizeof(char))) {
        // Read 1 char at a time
        if (lineBuf[0] != '\n' && lineBuf[0] != '\r') {
            workingStr[workingIndex] = lineBuf[0];
            workingIndex++;
        }
        else if (lastCharNewLine) {
            // If the next char read in is \r, skip this char
            if (lineBuf[0] == '\r') {
                lastCharNewLine = false;
                continue;
            }
            else {
                if (lineBuf[0] == '\n')
                {
                    lastCharNewLine = true;
                    continue;
                }
                else
                {
                    lastCharNewLine = false;
                    continue;
                }
                //workingStr[workingIndex] = lineBuf[0];
                //workingIndex++;
            }
        }
        else {
            // We've got a new line character, save the working str to our lines
            lines[linesIndex] = (char *)malloc(sizeof(char) * lineLength);
            strcpy(lines[linesIndex], workingStr);
            
            // Increment to the next line, set working index back to 0
            linesIndex++;
            workingIndex = 0;
            
            // Reallocate the working string
            memset(workingStr, 0, strlen(workingStr));
            
            // Signify the last char was a new line (therefor if there is a \r next time, we will skip it
            lastCharNewLine = true;
        }
    }
    // Free memory allocated for loop
    free(lineBuf);
    free(workingStr);

    // Check the args for line numbering flag, if true, add the line numbers
    if (args.lineNumbers == true)
        addLineNumbers(lines, linesIndex+1, args.squeeze);
    // Check the args for the non printing character flag, if true, append the $
    if (args.nonPrint == true)
        addDollarSign(lines, linesIndex+1);
    
    // Print the lines after they've been proccessed
    printLines(lines, linesIndex+1, args.squeeze);
    
    // Free anything dynamically allocated
    // Free all of the strings
    for (int index = 0; index < linesIndex-1; index++)
    {
        if (lines[index] != NULL)
            free(lines[index]);
    }
    free(lines);
}

int main(int argc, const char **argv) {
    int fileIndex = 1;
    int numFiles = argc - 1;
    struct Arguments args = {false, false, false};
    int *files = (int *)malloc(numFiles * sizeof(int));
   
    if (argc <= 1) {
        printf("Please supply parameters\n");
        free(files);
        return 1;
    }

    // Check for command arguments (ie; -ens)
    if (strstr(argv[1], "-"))
    {
        // We have command args, so set fileIndex to 2 so that we scan after the arguments
        fileIndex = 2;
        numFiles = argc - 2;
        
        if (strstr(argv[1], "e")) {
            args.nonPrint = true;
        }
        if (strstr(argv[1], "n")) {
            args.lineNumbers = true;
        }
        if (strstr(argv[1], "s")) {
            args.squeeze = true;
        }
    }
    
    for (int index = fileIndex; index < argc; index++) {
        /* Loop through each file and create an array of all of them */
        int newFile = open(argv[index], O_RDONLY);
        files[index - fileIndex] = newFile;
    }
    
    for (int index = 0; index <= numFiles-1; index++) {
        /* Loop through each file in the array and print it */
        printFile(files[index], args);
    }

    // Free files to release memory back
    free(files);
}
