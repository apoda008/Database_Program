#include "startup.h"




void folder_files_delete() {

    WIN32_FIND_DATA findData;
    HANDLE hFind;

    TCHAR folder_path[MAX_PATH];
    
    StringCchCopy(folder_path, MAX_PATH, TEXT("C:\\Users\\dan_a\\Desktop\\Database_folder\\*.*"));
    
    hFind = FindFirstFile(folder_path, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open directory %s\n", folder_path);
        return;
    }

    do {
        TCHAR file_path[MAX_PATH] = _T("C:\\Users\\dan_a\\Desktop\\Database_folder\\");
        
        // Skip "." and ".."
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }
        StringCchCat(file_path, MAX_PATH, findData.cFileName);    
        _tprintf(TEXT("the thing directory: %s\n"), file_path);

        if (DeleteFile(file_path)) {
            printf("Deleted: %ls\n", findData.cFileName);
        }
        else {
            printf("Failed to delete: %ls (Error %lu)\n", file_path, GetLastError());
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void random_bin_file_create() {
    //MediaData fake_file_one = { 0, "Sleeping Beauty", 1010, 1, {0}, "This is a movie about sleeping beauty", "Some/Random/Dir/Pos"};
    //MediaData fake_file_two = { 0, "Fired Up", 1000, 1, {0}, "Funny chearleaders", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_three = { 0, "Harry Potter 1", 1040, 1, {0}, "wizard boy gets violated by a hat", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_four = { 0, "Firewalker", 55, 1, {0}, "I think it has something to do with indians?", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_five = { 0, "Firefox", 25, 1, {0}, "Literally no idea what this movie is about", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_six = { 0, "Flight", 10500, 1, {0}, "something about a plane", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_seven = { 0, "Harry Potter 2", 1050, 1, {0}, "Worlds greatest wizard's ghost loses second fight with wizard boy and his crew", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_eight = { 0, "Fool's Gold", 84, 1, {0}, "Treasure hunting but with adults", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_nine = { 0, "U571", 1010, 1, {0}, "Submarine battles", "Some/Random/Dir/Pos" };;
    //MediaData fake_file_ten = { 0, "Fistful of Dollars", 99, 1, {0}, "Again literally no idea what this movie is about", "Some/Random/Dir/Pos" };

    MediaData array[10] = {
        { 0, "Sleeping Beauty", 1010, 1, {0}, "This is a movie about sleeping beauty", "Some/Random/Dir/Pos"},
        { 0, "Fired Up", 1000, 1, {0}, "Funny chearleaders", "Some/Random/Dir/Pos" },
        { 0, "Harry Potter 1", 1040, 1, {0}, "wizard boy gets violated by a hat", "Some/Random/Dir/Pos" },
        { 0, "Firewalker", 55, 1, {0}, "I think it has something to do with indians?", "Some/Random/Dir/Pos" },
        { 0, "Firefox", 25, 1, {0}, "Literally no idea what this movie is about", "Some/Random/Dir/Pos" },
        { 0, "Flight", 10500, 1, {0}, "something about a plane", "Some/Random/Dir/Pos" },
        { 0, "Harry Potter 2", 1050, 1, {0}, "Worlds greatest wizard's ghost loses second fight with wizard boy and his crew", "Some/Random/Dir/Pos" },
        { 0, "Fool's Gold", 84, 1, {0}, "Treasure hunting but with adults", "Some/Random/Dir/Pos" },
        { 0, "U571", 1010, 1, {0}, "Submarine battles", "Some/Random/Dir/Pos" },
        { 0, "Fistful of Dollars", 99, 1, {0}, "Again literally no idea what this movie is about", "Some/Random/Dir/Pos" }
    };

    FILE* file = fopen("C:\\Users\\dan_a\\Desktop\\Database_folder\\a.bin", "wb");

    if (file == NULL) {
        perror("error opening file in test\n");
    }
    
    //adjust i for number of objects
    for (int i = 0; i < 5; i++) {
        fwrite(&array[i], sizeof(MediaData), 1, file);
    }
    fclose(file);
}

