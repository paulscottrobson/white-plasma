rm plasma
sh build.sh
cd ../compiler && python2 cfc.py 
cd ../emulator && ./plasma  ../compiler/vmboot.bin 
