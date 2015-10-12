# ls

### Description
Prints out a list of the files in the current directory

### Flags
* `-a` - prints all files, including hidden (example: .gitingore)
* `-l` - long format with file sizes etc

### Notes
* Files in Unix
  * Hidden Files
    * `.` - is the current directory
    * `..` - is the previous/parent directory
* Directories are essentially files with lists of entries, which each entry is a file
* **File Properties**
  * Full list of File Properties
    1. **File Mode** - permission of the file/directory
      * **0**  \_  **1 2 3**  \_  **4 5 6** _ **7 8 9**
        * in file modes, the first index (0) dictates directory or file (`-` for file `d` for directory)
        * **Group 1 (1,2,3)** - are the user permissions for the currently logged in user
          * `1` - represents if the file is readable, if it is use `r`
          * `2` - represents if the file is writable, if it is, use `w`
          * `3` - represents if the file is executable, if it is, use `x`
          * For example, `rwx` means the file is readable, writable and executable
        * **Group 2 (4,5,6)** - permissions for the group
        * Modify the permissions using `chmod`
    * **Links** 1 or 2
    * **File Owner** - owner of the file
    * **Group** - group the file belongs to
    * **File Size** - number of bytes the file takes up
    * **Time Stamp** - when the file was last updated
    * **Name** - name of the file
  * To get the full list of properties, use [`stat()`](#stat)
* Permissions in Depth
  * The permission set `- rw- r-- r--` will also be defined as `0 110 100 100` which is also `0 6 4 4` in decimal
  * **Bit Fields**
      * `S_IRUSR`
        * `S_I` is the general prefix to the stat constants
        * `R` represents "read" for the file
        * `USR` represents that this is for the user
        * translates to `- r-- --- ---` or `0 100 000 000` or `0 4 0 0`
      * `S_IXGRP`
        * `S_I` is the general prefix to the stat constants for permissions
        * `X` represents execute for the file
        * `GRP` represents group
        * translates to `- --- --x ---` or `0 000 001 000` or `0 0 1 0`
      * If you bitwise `or` them together, you get:
        * `- r-- --x ---`
        * `0 100 001 000`
        * `0 4 1 0`
      * The bitfields look like the following
        * *User* - `S_IaUSR` where a is either (`r`, `w`, or `x`)
        * *Group* - `S_IaGRP` where a is either (`r`, `w`, or `x`)
        * *Other* - `S_IaOTH` where a is either (`r`, `w`, or `x`)
      * To check if a user/group/other has permission, bitwise `and` the user/group/other's permission field with the corresponding bitfield for what you're checking
        * See [`checkPerm()`](#checkperm)
      * To add different permissions together, use bitwise `or` aka `|` in C
      * These can be found [here](https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man2/chmod.2.html).
* Other System Calls
  * [`creat()`](#creat) to create a file with defined permissions
  * [`chmod()`](#chmod) to modify permissions of a file
  * [`cho()`](#cho) to change ownership of a file
  * [`utime()`](#utime) modify timestamp of file


### Pseudo Code
```
open directory
while (not end of directory)
  read directory entry
  display entry information
close directory
```

### Other Code
#### `dirent`
```
struct dirent { /* when _DARWIN_FEATURE_64_BIT_INODE is NOT defined */
        ino_t      d_ino;                /* file number of entry */
        __uint16_t d_reclen;             /* length of this record */
        __uint8_t  d_type;               /* file type, see below */
        __uint8_t  d_namlen;             /* length of string in d_name */
        char    d_name[255 + 1];   /* name must be no longer than this */
};
```

#### <a name="stat"></a>`stat`
```
int stat(char *fname, struct stat *buf)
```

#### <a name="checkperm"></a>Check if user has permission for execution
```
void checkperm()
{
  int u; // mode of the file

  if (u & S_IXUSR)
  {
    printf("User has permission to execute.");
  }
}
```

#### <a name="creat"></a>`creat`
```
int creat(char *fname, int mode);
```

#### <a name="chmod"></a>`chmod`
```
int chmod(const char *path, mode_t mode);
```

#### <a name="chown"></a>`cho`
```
int chown(const char *path, uid_t owner, gid_t group);
```
