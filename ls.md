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
