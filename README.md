# ESP32-S3-W5500-TCP-Server-to-Hercules-Client
ESP32 TCP Server Example using W5500 Ethernet Module. Configures ESP32 as a TCP server to communicate with Hercules TCP Client, enabling data exchange over Ethernet connection.

## Project Development Environment

This project was developed using Visual Studio Code with the PlatformIO IDE extension for ESP32-S3

## Network Configuration for ESP32 Server and PC Client

When using ESP32 as a server and a PC as a client, both devices must be on the same local network. Their IP addresses should share the same network prefix (first three octets) but have different host identifiers (last octet).

To configure the PC's IP address in Windows:

Open Control Panel

Navigate to Network and Internet → Network and Sharing Center

Click Change adapter settings

Right-click your Ethernet connection → select Properties

Select Internet Protocol Version 4 (TCP/IPv4) → click Properties

Configure the IP settings as shown in the below

(Image showing Settings-Images File)

Example Configuration:

ESP32 Server IP: 192.168.1.50

PC Client IP: 192.168.1.10

Subnet Mask: 255.255.255.0

Both devices will now be on the same network (192.168.1.x) with unique addresses.

## Library Modifications for ESP32 Compatibility

The original W5500 Ethernet library requires modifications to be compatible with ESP32's Server class architecture. Follow these steps to update the library files:

**1. Modify the Ethernet.h file:**

Locate the EthernetServer class definition and update it to be compatible with ESP32's Server class:

cpp
// Find and modify the EthernetServer class definition:

```
class EthernetServer : public Server {
private:
    uint16_t _port;
public:
    // Constructor compatible with ESP32 Server class
    EthernetServer(uint16_t port) : _port(port) { }
    
    // ESP32-compatible begin function (with port parameter)
    virtual void begin(uint16_t port = 0) {
        if (port != 0) {
            _port = port;
        }
        // Call the renamed begin function
        _begin();
    }
    
    // Rename original begin function to private
    void _begin();
    
    EthernetClient available();
    EthernetClient accept();
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual operator bool();
    using Print::write;
    
    // Comment out or remove this line:
    // void statusreport();

    // TODO: make private when socket allocation moves to EthernetClass
    static uint16_t server_port[MAX_SOCK_NUM];
};
```

**2. Modify the EthernetServer.cpp file:**

Rename the begin() function to _begin() in the implementation file:

```
// In EthernetServer.cpp file:

// Replace void EthernetServer::begin() with:
void EthernetServer::_begin()  // Renamed function
{
    uint8_t sockindex = Ethernet.socketBegin(SnMR::TCP, _port);
    if (sockindex < MAX_SOCK_NUM) {
        if (Ethernet.socketListen(sockindex)) {
            server_port[sockindex] = _port;
        } else {
            Ethernet.socketDisconnect(sockindex);
        }
    }
}
```

// Additionally, update all begin() calls to _begin() in available() and accept() functions:

// Example in available() function:

if (!listening) _begin();  // Changed from begin() to _begin()

// Make the same change in accept() function:

if (!listening) _begin();  // Changed from begin() to _begin()

**Important Notes:**

These modifications ensure proper inheritance from ESP32's Server class

The changes maintain backward compatibility while adding ESP32-specific functionality

The statusreport() function is removed as it's not part of ESP32's Server interface

The dual begin() function approach allows both parameterized and parameter-less calls

## PlatformIO Configuration for ESP32-S3
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
