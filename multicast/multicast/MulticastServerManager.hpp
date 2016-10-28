//
//  MulticastServerManager.hpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#ifndef MulticastServerManager_hpp
#define MulticastServerManager_hpp

#include "MulticastServerEntity.hpp"

#include <stdio.h>
#include <mutex>
#include <unordered_map>

class MulticastEchoServer;
class MulticastServerManager
{
public:
    static MulticastServerManager* Instance();
    ~MulticastServerManager(void);
    
public:
    void start(void);
    void dorun(void);
    void addMulticastServer(Poco::Net::SocketAddress& server, unsigned short http_port);
    void removeMulticastServer(Poco::Net::SocketAddress& server);
    int getServerCount(void);
    std::tuple<std::string, unsigned short> getServerAndPort(void);
    
protected:
    MulticastServerManager(void);
    
    
private:
    static MulticastServerManager* _instance;
    
    MulticastEchoServer* _localServer = nullptr;
    std::unordered_map<std::string, MulticastServerEntity*> _entitys;  //key为IP

};


#endif /* MulticastServerManager_hpp */
