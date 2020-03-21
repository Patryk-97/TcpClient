#include <iostream>
#include <string>
#include <memory>
#include <winsock2.h>
#include <windows.h>
#include "Socket.h"

#define DLL_WS_VER MAKEWORD(2, 2)

std::string getErrorMessage(void);

int main()
{
   WSADATA wsaData;
   std::unique_ptr<Socket> socket = nullptr;
   std::string ipAddress;
   uint16_t port;

   if (WSAStartup(DLL_WS_VER, &wsaData) != NO_ERROR)
   {
      printf("Winsock initialization error\n");
      return -1;
   }

   std::cout << "Enter ip address or DNS address: ";
   std::cin >> ipAddress;

   std::cout << "Enter port: ";
   std::cin >> port;

   socket = std::make_unique<Socket>();

   if (true == socket->init(Socket::IpProtocol::IPV4, Socket::TxProtocol::TCP))
   {
      std::cout << "Socket initialized\n";
   }
   else
   {
      std::cout << "Cannot initialiaze a socket\n";
      std::cout << "Error: " << getErrorMessage() << "\n";
      WSACleanup();
      return -1;
   }

   if (true == socket->connect(ipAddress.c_str(), port))
   {
      std::cout << "Socket connected to server [" << ipAddress << ", " << port << "]\n";
   }
   else
   {
      std::cout << "Cannot connect to server [" <<  ipAddress << ", " << port << "]\n";
      std::cout << "Error: " << getErrorMessage() << "\n";
      socket->close();
      std::cout << "Socket closed" << "\n";
      WSACleanup();
      return -1;
   }

   socket->close();
   std::cout << "Socket closed" << "\n";
   WSACleanup();
   return 0;
}

std::string getErrorMessage(void)
{
   std::string rV;

   switch (WSAGetLastError())
   {
      case WSAETIMEDOUT:
      {
         rV = "Connection timed out.";
         break;
      }
      case WSAECONNREFUSED:
      {
         rV = "Connection refused. Probably wrong port";
         break;
      }
      case WSAHOST_NOT_FOUND:
      {
         rV = "Host not found. Wrong ip address or DNS address";
         break;
      }
      default:
      {
         rV = "Unsupported error";
         break;
      }
   }

   return rV;
}