#include "WinsockManager.h"

WinsockManager::WinsockManager()
{
   this->wsaData = std::make_unique<WSAData>();
}

WinsockManager::~WinsockManager()
{

}

bool WinsockManager::startup(uint16_t dllWinsockVersion)
{
   // locals
   bool rV = true;

   if (WSAStartup(dllWinsockVersion, this->wsaData.get()) != NO_ERROR)
   {
      rV = false;
   }

   return rV;
}

void WinsockManager::cleanup(void)
{
   WSACleanup();
}

std::string WinsockManager::getErrorMessage(void)
{
   // locals
   std::string rV;
   int errorNo = WSAGetLastError();

   switch (errorNo)
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
         rV = "Unsupported error #" + std::to_string(errorNo);
         break;
      }
   }

   return rV;
}