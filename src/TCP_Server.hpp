/**
 * @file TCP_Server.hpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief TCP_Server Class Header File
 * @version 0.1
 * @date 2021-02-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/**
 * @defgroup TCP_Server TCP Server 
 */

#pragma once

#include <string>
#include <bits/exception.h>
extern "C"
{
  #include <stdint.h> 
  #include <unistd.h>
  #include <netdb.h> 
}

/**
 * @ingroup TCP_Server
 * @brief Class for creating and managing a TCP server
 * 
 * This class allows you to create, work and close a port for a TCP server,
 * as well as send and receive data. It implements its
 * own @link failure @endlink exceptions.
 */
class TCP_Server
{
  uint16_t port;    ///< Port number for connection
  int sockFD;       ///< Socket descriptor
  int connFD;       ///< Connection descriptor
  sockaddr_in serv; ///< Server settings structure
  
public:
  /**
   * @brief Error codes
   * 
   * An enumeration that contains error codes that can throw exceptions
   */
  enum codeFail
  {
    FAIL = 1,       ///< TCP Server Error
    SOCKET_CREATE,  ///< Server creation error
    SOCKET_BIND,    ///< Error while associating server address with socket
    SOCKET_LISTEN,  ///< Error listening on socket
    SERVER_ACCEPT,  ///< Error receiving request
    READ,           ///< Error reading data
    WRITE           ///< Error while writing data
  };
    
  /**
   * @ingroup TCP_Server
   * @brief TCP Server Exceptions
   * 
   * A class describing the exceptions that the server 
   * throws when errors occur in operations
   */
  class failure : public std::exception
  {
    friend class TCP_Server;

    std::string _message; ///< A message describing the exception
    codeFail _code;       ///< Exception number
    
    /**
     * @brief Construct a new failure object
     * 
     * When throwing an exception, you can pass the description message and its code as parameters. Default values:
     * @verbatim
      msg   = "TCP Server failed"
      code  = codeFail::FAIL
      @endverbatim
     * 
     * @param msg Exception description
     * @param code Exception code
     */
    failure(const std::string msg = "TCP Server failed", const codeFail code = codeFail::FAIL);
    
  public:    
    /**
     * @brief Getting a description of an exception
     * 
     * When catching an exception using this function, you can get a message describing it
     * This function overrides the standard what () function from the parent class
     * 
     * @return Exception description string
     */
    const char * what(void) const noexcept override;

    /**
     * @brief Getting the exception code
     * 
     * When catching an exception using this function, you can get the exception code
     * 
     * @return Exception code
     */
    const codeFail code(void) const noexcept;
  };

  // ---------------------------------- Constructor ---------------------------
  /**
   * @brief Construct a new tcp server object
   * 
   * When creating a TCP server object, you must specify the port
   * on which you want to create the server
   * 
   * @param port Server TCP port
   */
  TCP_Server(const uint16_t port);

  /**
   * @brief Destroy the tcp server object
   * 
   * When the TCP server object is destroyed, the connection and socket
   * are also closed if they were open
   * 
   */
  ~TCP_Server();

  // ---------------------------------- Service function ----------------------
  /**
   * @brief Server creation function
   * 
   * The function creates a TCP server socket, configures the server and socket, 
   * and binds the socket to the socket address
   * 
   * On errors, it can throw exceptions with codes @link SOCKET_CREATE @endlink
   * and @link SOCKET_BIND @endlink
   * 
   */
  void Create(void);

  /**
   * @brief Waiting for a connection to the server
   * 
   * The function prepares the server for client connections and waits for a
   * connection. @p connReq requests will be queued for receiving;
   * the rest will be rejected
   * 
   * On errors can throw an exception with the code @link SOCKET_LISTEN @endlink
   * 
   * @param connReq The number of connections that can be queued and will not be bounced
   */
  void Listen(const uint8_t connReq) const;

  /**
   * @brief Establishing a connection
   * 
   * Establishes a connection that is pending
   * 
   * If an error occurs, throws an exception with code @link SERVER_ACCEPT @endlink
   * 
   */
  void Accept(void);

  /**
   * @brief Closed connections
   * 
   * Closes the connection with the client if it was established
   * 
   */
  void CloseConnection(void);

  /**
   * @brief Closing a socket
   * 
   * Closes the socket if it was created
   * 
   */
  void CloseSocket(void);

  // ---------------------------------- Data function -------------------------
  /**
   * @brief Reading data from the client
   * 
   * The function expects a data packet from the client, accepts it and
   * returns the received string
   * 
   * If an error occurs, throws an exception with code @link READ @endlink
   * 
   * @return Data string received from the client
   */
  std::string Read(void) const;

  /**
   * @brief Sending data to the client
   * 
   * The function sends the line @p rMsg to the client
   * 
   * If an error occurs, throws an exception with code @link WRITE @endlink
   * 
   * @param rMsg The string to be sent to the client
   */
  void Write(const std::string & rMsg) const;
};
