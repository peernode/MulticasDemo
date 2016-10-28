//
//  MulticastServerManager.cpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#include "MulticastServerManager.hpp"
#include "MulticastEchoServer.h"

#include <iostream>

MulticastServerManager* MulticastServerManager::_instance = nullptr;

MulticastServerManager::MulticastServerManager(void)
{
}

MulticastServerManager::~MulticastServerManager(void)
{
    if(_localServer != nullptr)
    {
        delete _localServer;
    }
}

MulticastServerManager* MulticastServerManager::Instance()
{
    if (_instance == NULL)
    {
        _instance = new MulticastServerManager();
    }
    
    return _instance;
}

void MulticastServerManager::start()
{
    if(_localServer == nullptr)
    {
        _localServer = new MulticastEchoServer();
    }
}

void MulticastServerManager::dorun(void)
{
    for(auto it = _entitys.begin(); it != _entitys.end(); )
    {
        if(it->second->ifStop()){
            delete it->second;
            it = _entitys.erase(it);
            std::cerr << "remove MulticastServerEntity " << std::endl;
        }
        else
        {
            ++it;
        }

    }
}

void MulticastServerManager::addMulticastServer(Poco::Net::SocketAddress& server, unsigned short http_port)
{
    // 目前只保留一个server
    if(!_entitys.empty())
    {
        return;
    }
    
    auto search = _entitys.find(server.host().toString());
    if(search == _entitys.end())
    {
        _entitys[server.host().toString()] = new MulticastServerEntity(server, http_port);
        std::cerr << "add MulticastServerEntity " << server.host().toString() << " http port: "<<http_port << std::endl;
    }
}

int MulticastServerManager::getServerCount(void)
{
    return (int)_entitys.size();
}

std::tuple<std::string, unsigned short> MulticastServerManager::getServerAndPort(void)
{
    if (_entitys.empty()) {
        return std::make_tuple("", 0);
    }
    
    auto it = _entitys.begin();
    return std::make_tuple(it->first, it->second->httpPort());
}
