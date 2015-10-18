/** ls1.c
 **   purpose  list contents of directory or directories
 **   action   if no args, use .  else list files in args
 **/
#include	<stdio.h>
#include    <math.h>
#include	<sys/types.h>
#include	<dirent.h>
#include    <stdlib.h>
#include    <sys/ioctl.h>
#include    <string.h>

void do_ls(char []);
struct winsize get_screen_dimensions();
int compare(const void *str1, const void *str2);
int get_longest_word(char **array, int arrlen);

int main(int ac, char *av[])
{
    struct winsize win = get_screen_dimensions();
    printf("Screen Width: %d", win.ws_col);
    do_ls(".");
//	if (ac == 1)
//		do_ls(".");
//	else
//		while (--ac)
//        {
//			printf("%s:\n", *++av);
//			do_ls(*av);
//		}
}

/*
 *	list files in directory called dirname
 */
void do_ls( char dirname[] )
{
	DIR		*dir_ptr;		/* the directory */
	struct  dirent	*direntp;		/* each entry	 */
    char    **dirArray = (char **)malloc(sizeof(char) * 512);
    int     dirArrayLen = 0;
    
	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr,"ls1: cannot open %s\n", dirname);
	else
	{
		while ((direntp = readdir(dir_ptr)) != NULL)
        {
            dirArray[dirArrayLen] = direntp->d_name; 
            dirArrayLen++;
			//printf("%s\n", direntp->d_name);
        }
		closedir(dir_ptr);
	}
    
    struct winsize winset = get_screen_dimensions();
    unsigned short scrnwdth = winset.ws_col;
    qsort(dirArray, dirArrayLen, sizeof(char *), compare);
    int longestwordlen = get_longest_word(dirArray, dirArrayLen);

    // Get the number of columns
    int numcols = ceil(scrnwdth / (longestwordlen + 5));
    // Get the number of rows
    int numrows = ceil(dirArrayLen / numcols);
    
    // Create 2D Array
    int dirIndex = 0;
    char *print_dirs[numcols][numrows];

    for (int col = 0; col < numcols; col++)
    {
        for (int row = 0; row < numrows; row++)
        {
            print_dirs[row][col] = dirArray[dirIndex];
            dirIndex++;
        }
    }
    for (int col = 0; col < numcols; col++)
    {
        for (int row = 0; row < numrows; row++)
        {
            printf("Col: %d Row: %d Value: %s", col, row, print_dirs[row][col]);
        }
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
        // Get string length
        size_t currstrlen = strlen(array[index]);
        if (currstrlen > longestlen)
            longestlen = currstrlen;
    }
    return longestlen;
}

/*
 * Creates a winsize struct with the screen dimensions
 */
struct winsize get_screen_dimensions()
{
    struct winsize wbuf;

    if (ioctl(0, TIOCGWINSZ, &wbuf) != -1)
    {
        return wbuf;
    }
    return wbuf;
}
