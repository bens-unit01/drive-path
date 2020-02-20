 #!/bin/bash
 export PATH=/c/Development/BLE/NORDIC_DEV/nrf51822_sdk/arm-gcc/bin
 arm-none-eabi-gdb _build/SwitchBotNordic-log_s110.elf -x gdbinit
 #---- startup sequence
 #conf 1 --> marche bien
 file "_build/SwitchBotNordic-log_s110.elf"
 load
 mon reset
 break main

