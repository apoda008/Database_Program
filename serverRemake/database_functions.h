#pragma once
#ifndef DATABASE_COMMANDS_H
#define DATABASE_COMMANDS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "cJSON.h"
#include "data_structures.h"

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

//Parses input string for function execution
cJSON* input_string_parsing(char* user_input);
//gets the entire node of a requested media
MediaNode* get_media(char* title);


#endif