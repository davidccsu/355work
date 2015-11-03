/** myfind.c
 *
 * Purpose: find the file path to a specified file
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

/* Linked List for directories */
struct dirpath
{
    struct dirent dir;
    struct stat dir_info;
    struct dirpath *prev;
    struct dirpath *next;
};

void find(char *, char *);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int argc, char **argv)
{
    find("/Users/skylerlehan", argv[1]);
}

/* finds a file in a directory */
void find (char *filepath, char *name)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    
    if ((dir_ptr = opendir(filepath)) == NULL)
    {
        fprintf(stderr, "Cannot open %s\n", filepath);
        return;
    }
    else
    {
       /* Traverse directory */
        struct stat info;
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            if (stat(direntp->d_name, &info))
            {
                if (S_ISDIR(info.st_mode))
                    find(direntp->d_name, name);
            }
        }
    }
}

/* Gets info on a file */
void dostat(char *filename)
{
    /* Setup a struct to get the file info */
    struct stat info;

    /* Check if stat is possible to use/if it works */
    if (stat(filename, &info) == -1)
        perror(filename);
    else
        show_file_info(filename, &info);
}

/* Shows the information of the given filename/stat info */
void show_file_info(char *filename, struct stat *info_p)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];

    /* Convert the filemode to letters (rwx etc) */
    mode_to_letters(info_p->st_mode, modestr);

    /* Print the mode */
    printf("%s", modestr);
    /* Print how many hard links exist */
    printf("%4d ", (int)info_p->st_nlink);
    /* Print user ID of the file */
    printf("%-8s ", uid_to_name(info_p->st_uid));
    /* Print group ID of the file */
    printf("%-8s ", gid_to_name(info_p->st_gid));
    /* Print size of the file */
    printf("%8ld ", (long)info_p->st_size);
    /* Print last time file was modified */
    printf("%.12s ", 4+ctime(&info_p->st_mtimespec));
    /* Print file name */
    printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
    /* Set default aka no permissions */
    strcpy(str, "----------");

    /* Check if it is a dir */
    if (S_ISDIR(mode))
        str[0] = 'd';
    /* char devices */
    if (S_ISCHR(mode))
        str[0] = 'c';
    /* block device */
    if (S_ISBLK(mode))
        str[0] = 'b';

    /* Convert user permissions */
    if (mode & S_IRUSR)
        str[1] = 'r';
    if (mode & S_IWUSR)
        str[2] = 'w';
    if (mode & S_IXUSR)
        str[3] = 'x';

    /* Convert group permissions */
    if (mode & S_IRGRP)
        str[4] = 'r';
    if (mode & S_IWGRP)
        str[5] = 'w';
    if (mode & S_IXGRP)
        str[6] = 'x';

    /* Convert other permissions */
    if (mode & S_IROTH)
        str[7] = 'r';
    if (mode & S_IWOTH)
        str[8] = 'w';
    if (mode & S_IXOTH)
        str[9] = 'x';
}

/* Converts the UID to a username */
char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

/* Returns a pointer to a group name given a GID */
char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}
