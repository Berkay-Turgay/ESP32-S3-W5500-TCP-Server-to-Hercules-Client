# ESP32-S3-W5500-TCP-Server-to-Hercules-Client
ESP32 TCP Server Example using W5500 Ethernet Module. Configures ESP32 as a TCP server to communicate with Hercules TCP Client, enabling data exchange over Ethernet connection.

# Network Configuration for ESP32 Server and PC Client

When using ESP32 as a server and a PC as a client, both devices must be on the same local network. Their IP addresses should share the same network prefix (first three octets) but have different host identifiers (last octet).

To configure the PC's IP address in Windows:

Open Control Panel

Navigate to Network and Internet → Network and Sharing Center

Click Change adapter settings

Right-click your Ethernet connection → select Properties

Select Internet Protocol Version 4 (TCP/IPv4) → click Properties

Configure the IP settings as shown in the below

(Image showing example configuration)

Example Configuration:

ESP32 Server IP: 192.168.1.50

PC Client IP: 192.168.1.10

Subnet Mask: 255.255.255.0

Both devices will now be on the same network (192.168.1.x) with unique addresses.

# PlatformIO Configuration for ESP32-S3
When using PlatformIO with an ESP32-S3 board, the following configuration must be added to platformio.ini:

[env:esp32-s3-devkitc-1]

platform = espressif32

board = esp32-s3-devkitc-1

framework = arduino

monitor_speed = 115200

board_upload.flash_size = 4MB

board_build.partitions = default.csv

build_flags =

-DARDUINO_USB_CDC_ON_BOOT=1

-DBOARD_HAS_PSRAM

These parameters ensure proper USB CDC behavior, serial communication, and PSRAM support for ESP32-S3 boards.
