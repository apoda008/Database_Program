#include "mediaparse.h"

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

        if ( (newFileName[i] == ' ') && (newFileName[i+1] != '\0') ) {
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
    
    struct Memory* response = (struct Memory*) user_data;

    char *ptr = realloc(response->string, response->size + real_size + 1);

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
    
    //====this is solely to get the key for api call==== 
    //in real implentation this will ask for the users key
    FILE* file = fopen("C:\\Users\\dan_a\\Desktop\\key.txt", "r");
    if (file == NULL) {
        perror("TEMP FILE FAILED");
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
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *) &response); //needs to nulled for stdout
    
    //DELETE strictly for viewing what is being fully returned in the JSON, serves no function to the program overall
    //curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);

    curl_easy_setopt(hnd, CURLOPT_URL, search_buffer);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    headers = curl_slist_append(headers, authorization);
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    CURLcode ret = curl_easy_perform(hnd);
    
    if( ret != CURLE_OK) {
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

            cJSON *results = cJSON_GetObjectItemCaseSensitive(tmdb_json, "results");

            if (cJSON_IsArray) {
               
                cJSON* movie = NULL;

                /*
                TODO:
                will need to implement a query limit since tmdb requests less than 50
                queries a second
                */

                //grab first response and break out of loop
                cJSON_ArrayForEach(movie, results) {
                    cJSON *title = cJSON_GetObjectItemCaseSensitive(movie, "title");
                    cJSON *description = cJSON_GetObjectItemCaseSensitive(movie, "overview");
                    cJSON *id = cJSON_GetObjectItemCaseSensitive(movie, "id");
                    cJSON* genre_ids = cJSON_GetObjectItemCaseSensitive(movie, "genre_ids");
                    cJSON *media_type = cJSON_GetObjectItemCaseSensitive(movie, "media_type");

                    media_write(title, description, id, genre_ids, media_type, dir_position, create_folder_location);
                    break;
                    /*
                        NOTE: FOR INSTANCES WHERE THE MOVIE INFORMATION RETURNED WAS INCORRECT IT 
                        WILL HAVE TO BE FIXED BY THE USER AT A LATER DATE USING THE TO-BE-IMPLEMENTED 
                        DATABASE COMMANDS
                    */
                }
            }            
        }
    }//end of JSON parsing

    curl_easy_cleanup(hnd);
    free(response.string);

    return 1;
}//end of information_request 

///////////////////////////////////////////////////////////////
void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, char* dir_position, char* create_folder_location) {
    char* first_char_string = title->valuestring;
    char filename[6];

    //because strcat is a bitch
    filename[0] = tolower(first_char_string[0]);
    filename[1] = '.';
    filename[2] = 'b';
    filename[3] = 'i';
    filename[4] = 'n';
    filename[5] = '\0';

    char file_buffer[MAX_PATH] = "";
    strcat_s(file_buffer, MAX_PATH, create_folder_location);
    strcat_s(file_buffer, MAX_PATH, "\\");
    strcat_s(file_buffer, MAX_PATH, filename);
    
    printf("FILE_BUFFER: %s\n", file_buffer);

    bool is_movie = false;
    
    if (!strcmp(media_type->valuestring, "movie")) {
        is_movie = true;
    }
    
    //DIR NEEDS FILENAME WITH IT (CONCAT)
   
    MediaData temp = { 52, "", id->valuedouble, is_movie, {0}, "", ""};

    strcpy_s(temp.title, 260, title->valuestring);
    strcpy_s(temp.description, 2000, description->valuestring);
    strcpy_s(temp.dir_position_media, 260, dir_position);
    
    cJSON* genre_number;
    int i = 0;
    cJSON_ArrayForEach(genre_number, genre_ids) {       
        temp.genre_types[i] = (int)genre_number->valuedouble;
        i++;
    }
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

    ////////////////////END OF FILE WRITE//////////////////////////////////////
}//end of media_write
///////////////////////////////////////////////////////////////

/*
TODO:
Make a folder for all the db files. Ask where to store them?
move DB files to folder
Sort them after
return success or failure

*/

