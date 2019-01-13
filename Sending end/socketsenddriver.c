#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
// receive file name and destination address
// md5 the file
// encrypt it
//create a socket
//send the file

char* readFile(char* file_name)
{
    // read the zip file
    char *buffer;
    FILE *f;
    f=fopen(file_name,"r");
    fscanf(f,"%s",buffer);
    printf("the file was read successfully");
}
int main(int argc , char *argv[])
{
    struct sockaddr_in server;
    int socket_desc;
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    // store data in key file

    //MD5 calculation
    {
        //char *query="md5sum "+ argv[2] + " > md5sum.txt";
        char *query="md5sum ";
        strcat(query, argv[2]);
        strcat(query, " > md5sum.txt");
        
        system(&query);
    }
    // MD5 done
    printf("md5 done");
    // DES encryption
    {
        char *cmd="./run_des.o -e ";//+argv[2]+" "+argv[2]+".enc";
        strcat(cmd, argv[3]);
        strcat(cmd, " ");
        strcat(cmd, argv[2]);
        strcat(cmd, " ");
        strcat(cmd, argv[2]);
        strcat(cmd, ".enc");
        //command formed
        
        printf(cmd);
        //debugging print
        
        system(cmd);
    }
    // executed DES encryption
    printf("DES executed successfully");
    // creating a socket descriptor
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( 5678);
    //adding config of destination

    connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
    // create a connection

    //zip the two files
    {
        char *cmd="zip ";//+argv[2]+".zip "+argv[2]+".enc md5sum.txt";
        //start concatenation
            strcat(cmd, argv[2]);
            strcat(cmd, ".zip ");
            strcat(cmd, argv[2]);
            strcat(cmd, ".enc ");
            //strcat(cmd, argv[2]);
            strcat(cmd, "md5sum.txt");
        //concatenation over

        //debugging info
        printf(cmd);
        //execute commans
        system();
    }
    // send this zip file
    char *readcommand= argv[2];
    strcat(readcommand,".zip");
    char *message=readFile(readcommand) ;
    //read encrypted file file1.
    //write(sockfd,buffer,100);
    send(socket_desc , message , strlen(message) , 0);
}