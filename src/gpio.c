#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gpio.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


int gpioNumber;
int gpio_fd;

const int PULSE_LENGTH = 1750;
const int ONE_LENGTH = 1250;
const int ZERO_LENGTH = 400;


void zero() {
  write(gpio_fd, "1", 1);
  usleep(ZERO_LENGTH);
  write(gpio_fd, "0", 1);
  usleep(PULSE_LENGTH - ZERO_LENGTH);
}

void one() {
  write(gpio_fd, "1", 1);
  usleep(ONE_LENGTH);
  write(gpio_fd, "0", 1);
  usleep(PULSE_LENGTH - ONE_LENGTH);
}

void export_gpio() {
  int fd;
  char buf[10];
  fd = open("/sys/class/gpio/export", O_WRONLY);
  sprintf(buf, "%d", gpioNumber);
  write(fd, buf, strlen(buf));
  close(fd);
}

void getCommand(bool state, int device, int index, char* c) {
  char commandByte, addressByte;

  if (state) {
    if (index == 0) {
      commandByte = 0x84; // All On
    } else {
      commandByte = 0x88; // Individual On
    }
  } else {
    if (index == 0) {
      commandByte = 0x44; // All Off
    } else {
      commandByte = 0x48; // Individual Off
    }
  }

  // Bit shift device number by 3 and cast to byte
  addressByte = (char)(device << 3);

  switch(index) {
    case 1:
      bitWrite(addressByte, 2, 1);
      break;
    case 2:
      bitWrite(addressByte, 1, 1);
      break;
    case 3:
      bitWrite(addressByte, 2, 1);
      bitWrite(addressByte, 1, 1);
      break;
  }

  c[0] = commandByte;
  c[1] = addressByte;
  c[2] = 5; //B00000101;
}

void sendCommand(char command[]) {
  usleep(12500);

  int i, j;
  for(i=0; i<3; i++) {
    for(j=7; j>=0; j--) {
      if(bitRead(command[i], j) == 0) {
        zero();
      } else {
        one();
      }
    }
  }

  zero();
  zero();
  zero();
}


void repeatCommand(char command[]) {
  write(gpio_fd, "1", 1);

  zero();

  int i;
  for(i=0; i<5; i++) {
    sendCommand(command);
  }

  write(gpio_fd, "0", 1);
}

void output_gpio() {
  int fd;
  char buf[50];
  sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpioNumber);
  fd = open(buf, O_WRONLY);
  write(fd, "out", 3);
  close(fd);
}

void open_gpio() {
  char buf[50];
  sprintf(buf, "/sys/class/gpio/gpio%d/value", gpioNumber);
  gpio_fd = open(buf, O_WRONLY);
}

void close_gpio() {
  close(gpio_fd);
}

void init_sender_gpio(int selected_pin) {
  gpioNumber = selected_pin;
  export_gpio();
  output_gpio();
}

void send_command_gpio(bool isOn, int device, int index) {
  open_gpio();

  char c[3];
  getCommand(isOn, device, index, c);

  repeatCommand(c);

  close_gpio();
}
