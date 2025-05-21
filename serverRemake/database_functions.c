#include "database_functions.h"

//needs to be during creation during or after its sorted
int db_count_global;

//this is to track the current amount of TMDB requests
int tmdb_int_global;

//for holding pathing after the folders have been made
struct Master_Directory master_pathing;

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

		for (int i = 0; i < 19; i++) {
			
		}
	}

	fclose(file);
	return head;
}

void bin_write(char* database_file, MediaNode* head_ref) {
	FILE* file = fopen(database_file, "wb");
	if (file == NULL) {
		perror("Failed to open write file.");
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
*######################################################################
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
	/////////////////////
	
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
	//strcpy_s(global_dir_path, 260, folder_location);
	
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
		printf("GLOBAL: %s\n", master_pathing.movie_bin_path);

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
####################END OFSORTING COMMANDS#############################
#######################################################################
*/



//---------------------------------------------------------------------
//------------------------Search Commands------------------------------

//Will keep for now but may not be needed
MediaNode* search_linked_list_object(char* item_to_be_searched) {
	char file_path[MAX_PATH];
	
	size_t converted;
	wcstombs_s(&converted, file_path, MAX_PATH, master_pathing.movie_bin_path, _TRUNCATE);
	
	char bin_file[6] = "a.bin";
	bin_file[0] = tolower(item_to_be_searched[0]);
	
	//DELETE
	//printf("Bin_FILE for SEARCH: %s\n", bin_file);
	//printf("_FILE_PATH for SEARCH: %s\n", file_path);
	
	strcat_s(file_path, 260, "\\");
	strcat_s(file_path, 260, bin_file);
	
	//DELETE
	printf("FILE_PATH: %s\n", file_path);
	
	MediaNode* head = bin_read(file_path);
	
	while (head != NULL) {
		if (strcmp(item_to_be_searched, head->data.title) == 0) {
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


//---------------------END SEARCH COMMANDS------------------------------
//----------------------------------------------------------------------


/*
#####################USER INPUT FUNCTIONS################################
*/
cJSON* get_media(const char* title) {
	/*for simplicity right now it is done in Link Lists
	* in the future this will more than likely be adjusted to 
	* binary trees as well as returning specific things vs the 
	* entire node
	*/
	MediaNode* new_node = search_linked_list_object(title);

	cJSON* json_node = cJSON_CreateObject();
	if (!json_node) {
		printf("JSON initialization failed\n");
		return NULL;
	}
	cJSON_AddNumberToObject(json_node, "db_position", new_node->data.db_position);
	cJSON_AddStringToObject(json_node, "title", new_node->data.title);
	cJSON_AddNumberToObject(json_node, "tmdb_id", new_node->data.tmdb_id);
	cJSON_AddBoolToObject(json_node, "media_type", new_node->data.media_type);
	
	cJSON* genre_array = cJSON_CreateIntArray(new_node->data.genre_types, 19);
	if (!genre_array) {
		printf("Array generation failed\n");
		cJSON_Delete(json_node);
		return NULL;
	}
	cJSON_AddItemToObject(json_node, "genre_types", genre_array);
	cJSON_AddStringToObject(json_node, "description", new_node->data.description);
	cJSON_AddStringToObject(json_node, "dir_position_media", new_node->data.dir_position_media);

	//DELETE
	//char* j_print = cJSON_Print(json_node);
	//printf("JSON: %s", j_print);

	return json_node;

 }

//This is the JSON return API interface. There will be another for the LOCAL 
cJSON* input_string_parsing(char* user_input) {
	/*
	TODO:
	this will be adjusted after completion of skeleton app server and app
	ADD
	RENAME
	CHANGE DESCRIPTION
	DELETE
	GET
	
	*/
	char* context;
	char* token = strtok_s(user_input, " ", &context);
	
	//DELETE
	printf("1st token: %s\n", token);
	
	int tracker = 0;
	while (token != NULL) {
		if (strcmp(token, "ADD") == 0) {
			//TODO  
			break;
		}
		if (strcmp(token, "CHANGE") == 0) {
			token = strtok_s(NULL, " ", &context);
			if (strcmp(token, "DESCRIPTION") == 0) {
				//TODO
				break;
			}
			if (strcmp(token, "TITLE") == 0) {
				break;
				//TODO
			}
			if (strcmp(token, "GENRE") == 0) {
				break;
				//TODO
			}
		}		
		if (strcmp(token, "DELETE") == 0) {
			//TODO
		}
		if (strcmp(token, "GET") == 0) {
			/*Next token should be "Title, discription, genre
			* type, etc. 			
			*/
			token = strtok_s(NULL, " ", &context);
			
			//DELETE
			printf("2nd Token %s\n", token);
			
			if (strcmp(token, "TITLE") == 0) {
				//This will be the title
				token = strtok_s(NULL, " ", &context);
				
				//DELETE
				printf("3rd token: %s\n", token);
				
				cJSON* parsed_to_json = get_media(token);

				
				
				return parsed_to_json;


			}		
		}
		tracker += 1;
		token = strtok_s(NULL, " ", &context);
	}
}
//-----------------END OF USER FUNCTIONS-------------------------------- 



/*
#####################SOCKET//CONNECTION FUNCTIONS###############
*/

void api_connection() {
	//Start of connection
	WSADATA wsaData;
	SOCKET database_socket, client_socket;
	struct sockaddr_in database_addr, client_addr;
	int client_len = sizeof(client_addr);
	char buffer[4096] = { 0 };

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	database_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (database_socket == INVALID_SOCKET) {
		printf("Socket failed: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	database_addr.sin_family = AF_INET;
	database_addr.sin_port = htons(5001);
	//database_addr.sin_addr.s_addr = INADDR_ANY;
	
	//this will need to be adjusted for flexibilty;
	inet_pton(AF_INET, "192.168.4.81", &database_addr.sin_addr);

	if (bind(database_socket, (SOCKADDR*)&database_addr, sizeof(database_addr)) == SOCKET_ERROR) {
		printf("Bind Failed: %d\n", WSAGetLastError());
		closesocket(database_socket);
		return 1;
	}
	//WTF IS IS EVEN SOMAXCONN
	//WHY DOES WINDOWS SUCK ASS TO CODE IN???
	listen(database_socket, SOMAXCONN);

	printf("Database Online \n\n");

	client_socket = accept(database_socket, (SOCKADDR*)&client_addr, &client_len);
	if (client_socket == INVALID_SOCKET) {
		printf("Accept failed: %d \n", WSAGetLastError());
		closesocket(database_socket);
		WSACleanup();
		return 1;
	}

	while (1) {
		int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received > 0) {
			//this is where i will need to call the input parsing  
			//which will take in the request for whatever media info 
			//it wants
			buffer[bytes_received] = "\0";
			printf("Received: %s\n", buffer);

			if (strcmp(buffer, "EXIT") == 0) {
				break;
			}

			cJSON* result = input_string_parsing(buffer);
			char* j_print = cJSON_Print(result);
			printf("sending (as JSON) %s\n", j_print);
			//got to watch buffer size here, it can cut messages short
			//need to find a better way to dynamically alot the correct 
			//size for delivery. Hard coding doesn't allow for flexibilty
			send(client_socket, j_print, 4096, 0);
			//send(client_socket, "Hello", 6, 0);
		}
	}

	closesocket(client_socket);
	closesocket(database_socket);

	return 0;

}

/*
#####################END OF CONNECTION ##########################
*/



///////////////////////DIRECTORY STUFF///////////////////////////

int directorySearch(char* main_start, char* create_folder_location) {


	TCHAR dir_for_search[MAX_PATH];

	//To pass into parse_for_database
	char* dir_path[MAX_PATH];
	char* folder_path[MAX_PATH];
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, main_start, -1, dir_path, sizeof(dir_path), NULL, NULL);
	int sizeNeeded_folder = WideCharToMultiByte(CP_UTF8, 0, create_folder_location, -1, folder_path, sizeof(folder_path), NULL, NULL);

	errno_t result = _tcscpy_s(dir_for_search, _countof(dir_for_search), main_start);

	//concats wildcard (*) for full search
	StringCchCat(main_start, MAX_PATH, TEXT("\\*"));

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(main_start, &findFileData);

	//DELETE 
	_tprintf(TEXT("the thing directory: %s\n"), main_start);


	if (hFind == INVALID_HANDLE_VALUE) {
		printf("error handle value. Error: %lu\n", GetLastError());
		return 1;
	}
	else {
		do {

			//skip "." ".." file names
			if ((strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0)) {
				continue;
			}

			//puts the string into a readable form for directory and file names
			char narrowDir[MAX_PATH];
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, narrowDir, sizeof(narrowDir), findFileData.cFileName, _TRUNCATE);


			// Check if it's a directory or file
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				printf("[DIR]  %s\n", narrowDir);

				StringCchCat(dir_for_search, MAX_PATH, TEXT("\\"));
				StringCchCat(dir_for_search, MAX_PATH, findFileData.cFileName);

				directorySearch(&dir_for_search, create_folder_location);

			}
			else {
				printf("[FILE] %s\n", narrowDir);

				parse_for_database(narrowDir, dir_path, folder_path);
			}


		} while (FindNextFile(hFind, &findFileData) != 0); //this moves it to the next file

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			printf("FindNextFile Failed (%d)\n", GetLastError());
		}

		FindClose(hFind);
	}


	return 0;
}

//////////////////////END OF DIRECTORY///////////////////////////



//--------------------START UP FUNCTION-----------------------------
int create_folders(char* input_user_location) {
	/*This may be possible broken into smaller functions*/
		/*
		TODO:
		Make folders for the nodes .bin files
		make folder for genre .bin files
		make temp for error resolution (name collisions or name errors);
		Database_folder
			-database_movies
				-The Avengers
				-Fired Up
				-Harry Potter
				-....
			-database_shows
				-south park
				-band of brother
				-.....
			-genres
				-comedy
				-drama
				....
	*/

	if (!input_user_location) return 1;
	TCHAR master_path[MAX_PATH];
	size_t converted = 0;
	mbstowcs_s(&converted, master_path, MAX_PATH, input_user_location, _TRUNCATE);
	
	_tprintf(_T("Converted path: %s\n"), master_path);
	
	//sets global dir path
	_tcscpy_s(master_pathing.movie_bin_path, MAX_PATH, master_path);
	
	//DB folder creations
	if (CreateDirectory(master_path, NULL)) {
		_tprintf(TEXT("Folder created successfully: %s\n"), master_path);
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS) {
			_tprintf(TEXT("Database folder already exists: %s\n"), master_path);
		}
		else {
			_tprintf(TEXT("failed: %s\n"), master_path);
			return 1;
		}
	}
	
	//creates series folder
	TCHAR series_path[MAX_PATH];
	_tcscpy_s(series_path, MAX_PATH, master_path);

	_tcscat_s(series_path, MAX_PATH, _T("\\Series"));

	//sets global series dir
	_tcscpy_s(master_pathing.series_bin_path, MAX_PATH, series_path);
	
	if (CreateDirectory(series_path, NULL)) {
		_tprintf(TEXT("Series folder created successfully: %s\n"), series_path);
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS) {
			_tprintf(TEXT("Database folder already exists: %s\n"), series_path);
		}
		else {
			_tprintf(TEXT("failed: %s\n"), series_path);
			return 1;
		}
	}

	//creates genre folder
	TCHAR genre_path[MAX_PATH];
	_tcscpy_s(genre_path, MAX_PATH, master_path);

	_tcscat_s(genre_path, MAX_PATH, _T("\\Genre"));

	//sets global dir path for genre
	_tcscpy_s(master_pathing.genre_path, MAX_PATH, genre_path);

	if (CreateDirectory(genre_path, NULL)) {
		_tprintf(TEXT("Series folder created successfully: %s\n"), genre_path);
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS) {
			_tprintf(TEXT("Database folder already exists: %s\n"), genre_path);
		}
		else {
			_tprintf(TEXT("failed: %s\n"), genre_path);
			return 1;
		}
	}

	return 0;
}

int startUp() {

	//this will take user input eventually
	create_folders("C:\\Users\\dan_a\\Desktop\\Database_folder");
	_tprintf(TEXT("The global dir String: %s\n"), master_pathing.movie_bin_path);
	_tprintf(TEXT("The global series String: %s\n"), master_pathing.series_bin_path);
	_tprintf(TEXT("The global genre String: %s\n"), master_pathing.genre_path);


	//THIS WILL PROBABLY BE ADJUSTED WHEN ASKING FOR USER INPUT
	TCHAR test_Dir[MAX_PATH];
	TCHAR folder_creation_stand_in_Main[MAX_PATH];
	
	StringCchCopy(folder_creation_stand_in_Main, MAX_PATH, TEXT("C:\\Users\\dan_a\\Desktop\\Database_folder"));
	StringCchCopy(test_Dir, MAX_PATH, TEXT("C:\\Users\\dan_a\\Desktop\\Personal_Projects\\HomeServerProject\\testdirfolder"));
	
	_tprintf(TEXT("The Inputed String: %s\n"), test_Dir);
	
	/////////////////////////////////////////////////////////
	if (directorySearch(test_Dir, folder_creation_stand_in_Main) != 0) {
		printf("Database creation failed \n");
	}
	else {
		printf("Database created \n");
	}

	return 0;

}

//-------------------END OF START UP--------------------------------





//---------------------MEDIA PARSE FUNCTIONS--------------------

//cleans up the title for TMDB api request
void parse_for_database(char* filename, char* dir_position, char* create_folder_location) {
	size_t nameLen = strlen(filename);
	char newFileName[260];

	/*
	TODO:
	File verification: make sure file is of a media type (.avi, .mkv, etc)
	tracker of those failed files (number/location?)
	maybe move them to a separate folder?
	the steralization will also need adjustment for characters such as '_'
	*/

	for (int i = 0; i < (nameLen - 3); i++) {
		if (filename[i] == '.') {
			newFileName[i] = ' ';
			if ((isdigit(filename[i + 1])) && (isdigit(filename[i + 2])) && (isdigit(filename[i + 3]))) {
				newFileName[i] = '\0';
				break;
			}
		}
		else if (filename[i] == '(') {
			newFileName[i] = '\0';
			break;
		}
		else {
			newFileName[i] = filename[i];
		}
	}
	//removes .filetype
	newFileName[nameLen - 3] = '\0';

	char buffertwo[260];
	size_t j = 0;

	for (int i = 0; i < nameLen; i++) {
		if ((newFileName[i] == ' ') && (newFileName[i + 1] == '\0')) {
			break;
		}

		if ((newFileName[i] == ' ') && (newFileName[i + 1] != '\0')) {
			buffertwo[j] = '%';
			buffertwo[j + 1] = '2';
			buffertwo[j + 2] = '0';
			j += 3;
		}
		else {
			buffertwo[j] = newFileName[i];
			j++;
		}

	}
	buffertwo[j] = '\0';

	//Will also call the file writing function
	information_Request(buffertwo, dir_position, create_folder_location);


}

//////////////////////////////CURL AND JSON PARSING/////////////////////////////

//allocates memory for incoming response.
size_t write_chunk(void* data, size_t size, size_t nmemb, void* user_data) {
	size_t real_size = size * nmemb;

	struct Memory* response = (struct Memory*)user_data;

	char* ptr = realloc(response->string, response->size + real_size + 1);

	if (ptr == NULL) {
		return CURL_WRITEFUNC_ERROR;
	}

	response->string = ptr;
	memcpy(&(response->string[response->size]), data, real_size);
	response->size += real_size;
	response->string[response->size] = '\0';

	return real_size;
}

//sourced by themoviedb.org api system
void information_Request(const char* movie_title, char* dir_position, char* create_folder_location) {

	//tmbd request control 
	if (tmdb_int_global >= 38) {
		printf("Reached maximum TMDB requests, please wait\n\n");
		Sleep(10000);
		tmdb_int_global = 0;
	}
	
	//====this is solely to get the key for api call==== 
	//in real implentation this will ask for the users key
	FILE* file = fopen("C:\\Users\\dan_a\\Desktop\\key.txt", "r");
	if (file == NULL) {
		perror("TEMP FILE FAILED\n");
	}
	char authorization[267] = "";

	fgets(authorization, 267, file);
	fclose(file);
	//==================================================

	CURL* hnd = curl_easy_init();

	char search_buffer[260] = "";
	snprintf(search_buffer, sizeof(search_buffer), "https://api.themoviedb.org/3/search/multi?query=%s&include_adult=false&language=en-US", movie_title);
	
	if (!hnd) {
		fprintf(stderr, "Error with curl initialization \n");
		return 0;
	}

	//object for response
	struct Memory response;
	response.string = malloc(1);
	response.size = 0;

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_chunk); //needs to be nulled for stdout
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void*)&response); //needs to nulled for stdout

	//DELETE strictly for viewing what is being fully returned in the JSON, serves no function to the program overall
	//curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);

	curl_easy_setopt(hnd, CURLOPT_URL, search_buffer);

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "accept: application/json");
	headers = curl_slist_append(headers, authorization);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	CURLcode ret = curl_easy_perform(hnd);
	
	if (ret != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
	}
	else {
		//JSON parsing
		cJSON* tmdb_json = cJSON_Parse(response.string);
		if (!tmdb_json) {
			fprintf(stderr, "Error parsing JSON\n");
			return 0;
		}
		else {

			cJSON* results = cJSON_GetObjectItemCaseSensitive(tmdb_json, "results");

			if (cJSON_IsArray) {

				cJSON* movie = NULL;

				//grab first response and break out of loop
				cJSON_ArrayForEach(movie, results) {
					//DELETE
					//char* json_str2 = cJSON_Print(movie);
					//printf("THING: %s\n", json_str2);
					
					cJSON* title = cJSON_GetArrayItem(movie, 3);
					cJSON* description = cJSON_GetObjectItemCaseSensitive(movie, "overview");
					cJSON* id = cJSON_GetObjectItemCaseSensitive(movie, "id");
					cJSON* genre_ids = cJSON_GetObjectItemCaseSensitive(movie, "genre_ids");
					cJSON* media_type = cJSON_GetObjectItemCaseSensitive(movie, "media_type");
					
					media_write(title, description, id, genre_ids, media_type, dir_position, create_folder_location);
					break;
	
					//media_write(title, description, id, genre_ids, media_type, dir_position, create_folder_location);
					
					/*
						NOTE: FOR INSTANCES WHERE THE MOVIE INFORMATION RETURNED WAS INCORRECT IT
						WILL HAVE TO BE FIXED BY THE USER AT A LATER DATE USING THE TO-BE-IMPLEMENTED
						DATABASE FUNCTIONS
					*/
				}
			}
		}
	}//end of JSON parsing

	curl_easy_cleanup(hnd);
	free(response.string);
	tmdb_int_global += 1;
	return 0;
}//end of information_request 


void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, char* dir_position, char* create_folder_location) {
	char* first_char_string = title->valuestring;
	char filename[6] = "a.bin";
	
	//because strcat is a bitch
	filename[0] = tolower(first_char_string[0]);
	
	char file_buffer[MAX_PATH] = "";
	strcat_s(file_buffer, MAX_PATH, create_folder_location);
	strcat_s(file_buffer, MAX_PATH, "\\");

	//set global bin file path for quick reference 
	//strcpy_s(master_pathing.movie_bin_path, MAX_PATH, file_buffer);
	
	strcat_s(file_buffer, MAX_PATH, filename);

	//DELETE
	printf("FILE_BUFFER: %s\n", file_buffer);

	//set global bin file path for quick reference 
	
	//printf("GLOBAL DIR PATH: %s", master_pathing.movie_bin_path);
	_tprintf(_T("GLOBAL DIR PATH: %s\n"), master_pathing.movie_bin_path);
	
	bool is_movie = false;

	if (!strcmp(media_type->valuestring, "movie")) {
		is_movie = true;
	}

	MediaData temp = { 52, "", id->valuedouble, is_movie, {0}, "", "" };

	strcpy_s(temp.title, 260, title->valuestring);
	strcpy_s(temp.description, 2000, description->valuestring);
	strcpy_s(temp.dir_position_media, 260, dir_position);

	cJSON* genre_number;
	int i = 0;
	cJSON_ArrayForEach(genre_number, genre_ids) {
		temp.genre_types[i] = (int)genre_number->valuedouble;
		i++;
		printf("GENRE: %d\n", temp.genre_types);
		switch (temp.genre_types[i]) {
			case ACTION:
				genre_write("ACTION", temp.title);
				break;
			case ADVENTURE:
				genre_write("ADVENTURE", temp.title);
				break;
			case ANIMATION:
				genre_write("ANIMATION", temp.title);
				break;
			case COMEDY:
				genre_write("COMEDY", temp.title);
				break;
			case CRIME:
				genre_write("CRIME", temp.title);
				break;
			case DOCUMENTARY:
				genre_write("DOCUMENTARY", temp.title);
				break;
			case DRAMA:
				genre_write("DRAMA", temp.title);
				break;
			case FAMILY:
				genre_write("FAMILY", temp.title);
				break;
			case FANTASY:
				genre_write("FANTASY", temp.title);
				break;
			case HISTORY:
				genre_write("HISTORY", temp.title);
				break;
			case HORROR:
				genre_write("HORROR", temp.title);
				break;
			case MUSIC:
				genre_write("MUSIC", temp.title);
				break;
			case MYSTERY:
				genre_write("MYSTERY", temp.title);
				break;
			case ROMANCE:
				break;
				genre_write("ROMANCE", temp.title);
			case SCIENCE_FICTION:
				genre_write("SCIENCE_FICTION", temp.title);
				break;
			case TV_MOVIE:
				genre_write("TV_MOVIE", temp.title);
				break;
			case THRILLER:
				genre_write("THRILLER", temp.title);
				break;
			case WAR:
				genre_write("WAR", temp.title);
				break;
			case WESTERN:
				genre_write("WESTER", temp.title);
				break;		
		}
	}

	//DELETE
	printf("\n");

	cJSON_Delete(genre_number);

	////////////////FILE WRITE///////////////////////

	FILE* file = fopen(file_buffer, "ab");

	if (file == NULL) {
		perror("error opening file");
		return 0;
	}

	fwrite(&temp, sizeof(MediaData), 1, file);


	fclose(file);

}//end of media_write

void genre_write(char* genre_type, char* title) {

	//this needs adjustment at file open 
	size_t converted;
	char file_path[MAX_PATH];
	wcstombs_s(&converted, file_path, MAX_PATH, master_pathing.genre_path, _TRUNCATE);
	
	strcat_s(file_path, MAX_PATH, "\\");
	strcat_s(file_path, MAX_PATH, genre_type);
	strcat_s(file_path, MAX_PATH, ".bin");

	printf("ENTERED genre_WRITE\n");

	//needs error checking
	FILE* genre = fopen(file_path, "ab");
	
	fwrite(&title, sizeof(MAX_PATH), 1, genre);
	
	fclose(genre);

}
/////////////////////END OF CURL AND JSON PARSING/////////////////////////////////

