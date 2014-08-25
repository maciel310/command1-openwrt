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
    make package/command1-openwrt/compile


You should then have a file named command1_*.ipk somewhere in your bin/ folder. The exact file name and path will vary based on your architecture and SDK version.

Once you have the ipk file, get it to your router somehow (scp, wget, etc), then run `opkg install command1.ipk` (replacing comand1.ipk with the actual file name).



Running
================

A daemon will be installed that will automatically run the command1 client on boot. If you don't want this to run, you can disable it by running `/etc/init.d/command1 disable`.

You can also manually start the command1 binary using the following commandline arguments:

Usage: command1 [OPTION...]
  -s, --use-serial=     Use Serial sender
  -g, --use-gpio=       Use GPIO sender
  -d, --device=         Device to command
  -1, --on=             Turns selected device on
  -0, --off=            Turns selected device off
  -f, --firebase=       Firebase hostname to connect to.

The {device, on, off} options and the {firebase} options are mutually exclusive. By specifying the `firebase` option the program will enter daemon mode, listening for commands from the firebase server indicated.
