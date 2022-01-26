/**
 *  Configuration.cpp
 *
 *  This file is part of the MRH project.
 *  See the AUTHORS file for Copyright information.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// C / C++
#include <cstring>

// External
#include <libmrhbf.h>

// Project
#include "./Configuration.h"

// Pre-defined
#ifndef MRH_USER_CONFIGURATION_PATH
    #define MRH_USER_CONFIGURATION_PATH "/usr/local/etc/mrh/mrhpservice/User.conf"
#endif

namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_SERVER = 0,
        
        // Server Key
        SERVER_ACCOUNT_MAIL = 1,
        SERVER_ACCOUNT_PASSWORD = 2,
        SERVER_DEVICE_KEY = 3,
        SERVER_DEVICE_PASSWORD = 4,
        SERVER_CONNECTION_ADDRESS = 5,
        SERVER_CONNECTION_PORT = 6,
        SERVER_COMMUNICATION_CHANNEL = 7,
        SERVER_TIMEOUT_S,
        
        // Bounds
        IDENTIFIER_MAX = SERVER_TIMEOUT_S,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "Server",
        
        // Server Key
        "AccountMail",
        "AccountPassword",
        "DeviceKey",
        "DevicePassword",
        "ConnectionAddress",
        "ConnectionPort",
        "CommunicationChannel",
        "TimeoutS"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Configuration::Configuration() noexcept : s_ServerAccountMail(""),
                                          s_ServerAccountPassword(""),
                                          s_ServerDeviceKey(""),
                                          s_ServerDevicePassword(""),
                                          s_ServerConnectionAddress("127.0.0.1"),
                                          i_ServerConnectionPort(16096),
                                          s_ServerCommunicationChannel("de.mrh.speech"),
                                          u32_ServerTimeoutS(60)
{}

Configuration::~Configuration() noexcept
{}

//*************************************************************************************
// Singleton
//*************************************************************************************

Configuration& Configuration::Singleton() noexcept
{
    static Configuration c_Configuration;
    return c_Configuration;
}

//*************************************************************************************
// Load
//*************************************************************************************

void Configuration::Load()
{
    static bool b_IsLoaded = false;
    
    if (b_IsLoaded == true)
    {
        throw Exception("Configuration already loaded! Reloading blocked to prevent issues.");
    }
    
    b_IsLoaded = true;
    
    try
    {
        MRH_BlockFile c_File(MRH_USER_CONFIGURATION_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_SERVER]) == 0)
            {
                s_ServerAccountMail = Block.GetValue(p_Identifier[SERVER_ACCOUNT_MAIL]);
                s_ServerAccountPassword = Block.GetValue(p_Identifier[SERVER_ACCOUNT_PASSWORD]);
                s_ServerDeviceKey = Block.GetValue(p_Identifier[SERVER_DEVICE_KEY]);
                s_ServerDevicePassword = Block.GetValue(p_Identifier[SERVER_DEVICE_PASSWORD]);
                s_ServerConnectionAddress = Block.GetValue(p_Identifier[SERVER_CONNECTION_ADDRESS]);
                i_ServerConnectionPort = std::stoi(Block.GetValue(p_Identifier[SERVER_CONNECTION_PORT]));
                s_ServerCommunicationChannel = Block.GetValue(p_Identifier[SERVER_COMMUNICATION_CHANNEL]);
                u32_ServerTimeoutS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SERVER_TIMEOUT_S])));
            }
        }
    }
    catch (std::exception& e)
    {
        throw Exception("Could not read configuration: " + std::string(e.what()));
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Configuration::GetServerAccountMail() noexcept
{
    return s_ServerAccountMail;
}

std::string Configuration::GetServerAccountPassword() noexcept
{
    return s_ServerAccountPassword;
}

std::string Configuration::GetServerDeviceKey() noexcept
{
    return s_ServerDeviceKey;
}

std::string Configuration::GetServerDevicePassword() noexcept
{
    return s_ServerDevicePassword;
}

std::string Configuration::GetServerConnectionAddress() noexcept
{
    return s_ServerConnectionAddress;
}

int Configuration::GetServerConnectionPort() noexcept
{
    return i_ServerConnectionPort;
}

std::string Configuration::GetServerCommunicationChannel() noexcept
{
    return s_ServerCommunicationChannel;
}

MRH_Uint32 Configuration::GetServerTimeoutS() noexcept
{
    return u32_ServerTimeoutS;
}
