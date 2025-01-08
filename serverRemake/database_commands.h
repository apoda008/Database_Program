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

MediaNode* createNode(int db_position, double tmdb_id, bool media_type, int genre_types,
    char* description, char* dir_position);

void free_linked_list(MediaNode* header);
void free_individual_node(MediaNode* pointer);


int database_sort_all(char* folder_location);
int database_sort_individual(char* database_file, char* folder_location);
//int parse_into_datastruct(char* file);


//TODO
//void add_to_database();
//void delete_from_database();
//void search_database();
//void adjust_info();




#endif