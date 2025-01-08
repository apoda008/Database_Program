#include "startup.h"



int main() {
	printf("starting up\n");
	/*
	TODO: scan for db files?
	
	*/
	startUp();

	char* test = "C:\\Users\\dan_a\\Desktop\\Database_folder";
	database_sort_all(test);

}