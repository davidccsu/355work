# cp

### Description
`cp` copies the data from one file to another

### Notes
- The complexity of the above pseudo code is linear or `O(n)`
- `buffer`
  * If the buffer is 1 byte:
    * You will have to do `n * bytes` iterations
    * The Kernel will access 1 sector of the hard disk every read call
      the trouble with that is that if you're asking for 1 byte, it will be accessing 1 byte out of the entire sector, each subsequent read call will get the same sector again
  * Typically recommended to use **512 bytes** for the buffer

### Pseudo code
```
open source
open destination

while not end of file @ source
  read source -> buffer
  write buffer -> destination

close source
close destination
  ```
