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

/**
 * @defgroup Core
 */

#include <iostream>
#include "TCP_Server.hpp"

const uint16_t PORT = 3390;   ///< Port number on which the server will be created

/**
 * @ingroup Core
 * 
 * @brief Program entry point
 * 
 * Here's the main loop of the program:
 * Create a TCP server, configure it, establish a client connection and
 * implement the server as an echo
 * 
 * @return Program completion status
 * @retval 0 If the program finished successfully
 * @retval 1 If the program fails
 */ 
int main(void)
{
  TCP_Server tcp(PORT);

  try
  {
    tcp.Create();
    tcp.Listen(1);
    tcp.Accept();

  while(true)
  { 
    std::string msg = tcp.Read();
    std::cout << "> " << msg; 

    if(msg.compare(0, 4, "exit") == 0)
    {
      if(msg[4] == '\n' || msg[4] == '\r' || msg[4] == '\0') { break; }      
    }

    tcp.Write(msg); 
    std::cout << "< " << msg; 
  }

  return EXIT_SUCCESS;
  }
  catch(const TCP_Server::failure & e)
  {
    std::cerr << "EchoServer: " << e.what() << " " << e.code() << std::endl;
    exit(EXIT_FAILURE);
  }
}
