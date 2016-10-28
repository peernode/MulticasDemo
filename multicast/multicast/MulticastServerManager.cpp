//
//  MulticastServerManager.cpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#include "MulticastServerManager.hpp"

#include <iostream>

MulticastServerManager* MulticastServerManager::_instance = NULL;

MulticastServerManager::MulticastServerManager(void)
{
}
MulticastServerManager::~MulticastServerManager(void)
{
}

MulticastServerManager* MulticastServerManager::Instance()
{
    if (_instance == NULL)
    {
        _instance = new MulticastServerManager();
    }
    
    return _instance;
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

void MulticastServerManager::addMulticastServer(Poco::Net::SocketAddress& server)
{
    auto search = _entitys.find(server.host().toString());
    if(search == _entitys.end())
    {
        _entitys[server.host().toString()] = new MulticastServerEntity(server);
        std::cerr << "add MulticastServerEntity " << server.host().toString() << std::endl;
    }
}

int MulticastServerManager::getServerCount(void)
{
    return (int)_entitys.size();
}
