#pragma once
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
//#include "database_commands.h"

#include <stdbool.h>
#include <stdlib.h>

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

typedef struct TreeNode {
    MediaData data;

    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/*
===========LINKED LIST==================
*/
//inserts a single node into list
void insert_node(MediaNode* node);

void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref);
//function for merge_sort
MediaNode* merge_list(MediaNode* a, MediaNode* b);
//merge sorts the bin file
void merge_sort(MediaNode** headRef);

//MAYBE??
void insert_node_LL(MediaNode* source);
//Prints list
void print_list(MediaNode*, char* choice);
//frees linked list
void free_linked_list(MediaNode* header);
//frees individual node (todo) 
void free_individual_node(MediaNode* pointer, void* ref_obj);


/*
===========BINARY TREE===================
*/

//creates and mallocs a single tree node
TreeNode* create_tree_node(MediaData item);

//takes a sorted linked list and builds bottom up into a binary tree
TreeNode* sorted_to_bst(MediaNode** source, int start, int end);

//prints out a sorted binary tree
void inorder_traversal_print(TreeNode* root);

//frees mem from a binary tree
TreeNode* free_binary_tree(TreeNode* root);

#endif // !DATA_STRUCTURES_H
