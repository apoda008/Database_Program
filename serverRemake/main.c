#include "startup.h"
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

	if (startUp() == 0) {
		printf("Start up successful");
	}
	else {
		printf("ERROR ON START");
	}

	//Start of connection
	WSADATA wsaData;
	SOCKET database_socket, client_socket;
	struct sockaddr_in database_addr, client_addr;
	int client_len = sizeof(client_addr);
	char buffer[1024];
	
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	database_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (database_socket == INVALID_SOCKET) {
		printf("SOcket failed: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	database_addr.sin_family = AF_INET;
	database_addr.sin_port = htons(5001);
	database_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(database_socket, (SOCKADDR*)&database_addr, sizeof(database_addr)) == SOCKET_ERROR) {
		printf("Bind Failed: %d\n", WSAGetLastError());
		closesocket(database_socket);
		return 1;
	}
	//WTF IS IS EVEN SOMAXCONN
	//WHY DOES WINDOWS SUCK ASS TO CODE IN???
	listen(database_socket, SOMAXCONN);

	printf("Database Online \n");

	client_socket = accept(database_socket, (SOCKADDR*) &client_addr, &client_len);
	if (client_socket == INVALID_SOCKET) {
		printf("Accept failed: %d \n", WSAGetLastError());
		closesocket(database_socket);
		WSACleanup();
		return 1; 
	}

	printf("Connected to client: %s\n", inet_ntoa(client_addr.sin_addr));

	int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received > 0) {
		//this is where i will need to call the input parsing  
		//which will take in the request for whatever media info 
		//it wants
		buffer[bytes_received] = "\0";
		printf("Received: %s\n", buffer);
		
		/*
		* This is where it will be sent back 
		* might consider using cURL
		*/
		send(client_socket, "Hello", 17, 0);
	}

	closesocket(client_socket);
	closesocket(database_socket);
	
	return 0;
	



	/* TEST
	
	folder_files_delete();
	random_bin_file_create();
	char* test = "C:\\Users\\dan_a\\Desktop\\Database_folder";
	database_sort_all(test);
	*/
}