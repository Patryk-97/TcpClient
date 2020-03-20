#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "Socket.h"

int main()
{
   WSADATA wsaData;

   int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (result != NO_ERROR)
      printf("Initialization error.\n");

   Socket socket;
   if (socket.init(Socket::IpProtocol::IPV4, Socket::TxProtocol::TCP) == true)
   {
      std::cout << "Socket init\n";
   }
   if (socket.connect("127.0.0.1", 54000) == true)
   {
      std::cout << "Socket connect\n";
   }
   socket.close();

   WSACleanup();
   return 0;
}