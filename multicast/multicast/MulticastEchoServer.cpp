#include "MulticastEchoServer.h"
#include "MulticastServerManager.hpp"
#include "PacketUtil.hpp"

#include "Poco/Timespan.h"
#include <iostream>

using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::NetworkInterface;


MulticastEchoServer::MulticastEchoServer():
	_group("239.255.1.2", 12345),
	_if(findInterface()),
	_thread("MulticastEchoServer"),
	_stop(false)
{
//	_socket.bind(SocketAddress(IPAddress(), _group.port()), true);
	_socket.joinGroup(_group.host(), _if);
	_thread.start(*this);
	_ready.wait();
}


MulticastEchoServer::~MulticastEchoServer()
{
	_stop = true;
	_thread.join();
	_socket.leaveGroup(_group.host(), _if);
}


Poco::UInt16 MulticastEchoServer::port() const
{
	return _socket.address().port();
}


void MulticastEchoServer::run()
{
	_ready.set();
	Poco::Timespan span(250000);
	while (!_stop)
	{
        MulticastServerManager::Instance()->dorun();
        
        if(MulticastServerManager::Instance()->getServerCount() <= 0)
        {
            Poco::Thread::sleep(5000);
            std::string hello_json = PacketUtil::getHelloPacket();
            size_t sended = _socket.sendTo(hello_json.c_str(), (int)hello_json.size(), _group);
            std::cerr << "MulticastEchoServer send: " << sended << std::endl;
        }
        
		if (_socket.poll(span, Socket::SELECT_READ))
		{
			try
			{
				char buffer[256];
				SocketAddress sender;
				int n = _socket.receiveFrom(buffer, sizeof(buffer), sender);
                
                std::string recv_msg(buffer, 0, n);
                std::cerr << "MulticastEchoServer Recv: " << recv_msg << std::endl;
				//_socket.sendTo(buffer, n, sender);
                std::string client_ip = sender.host().toString();
                int client_port = sender.port();
#if 0
                std::vector<NetworkInterface> self_if_list = _if.list();

                std::vector<NetworkInterface>::iterator iter;
                
                for(iter = self_if_list.begin(); iter != self_if_list.end(); ++iter)
                    std::cerr << "If Address: " << (*iter).address().toString() << std::endl;
#endif

                std::cerr << "MulticastEchoServer: " << client_ip << " port:" <<  client_port << std::endl;
                
                if(PacketUtil::isMulticastServer(recv_msg))
                {
                    MulticastServerManager::Instance()->addMulticastServer(sender, PacketUtil::getServerPort(recv_msg));
                }
			}
			catch (Poco::Exception& exc)
			{
                
				std::cerr << "MulticastEchoServer: " << exc.displayText() << std::endl;
			}
		}
	}
}


const SocketAddress& MulticastEchoServer::group() const
{
	return _group;
}


const NetworkInterface& MulticastEchoServer::interfc() const
{
	return _if;
}


Poco::Net::NetworkInterface MulticastEchoServer::findInterface()
{
	NetworkInterface::Map m = NetworkInterface::map();
	for (NetworkInterface::Map::const_iterator it = m.begin(); it != m.end(); ++it)
	{

        if (it->second.supportsIPv4() &&
			it->second.firstAddress(IPAddress::IPv4).isUnicast() && 
			!it->second.isLoopback() && 
			!it->second.isPointToPoint())
		{
            return it->second;
		}
	}
	return NetworkInterface();
}

