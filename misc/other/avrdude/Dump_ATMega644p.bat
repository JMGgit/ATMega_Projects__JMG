avrdude -p m644p -c stk500v2 -P COM3 -b 115200 -U flash:r:flash_r.bin:r
avrdude -p m644p -c stk500v2 -P COM3 -b 115200 -U flash:r:flash_i.hex:i
avrdude -p m644p -c stk500v2 -P COM3 -b 115200 -U eeprom:r:eeprom_r.bin:r
avrdude -p m644p -c stk500v2 -P COM3 -b 115200 -U eeprom:r:eeprom_i.hex:i