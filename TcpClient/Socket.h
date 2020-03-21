#pragma once

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <memory>
#include <Ws2tcpip.h>
#include <string>
#include <iostream>

class Socket
{
public:
   enum class IpProtocol
   {
      IPV4,
      IPV6,
      UNDEFINED
   };

   enum class TxProtocol
   {
      TCP,
      UDP,
      UNDEFINED
   };

   Socket();
   ~Socket();
   void reset(void);
   bool init(IpProtocol ipProtocol, TxProtocol txProtocol);
   bool connect(const char* address, const int port);
   void close();

private:
   SOCKET socket;
   IpProtocol ipProtocol;
   TxProtocol txProtocol;
   std::unique_ptr<sockaddr_in> socketAddr;
};


#endif //__SOCKET_H__