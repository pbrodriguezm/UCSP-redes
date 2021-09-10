 /* Client code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <string.h>
 #include <iostream> 
#include <thread>
  
  using namespace std;

  int SocketFD;
  char answer = 'N';

  
void read_server(){

	string buf;
	for(;;){
    char buffer[256];
		bzero(buffer,256);
		read(SocketFD, buffer, 1);
		printf("ENTRA: %s\n",buffer);
   
        switch(buffer[0]) 
          {
              case 'L': 
              {
          
                bzero(buffer,256);
                read(SocketFD, buffer , 2);
                printf("ENTRA LISTA: %s\n",buffer);
                int size = atoi (buffer);
                read(SocketFD, buffer , size);
                printf("Server replay: [%s]\n",buffer);
                break;
              } 

              case 'M': 
              {
                bzero(buffer,256);
                read(SocketFD, buffer , 255);
                printf("ENTRA MENSAJE: %s\n",buffer);
                int size = atoi (buffer);
                read(SocketFD, buffer , size);
                printf("<< %s\n",buffer);
                break;
              } 


              case 'Q': 
              {
                answer= 'Q';
                printf("Bye\n");
                break;
              } 


    }
    
				
		if(SocketFD < 0) break;
	}
}

 
  int main(int argc, char *argv[])
  {
    struct sockaddr_in stSockAddr;
    int Res;
    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
    char buffer[256];

   
    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
 
 	
 	
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
 
    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    

 while (1) {
        printf("What do you want to say? ");
        bzero(buffer,256);
        scanf("%s", buffer);

        n = write(SocketFD,buffer,strlen(buffer));

        if (n < 0){
            perror("ERROR while writing to socket");
            exit(1);
        }

        bzero(buffer,256);
        n = read(SocketFD, buffer, 255);

        if (n < 0){
            perror("ERROR while reading from socket");
            exit(1);
        }
        printf("server replied: %s \n", buffer);

        // escape this loop, if the server sends message "quit"

        if (!bcmp(buffer, "quit", 4))
            break;
    }


    //   string command;
    //   cout << "Ingrese nickname: " << endl;
    //   cin >> command;

    //   write(SocketFD, command.c_str(),strlen(command.c_str()));
    //   read(SocketFD, bufF , 255);
    //   cout<<"===="<<bufF<<"===="<<endl;
      
    //   bzero(bufF,256);
    //   while ( answer == 'N' ){
    //     write(SocketFD, command.c_str(),strlen(command.c_str()));
    //     cout << ">>" << endl;
    //     cin >> command;
    //     thread(read_server).detach();
    
        
    //   }
 
    // shutdown(SocketFD, SHUT_RDWR);
 
    // close(SocketFD);
    return 0;
  }
