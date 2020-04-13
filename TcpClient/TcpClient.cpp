#include <iostream>
#include <string>
#include <memory>
#include "ClientSocket.h"
#include "WinsockManager.h"

#define RECV_BUFF_SIZE 4096
#define DLL_WINSOCK_VERSION MAKEWORD(2, 2)

int main()
{
   std::unique_ptr<WinsockManager> winsockManager = std::make_unique<WinsockManager>();
   std::unique_ptr<ClientSocket> clientSocket = nullptr;
   std::string ipAddress;
   uint16_t port;
   char recvBuff[RECV_BUFF_SIZE];
   std::string sendBuff;
   char decision = 'Y';
   int bytesSend = 0;
   int bytesReceived = 0;

   if (false == winsockManager->startup(DLL_WINSOCK_VERSION))
   {
      printf("Winsock initialization error\n");
      return -1;
   }

   std::cout << "Enter ip address or DNS address: ";
   std::cin >> ipAddress;

   std::cout << "Enter port: ";
   std::cin >> port;

   clientSocket = std::make_unique<ClientSocket>();

   if (true == clientSocket->init(IpProtocol::IPV4, TxProtocol::TCP))
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

   if (true == clientSocket->connect(ipAddress.c_str(), port))
   {
      std::cout << "Socket connected to server [" << ipAddress << ", " << port << "]\n";
   }
   else
   {
      std::cout << "Cannot connect to server [" <<  ipAddress << ", " << port << "]\n";
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      clientSocket->close();
      std::cout << "Socket closed" << "\n";
      winsockManager->cleanup();
      return -1;
   }

   do
   {
      std::cout << "Input message to server: ";
      std::cin.ignore();
      std::getline(std::cin, sendBuff);

      if (sendBuff.length() > 0 && true == clientSocket->send(sendBuff, bytesSend))
      {
         if ((bytesReceived = clientSocket->recv(recvBuff, RECV_BUFF_SIZE)) > 0)
         {
            std::cout << "Output from server: " << recvBuff << "\n";
            std::cout << "Bytes send: " << bytesSend << ", bytes received: " << bytesReceived << "\n";

            std::cout << "Keep doing? Y/N: ";
            std::cin >> decision;
         }
         else
         {
            std::cout << "Connection has been terminated\n";
         }
      }

   } while ((decision == 'Y' || decision == 'y') && (bytesReceived > 0));

   clientSocket->close();
   std::cout << "Socket closed" << "\n";
   winsockManager->cleanup();
   std::cin.get();
   return 0;
}