//
//  HeartBeatEntity.hpp
//  multicast
//
//  Created by 徐俊扬 on 16/10/28.
//  Copyright © 2016年 徐俊扬. All rights reserved.
//

#ifndef HeartBeatEntity_hpp
#define HeartBeatEntity_hpp

#include <stdio.h>

#include "Poco/Net/Net.h"


#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"


class MulticastServerEntity: public Poco::Runnable
/// A simple sequential Multicast echo server.
{
public:
    MulticastServerEntity(Poco::Net::SocketAddress& sender);
    /// Creates the MulticastEchoServer.
    
    ~MulticastServerEntity();
    /// Destroys the MulticastEchoServer.
    
    Poco::UInt16 port() const;
    /// Returns the port the echo server is
    /// listening on.
    
    void run();
    /// Does the work.
    
    bool ifStop(){ return _stop;}
    
private:
    Poco::Net::DatagramSocket  _socket;
    Poco::Net::SocketAddress    _server;
    Poco::Thread _thread;
    bool         _stop;
    
    static const int SENDINTERVAL = 2500; //毫秒
    static const int RECVWAIT = 3000000; //微秒
};



#endif /* HeartBeatEntity_hpp */
