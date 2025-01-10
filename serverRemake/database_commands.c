#include "database_commands.h"
void print_list(MediaNode* head, char* choice) {
	//all data in the node
	while (head != NULL) {
		if (strcmp(choice, "all") == 0) {

			printf("================DATA ALL: ================\n");
			printf("db_pos: %d\n", head->data.db_position);
			printf("Title: %s\n", head->data.title);
			printf("tmdb_id: %f\n", head->data.tmdb_id);
			printf("media: %d\n", head->data.media_type);
			for (int i = 0; i < 19; i++) {
				printf("genre: %d\n", head->data.genre_types[i]);
			}
			printf("description: %s\n", head->data.description);
			printf("dir position: %s\n", head->data.dir_position_media);
			printf("===============DATA ALL: ================\n");

		}
		else if (strcmp(choice, "db_pos") == 0) {
			printf("db_pos: %d\n", head->data.db_position);
		}
		else if (strcmp(choice, "title") == 0) {
			printf("Title: %s\n", head->data.title);
		}
		else if (strcmp(choice, "tmdb_id") == 0) {
			printf("tmdb_id: %f\n", head->data.tmdb_id);
		}
		else if (strcmp(choice, "media_type") == 0) {
			printf("media: %d\n", head->data.media_type);
		}
		else if (strcmp(choice, "genre") == 0) {
			for (int i = 0; i < 19; i++) {
				printf("genre: %d\n", head->data.genre_types[i]);
			}
		}
		else if (strcmp(choice, "descr") == 0) {
			printf("description: %s\n", head->data.description);
		}
		else if (strcmp(choice, "dir") == 0) {
			printf("dir position: %s\n", head->data.dir_position_media);
		}
		else {
			printf("FAILED: improper second argument");
		}
		head = head->next;
	}
}

//might remove
int title_compare(char* node, char* next_node) {
	int string_compare = strcmp(node, next_node);
	if (string_compare < 0) {
		return 1;
	}
	else if (string_compare > 0) {
		return 2; 
	}
	else {
		return 0; //they match meaning duplicate
	}
}


//*********************EVERYTHING FOR MERGE SORT***********************
void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref) {
	MediaNode* slow = source;
	MediaNode* fast = source->next;

	while (fast != NULL) {
		fast = fast->next;
	}
	if (fast != NULL) {
		slow = slow->next;
		fast = fast->next;
	}
	
	*front_ref = source;
	*back_ref = slow->next;
	slow->next = NULL;
}

MediaNode* merge_list(MediaNode* a, MediaNode* b) {
	if (a == NULL) { return b; }
	if (b == NULL) { return a; }

	MediaNode* result = NULL;

	if (strcmp(a->data.title, b->data.title) <= 0) {
		result = a;
		result->next = merge_list(a->next, b);
	}
	else {
		result = b;
		result->next = merge_list(a, b->next);
	}

	return result;
}

void merge_sort(MediaNode** headRef) {
	MediaNode* head = *headRef;

	if (head == NULL || head->next == NULL) {
		return; //0 or 1 node
	}

	MediaNode* front; 
	MediaNode* back;

	split_list(head, &front, &back);

	merge_sort(&front);
	merge_sort(&back);

	*headRef = merge_list(front, back);
	
}

//*********************************************************************

int database_sort_individual(char* database_file) {
	
	printf("DATABASE FILE: %s\n", database_file);
	FILE* file = fopen(database_file, "rb");
	if(file == NULL) {
		perror("Failed to open file");
		return 0;
	}
	
	printf("opened file\n");
	
	MediaNode* head = NULL;
	MediaNode* tail = NULL;
	MediaData temp;
	
	printf("starting read\n");
	
	while ( (fread(&temp, sizeof(MediaData), 1, file)) == 1) {
		MediaNode* new_node = (MediaNode*)malloc(sizeof(MediaNode));
		if (new_node == NULL) {
			perror("MediaData Memory allocation failed");
			fclose(file);
			return 0;
		}

		memcpy(&new_node->data, &temp, sizeof(MediaData));
		new_node->next = NULL;

		if (head == NULL) {
			head = new_node;
			tail = new_node;
		}
		else {
			tail->next = new_node;
			tail = new_node;
		}
		//DELETE
		printf("Title order: %s\n", new_node->data.title);
	}

	fclose(file);
	
	//DELETE
	printf("\n");	
	merge_sort(&head);
	print_list(head, "title");

/*
TODO:
Sort the newly established link list
re-iterate and assign a db_num value
then resave into a sorted bin file

*/
	
	return 1;
}

int database_sort_all(char* folder_location) {

	//might need adjustment for unknowns (numbers/symbols)
	printf("Sorting all Database files\n\n");

	char* folder_stand_in = "";

	char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j',
		'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	
	char file_name[260] = "l.bin";
	char file_first[260]= "";
	strcat_s(file_first, 260, folder_location);
	for (int i = 0; i < 26; i++) {
		file_name[0] = alphabet[i];
		strcat_s(file_first, 260, "\\");
		strcat_s(file_first, 260, file_name);
		printf("strcat %s\n", file_first);
		
		if ((database_sort_individual(&file_first)) == 1) {
			printf("Database_sort completed\n");
		}
		else {
			printf("Failed\n");
		}
		strcpy_s(file_first, 260, folder_location);
	}

	return 1;
}