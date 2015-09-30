#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main()
{
	int sock, length, n, reglog = 0;
	struct sockaddr_in server, from;
	char buffer[1024];
	struct hostent *host;
	int choice,tot,tot2,sel;
	char user[20],pass[20];
	
	host= (struct hostent *) gethostbyname((char *)"127.0.0.1");
	
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		error("Opening Socket Error");
	}
	
	server.sin_family=AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server.sin_zero),8);

	length = sizeof(struct sockaddr_in);
	do{
	printf("1. Login \n2. Register \nPlease Select : ");
	scanf("%s",buffer);
	reglog = strtol(buffer,NULL,10);
	n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
	if(n<0){error("Send Error");}
	fflush(stdin);
		if(reglog==2){
			printf("Please enter Username: ");
			scanf("%s",buffer);
			n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
			if(n<0){error("Send Error");}
			fflush(stdin);
			printf("Please enter Password: ");
			scanf("%s",buffer);
			n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
			if(n<0){error("Send Error");}
			fflush(stdin);
			printf("Enter Your Name :");
			scanf("%s",buffer);
			n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
			if(n<0){error("Send Error");}
			fflush(stdin);
		}
	
	}while(reglog!=1);
	
	
	
	
	printf("Please enter Username: ");
	
	bzero(buffer,256);
	scanf("%s",buffer);
	n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
	if(n<0){error("Send Error");}

	printf("Please enter Password: ");
	bzero(buffer,256);
	scanf("%s",buffer);fflush(stdin);
	n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
	if(n<0){error("Send Error");}
	
	n=recvfrom(sock,buffer,256,0,(struct sockaddr *)&from,&length);
	if(n<0){error("Send Error");}
	printf("%s", buffer);

	while(buffer!="")
	{
		printf("\n===============================\n");
		printf("Please Select : \n");
		printf("1 : Upload File \n");
		printf("2 : Download File \n");
		printf("3 : Create Folder \n");
		printf("4 : Delete Folder \n");
		printf("5 : Exit\n");
		printf("\n===============================\n");
		scanf("%s",buffer);
		choice = strtol(buffer,NULL,10);
		n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);
		if(n<0){error("Send Error");}
		switch(choice)
		{
			case 1:
				break;	
			case 2:
				break;	
			case 3:
				printf("Enter the Directory Name : ");
				scanf("%s",buffer);
				n = sendto(sock,buffer,256,0,(struct sockaddr *)&server,length);if(n<0){error("Send Error");}
				n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from,&length);if(n<0){error("Send Error");}
				printf("%s", buffer);
				break;
			case 4:
				break;
			case 5:return 0;
			default:break;	
		}
		
	}	
	
	
	return 0;	
}
