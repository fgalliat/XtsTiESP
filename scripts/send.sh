# BIG 2MB flash PRGM (less data size...)
# based on Arduino IDE 1.8.12 
# Setting : Minimal SPIFFS - (Large Apps w/ OTA)

prgm="XtsTiESP"
outDir="/tmp/arduino_build_294942_xttesp"

homDir="/home/fgalliat"
verbose="-verbose"
verbose=""

partition="min_spiffs"
partition="default"

if [ -e /vm_mnt/opt/devl/Arduino ] ; then
 # WSL INSTALL
 # COM3 windows -> auto mapped w/ WSL
comPort="/dev/ttyS3"
else 
# 'nux
comPort="/dev/ttyUSB0"
fi


# send "Default"
# python /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/gen_esp32part.py -q /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/default.csv $outDir/$prgm.ino.partitions.bin
# send "Minimal SPIFFS - (Large Apps w/ OTA)"
python $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/gen_esp32part.py -q $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/$partition.csv $outDir/$prgm.ino.partitions.bin


python $homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 elf2image --flash_mode dio --flash_freq 80m --flash_size 4MB -o $outDir/$prgm.ino.bin $outDir/$prgm.ino.elf

python $homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 --port $comPort --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/boot_app0.bin 0x1000 $homDir/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/sdk/bin/bootloader_qio_80m.bin 0x10000 $outDir/$prgm.ino.bin 0x8000 $outDir/$prgm.ino.partitions.bin 
