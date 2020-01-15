// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int		setup_client(void)
{
	int sock;
	struct sockaddr_in serv_addr;

	printf("%s\n", "socket");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	printf("%s\n", "inet_pton");
	if(inet_pton(AF_INET, "77.136.40.58", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	printf("%s%d\n", "Address : ", serv_addr.sin_addr);
	printf("%s\n", "connect");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	return (sock);
}

int main(int argc, char const *argv[])
{
	int valread;
	int sock;
	char *hello = "Hello from client";
	char buffer[1024] = {0};

	sock = setup_client();
	while (sock >= 0 && 1)
	{
		send(sock , hello , strlen(hello) , 0 );
		printf("Hello message sent\n");
		valread = read( sock , buffer, 1024);
		printf("read Client : %s\n",buffer );
	}
	return 0;
}
