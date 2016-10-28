//
//  main.cpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#include <iostream>
#include <string>

#include "MulticastEchoServer.h"
#include "MulticastServerManager.hpp"

#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"

using Poco::Net::Socket;
using Poco::Net::MulticastSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

unsigned int resp_parser(std::string& resp)
{
    Parser parser;
    
    Var result;
    try
    {
        result = parser.parse(resp);
    }
    catch(JSONException& jsone)
    {
        std::cout << jsone.message() << std::endl;
    }
    
    Object::Ptr object = result.extract<Object::Ptr>();
    Var http_port_var = object->get("service_port");
    unsigned int http_port  = http_port_var;
    
    return http_port;
    
}

void testMulticast()
{
    MulticastServerManager::Instance()->start();
    
//    MulticastEchoServer echoServer;
//    MulticastSocket ms;
//    
//    std::string hello_json =
//    "{"
//    "\"server\": 0,"
//    "\"mac\": \"AABBCCDDEEFF\","
//    "\"dev\": \"iPhone_9.3.4_iPhone7.1\""
//    "}";
//    
//    Poco::Net::SocketAddress    _group("239.255.1.2", 12345);
////    ms.bind(SocketAddress(IPAddress(), _group.port()), true);
//
//    int n = ms.sendTo(hello_json.c_str(), hello_json.size(), _group);
//    
//    char buffer[256];
//    // n = ms.receiveBytes(buffer, sizeof(buffer));
//    SocketAddress sender;
//    n = ms.receiveFrom(buffer, sizeof(buffer), sender);
//    
//    ms.close();
//    
//    std::string client_ip = sender.host().toString();
//    int client_port = sender.port();
//    
//    std::string json_resp(buffer, n);
//    
//    int http_port = resp_parser(json_resp);
//    
//    std::cout<<"Found server IP: " << client_ip << "UDP Port:" << client_port << " Web Port: " << http_port;
    
    getchar();
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    testMulticast();
    
    getchar();
    
    return 0;
}
