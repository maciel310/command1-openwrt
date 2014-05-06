#include "firebase.h"

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

void server_sent_event_callback(char* event, char* dataString) {
  printf("%s: %s\n", event, dataString);
  if(strcmp(event, "put") == 0) {
    cJSON *json = cJSON_Parse(dataString);
    if(strcmp("/event", cJSON_GetObjectItem(json,"path")->valuestring) == 0) {
      char *lightEvent = cJSON_GetObjectItem(json,"data")->valuestring;

      printf("Event: %s\n\n", lightEvent);
    }
  }
}

size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *d) {
  int realsize = size*nmemb;

  char str[realsize+1];
  char* cpy;
  char* line;
  char event[255];
  char data[255];
  *str = '\0';
  strncat(str, ptr, realsize);

  cpy = strdup(str);
  while((line = strsep(&cpy, "\n")) != NULL) {
    if(strncmp("event: ", line, 7) == 0) {
      sprintf(event, "%s", line+7);
    } else if(strncmp("data: ", line, 6) == 0) {
      sprintf(data, "%s", line+6);
    }
  }
  
  server_sent_event_callback(event, data);
  return realsize;
}

void firebase_subscribe(char* url) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: text/event-stream");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
}
