#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
   this->socket = INVALID_SOCKET;
   this->ipProtocol = IpProtocol::IPV4;
   this->txProtocol = TxProtocol::TCP;
   this->socketAddr = std::make_unique<sockaddr_in>();
}

ClientSocket::~ClientSocket()
{
   this->reset();
}

void ClientSocket::reset(void)
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

bool ClientSocket::init(IpProtocol ipProtocol, TxProtocol txProtocol)
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

bool ClientSocket::connect(const char* address, const uint16_t port)
{
   // locals
   bool rV = true;

   this->fillIpProtocolFamily();
   this->fillPort(port);
   rV = this->fillNetworkAddressStructure(address);

   if (true == rV && 
       ::connect(this->socket, (sockaddr*)this->socketAddr.get(), sizeof(*this->socketAddr)) == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

bool ClientSocket::send(const std::string& sendBuff, int& bytesSend)
{
   // locals
   bool rV = true;

   bytesSend = ::send(this->socket, sendBuff.c_str(), sendBuff.length() + 1, 0);

   if (bytesSend == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

int ClientSocket::recv(char* recvBuff, int recvBuffSize)
{
   // locals
   int rV = 0;

   memset(recvBuff, 0, recvBuffSize);
   rV = ::recv(this->socket, recvBuff, recvBuffSize, 0);

   return rV;
}

void ClientSocket::close()
{
   if (this->socket != INVALID_SOCKET)
   {
      closesocket(this->socket);
   }
}

std::string ClientSocket::getIpAddress(void) const
{
   // locals
   std::string rV = "\"";

   rV += std::to_string(this->socketAddr->sin_addr.S_un.S_un_b.s_b1) + ".";
   rV += std::to_string(this->socketAddr->sin_addr.S_un.S_un_b.s_b2) + ".";
   rV += std::to_string(this->socketAddr->sin_addr.S_un.S_un_b.s_b3) + ".";
   rV += std::to_string(this->socketAddr->sin_addr.S_un.S_un_b.s_b4) + "\"";

   return rV;
}

std::string ClientSocket::getPort(void) const
{
   // locals
   uint16_t port = (this->socketAddr->sin_port & 0xFF00) >> 8 | (this->socketAddr->sin_port & 0x00FF) << 8;
   std::string rV = std::to_string(port);

   return rV;
}

std::string ClientSocket::getIpProtocolStr(void) const
{
   // locals
   std::string rV;

   rV = this->ipProtocol == IpProtocol::IPV4 ? "IPV4" : "IPV6";
   return rV;
}

ClientSocket::IpProtocol ClientSocket::getIpProtocol(void) const
{
   return this->ipProtocol;
}

std::string ClientSocket::getTxProtocolStr(void) const
{
   // locals
   std::string rV;

   rV = this->txProtocol == TxProtocol::TCP ? "TCP" : "UDP";
   return rV;
}

ClientSocket::TxProtocol ClientSocket::getTxProtocol(void) const
{
   return this->txProtocol;
}

void ClientSocket::fillAddrInfoCriteria(addrinfo* hints) const
{
   memset(hints, 0, sizeof(*hints));
   hints->ai_socktype = this->txProtocol == TxProtocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
   hints->ai_family = this->ipProtocol == IpProtocol::IPV4 ? AF_INET : AF_INET6;
}

bool ClientSocket::fillNetworkAddressStructure(const char* address)
{
   // locals
   bool rV = true;
   std::unique_ptr<addrinfo> hints = std::make_unique<addrinfo>();
   addrinfo *res = nullptr;

   if (address[0] >= '0' && address[0] <= '9')
   {
      if (::inet_pton(AF_INET, address, &this->socketAddr->sin_addr) != 1)
      {
         rV = false;
      }
   }
   else
   {
      this->fillAddrInfoCriteria(hints.get());
      if (0 != ::getaddrinfo(address, nullptr, hints.get(), &res))
      {
         rV = false;
      }
      else
      {
         this->socketAddr->sin_addr.S_un = ((sockaddr_in*)(res->ai_addr))->sin_addr.S_un;
         ::freeaddrinfo(res);
      }
   }

   return rV;
}

void ClientSocket::fillPort(uint16_t port)
{
   this->socketAddr->sin_port = htons(port);
}

void ClientSocket::fillIpProtocolFamily(void)
{
   if (this->ipProtocol == IpProtocol::IPV4)
   {
      this->socketAddr->sin_family = AF_INET;
   }
   else
   {
      this->socketAddr->sin_family = AF_INET6;
   }
}