#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <popt.h>
#include "firebase.h"
#include "serial.h"
#include "gpio.h"
#include "cjson/cJSON.h"


enum sender_type { SERIAL, GPIO };
enum sender_type selected_sender = SERIAL;
int selected_gpio_pin = 0;
int DEFAULT_DEVICE = 28; // Old hard-coded device (for backwards compatibility)


void init_sender() {
  if (selected_sender == SERIAL) {
    init_sender_serial();
  } else {
    init_sender_gpio(selected_gpio_pin);
  }
}

void send_command(bool isOn, int device, int index) {
  if (selected_sender == SERIAL) {
    send_command_serial(isOn, device, index);
  } else {
    send_command_gpio(isOn, device, index);
  }
}

void parse_event_string(char* event) {
  bool isOn;
  int device, index;

  if (strchr(event, '{') == NULL) {
    // Old message format: Use hard-coded device.
    isOn = (strncmp(event, "on", 2) == 0);
    index = event[strlen(event)-1] - '0';
    device = DEFAULT_DEVICE;
  } else {
    cJSON *eventJson = cJSON_Parse(event);
    isOn = cJSON_GetObjectItem(eventJson,"command")->type == cJSON_True;
    device = cJSON_GetObjectItem(eventJson,"device")->valueint;
    index = cJSON_GetObjectItem(eventJson,"index")->valueint;
  }

  send_command(isOn, device, index);
}

int main(int argc, char *argv[]) {
  int c;
  int device = DEFAULT_DEVICE;
  int index = 0;
  bool isOn = false;
  char firebase_url[255];
  char *firebase_host = "";

  poptContext pc;
  struct poptOption po[] = {
    {"use-serial", 's', POPT_ARG_VAL, &selected_sender, SERIAL, "Use Serial sender", ""},
    {"use-gpio", 'g', POPT_ARG_VAL, &selected_sender, GPIO, "Use GPIO sender", ""},
    {"device", 'd', POPT_ARG_INT, &device, 0, "Device to command", ""},
    {"index", 'i', POPT_ARG_INT, &index, 0, "Device index to command", ""},
    {"on", '1', POPT_ARG_VAL, &isOn, true, "Turns selected device on", ""},
    {"off", '0', POPT_ARG_VAL, &isOn, false, "Turns selected device off", ""},
    {"firebase", 'f', POPT_ARG_STRING, &firebase_host, 0, "Firebase hostname to connect to.", ""},
    {"gpio-pin", 'p', POPT_ARG_INT, &selected_gpio_pin, 0, "GPIO pin to use", ""},
    POPT_AUTOHELP
    {NULL}
  };

  pc = poptGetContext(NULL, argc, (const char **)argv, po, 0);

  if (argc < 2) {
    poptPrintUsage(pc, stderr, 0);
    return 1;
  }

  int val;
  while ((val = poptGetNextOpt(pc)) >= 0) {
  }

  if (val != -1) {
    printf("ERROR: %d\n\n", val);
  }

  init_sender();

  if (firebase_host[0] == 0) {
    printf("Commanding remote\n\n");
  
    send_command(isOn, device, index);
  } else {
    sprintf(firebase_url, "https://%s.firebaseio.com/me.json", firebase_host);
    firebase_set_url(firebase_url);
    firebase_set_callback("/event", parse_event_string);

    sleep(10);
    while (true) {
      firebase_subscribe();
      sleep(30);
    }
  }

  return 0;
}
