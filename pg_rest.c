#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include <curl/curl.h>

/*
    Data structures and utility functions for libcurl
*/
struct memory{
  char *response;
  size_t size;
};

size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   struct memory *mem = (struct memory *)userp;

   char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
   if(ptr == NULL)
     return 0;  /* out of memory! */

   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;

   return realsize;
}

/*
    PG Functions
*/
PG_MODULE_MAGIC;

/*
    Test function
*/
PG_FUNCTION_INFO_V1(add_numbers);

Datum add_numbers(PG_FUNCTION_ARGS) {
    int32 x = PG_GETARG_INT32(0);
    int32 y = PG_GETARG_INT32(1);

    PG_RETURN_INT32(x + y);
}

/*
    REST GET
*/
PG_FUNCTION_INFO_V1(rest_get);

Datum rest_get(PG_FUNCTION_ARGS) {
    curl_global_init(CURL_GLOBAL_ALL);

    text *input = PG_GETARG_TEXT_P(0);
    const char *url = text_to_cstring(input);
    
    CURL *curl;
    CURLcode res;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* ask libcurl to show us the verbose output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url);    

        // SSL Certificate bypassed
        //TODO: SSL certificate verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Header
        //struct curl_slist *chunk = NULL;
        //chunk = curl_slist_append(chunk, "Accept: application/json");
        //chunk = curl_slist_append(chunk, "Content-type: application/json");
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        // Output buffer
        struct memory buffer = {0};

        // Will send all data to this function 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        // Set output buffer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);

        /* Perform the request, res will get the return code */
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "[pg_rest] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            PG_RETURN_CSTRING("Libcurl error. Consult log.");
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

        text *result_text = cstring_to_text(buffer.response);
        PG_RETURN_TEXT_P(result_text);
    }

    curl_global_cleanup();
    PG_RETURN_CSTRING("FAIL");
}

/*
    REST POST
*/
PG_FUNCTION_INFO_V1(rest_post);

Datum rest_post(PG_FUNCTION_ARGS) {
    curl_global_init(CURL_GLOBAL_ALL);

    text *input = PG_GETARG_TEXT_P(0);
    const char *url = text_to_cstring(input);
    text *input2 = PG_GETARG_TEXT_P(1);
    const char *data = text_to_cstring(input2);

    CURL *curl;
    CURLcode res;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* ask libcurl to show us the verbose output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url);    

        // SSL Certificate bypassed
        //TODO: SSL certificate verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Header

        // Data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        // Output buffer
        struct memory buffer = {0};

        // Will send all data to this function 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        // Set output buffer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);

        /* Perform the request, res will get the return code */
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "[pg_rest] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            PG_RETURN_CSTRING("Libcurl error. Consult log.");
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

        text *result_text = cstring_to_text(buffer.response);
        PG_RETURN_TEXT_P(result_text);
    }

    curl_global_cleanup();
    PG_RETURN_CSTRING("FAIL");
}

/*
    REST Generic Method
*/
PG_FUNCTION_INFO_V1(rest_call);

Datum rest_call(PG_FUNCTION_ARGS) {
    curl_global_init(CURL_GLOBAL_ALL);

    text *input0 = PG_GETARG_TEXT_P(0);    
    const char *method = text_to_cstring(input0);
    text *input1 = PG_GETARG_TEXT_P(1);
    const char *url = text_to_cstring(input1);
    text *input2 = PG_GETARG_TEXT_P(2);
    const char *data = text_to_cstring(input2);
    

    CURL *curl;
    CURLcode res;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* ask libcurl to show us the verbose output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url);    

        // SSL Certificate bypassed
        //TODO: SSL certificate verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Header

        // Data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        // Output buffer
        struct memory buffer = {0};

        // Will send all data to this function 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        // Set output buffer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);

        /* Perform the request, res will get the return code */
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "[pg_rest] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            PG_RETURN_CSTRING("Libcurl error. Consult log.");
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

        text *result_text = cstring_to_text(buffer.response);
        PG_RETURN_TEXT_P(result_text);
    }

    curl_global_cleanup();
    PG_RETURN_CSTRING("FAIL");
}

