//
//  MulticastServerManager.cpp
//  multicast
//
//  Created by xu.junyang on 16/10/28.
//  Copyright © 2016年 xu.junyang. All rights reserved.
//

#include "MulticastServerManager.hpp"
#include "MulticastEchoServer.h"

#include <iostream>

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"

using namespace Poco::JSON;
using namespace Poco::Dynamic;


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
    auto search = _entitys.find(server.host().toString());
    if(search == _entitys.end())
    {
        _entitys[server.host().toString()] = new MulticastServerEntity(server, http_port);
        std::cerr << "add MulticastServerEntity " << server.host().toString() << " http port: "<<http_port << std::endl;
    }
    
    getServerInfo();
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

std::string MulticastServerManager::getServerInfo(void)
{
    Object root_obj(true);
    
    if(_entitys.empty())
    {
        root_obj.set("rc", 0);
    }
    else
    {
        root_obj.set("rc", 1);
        
        std::string json = "[]";
        Parser parser;
        Var result = parser.parse(json);
        Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
        
        int index = 0;
        for(auto& it : _entitys)
        {
            Object obj;
            obj.set("ip", it.second->host());
            obj.set("port", it.second->httpPort());
            array->set(0,obj);
            
            index++;
        }
        root_obj.set("server", result);
    }
    
    std::ostringstream oss;
    Poco::JSON::Stringifier::stringify(root_obj, oss);
    return oss.str();
}
