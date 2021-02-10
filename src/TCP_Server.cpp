/**
 * @file TCP_Server.cpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief TCP_Server Class Source File
 * @version 0.1
 * @date 2021-02-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TCP_Server.hpp"

using namespace std;

// ==================================== Exception =============================

TCP_Server::failure::failure(const string msg, const TCP_Server::codeFail code)
  : _message(""), _code(TCP_Server::codeFail::FAIL)
{
  this->_message = msg;
  this->_code = code;

  throw *this;
}

const TCP_Server::codeFail TCP_Server::failure::code(void) const noexcept
{
  return this->_code;
}

char const * TCP_Server::failure::what(void) const noexcept
{
  return this->_message.c_str();
}

// ==================================== Server ================================

TCP_Server::TCP_Server(const uint16_t port) : port(port), sockFD(-1), connFD(-1) { }

TCP_Server::~TCP_Server()
{
  this->CloseSocket();
}

void TCP_Server::Create(void)
{
  this->sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(this->sockFD == -1)
  { 
    failure("Socket creation failed", codeFail::SOCKET_CREATE);
  }
  setsockopt(this->sockFD, SOL_SOCKET, SO_REUSEADDR, 
    (const void *)&this->connFD, (socklen_t)sizeof(this->connFD));

  // Filling server information 
  this->serv.sin_family       = (sa_family_t)AF_INET; // IPv4 
  this->serv.sin_addr.s_addr  = (in_addr_t)htonl(INADDR_ANY); 
  this->serv.sin_port         = (in_port_t)htons(this->port);

  // Bind the socket with the server address 
  if(bind(this->sockFD, (const sockaddr *)&this->serv, 
    (socklen_t)sizeof(this->serv)) < 0) 
  { 
    close(this->sockFD);
    failure("Socket binded failed", codeFail::SOCKET_BIND);
  }
}

void TCP_Server::Listen(const uint8_t connReq) const
{
  // Now server is ready to listen and verification
  if(listen(this->sockFD, connReq) == -1)
  {
    close(this->sockFD);
    failure("Listen failed", codeFail::SOCKET_LISTEN);
  }
}

void TCP_Server::Accept(void)
{
  this->connFD = accept(this->sockFD, NULL, NULL);
  if(this->connFD < 0)
  {
    close(this->sockFD);
    failure("Server accept failed", codeFail::SERVER_ACCEPT); 
  }
}

void TCP_Server::CloseConnection(void)
{
  if(this->connFD != -1)
  {
    close(this->connFD);
    this->connFD = -1;
  }
}

void TCP_Server::CloseSocket(void)
{
  this->CloseConnection();
  if(sockFD != -1)
  {
    close(this->sockFD);
    sockFD = -1;
  }
}

string TCP_Server::Read(void) const
{
  char buffer[256] = {0};
  try
  {
    int n = read(this->connFD, (void *)buffer, sizeof(buffer)); 

    if(n <= 0) { failure("Read operation failed", codeFail::READ); }
    buffer[n] = '\0';
  }
  catch( ... ) { failure("Read operation failed", codeFail::READ); }
  
  return string(buffer);
}

void TCP_Server::Write(const string & rMsg) const
{
  try
  {
    int n = write(this->connFD, (const void *)rMsg.c_str(), (size_t)rMsg.length());

    if(n == -1) { failure("Write operation failed", codeFail::WRITE); }
  }
  catch( ... ) { failure("Write operation failed", codeFail::WRITE); }
}
