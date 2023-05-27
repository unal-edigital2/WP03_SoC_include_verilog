#!/bin/bash

./load.py
echo 'Load done'
echo '================================'
cd firmware
make -C firmware clean all
echo '================================'
echo 'pulse el boton reset del procesador para cargar el firmware'
echo '================================'
cd .. 
litex_term /dev/ttyUSB2 --kernel firmware/firmware.bin
