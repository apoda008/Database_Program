#pragma once
#ifndef DATABASE_COMMANDS_H
#define DATABASE_COMMANDS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MediaData {
    int db_position;
    char title[260];
    double tmdb_id;
    bool media_type; // (0 or 1) 1 for movie, 0 for show
    int genre_types[19];
    char description[2000];
    char dir_position_media[260];
} MediaData;


typedef struct MediaNode {
    MediaData data;
    struct MediaNode* next;
} MediaNode;

enum ObjectDataType {
    DB_POS,
    TITLE,
    TMDB_ID,
    MOVIE,
    GENRE,
    DESCRIPTION,
    DIR_POS //may not need
};

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

typedef struct TreeNode {
    MediaData data;
    
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

//=========LIST/NODE MANAGEMENT====================
//function for merge_sort
void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref);
//function for merge_sort
MediaNode* merge_list(MediaNode* a, MediaNode* b);
//merge sorts the bin file
void merge_sort(MediaNode** headRef);

//prints specified information to terminal
void print_list(MediaNode*, char* choice);
//frees linked list
void free_linked_list(MediaNode* header);
//frees individual node (TODO)
void free_individual_node(MediaNode* pointer, void* ref_obj);
//inserts a single node into list
void insert_node(MediaNode* node);


//-------------SORTING COMMANDS---------------------
//sorts all possible bin files in the folder
int database_sort_all(char* folder_location);
//sorts a specified bin file
int database_sort_individual(char* database_file);

//------------FILE MANAGEMENT---------------------
//reads a bin file into a MediaNode linked list
MediaNode* bin_read(char* database_file);
//writes a MediaNode linked list into a bin file
void bin_write(char* database_file, MediaNode* head_ref);

//------------Search Commands---------------------

//returns requested info
void* search_database_info(void* requested_info, char* designator);
//returns entire node
MediaNode* search_database_obj(void* requested_info);

//TODO
//void add_to_database();
//void delete_from_database();
//void search_database();
//void adjust_info();
//void delete_bin();




#endif