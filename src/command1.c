#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "firebase.h"
#include "serial.h"
#include "gpio.h"


bool useSerial = true;

void init_sender() {
  if (useSerial) {
    init_sender_serial();
  } else {
    init_sender_gpio();
  }
}

void send_command(bool isOn, int device) {
  if (useSerial) {
    send_command_serial(isOn, device);
  } else {
    send_command_gpio(isOn, device);
  }
}

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
