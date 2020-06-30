#ifndef SOCIAL_NETWORK_MICROSERVICES_MYGENERICCLIENT_H
#define SOCIAL_NETWORK_MICROSERVICES_MYGENERICCLIENT_H

#include <string>

namespace social_network {

class MyGenericClient{
 public:
  virtual ~MyGenericClient() = default;
  virtual void Connect() = 0;
  virtual void KeepAlive() = 0;
  virtual void KeepAlive(int) = 0;
  virtual void Disconnect() = 0;
  virtual bool IsConnected() = 0;

//  protected:
//   std::string _addr;
//   int _port;
};

} // namespace social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYGENERICCLIENT_H
