/**
 * @file main.cpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file for main body of program
 * @version 0.1
 * @date 2021-02-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>

extern "C"
{
  #include <stdlib.h> 
  #include <string.h> 
  #include <sys/socket.h> 
  #include <sys/types.h> 
  #include <unistd.h>
  #include <netinet/in.h> 
}

using namespace std;

const uint16_t PORT = 3390;
const uint16_t MAXSYM = 256;

void foo(int sockfd)
{
  char buffer[MAXSYM] = {0};
  while(true)
  {
    int n = recv(sockfd, buffer, MAXSYM, 0); 
    cout << "Client : " << buffer; 

    n = write(sockfd, buffer, strlen(buffer)); 
    cout << "Message sent." << endl << endl; 

    memset(buffer, 0, MAXSYM);
  }
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
  sockaddr_in serv, cli;

  // Creating socket file descriptor 
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1)
  { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  }

  // Filling server information 
  serv.sin_family       = AF_INET; // IPv4 
  serv.sin_addr.s_addr  = INADDR_ANY; 
  serv.sin_port         = htons(PORT);

  // Bind the socket with the server address 
  if (bind(sockfd, (sockaddr*)&serv, sizeof(serv)) < 0) 
  { 
    perror("socked bind failed"); 
    exit(EXIT_FAILURE); 
  }

  // Now server is ready to listen and verification
  if(listen(sockfd, 5) != 0)
  {
    perror("listen failed"); 
    exit(EXIT_FAILURE); 
  }

  int len = sizeof(cli);  //len is value/resuslt 
  if(accept(sockfd, (sockaddr*)&cli, (socklen_t*)&len) < 0)
  {
    perror("server accept failed"); 
    exit(EXIT_FAILURE); 
  }
  else
  {
    cout << "Server accept the client" << endl;
  }

  foo(sockfd);

  close(sockfd);
  cout << "end";

  return 0;
}
