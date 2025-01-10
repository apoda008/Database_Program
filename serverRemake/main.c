#include "startup.h"
#include "test_suite_db.h"


int main() {
	printf("starting up\n");
	/*
	TODO: scan for db files?
	
	*/
	//startUp();


	folder_files_delete();
	random_bin_file_create();
	char* test = "C:\\Users\\dan_a\\Desktop\\Database_folder";
	database_sort_all(test);
	
}