  /* Server code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <map> 
  #include <iostream> 
  #include <thread>
  
  using namespace std;


map<string, int> clients;

  string print_clients(const std::map<std::string, int>& m)
  {
    string clients="";
      for (const auto& [key, value] : m) {
          std::cout << key << " = " << value << "; ";
          clients+=" "+key;
      }
      
      std::cout << "\n";
      return clients;
  }

  void read_cli(int ConnectFD){

while(1){



      char buffer[256];
      char buffer2[256];
      int n;

      bzero(buffer,256);
      bzero(buffer2,256);
      n = read(ConnectFD,buffer,1);
      if (n > 0) 
      {
        
        printf("Request: [%s]",buffer);   

        switch(buffer[0]) 
          {
              case 'M': 
              {
                //get client M03LIA04HOLA
                printf("[Menssage], ");   
                read(ConnectFD,buffer,2); //Read bits of client
                printf("Size: [%s], ",buffer);
                int size = atoi (buffer);
                read(ConnectFD,buffer,size); //Read client
                printf("TO: [%s], ",buffer);
                write(ConnectFD,"Mensaje Enviado...",18);

                int idConnectFD = clients[buffer]; //get client of map m.find('c');  
                printf("IDdest: [%d], ",idConnectFD); 
                read(ConnectFD,buffer,2); //Read bits of menssagge
                int sizemsg = atoi (buffer);
                read(ConnectFD,buffer,sizemsg); //Read msg
                printf("MSG: [%s]\n",buffer);
                write(idConnectFD,buffer,sizemsg);

                break;
            }

            case 'R': 
            {
              printf("[Registro], ");   
              read(ConnectFD,buffer,2);
              printf("Size: [%s], ",buffer);
              int size = atoi (buffer);
              read(ConnectFD,buffer,size); 
              printf("CLI: [%s]\n",buffer);
              clients.insert({buffer , ConnectFD });
              write(ConnectFD,"Registro ok",11);
              break;
            }
            
            
            case 'Q': 
            {
              write(ConnectFD,"Q",1);
              shutdown(ConnectFD, SHUT_RDWR);
              close(ConnectFD);
              break;
            }
            
            case 'S':
            {
              printf("[Clients], ");    
              string clilist = print_clients(clients);
              string adzero="";
              if(clilist.size() <10) {
                adzero="0";
              }
              clilist="L"+adzero+to_string(clilist.size())+clilist;
              cout<<clilist<<"\n";
              n = write(ConnectFD,clilist.c_str(),clilist.size());
              break;
            }
            
            default: 
            {
                  //shutdown(ConnectFD, SHUT_RDWR);
 
                 // close(ConnectFD);
            }
        }
      }

       
     	 sleep(2);

   //  if (n < 0) perror("ERROR writing to socket");
       //shutdown(ConnectFD, SHUT_RDWR);
 
      //close(ConnectFD);
}

}


  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

 
    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    for(;;)
    {
      int ConnectFD = accept(SocketFD, NULL, NULL);
      cout<<"Conxion:"<<ConnectFD<<endl;
 
      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
    
      thread(read_cli, ConnectFD).detach();
    }
 
    close(SocketFD);
    return 0;
  }
