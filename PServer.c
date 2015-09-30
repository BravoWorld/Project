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
#include <sys/stat.h>

#define LENGTH 512 

void error(char *msg)
{
	perror(msg);
	exit(0);
}

struct User{
	int Account;
	char Username[20];
	char Password[20];
	char Name[50];
};

struct folder{
	int numfile;
	int numacc;
	char fldrnm[50];
};



struct stat st = {0};

int main()
{
	int sock, nsockfd, length, fromlen, n;
	struct sockaddr_in server, from;
	struct User U[100];
	struct folder F[100];
	char buf[1024];
	int x,y = -1,j = 0, acc = 1001;
	char usern[20], pw[20], choice[20];
	int reglog, e;
	char record[200];
	char filep[100],filep2[100];
	char revbuf[LENGTH];
	FILE *fp;
	FILE *fpf;

	fp = fopen("database.txt","r");
	
	if(fp<0)
	{printf("Error");exit(0);}

	while(!feof(fp))
	{
		fscanf(fp,"%d %s %s %s\n",&U[j].Account,U[j].Username,U[j].Password,U[j].Name);
		j++;
	}
	fclose(fp);

	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		error("Opening Socket Error");
	}
	

	server.sin_family=AF_INET;
	server.sin_port = htons(5000);
        server.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server.sin_zero),8);
	
	if(bind(sock,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1)
	{
		error("binding");
	}
	fromlen = sizeof(struct sockaddr_in);


	do{
	n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	if(n<0){error("Receive Data Error");}
	printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
	reglog = strtol(buf,NULL,10);
		if(reglog==1){
			printf("Login \n");
		}
		else if(reglog==2){

			FILE *fp;
			fp = fopen("database.txt","w");
			printf("Register \n");
			U[j].Account = acc + j;
			printf("%d",j);
			n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
			if(n<0){error("Receive Data Error");}
			printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
			strcpy(U[j].Username, buf);
			printf("%s ", U[j].Username);

			n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
			if(n<0){error("Receive Data Error");}
			printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
			strcpy(U[j].Password, buf);
			printf("%s ", U[j].Password);

			n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
			if(n<0){error("Receive Data Error");}
			printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
			strcpy(U[j].Name, buf);
			printf("%s ", U[j].Name);
			j++;

			for(e=0;e<j;e++)
			{
				fprintf(fp,"%d %s %s %s\n",U[e].Account,U[e].Username,U[e].Password,U[e].Name);			
			}
			fclose(fp);
		}
	}while(reglog!=1);
	
	
	
	n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	if(n<0){error("Receive Data Error");}
	printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
	strcpy(usern, buf);
	printf("%s ", usern);
	n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	if(n<0){error("Receive Data Error");}
	printf("\nFrom (%s) : ",inet_ntoa(from.sin_addr));
	strcpy(pw, buf);
	printf("%s ", pw);

		for(x=0;x<j;x++)
		{
			if(strcmp(usern, U[x].Username) == 0 &&strcmp(pw, U[x].Password) == 0)
				y = x;
			printf("%d",y);
		}

		if(y!=-1)
		{
			strcpy(record,"Account Confirm\n User Name : ");
			strcat(record, usern);
			strcat(record, "\n");
			n = sendto(sock,record,1024,0,(struct sockaddr *)&from,sizeof(struct sockaddr));
			if(n<0){error("Send Error");}
		}
		else
		{
			n = sendto(sock,"There is not Account record\n",1024,0,(struct sockaddr *)&from,sizeof(struct sockaddr));
			if(n<0){error("Send Error");}
			exit(0);
		}

		strcpy(filep, usern);strcat(filep, "/");
		printf("%s\n",filep);
		if (stat(filep, &st) == -1) {
    					mkdir(filep, 0777);
		}
	while(1)
	{
		if(y==-1){break;}
		n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
		if(n<0){error("Receive Data Error");}
		int z;
		z = strtol(buf,NULL,10);
		switch(z)
		{
			case 1:
				
				break;
			case 2:
				
				break;
			case 3:
				fpf = fopen("database3.txt","a+");
				n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
				strcpy(filep2, filep);
				strcat(filep2, buf);
				if (stat(filep2, &st) == -1) {
    					mkdir(filep2, 0777);
					fprintf(fpf,"%d %s \n",U[y].Account ,filep2);
					strcpy(buf, "Created Directory");
					printf("\nFrom (%s) : Done",inet_ntoa(from.sin_addr));
					n = sendto(sock,buf,1024,0,(struct sockaddr *)&from,sizeof(struct sockaddr));
				}
				else{
					strcpy(buf, "Had a Same Directory Name");
					printf("\nFrom (%s) : Same Directory",inet_ntoa(from.sin_addr));
					n = sendto(sock,buf,1024,0,(struct sockaddr *)&from,sizeof(struct sockaddr));
				}
				fclose(fpf);
				break;
			case 4:
				break;
			case 5:
				return 0;
			default:
				break;
		}
	}
	return 0;
}
