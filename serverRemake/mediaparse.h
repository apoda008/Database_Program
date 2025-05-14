#ifndef MEDIAPARSE_H
#define MEDIAPARSE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"
#include <curl/curl.h>
#include <stdbool.h>
#include "database_functions.h"


struct Memory {
    char* string;
    size_t size;
};

//cleans up the title for TMDB api request
void parse_for_database(char* filename, char* dir_position, char* create_folder_location);

//makes TMDB request for cursory information
void information_Request(const char* movie_title, char* dir_position, char* create_folder_location);

int genre_parsing(char* int_arry);

//writes to a db file
void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, char *dir_position, char* create_folder_location);

#endif