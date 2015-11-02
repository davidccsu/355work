## Connection Control

* Files vs Devices
  * **Devices**
  * found in /dev
    * The files in this directory correspond to devices on the OS
    * Each file is a driver to a hardware component
    * Terminals (`tty`) are often found here
      * Displays and keyboards will use these
  * All device drivers are files
    * They can use the following system calls
      * `open`
        * returns a file descriptor
      * `close`
      * `read`
      * `write`
      * `stat`
      * `lseek`
      * For example, a printer will open the printer driver in `/dev` and write to it, but cannot read from it.
