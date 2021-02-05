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
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "main.hpp"

using namespace std;

const uint16_t PORT = 3390;
const uint16_t MAXSYM = 256;

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
  char buffer[MAXSYM] = {0};
  sockaddr_in serv, cli;

  // Creating socket file descriptor 
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0)
  { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  }

  memset(&serv, 0, sizeof(serv)); 

  // Filling server information 
  serv.sin_family       = AF_INET; // IPv4 
  serv.sin_addr.s_addr  = INADDR_ANY; 
  serv.sin_port         = htons(PORT);

  // Bind the socket with the server address 
  if (bind(sockfd, (const sockaddr*)&serv, sizeof(serv)) < 0) 
  { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  }


  while(true)
  {
    socklen_t len = sizeof(cli);  //len is value/resuslt 
    int n = recvfrom(sockfd, (char*)buffer, MAXSYM, MSG_WAITALL, (sockaddr*)&cli, &len); 
    buffer[n] = '\0'; 
    cout << "Client : " << buffer; 

    sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const sockaddr*) &cli, len); 
    cout << "Message sent." << endl << endl; 
  }

  return 0;
}
