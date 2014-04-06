command1-openwrt
================

OpenWRT Package for controlling Command1 devices


Hardware
================

To use this package you must have a 433Mhz transmitter connected to one of the GPIO pins of your router. Any ASK (Amplitude Shift Keying) transmitter should work, but I have specifically tested with [these](http://www.amazon.com/gp/product/B008A4UWK6) from Amazon.


Building & Installing
================

Download/build the appropriate SDK for your version and architecture of OpenWRT. From the SDK path, run the following:

    cd package/
    git clone https://github.com/maciel310/command1-openwrt.git
    cd ..
    make


You should then have a file named command1_*.ipk somewhere in your bin/ folder. The exact file name and path will vary based on your architecture and SDK version.

Once you have the ipk file, get it to your router somehow (scp, wget, etc), then run `opkg install command1.ipk` (replacing comand1.ipk with the actual file name).



Running
================

SSH/Telnet into your router, then run `command1` to start the controller. The command line parameter syntax is still a work in progress.
