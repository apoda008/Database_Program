#include "startup.h"

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
        return 0;
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


    return 1;
}


int startUp() {

    //printf("Starting up Media DataBase \n");
    //printf("Please enter the location of the main directory to be added\n");
    ////scanf_s("%100s", userInput);
    ////printf("Format: C:directory\...\n");
    //printf("scanning...\n");
    //printf("\n");

    /*
        Will need to ask if it a show/movie folder
        unforunately there is no way currently to get that 
        information just from the media files themselves. At
        least not consistently
    */


    //THIS CAN PROBABLY BE DELETED
    //char cwd[FILENAME_MAX];
    //if (_getcwd(cwd, sizeof(cwd)) != NULL) {
    //    printf("Current working directory: %s\n", cwd);
    //}
    //else {
    //    perror("getcwd() error");
    //}
    //Folder creation
    TCHAR folder_creation_stand_in[MAX_PATH];

    StringCchCopy(folder_creation_stand_in, MAX_PATH, TEXT("C:\\Users\\dan_a\\Desktop\\Database_folder"));
    //wchar_t folder_creation_stand_in[MAX_PATH] = L"C:\\Users\\dan_a\\Desktop\\Database_folder";

    if (CreateDirectory(folder_creation_stand_in, NULL)) {
        _tprintf(TEXT("Folder created successfully: %s\n"), folder_creation_stand_in);

    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            _tprintf(TEXT("Folder already exists: %s\n"), folder_creation_stand_in);
            //printf("Folder already created: %s\n", folder_creation_stand_in);
        }
        else {
            _tprintf(TEXT("failed: %s\n"), folder_creation_stand_in);
            //printf("Failed to create folder. Error Code: %lu\n", error);
        }
    }




    //THIS WILL PROBABLY BE ADJUSTED WHEN ASKING FOR USER INPUT
    TCHAR test_Dir[MAX_PATH];
    
    StringCchCopy(test_Dir, MAX_PATH, TEXT("C:\\Users\\dan_a\\Desktop\\Personal_Projects\\HomeServerProject\\testdirfolder"));
    _tprintf(TEXT("The Inputed String: %s\n"), test_Dir);
    /////////////////////////////////////////////////////////
    if (directorySearch(test_Dir, folder_creation_stand_in) != 1) {
        printf("Database creation failed \n");
    }
    else {
        printf("Database created \n");
    }
  
    return 0;

}
