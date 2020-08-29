# BIG 2MB flash PRGM (less data size...)
# based on Arduino IDE 1.8.12 
# Setting : Minimal SPIFFS - (Large Apps w/ OTA)

prgm="XtsTiESP"
cahDir="/tmp/arduino_cache_294942_xttesp"
outDir="/tmp/arduino_build_294942_xttesp"

partition="min_spiffs"
partition="default"

# M5
FQBN="-fqbn=esp32:esp32:m5stack-core-esp32:FlashMode=qio,FlashFreq=80,PartitionScheme=$partition,UploadSpeed=921600,DebugLevel=none"
# WEMOS D1 MINI ESP32
FQBN="-fqbn=esp32:esp32:d1_mini32:FlashFreq=80,PartitionScheme=$partition,CPUFreq=240,UploadSpeed=921600 -vid-pid=10C4_EA60 "

if [ -e /vm_mnt/opt/devl/Arduino ] ; then
 # WSL INSTALL
 ardDir="/vm_mnt/opt/devl/Arduino/arduino-1.8.12/";
else
 ardDir="/vm_mnt/opt/Arduino/arduino-1.8.12";
fi
homDir="/home/fgalliat"
verbose="-verbose"
verbose=""



mkdir $outDir 2>/dev/null

#$ardDir/arduino-builder -dump-prefs -logger=machine -hardware $ardDir/hardware -hardware $homDir/.arduino15/packages -tools $ardDir/tools-builder -tools $ardDir/hardware/tools/avr -tools $homDir/.arduino15/packages -built-in-libraries $ardDir/libraries -libraries $homDir/Arduino/libraries -fqbn=esp32:esp32:m5stack-core-esp32:FlashMode=qio,FlashFreq=80,PartitionScheme=min_spiffs,UploadSpeed=921600,DebugLevel=none -ide-version=10812 -build-path $outDir -warnings=none -build-cache $cahPath -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.mkspiffs.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.mkspiffs-0.2.3.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.esptool_py.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 -prefs=runtime.tools.esptool_py-2.6.1.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 -prefs=runtime.tools.xtensa-esp32-elf-gcc.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 -prefs=runtime.tools.xtensa-esp32-elf-gcc-1.22.0-80-g6c4433a-5.2.0.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 $verbose /vm_mnt/devl/Arduino/M5/M5XtsOs/M5XtsOs.ino

# "Default partition scheme" -- 1MB for App
# $ardDir/arduino-builder -compile -logger=machine -hardware $ardDir/hardware -hardware $homDir/.arduino15/packages -tools $ardDir/tools-builder -tools $ardDir/hardware/tools/avr -tools $homDir/.arduino15/packages -built-in-libraries $ardDir/libraries -libraries $homDir/Arduino/libraries -fqbn=esp32:esp32:m5stack-core-esp32:FlashMode=qio,FlashFreq=80,PartitionScheme=default,UploadSpeed=921600,DebugLevel=none -vid-pid=10C4_EA60 -ide-version=10812 -build-path /tmp/arduino_build_3341 -warnings=none -build-cache /tmp/arduino_cache_25552 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.mkspiffs.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.mkspiffs-0.2.3.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.xtensa-esp32-elf-gcc.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 -prefs=runtime.tools.xtensa-esp32-elf-gcc-1.22.0-80-g6c4433a-5.2.0.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 -prefs=runtime.tools.esptool_py.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 -prefs=runtime.tools.esptool_py-2.6.1.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 $verbose /vm_mnt/sync_box/devl/Arduino/M5Stack/ESP_own/M5XtsOs/M5XtsOs.ino

# Minimal SPIFFS - (Large Apps w/ OTA) -- 2MB for App
$ardDir/arduino-builder -compile -logger=machine -hardware $ardDir/hardware -hardware $homDir/.arduino15/packages -tools $ardDir/tools-builder -tools $ardDir/hardware/tools/avr -tools $homDir/.arduino15/packages -built-in-libraries $ardDir/libraries -libraries $homDir/Arduino/libraries $FQBN -ide-version=10812 -build-path "$outDir" -warnings=none -build-cache "$cahDir" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.mkspiffs.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.mkspiffs-0.2.3.path=$homDir/.arduino15/packages/esp32/tools/mkspiffs/0.2.3 -prefs=runtime.tools.esptool_py.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 -prefs=runtime.tools.esptool_py-2.6.1.path=$homDir/.arduino15/packages/esp32/tools/esptool_py/2.6.1 -prefs=runtime.tools.xtensa-esp32-elf-gcc.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 -prefs=runtime.tools.xtensa-esp32-elf-gcc-1.22.0-80-g6c4433a-5.2.0.path=$homDir/.arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/1.22.0-80-g6c4433a-5.2.0 $verbose ./$prgm.ino
