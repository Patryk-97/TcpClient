#include "Socket.h"

Socket::Socket()
{
   this->socket = INVALID_SOCKET;
   this->ipProtocol = IpProtocol::IPV4;
   this->txProtocol = TxProtocol::TCP;
   this->socketAddr = std::make_unique<sockaddr_in>();
}

Socket::~Socket()
{
   this->reset();
}

void Socket::reset(void)
{
   if (this->socket != INVALID_SOCKET)
   {
      closesocket(this->socket);
   }
   this->ipProtocol = IpProtocol::IPV4;
   this->txProtocol = TxProtocol::TCP;
   this->socketAddr.reset(new sockaddr_in());
   this->socket = INVALID_SOCKET;
}

bool Socket::init(IpProtocol ipProtocol, TxProtocol txProtocol)
{
   // locals
   int family = AF_INET;
   int type = SOCK_STREAM;
   int proto = IPPROTO_TCP;
   bool rV = true;

   if (ipProtocol == IpProtocol::IPV6)
   {
      family = AF_INET6;
      this->ipProtocol = IpProtocol::IPV6;
   }
   if (txProtocol == TxProtocol::UDP)
   {
      type = SOCK_DGRAM;
      this->txProtocol = TxProtocol::UDP;
      proto = IPPROTO_UDP;
   }

   this->socket = ::socket(family, type, proto);
   if (this->socket == INVALID_SOCKET)
   {
      rV = false;
   }
   return rV;
}

bool Socket::connect(const char* address, const int port)
{
   // locals
   bool rV = true;

   if (this->ipProtocol == IpProtocol::IPV4)
   {
      this->socketAddr->sin_family = AF_INET;
   }
   else
   {
      this->socketAddr->sin_family = AF_INET6;
   }
   this->socketAddr->sin_port = htons(port);
   inet_pton(AF_INET, address, &this->socketAddr->sin_addr);

   if (::connect(this->socket, (sockaddr*)&(*this->socketAddr), sizeof(this->socketAddr)) == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

void Socket::close()
{
   if (this->socket != INVALID_SOCKET)
   {
      closesocket(this->socket);
   }
}