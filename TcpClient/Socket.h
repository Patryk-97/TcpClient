#pragma once

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <memory>
#include <Ws2tcpip.h>
#include <string>

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
   bool connect(const char* address, const uint16_t port);
   bool send(const std::string& sendBuff);
   int recv(char* recvBuff, int recvBuffSize);
   void close();
   std::string getIpAddress(void) const;
   std::string getPort(void) const;
   std::string getIpProtocolStr(void) const;
   IpProtocol getIpProtocol(void) const;
   std::string getTxProtocolStr(void) const;
   TxProtocol getTxProtocol(void) const;

private:
   
   // private methods
   void fillAddrInfoCriteria(addrinfo* hints) const;
   bool fillNetworkAddressStructure(const char* address);
   void fillPort(uint16_t port);
   void fillIpProtocolFamily(void);

private:
   
   // attributes
   SOCKET socket;
   IpProtocol ipProtocol;
   TxProtocol txProtocol;
   std::unique_ptr<sockaddr_in> socketAddr;
};

#endif //__SOCKET_H__