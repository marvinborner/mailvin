#include "env.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
	char *ptr;
	size_t len;
};

void init_string(struct string *s)
{
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size * nmemb;
}

int request(CURL *curl, const char *req, struct string *resp)
{
	init_string(resp);
	curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);
	curl_easy_setopt(curl, CURLOPT_URL, req);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 1;
	} else {
		return 0;
	}
}

int main()
{
	CURL *curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "curl initialization failed\n");
		exit(EXIT_FAILURE);
	}

	struct string s;
	if (!request(curl, URL "/" BOX ";UID=*", &s))
		printf("%s", s.ptr);
	free(s.ptr);

	curl_easy_cleanup(curl);
	return 0;
}
