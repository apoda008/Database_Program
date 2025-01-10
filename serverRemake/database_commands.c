#include "database_commands.h"
void print_list(MediaNode* head, char* choice) {
	if (strcmp(choice, "all") == 0) {
		while (head != NULL) {
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
			head = head->next;
		}
	}
	if (strcmp(choice, "db_pos") == 0) { printf("db_pos: %d\n", head->data.db_position); }
	if (strcmp(choice, "title") == 0) { printf("Title: %s\n", head->data.title); }
	if (strcmp(choice, "tmdb_id") == 0) { printf("tmdb_id: %f\n", head->data.tmdb_id); }
	if (strcmp(choice, "media_type") == 0) { printf("media: %d\n", head->data.media_type); }
	if (strcmp(choice, "genre") == 0) {
		for (int i = 0; i < 19; i++) {
			printf("genre: %d\n", head->data.genre_types[i]);
		}
	}
	if (strcmp(choice, "descr") == 0) { printf("description: %s\n", head->data.description); }
	if (strcmp(choice, "dir") == 0) { printf("dir position: %s\n", head->data.dir_position_media); }
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
	
	//============Sorts the linked list=================
	MediaNode* current = head;
	MediaNode* prev = head;
	MediaNode* next_node = head->next;

	int db_pos = 0;
	while (current->next != NULL) {
		db_pos += 1;
		//might remove this function altogether
		printf("Prev: %s\n", prev->data.title);
		printf("current: %s\n", current->data.title);
		printf("next: %s\n", next_node->data.title);
		
		int result = title_compare(current->data.title, next_node->data.title);
		printf("result %d\n", result);
		
		if (result == 2) {
			if (current == head) {
				head = next_node;
				prev = next_node;
				current->next = next_node->next;
				next_node->next = current;
				current = next_node;
			}
			else {
			//current->next = next_node->next;
			next_node->next = current;
			prev->next = next_node;
			current = next_node;
			}
		}
		
		//for if even or 1
		if (next_node->next == NULL) {
			if ((db_pos > 1) && (current != head)) {
				prev = prev->next;
				printf("LOOK AT THIS ======?\n");
			}
			current = current->next;
			next_node = current->next;
			
			printf("NEXT CYCLE \n");
		}
		else {
			next_node = next_node->next;
		}
		
	}
//========================================================



//iteration for error checking
	print_list(head, "all");

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