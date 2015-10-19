# who
---

#### Description
`who` displays a list of users that are currently logged into the system.

## Notes
- How does it do it?
  * `man who` will tell you what it does
  * `utmp` - a structure that the kernel maintains about the users who are
  currently logged in (`utmpx` in OS X)
    * [`umtp`](#utmp)

- For Project:
  1. Open `utmp` file
  2. Loop through each line in the file
     * read record
     * display record
  3. Close `utmp` file

- Opening files
  * `fopen FILENAME` - C system call *wrapper* to open a file, but will not be used
  * `int fd = open(char *name, int how)`
    * Used to open a file
    * If `open()` returns 0, the file was unable to be opened, otherwise the index of the file
    * **Parameters**
      * The kernel has many files open currently, each file has an index `open()`
      returns that index
      * `fd` - "file descriptor" or index to the file in the Kernel's array of
      open files
      * `name` - path to the file
      * `how` - mode of which the file will be opened (read, write, append)
  * `size_t numRead = read(int fd, void *buf, size_t qty)`
    * function used to actually read files
    * **Parameters**
      * `fd` - index of the file out of all open files in the Kernel
      * `*buf` - pointer to buffer the data will be stored in
      * `qty` - how many bytes you want to read from the file
  * `close(int fd)`
    * Closes the file specified by `fd`
    * **Parameters**
      * `fd` - index of the file to be read
---

## Code Examples
#### <a name="utmp"></a>`utmp` or `utmpx` struct
```
 struct utmp {
   char ut_line[...];
   char ut_name[...];
   char ut_host[...];
   time_t ut_time;
 }

 #define UTMP_FILE /path/to/utmp_file    // Most Linux systems
 #define _PATH_UTMP /path/to/utmp_file   // OS X
 ```

#### <a name="stdio"></a>stdio
```
#define O_RDONLY int_that_represents_read_only
```

#### <a name="skeleton"></a>`who` skeleton
```
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>

int main(int argc, const char *argv[]) {
  // Open file for read only access
  int utmpFile = open(UTMP_FILE, O_RDONLY);

  // Check the file and if it is 0, display an error message
  if (utmpFile == 0) {
    printf("error message");
    return 1;
  } else {
    // Create utmp struct to hold the record and create int to hold the length
    struct utmp record;
    int recLen = sizeof(record);

    // Loop through each record in the utmp/utmpx file
    while (read(utmpFile, &record, recLen) == recLen) {
      // Display record somehow
      printf("...");
    }
  }

}
```
