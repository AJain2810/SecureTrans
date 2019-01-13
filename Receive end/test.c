#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include<string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFERT 512
#define BACKLOG 1

int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_server_socket (int port);

struct sockaddr_in sock_serv,sock_clt;


int main(int argc,char** argv){
    while(1>0)
    {
    int sfd,fd;
    unsigned int length=sizeof(struct sockaddr_in);
    long int n, m,count=0;
    unsigned int nsid;
    ushort clt_port;
    char buffer[BUFFERT],filename[256];
    char dst[INET_ADDRSTRLEN];
    //exec: ./server portnumber
    // Variable declarations
	time_t intps;
	struct tm* tmi;
    
    if(argc!=2) {
        perror("utilisation ./a.out <num_port> <file2send>\n");
        exit(3);
    }
    
    sfd = create_server_socket(15678);
    
    bzero(buffer,BUFFERT);
    listen(sfd,BACKLOG);
    
    //Connecting with client
    nsid=accept(sfd,(struct sockaddr*)&sock_clt,&length);
    {
        clt_port=ntohs(sock_clt.sin_port);
        printf("Connection established with : %s:%d\n",dst,clt_port);
        
        //Processing the filename
        bzero(filename,256);
        intps = time(NULL);
        tmi = localtime(&intps);
        bzero(filename,256);
        //sprintf(filename,"clt.%d.%d.%d.%d.%d.%d",tmi->tm_mday,tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);
        sprintf(filename,"%s","file1.zip");
        //printf("Creating the copied output file : %s\n",filename);
        
        if ((fd=open(filename,O_CREAT|O_WRONLY,0600))==-1)
        {
            perror("open fail");
            exit (3);
        }
        bzero(buffer,BUFFERT);
        
        n=recv(nsid,buffer,BUFFERT,0);
        while(n) {
            if(n==-1){
                perror("recv fail");
                exit(5);
            }
            if((m=write(fd,buffer,n))==-1){
                perror("write fail");
                exit (6);
            }
            count=count+m;
            bzero(buffer,BUFFERT);
            n=recv(nsid,buffer,BUFFERT,0);
        }
        // receive successful
        //start modifications
        //printf("Unzip begin");
        //unzip it
            {
                char cmd[50]="unzip ";//+argv[2]+".zip "+argv[2]+".enc md5sum.txt";
                snprintf(cmd, sizeof(cmd),"%s%s","unzip ",filename);
                //char *cmd="unzip ";
                //strcat(cmd, fname);//+fname;
                system(cmd);
            }
            //unzipped the file contents
            //printf("\nUnzip successful\n");
            char md5val[55]="";
            char file_name_final[50]="";
            //variable declaration part 2

            //read md5 file
            {
                //char md5val[55];
                //char file_name_final[50];
                int i=0,j=0;
                int cse=1;
                FILE *fp;
                char *filename = "md5sum.txt";
                char ch;
                //printf("Read begin\n");
                fp = fopen(filename, "r");
                //Sprintf("End of read");
                while( (ch = getc(fp)) != EOF ) {
                        //printf("%d %c- \n", (int)ch, ch);
                    if((int) ch==32)
                        {
                            cse=2;
                            //printf("\n\nCase fulfilled\n\n");
                        }
                    if (cse==1)
                    {
                        md5val[i]=ch;
                        i++;
                    }
                    if (cse==2 && ((int) ch!=32 &&(int) ch!=10))
                    {
                        //printf("\nEntered in cse2\n");
                        file_name_final[j]=ch;
                        j++;
                    }
                }
                //file_name_final[j]='1';
            }
            //reading done
            //printf("\nmd5 reading successful");
            //printf("File Name:%s",filename);
            printf("New file name: %s\n", file_name_final);
            //char file_name_final_final[50];
            // decrypt them
            {
                //./run_des.o -d keyfile.key Resume.enc AJResume.pd
                char cmd[250]="";//+argv[2]+".zip "+argv[2]+".enc md5sum.txt";
                //char ch='.';
                //char newfilename[50];
                //int i=0;
                //while(filename[i]!=ch)
                //{
                    //newfilename[i]=file_name_final[i];
                    //i++;
                //}
                //remove last chhar of fule_name_final
                
                snprintf(cmd, sizeof(cmd),"%s%s%s%s%s","./run_des.o -d keyfile.key ",file_name_final,".enc"," ",file_name_final);
                //Bug: modif filename: reduce value after .
                //printf("\n\n%s",cmd);

                /*"./run_des.o -d "+key_file_address+" "+fname+".enc "file_name_final*/
                system(cmd);         
            }
            //decryption done
            //printf("\nDecrypt successful");
            //compare hash values
            //md5 system call
            {
                //char *query="md5sum "+ argv[2] + " > md5sum.txt";
                char query[200]="md5sum ";
                snprintf(query, sizeof(query),"%s%s%s","md5sum ",file_name_final," > md5sum2.txt");
                //snprintf( target, sizeof( target ), "%s%s%s", str1, str2, str3 );     
                //printf("Command created successfully");
                //printf("%s",query);
                system(query);
            }
            //use md5al sys call text file generated. read until space symbol
            //reading md5 value
            char md5val2[50]="";
            {
                int i=0;
                int cse=1;
                FILE *fp;
                char *filename = "md5sum2.txt";
                char ch;
                fp = fopen(filename, "r");
                while( (ch = getc(fp)) != EOF ) {
                        //printf("%d %c- \n", (int)ch, ch);
                    if((int) ch==32)
                        {
                            cse=2;
                            //printf("\n\nCase fulfilled\n\n");
                        }
                    if (cse==1)
                    {
                        md5val2[i]=ch;
                        i++;
                    }
                }
            }
            //compare with md5val array
            {
                //comparing md5 values
                int res=strcmp(md5val, md5val2);
                if(res!=0)
                {
                    printf("MD5 Values dont match\n");
                    printf("Integrity of information not upheld\n");
                }
                else{
                    printf("Integrity of information upheld\n");
                }
            }

            {
                char cmd[250]="";
                snprintf(cmd, sizeof(cmd),"%s%s%s","rm md5sum.txt md5sum2.txt file1.zip ",file_name_final,".enc");
                system(cmd);
            }
            //check for equality
            //if true, continue
            //else notify the user

        close(sfd);
        close(fd);
    }
    close(nsid);
    
    //printf("End of transmission with %s.%d\n",dst,clt_port);
    //printf("Number of byte received: %ld \n",count);
    
    return EXIT_SUCCESS;
}}

/* Function allowing the creation of a socket and its attachment to the system
 * Returns a file descriptor in the Process descriptor table
 * BIND allows its definition to the system
 */
int create_server_socket (int port){
    int l;
	int sfd;
    int yes=1;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_SUCCESS;
	}
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 ) {
        perror("setsockopt erreur");
        exit(5);
    }
    
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    
	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){
		perror("bind fail");
		return EXIT_FAILURE;
	}
    
    
    return sfd;
}