#include "database_commands.h"

int db_count_global;
char global_dir_path[260];


/////////////////////////READ/WRITE////////////////////////////////////
MediaNode* bin_read(char* database_file) {
	printf("DATABASE FILE: %s\n", database_file);
	FILE* file = fopen(database_file, "rb");
	if (file == NULL) {
		perror("Failed to open file");
		return 0;
	}

	MediaNode* head = NULL;
	MediaNode* tail = NULL;
	MediaData temp;

	while ((fread(&temp, sizeof(MediaData), 1, file)) == 1) {
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
	return head;
}

void bin_write(char* database_file, MediaNode* head_ref) {
	FILE* file = fopen(database_file, "wb");
	if (file == NULL) {
		perror("Failed to open write file.");
		return 0;
	}

	int db_num = 1;
	MediaNode* current = head_ref;
	while (current != NULL) {
		current->data.db_position = db_num;
		db_num += 1;
		//delete
		printf("db_num in obj: %d", current->data.db_position);
		fwrite(&current->data, sizeof(MediaData), 1, file);
		current = current->next;
	}

	//THE WAY TO FIND GLOBAL WILL NEED TO BE FIXED -UPDATE-
	db_count_global = db_num;

	fclose(file);
}
///////////////////////////////////////////////////////////////////////


//*********************LIST/NODE MANAGEMENT****************************
//*********************************************************************
void free_linked_list(MediaNode* header) {
	MediaNode* current = header;
	
	while (current != NULL) {
		MediaNode* to_delete = current;
		current = current->next;
		free(to_delete);
		to_delete = NULL;
	}
}

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

void insert_node(MediaNode* node) {
	//TODO:
}

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

	//this part will need adjustment if comparison parameters chage 
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
//---------------------------TREES????-------------------------------------------
TreeNode* create_tree_node(MediaData item) {
	TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
	new_node->data = item;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}


TreeNode* sorted_to_bst(MediaNode** source, int start, int end) {
	if (start > end) {
		return NULL;
	}
	MediaNode* current = source;
	int mid = start + (end - start) / 2;
	TreeNode* left = sorted_to_bst(source, start, mid - 1);
	TreeNode* root = create_tree_node((*source)->data);
	root->left = left;
	*source = (*source)->next;
	root->right = sorted_to_bst(source, mid + 1, end);

	return root;

}

void inorder_traversal(TreeNode* root) {
	if (root == NULL) {
		return;
	}
	printf("ROOT: Title: %s\n", root->data.title);
	inorder_traversal(root->left);

	printf("Title: %s\n", root->data.title);
	
	inorder_traversal(root->right);
}

//*********************************************************************
//*********************************************************************


/*
##########################SORTING COMMANDS#############################
*/
int database_sort_individual(char* database_file) {
		
	MediaNode* head = bin_read(database_file);

	merge_sort(&head);
	
	//Temporary solution until a global int size can be developed
	MediaNode* current = head;
	int n = 0;
	while (current != NULL) {
		printf("TITLE ORDERED : % s\n", current->data.title);
		n += 1;
		current = current->next;
	}

	//printf("GLOBAL %d", db_count_global);
	TreeNode* root = sorted_to_bst(&head, 0, n - 1);
	inorder_traversal(root);

	/*if (head != NULL) {
		bin_write(database_file, head);
	}*/
	
	free_linked_list(head);
		
	return 1;
}

int database_sort_all(char* folder_location) {
	/*
	TODO:
	will need adjust for non alphabet characters.
	consider designing like SQL
	may need additional DB files such as genre, movie/show, description
	*/

	//assigns global dir path for searching
	strcpy_s(global_dir_path, 260, folder_location);
	
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

		//DELETE
		printf("strcat %s\n", file_first);
		printf("GLOBAL: %s\n", global_dir_path);

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

/*
##########################SORTING COMMANDS#############################
*/

//------------------------Search Commands------------------------------

MediaNode* search_linked_list_object(char* title) {
	char file_path[260] = global_dir_path;
	char bin_file[6] = "a.bin";
	bin_file[0] = tolower(title[0]);
	strcat_s(file_path, 260, "\\");
	strcat_s(file_path, 260, bin_file);
	MediaNode* head = bin_read(title);
	
	while (head != NULL) {
		if (strcmp(title, head->data.title)) {
			return head;
		}
		head = head->next;
	}
}
	
MediaNode* search_database_obj(void* requested_info);

//---------------------------------------------------------------------