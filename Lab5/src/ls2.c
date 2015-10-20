/** ls2.c
 **/
#include    <stdio.h>
#include    <math.h>
#include    <sys/types.h>
#include    <dirent.h>
#include    <stdlib.h>
#include    <sys/ioctl.h>
#include    <string.h>

void do_ls(char []);
struct winsize *get_screen_dimensions();
int compare(const void *str1, const void *str2);
int get_longest_word(char **array, int arrlen);
void addspaces(char *str, int maxstrlen);
void setup_2d_array(int **arr, int numrows, int numcols);

int main(int ac, char *av[])
{
    if (ac == 1)
        do_ls(".");
    else
    {
        while (--ac)
        {
            do_ls(*av);
        }
    }
}

/*
 *	list files in directory called dirname
 */
void do_ls( char dirname[] )
{
    DIR     *dir_ptr;		/* the directory */
    struct  dirent	*direntp;		/* each entry	 */
    char    **dirArray = (char **)malloc(sizeof(char *) * 512);
    int     dirArrayLen = 0;
    
	if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"ls1: cannot open %s\n", dirname);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            if (direntp->d_name[0] == '.')
                continue;
            dirArray[dirArrayLen] = direntp->d_name; 
            dirArrayLen++;
        }
        closedir(dir_ptr);
    }
   
    /* Get the size of the window */
    struct winsize *winset = get_screen_dimensions();
    /* From the winset struct, get the width of the window in columns */
    unsigned short scrnwdth = winset->ws_col;
    /* Get the size of each string */
    int stringLen = sizeof(dirArray) / sizeof(char *);
    /* Sort the array using the qsort() library function */
    qsort(dirArray, stringLen, sizeof(char *), compare);
    /* Find the longest word in the array (as this will be used as reference for printing */
    int longestwordlen = get_longest_word(dirArray, dirArrayLen);

    /* Get the number of columns */
    int numcols = floor(((double)scrnwdth / (longestwordlen + 1)));
    /* Get the number of rows */
    int numrows = ceil(((double)dirArrayLen / numcols));
   
    /* Create 2D Array */
    /* This array will store the indices of each string */
    int **print_dirs;
    /* Allocate memory to the pointers */
    print_dirs = (int **)malloc(numrows * sizeof(*print_dirs));
    for (int index = 0; index < numrows; index++)
        print_dirs[index] = (int *)malloc(numcols * sizeof(*print_dirs[index]));
    /* Fill in their respective values */
    setup_2d_array(print_dirs, numrows, numcols);

    /* Loop through the matrix, printing each directory, correctly spaced and alphabetically */
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcols; col++)
        {
            /* If the index is out of bounds, do not print it */
            if (print_dirs[row][col] > (dirArrayLen - 1))
                continue;
            else
            {
                addspaces(dirArray[print_dirs[row][col]], longestwordlen);
                printf("%s", dirArray[print_dirs[row][col]]);
            }
        }
        /* Print a new line to move to the next line */
        printf("\n");
    }
    /* Free memory from leaks */

    for (int index = 0; index < numrows; index++)
        free(print_dirs[index]);
    free(print_dirs);
    free(dirArray);
    free(winset);
}

/*
 * Sets up the 2D array of indices
 */
void setup_2d_array(int **arr, int numrows, int numcols)
{
    /* When looping, this variable represents each index of dirArray */
    int dirIndex = 0;
    /* Loop through each row/col in the matrix, and add the indices */
    for (int col = 0; col < numcols; col++)
    {
        for (int row = 0; row < numrows; row++)
        {
            arr[row][col] = dirIndex;
            dirIndex++;
        }
    }
}

/*
 * Adds a number of spaces to the end of a string to get it
 * to a certain length
 */
void addspaces(char *str, int maxstrlen)
{
   /* Get the current string length */
   size_t curr_strlen = strlen(str);

   /* Get how many spaces would need to be added */
   int numspcs = maxstrlen - curr_strlen;

   /* Add the spaces */
   for (int index = 0; index <= numspcs; index++)
   {
        strcat(str, " ");
   }
}
/*
 * Compares the strings and returns their result using strcmp
 */
int compare(const void *str1, const void *str2)
{
    const char *istr1 = (const char *)str1;
    const char *istr2 = (const char *)str2;
    return strcmp(istr1, istr2);
}
/*
 * Returns the length of the longest word
 */
int get_longest_word(char **array, int arrlen)
{
    size_t longestlen = 0;
    for (int index = 0; index < arrlen; index++)
    {
        /* Get string length */
        size_t currstrlen = strlen(array[index]);
        if (currstrlen > longestlen)
            longestlen = currstrlen;
    }
    return longestlen;
}

/*
 * Creates a winsize struct with the screen dimensions
 */
struct winsize *get_screen_dimensions()
{
    struct winsize *wbuf = (struct winsize *)malloc(sizeof(struct winsize));
    ioctl(0, TIOCGWINSZ, wbuf);
    return wbuf;
}
