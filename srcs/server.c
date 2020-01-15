// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 51110

int		setup_server(void)
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
	printf("%s\n", "SOCKET");
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	printf("%s\n", "setsockopt");
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	printf("%s\n", "bind");
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", "listen");
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("%s%d\n", "Address : ", address.sin_addr.s_addr);
	printf("%s\n", "accept");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", "END");
	return (new_socket);
}


int main(int argc, char const *argv[])
{
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	int new_socket, valread;

	new_socket = setup_server();
	while (new_socket >= 0 && 1)
	{
		valread = read( new_socket , buffer, 1024);
		printf("read Server : %s\n",buffer );
		send(new_socket , hello , strlen(hello) , 0 );
	}
	printf("Hello message sent\n");
	return 0;
}
