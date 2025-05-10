#include "database_commands.h"


//problem is that it is being assigned during re-writing to bin
//needs to be during creation during or after its sorted
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

/*
##########################SORTING COMMANDS#############################
TODO:
	Still needs to handle duplicates
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
	inorder_traversal_print(root);

	/*if (head != NULL) {
		bin_write(database_file, head);
	}*/

	//SEARCH TEST DELETE
	TreeNode* result = binary_tree_search(root, "U571");
	if (result == NULL) {
		printf("failed search\n");
	}
	else {
	printf("RESULT: %s\n", result->data.title);
	}
	
	
	free_linked_list(head);
	free_binary_tree(root);
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

//Will keep for now but may not be needed
MediaNode* search_linked_list_object(char* title) {
	char file_path[260] = { global_dir_path };
	char bin_file[6] = "a.bin";
	bin_file[0] = tolower(title[0]);
	strcat_s(file_path, 260, "\\");
	strcat_s(file_path, 260, bin_file);
	MediaNode* head = bin_read(file_path);
	
	while (head != NULL) {
		if (strcmp(title, head->data.title)) {
			return head;
		}
		head = head->next;
	}
}
	
//this will need some adjustment to account for when the tree is being read 
//from memory 
TreeNode* binary_tree_search(TreeNode* root, char* search_request) {
	if (root == NULL) {
		return NULL;
	}

	int search = strcmp(root->data.title, search_request);
	if (search == 0) {
		return root;
	}
	else if (search > 0) {
		return binary_tree_search(root->left, search_request);
	}
	else  {
		return binary_tree_search(root->right, search_request);
	}
}
//---------------------------------------------------------------------

void input_string_parsing(char* user_input) {
	//SEARCH 
	//ADD
	//DELETE
	//RENAME
	//CHANGE DESCRIPTION
	//
}