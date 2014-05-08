#include <stdio.h>
#include <stdbool.h>

#include "firebase.h"
#include "serial.h"


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
