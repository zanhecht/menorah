@PATH=%path%C:\Program Files\avr8-gnu-toolchain-win32_x86_64\bin;C:\Program Files (x86)\GnuWin32\bin;C:\Program Files\avrdude;
make -f Makefile
make -f Makefile install
del *.bin *.elf *.hex *.lst *.map *.o *.srec
@PAUSE
