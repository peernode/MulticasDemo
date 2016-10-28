//
//  HeartBeatEntity.cpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#include "MulticastServerEntity.hpp"
#include "MulticastServerManager.hpp"
#include "PacketUtil.hpp"

#include "Poco/Timespan.h"
#include <iostream>

using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;

MulticastServerEntity::MulticastServerEntity(Poco::Net::SocketAddress& sender, Poco::UInt16 http_port):
_server(sender),
_http_port(http_port),
_thread("MulticastServerEntity")
{
    _socket.bind(SocketAddress(IPAddress(), 12364), true);
    _thread.start(*this);
}

MulticastServerEntity::~MulticastServerEntity()
{
    
}

Poco::UInt16 MulticastServerEntity::port() const
{
    return _socket.address().port();
}

Poco::UInt16 MulticastServerEntity::httpPort()const
{
    return _http_port;
}

void MulticastServerEntity::run()
{
    std::string hello_json = PacketUtil::getHelloPacket();
    
    Poco::Timespan span(RECVWAIT);
    int count = 0;
    while (!_stop)
    {
        Poco::Thread::sleep(SENDINTERVAL);
        count++;
        size_t sended = _socket.sendTo(hello_json.c_str(), (int)hello_json.size(), _server);
        std::cerr << "MulticastServerEntity send: " << sended << std::endl;

        
        if (_socket.poll(span, Socket::SELECT_READ))
        {
            try
            {
                char buffer[256];
                SocketAddress sender;
                int n = _socket.receiveFrom(buffer, sizeof(buffer), sender);
                
                std::string recv_msg(buffer, 0, n);
                std::cerr << "MulticastServerEntity Recv: " << recv_msg << std::endl;
                //_socket.sendTo(buffer, n, sender);
                std::string client_ip = sender.host().toString();
                int client_port = sender.port();

                
                std::cerr << "MulticastServerEntity: " << client_ip << " port:" <<  client_port << " count:"<<count << std::endl;
            }
            catch (Poco::Exception& exc)
            {
                std::cerr << "MulticastServerEntity: " << exc.displayText() << std::endl;
            }
        }
        else{
            _stop = true;
        }
    }
}
