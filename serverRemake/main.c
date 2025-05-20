#include "database_functions.h"
#include "test_suite_db.h"


int main() {
	printf("starting up\n");
	/*
	TODO:
		-Runs start up(), which will find the media files through a directed input. Then parse those
		files, and put them into MediaData format and write that format to a .bin file.
		-intiates TCP connection for user inputs and awaits further commands
		-terminates when it receives an exit command
	*/

	//if (startUp() == 0) {
	//	printf("Start up successful");
	//}
	//else {
	//	printf("ERROR ON START");
	//}





	// TEST
	
	folder_files_delete();
	random_bin_file_create();
	char* test = "C:\\Users\\dan_a\\Desktop\\Database_folder";
	database_sort_all(test);
	
}