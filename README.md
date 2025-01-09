# AutoESP

AutoESP is a simple firmware for the ESP8266 microcontroller. This particular firmware was installed to a flashed Sonoff S31 by soldering wire onto the header pins of the device.

Through testing with a NodeMCU ESP32, I confirm that this firmware will still work for the newer MCU.

Features:
- Alarm (Toggle hi/low state daily, at a specific time)
- Hardware clock sync
- GPIO hi/low toggle
- WiFI AP

AutoESP uses a HTTP server for communication with the device.

Daily clock syncing with  an internet connected is recommended for alarm accuracy. Or better yet, it may prove more beneficial to have a internet connected  device connect to the MCU via WiFi and HTTP daily to toggle states.

![image](https://github.com/user-attachments/assets/74255b8d-d16c-49b4-9819-d79ddbdae7ca)

Credits to Tinkerman for the image. (tinkerman.cat)

### Building from source:
After configuring build environment according to Espressif specifications.
```
git clone https://github.com/rackd/autoesp
cd autoesp
python $IDF_PATH/tools/idf.py build
```

### Flashing

```
esptool.py --chip esp8266 --no-stub  --port YOUR_SERIAL_PORT --baud 115200 --before default_reset --after soft_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0x10000 build/autoesp.bin
```

 
### Logs via UART:
```
python $IDF_PATH/tools/idf.py --port YOUR_SERIAL_PORT --baud 115200 monitor
```

Be sure to change YOUR_SERIAL_PORT to a valid serial port. On a Raspberry pi, this is: ```/dev/serial0```.


