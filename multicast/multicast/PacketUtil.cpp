//
//  PacketManager.cpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#include "PacketUtil.hpp"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"

using namespace Poco::JSON;
using namespace Poco::Dynamic;


std::string PacketUtil::getHelloPacket()
{
    Object root_obj(true);
    
    root_obj.set("rc", 0);
    root_obj.set("mac", "AABBCCDDEEFF");
    root_obj.set("dev", "iPhone_9.3.4_iPhone7.1");
    
    std::ostringstream os;
    root_obj.stringify(os, 2);
    
    return os.str();
}

bool PacketUtil::isMulticastServer(const std::string &packet)
{
    Parser parser;
    
    Var result;
    try
    {
        result = parser.parse(packet);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var http_server_var = object->get("server");
        unsigned int server  = http_server_var;
        
        return server == 1;
    }
    catch(JSONException& jsone)
    {
        std::cout << jsone.message() << std::endl;
    }
    catch(...)
    {
        std::cout << "error" << std::endl;
    }
    
    return  false;
}

int PacketUtil::getServerPort(const std::string& packet)
{
    Parser parser;
    
    Var result;
    try
    {
        result = parser.parse(packet);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var server_port_var = object->get("service_port");
        unsigned int server_port  = server_port_var;
        
        return server_port;
    }
    catch(JSONException& jsone)
    {
        std::cout << jsone.message() << std::endl;
    }
    catch(...)
    {
        std::cout << "error" << std::endl;
    }
    
    return  0;
}



