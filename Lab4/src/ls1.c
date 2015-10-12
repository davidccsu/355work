/*
 * ls1 imitates the ls command built into unix
 * Author: Skyler Lehan
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

/* Structs */
typedef struct Flags {
    int all;
} Flags;

void do_ls(char dirname[], Flags flags) {
    DIR           *dir_ptr;               // The directory itself
    struct dirent *direntp;               // Each entry in the directory                 

    /* Attempt to open the path/directory */
    if ((dir_ptr = opendir(dirname)) == NULL)
        // If opening fails, display error message and close program
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    /* If it is successful, then list files in directory */
    else
    {
        /* Read the directory entry until the end */
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            /* Get the directory name from the entry */
            char *d_name = direntp->d_name;

            /* If we're not printing all files and the filename doesn't start with ., skip it */
            if (!flags.all && d_name[0] == '.')
                continue;

            /* Print the file/directory */
            printf("%s\n", direntp->d_name);
        }
        /* Close the directory */
        closedir(dir_ptr);
    }
}
int main(int argc, char **argv) {
    Flags flags = {0};
    
    // Check to see if we have no arguments, if so run ls1 
    // on the current directory
    if (argc == 1)
    {
        do_ls(".", flags);
        return 1;
    }
    // Otherwise, if we have more than one argument, check to
    // see if it is a flag we need
    else
    {
        if (strstr(argv[1], "a"))
            flags.all = 1;
    }

    // Now that the flags are set, if we have just a flag and 
    // no directory, run ls1 on the current directory
    if (argc == 2 && flags.all)
    {
        do_ls(".", flags);
        return 1;
    }
    else if (argc == 2)
    {
        do_ls(argv[1], flags);
        return 1;
    }
    // Otherwise, run it with the path and flags
    else if (argc == 3)
    {
        do_ls(argv[2], flags);
        return 1;
    }
}
