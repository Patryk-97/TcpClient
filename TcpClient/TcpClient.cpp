#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "Socket.h"

#define IP_ADDRESS "127.0.0.1"
#define PORT 54000
#define DLL_WS_VER MAKEWORD(2, 2)

int main()
{
   WSADATA wsaData;

   int result = WSAStartup(DLL_WS_VER, &wsaData);
   if (result != NO_ERROR)
      printf("Winsock initialization error\n");

   Socket socket;
   if (socket.init(Socket::IpProtocol::IPV4, Socket::TxProtocol::TCP) == true)
   {
      std::cout << "Socket init\n";
   }
   if (socket.connect(IP_ADDRESS, PORT) == true)
   {
      std::cout << "Socket connect\n";
   }
   socket.close();

   WSACleanup();
   return 0;
}