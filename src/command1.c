#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "firebase.h"
#include "serial.h"
#include "gpio.h"

#define _USE_SERIAL_
#ifdef _USE_SERIAL_
  void (*send_command)(bool, int) = send_command_serial;
  void (*init_sender)() = init_sender_serial;
#else
  void (*send_command)(bool, int) = send_command_gpio;
  void (*init_sender)() = init_sender_gpio;
#endif

void parse_event_string(char* event) {
  bool isOn = (strncmp(event, "on", 2) == 0);

  send_command(isOn, event[strlen(event)-1] - '0');
}

int main(int argc, char *argv[]) {
  init_sender();

  if(argc > 1) {
    printf("Commanding remote\n\n");
  
    bool isOn = strcmp(argv[1], "on") == 0 ? true : false;
  
    send_command(isOn, argv[2][0] - '0');
  } else {
    firebase_set_url("https://test.firebaseio.com/me.json");
    firebase_set_callback("/event", parse_event_string);

    sleep(10);
    while (true) {
      firebase_subscribe();
      sleep(30);
    }
  }

  return 0;
}
