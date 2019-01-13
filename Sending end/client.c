#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFERT 512

int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_client_socket (int port, char* ipaddr);

struct sockaddr_in sock_serv;

int main (int argc, char**argv){
	struct timeval start, stop, delta;
    int sfd,fd;
    char buf[BUFFERT];
    off_t count=0, m,sz;//long
	long int n;
	//exec: client 127.0.0.1 15678 test.pdf keyfile.key
    int l=sizeof(struct sockaddr_in);
	struct stat buffer;
    
	/*if (argc != 4){
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}*/
    
    sfd=create_client_socket(15678, argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    
	if (stat(argv[3],&buffer)==-1){
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		sz=buffer.st_size;
    
	bzero(&buf,BUFFERT);
    
    if(connect(sfd,(struct sockaddr*)&sock_serv,l)==-1){
        perror("connection error\n");
        exit (3);
    }

	//start
	{
        //char *query="md5sum "+ argv[2] + " > md5sum.txt";
        char query[200]="md5sum ";
        snprintf(query, sizeof(query),"%s%s%s","md5sum ",argv[3]," > md5sum.txt");
        //snprintf( target, sizeof( target ), "%s%s%s", str1, str2, str3 );     
        printf("Command created successfully");
        printf("%s",query);
        system(&query);
    }
    // MD5 done

	 // DES encryption
    {
        char query[200]="md5sum ";
        snprintf(query, sizeof(query),"%s%s%s%s%s%s%s","./run_des.o -e ",argv[4]," ",argv[3]," ",argv[3],".enc");
        //command formed
        
        printf(query);
		printf("\n");
        //debugging print
        
        system(query);
    }
    // executed DES encryption
    printf("DES executed successfully");

   //zip the two files
    {
        char *cmd="zip ";//+argv[2]+".zip "+argv[2]+".enc md5sum.txt";
        //start concatenation
		char query[200]="md5sum ";
        snprintf(query, sizeof(query),"%s%s%s%s%s%s","zip ",argv[3],".zip ",argv[3],".enc ","md5sum.txt");
        //command formed
        //concatenation over
        //debugging info
        //execute commans
        system(query);
    }
    // send this zip file
	//end
	gettimeofday(&start,NULL);
	char file_name[50];
	snprintf(file_name, sizeof(file_name),"%s%s",argv[3],".zip");
	if ((fd = open(file_name,O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    n=read(fd,buf,BUFFERT);
	while(n){
		if(n==-1){
			perror("read fails");
			return EXIT_FAILURE;
		}
		m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
		if(m==-1){
			perror("send error");
			return EXIT_FAILURE;
		}
		count+=m;
		bzero(buf,BUFFERT);
        n=read(fd,buf,BUFFERT);
	}
	m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
	gettimeofday(&stop,NULL);
	duration(&start,&stop,&delta);
    
	printf("Bytes transferred : %lld\n",count);
	//printf("Total size of: %lld \n",sz);
	//printf("For a total duration of: %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
    close(sfd);
	return EXIT_SUCCESS;
}

int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}

int create_client_socket (int port, char* ipaddr){
    int l;
	int sfd;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0){
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
    return sfd;
}