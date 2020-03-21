#include <iostream>
#include <memory>
#include <winsock2.h>
#include <windows.h>
#include "Socket.h"

#define IP_ADDRESS "localhost"
#define PORT 54000
#define DLL_WS_VER MAKEWORD(2, 2)

int main()
{
   WSADATA wsaData;
   std::unique_ptr<Socket> socket = nullptr;

   int result = WSAStartup(DLL_WS_VER, &wsaData);
   if (result != NO_ERROR)
   {
      printf("Winsock initialization error\n");
      return -1;
   }

   socket = std::make_unique<Socket>();

   if (true == socket->init(Socket::IpProtocol::IPV4, Socket::TxProtocol::TCP))
   {
      std::cout << "Socket initialized\n";
   }
   else
   {
      std::cout << "Cannot initialiaze a socket\n";
      std::cout << "Error #" << WSAGetLastError() << "\n";
      WSACleanup();
      return -1;
   }

   if (true == socket->connect(IP_ADDRESS, PORT))
   {
      std::cout << "Socket connected to server [" << IP_ADDRESS << ", " << PORT << "]\n";
   }
   else
   {
      std::cout << "Cannot connect to server [" << IP_ADDRESS << ", " << PORT << "]\n";
      std::cout << "Error #" << WSAGetLastError() << "\n";
      socket->close();
      WSACleanup();
      return -1;
   }

   socket->close();
   WSACleanup();
   return 0;
}