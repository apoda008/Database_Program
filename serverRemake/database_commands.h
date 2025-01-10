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

//=========LIST/NODE MANAGEMENT====================
void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref);
MediaNode* merge_list(MediaNode* a, MediaNode* b);
void merge_sort(MediaNode** headRef);

void print_list(MediaNode*, char* choice);
void free_linked_list(MediaNode* header);
void free_individual_node(MediaNode* pointer, void* ref_obj);
void insert_node(MediaNode* node);


//-------------SORTING COMMANDS---------------------
int database_sort_all(char* folder_location);
int database_sort_individual(char* database_file);
//int parse_into_datastruct(char* file);

//------------FILE MANAGEMENT---------------------
MediaNode* bin_read(char* database_file);
void bin_write(char* database_file, MediaNode* head_ref);

//------------Search Commands---------------------

//returns requested info
void* search_database_info(void* requested_info);
//returns entire node
MediaNode* search_database_obj(void* requested_info);

//TODO
//void add_to_database();
//void delete_from_database();
//void search_database();
//void adjust_info();
//void delete_bin();




#endif