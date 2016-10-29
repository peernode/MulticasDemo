//
//  PacketManager.hpp
//  multicast
//
//  Created by xu.junyang on 16/10/28.
//  Copyright © 2016年 xu.junyang. All rights reserved.
//

#ifndef PacketManager_hpp
#define PacketManager_hpp

#include <stdio.h>
#include <string>


class PacketUtil
{
public:
    static std::string getHelloPacket();
    static bool isMulticastServer(const std::string& packet);
    static unsigned short getServerPort(const std::string& packet);

};

#endif /* PacketManager_hpp */
