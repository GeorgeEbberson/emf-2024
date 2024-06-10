# Alex, George and Max's EMF 2024 LED Tent
At [EMF 2024][emf2024], we made our tent glow with different colours depending on what you requested
on the phone. This is the code for that.

## Developing
The project uses [PlatformIO][platform_io] to compile the embedded project for [ESP32][esp32].
Installing [the VSCode extension][vsc_extension] is the easiest way to build, upload, and then
monitor (by serial over USB) the code on the device.

## WiFi Credentials
To connect your ESP32 to your WiFi, duplicate [the credentials template][creds_file], strip the
`.template` extension, and update the SSID and password. The file is git ignored, but be extra sure
not to mistakenly check it in!

Testing

[emf2024]: https://www.emfcamp.org/
[platform_io]: https://platformio.org/
[esp32]: https://www.espressif.com/en/products/socs/esp32
[vsc_extension]: https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide
[creds_file]: espProject/src/wifiCreds.h.template
