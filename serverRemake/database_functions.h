#pragma once
#ifndef DATABASE_COMMANDS_H
#define DATABASE_COMMANDS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cJSON.h"
#include "data_structures.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <ctype.h>
#include <curl/curl.h>

struct Master_Directory {
    TCHAR movie_bin_path[MAX_PATH];
    TCHAR genre_path[MAX_PATH];
    TCHAR series_bin_path[MAX_PATH];

};

enum ObjectDataType {
    DB_POS,
    TITLE,
    TMDB_ID,
    MOVIE,
    GENRE,
    DESCRIPTION,
    DIR_POS //may not need
};


//This is the genre/number relation for Movies from TMDB as of 1/13/2025
enum GenreTypeMovie {
    ACTION = 28,
    ADVENTURE = 12,
    ANIMATION = 16,
    COMEDY = 35,
    CRIME = 80,
    DOCUMENTARY = 99,
    DRAMA = 18,
    FAMILY = 10751,
    FANTASY = 14,
    HISTORY = 36,
    HORROR = 27,
    MUSIC = 10402,
    MYSTERY = 9648,
    ROMANCE = 10749,
    SCIENCE_FICTION = 878,
    TV_MOVIE = 10770,
    THRILLER = 53,
    WAR = 10752,
    WESTERN = 37
};


//-------------SORTING COMMANDS---------------------
//sorts all possible bin files in the folder
int database_sort_all(char* folder_location);
//sorts a specified .bin file
int database_sort_individual(char* database_file);

//------------FILE MANAGEMENT---------------------
//reads a bin file into a MediaNode linked list
MediaNode* bin_read(char* database_file);
//writes a MediaNode linked list into a bin file
void bin_write(char* database_file, MediaNode* head_ref);

//------------Search Commands---------------------

//returns requested info
MediaNode* search_linked_list_object(void* requested_info, char* designator);

//searched sorted binary tree and returns the node
TreeNode* binary_tree_search(TreeNode* root, char* search_request);
//TODO
//void add_to_database();
//void delete_from_database();
//void search_database();
//void adjust_info();
//void delete_bin();

//------------Directory--------------------------------n

//takes the requested directory file and searches through for movie files
int directorySearch(char* mainStart, char* create_folder_location);

//------------MISC -----------------------------------
//Parses input string for function execution
cJSON* input_string_parsing(char* user_input);
//recovers node and parses into JSON for delivery
cJSON* get_media(char* title);

//request directory file to be searched
int startUp();

//ALL THE CRAP FROM MEDIAPARSE.H 


//cleans up the title for TMDB api request
void parse_for_database(char* filename, char* dir_position, char* create_folder_location);

//makes TMDB request for cursory information
void information_Request(const char* movie_title, char* dir_position, char* create_folder_location);

char genre_write(char* genre_type, char* title);

//writes to a db file
void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, char* dir_position, char* create_folder_location);


#endif //END