# Software for LED Mini Hanukkah Menorah kits
### Kits and original software by Evil Mad Scientist Laboratories, www.evilmadscientist.com

### Software modified by zanhecht, https://github.com/zanhecht/

* Project details and wiring diagram at https://www.evilmadscientist.com/2006/holiday-electronics-projects-technical-details-and-reference-data/

* Detailed assembly instructions at https://cdn.evilmadscientist.com/KitInstrux/menorahinstrux.pdf

## Programming

Programming the chips can be done either using an Arduino as an ISP programmer or using a USBTinyISP. See https://www.evilmadscientist.com/2007/using-avr-microcontrollers-minimalist-target-boards/ for basic instructions on using the latter, although it is slightly more complicated WinAVR is not supported on modern 64-bit operating systems.

Instead, to use a USBTinyISP, install the following:

* _AVR 8-Bit Toolchain (Windows)_ from https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers
  * Download the first tile in the list, which should start `AVR` and ending with `(Windows)`
  * Unzip the archive and copy the folder inside the archive to `C:\Program Files` (i.e. your files should end up in `C:\Program Files\avr8-gnu-toolchain-win32_x86_64`)
* _GNUWin32 Make_ from https://gnuwin32.sourceforge.net/downlinks/make.php
  * Install in default location (i.e. `C:\Program Files (x86)\GnuWin32`)
* _AVRDUDE for Windows_ from https://github.com/mariusgreuel/avrdude/releases
  * Download the file ending in `windows-x64.zip`
  * Unzip the archive and copy the folder inside the archive to `C:\Program Files` (i.e. your files should end up in `C:\Program Files\avrdude`)
* _Zadig_ from http://zadig.akeo.ie/
  * Download and run the exe file.
  * Plug your USBTiny into your computer, and selec tthe "USBtinySPI" device in the list.
  * Select `libusb-win32` in the box to the right of the green arow.
  * CLick "Install Driver". It may take a while, but you should only have to do this once.
* Optionally add the following locations to your PATH environmental variable in Windows:
    * `C:\Program Files\avr8-gnu-toolchain-win32_x86_64\bin`
    * `C:\Program Files (x86)\GnuWin32\bin`
    * `C:\Program Files\avrdude`
  * If you don't add them to your path, you will need to run the following command in the command prompt before flashing each unless you use the makeinstall.bat file:
    ```
    PATH=%path%C:\Program Files\avr8-gnu-toolchain-win32_x86_64\bin;C:\Program Files (x86)\GnuWin32\bin;C:\Program Files\avrdude;
    ```
    
Check that everything's working by entering the following commands into a command prompt:
* `"C:\Program Files\avr8-gnu-toolchain-win32_x86_64\bin\avr-gcc.exe" --version`
  * You should get a response that starts with `avr-gcc.exe (AVR_8_bit_GNU_Toolchain_`...
  * If not, check your installation of the AVR Toolchain and that it installed in the correct directory
* `"C:\Program Files (x86)\GnuWin32\bin\make.exe" -v`
  * You should get a response that starts with `GNU Make 3.`...
  * If not, check that you unzipped GNUWin32 Make into the correct directory
* `"C:\Program Files\avrdude\avrdude.exe" -c usbtiny -p t2313`
  * If your USBTinyISP is plugged in and the driver were installed correctly with Zadig, you should get a response that starts `avrdude.exe: AVR device initialized and ready to accept instructions`. You will get errors after that about the device signature if you don't have the USBTinyISP hooked up to the chip you're going to program.
  * If you get a message starting with `avrdude.exe: Error: Could not find USBtiny device`, make sure the drivers were installed with Zadig and that the device is connected to USB and the power light, if it has one, is on.
  * If you get an error about it not being a recognized internal or external command, check that you unzipped AVRDUDE for Windows into the correct directory.

You can build the program and flash it using the `makeinstall.bat` file.
