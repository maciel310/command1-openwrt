#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cjson/cJSON.h"
#include "firebase.h"
#include "serial.h"


void send_command(int fd, bool isOn, int device) {
  char cmd[2];
  cmd[0] = (isOn ? '1' : '0');
  cmd[1] = '0' + device;
  cmd[2] = '\0';

  write(fd, cmd, 2);

  char buf[4];
  int n = read(fd, buf, sizeof buf);
}

void parse_event_string(char* event) {
  int fd = open_port();
  bool isOn = (strncmp(event, "on", 2) == 0);

  send_command(fd, isOn, event[strlen(event)-1] - '0');
}

void server_sent_event_callback(char* event, char* dataString) {
  if(strcmp(event, "put") == 0) {
    cJSON *json = cJSON_Parse(dataString);
    if(strcmp("/event", cJSON_GetObjectItem(json,"path")->valuestring) == 0) {
      char *lightEvent = cJSON_GetObjectItem(json,"data")->valuestring;

      parse_event_string(lightEvent);
    }
  }
}

int main(int argc, char *argv[]) {
  if(argc > 1) {
    printf("Commanding remote\n\n");
  
    int fd = open_port();
    bool isOn = strcmp(argv[1], "on") == 0 ? true : false;
  
    send_command(fd, isOn, argv[2][0] - '0');
  } else {
    while (true) {
      firebase_subscribe("https://test.firebaseio.com/me.json", server_sent_event_callback);
    }
  }

  return 0;
}
