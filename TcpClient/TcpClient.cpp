#include <iostream>
#include <string>
#include <memory>
#include "Socket.h"
#include "WinsockManager.h"

#define RECV_BUFF_SIZE 4096
#define DLL_WINSOCK_VERSION MAKEWORD(2, 2)

int main()
{
   std::unique_ptr<WinsockManager> winsockManager = std::make_unique<WinsockManager>();
   std::unique_ptr<Socket> socket = nullptr;
   std::string ipAddress;
   uint16_t port;
   char recvBuff[RECV_BUFF_SIZE];
   std::string sendBuff;
   char decision = 'Y';

   if (false == winsockManager->startup(DLL_WINSOCK_VERSION))
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
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      winsockManager->cleanup();
      return -1;
   }

   if (true == socket->connect(ipAddress.c_str(), port))
   {
      std::cout << "Socket connected to server [" << ipAddress << ", " << port << "]\n";
   }
   else
   {
      std::cout << "Cannot connect to server [" <<  ipAddress << ", " << port << "]\n";
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      socket->close();
      std::cout << "Socket closed" << "\n";
      winsockManager->cleanup();
      return -1;
   }

   do
   {
      std::cout << "Input message to server: ";
      std::cin.ignore();
      std::getline(std::cin, sendBuff);

      if (sendBuff.length() > 0 && true == socket->send(sendBuff))
      {
         if (socket->recv(recvBuff, RECV_BUFF_SIZE) > 0)
         {
            std::cout << "Output from server: " << recvBuff << "\n";
         }
      }

      std::cout << "Keep doing? Y/N: ";
      std::cin >> decision;

   } while (decision == 'Y' || decision == 'y');

   socket->close();
   std::cout << "Socket closed" << "\n";
   winsockManager->cleanup();
   return 0;
}