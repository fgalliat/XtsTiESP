# BIG 2MB flash PRGM (less data size...)
# based on Arduino IDE 1.8.12 
# Setting : Minimal SPIFFS - (Large Apps w/ OTA)

prgm="tiEsp"
outDir="/tmp/arduino_build_294943_tiesp"

homDir="/home/fgalliat"
verbose="-verbose"
verbose=""

# "Minimal SPIFFS - (Large Apps w/ OTA)"
partition="min_spiffs"
# "Default"
partition="default"

# M5 Stack
booter="bootloader_qio_80m.bin"
# WEMOS D1 MINI - w/ custom UART1(9,16) - else boot crash loop
booter="bootloader_dio_80m.bin"

if [ -e /vm_mnt/opt/devl/Arduino ] ; then
 # WSL INSTALL
 # COM3 windows -> auto mapped w/ WSL
comPort="/dev/ttyS3"
else 
# 'nux
comPort="/dev/ttyUSB0"
fi



python $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/gen_esp32part.py -q $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/$partition.csv $outDir/$prgm.ino.partitions.bin
python $homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 elf2image --flash_mode dio --flash_freq 80m --flash_size 4MB -o $outDir/$prgm.ino.bin $outDir/$prgm.ino.elf
python $homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 --port $comPort --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/boot_app0.bin 0x1000 $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/sdk/bin/$booter 0x10000 $outDir/$prgm.ino.bin 0x8000 $outDir/$prgm.ino.partitions.bin 

# even if always ...
# --flash_mode dio

#bootloader_qio_80m.bin -> can't start custom UART1 (16,9) -- M5Stack config
#bootloader_dio_80m.bin -> OK

# python /home/fgalliat/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/boot_app0.bin 0x1000 /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/sdk/bin/bootloader_dio_80m.bin 0x10000 $outDir/tiEsp.ino.bin 0x8000 $outDir/tiEsp.ino.partitions.bin 