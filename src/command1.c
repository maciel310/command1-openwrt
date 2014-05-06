#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "cjson/cJSON.h"

#include "firebase.h"

int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                //error_message ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                //error_message ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                //error_message ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
}

int open_port() {
  char *portname = "/dev/ttyACM0";
  int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    return;
  }

  set_interface_attribs (fd, B9600, 0);
  set_blocking (fd, 0);

  return fd;
}

void send_command(int fd, bool isOn, int device) {
  char cmd[2];
  cmd[0] = (isOn ? '1' : '0');
  cmd[1] = '0' + device;

  write(fd, cmd, 2);

  char buf[4];
  int n = read(fd, buf, sizeof buf);
}

int main(int argc, char *argv[]) {
  if(argc > 1) {
    printf("Commanding remote\n\n");
  
    int fd = open_port();
    bool isOn = strcmp(argv[1], "on") == 0 ? true : false;
  
    send_command(fd, isOn, argv[2][0] - '0');
  } else {
    firebase_subscribe("https://test.firebaseio.com/me.json");
  }

  return 0;
}
