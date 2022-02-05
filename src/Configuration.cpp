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
        BLOCK_SOURCE = 0,
        BLOCK_DESTINATION = 1,
        BLOCK_USER_CONTENT = 2,
        BLOCK_SERVER = 3,
        
        // Source Key
        SOURCE_DIR_PATH = 4,
        
        // Link Key
        LINK_CONTENT_DIR_PATH = 5,
        LINK_PACKAGE_DIR_PATH = 6,
        
        // User Content Key
        USER_CONTENT_DOCUMENTS = 7,
        USER_CONTENT_PICTURES,
        USER_CONTENT_MUSIC,
        USER_CONTENT_VIDEOS,
        USER_CONTENT_DOWNLOADS,
        USER_CONTENT_CLIPBOARD,
        USER_CONTENT_INFO_PERSON,
        USER_CONTENT_INFO_RESIDENCE,
        
        // Server Key
        SERVER_ACCOUNT_MAIL,
        SERVER_ACCOUNT_PASSWORD,
        SERVER_DEVICE_KEY,
        SERVER_DEVICE_PASSWORD,
        SERVER_ADDRESS,
        SERVER_PORT,
        SERVER_TIMEOUT_S,
        SERVER_RETRY_WAIT_S,
        SERVER_CLIENT_UPDATE_S,
        
        // Bounds
        IDENTIFIER_MAX = SERVER_CLIENT_UPDATE_S,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "UserSource",
        "UserDestination",
        "UserContent",
        "Server",
        
        // Source Key
        "SourceDirPath",
        
        // Link Key
        "ContentLinkDirPath",
        "PackageLinkDirPath",
        
        // User Content Key
        "DocumentsDir",
        "PicturesDir",
        "MusicDir",
        "VideosDir",
        "DownloadsDir",
        "ClipboardFile",
        "InfoPersonFile",
        "InfoResidenceFile",
        
        // Server Key
        "AccountMail",
        "AccountPassword",
        "DeviceKey",
        "DevicePassword",
        "Address",
        "Port",
        "TimeoutS",
        "RetryWaitS",
        "ClientUpdateS"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Configuration::Configuration() : s_SourceDirPath("/var/mrh/mrhpsuser/"),
                                 s_ContentLinkDirPath("/var/mrh/mrhpsuser/_User/"),
                                 s_PackageLinkDirPath("FSRoot/_User/"),
                                 s_DocumentsDir("Documents"),
                                 s_PicturesDir("Pictures"),
                                 s_MusicDir("Music"),
                                 s_VideosDir("Videos"),
                                 s_DownloadsDir("Downloads"),
                                 s_ClipboardFile("Clipboard.txt"),
                                 s_InfoPersonFile("UserPerson.conf"),
                                 s_InfoResidenceFile("UserResidence.conf"),
                                 s_ServerAccountMail(""),
                                 s_ServerAccountPassword(""),
                                 s_ServerDeviceKey(""),
                                 s_ServerDevicePassword(""),
                                 s_ServerAddress("127.0.0.1"),
                                 i_ServerPort(16096),
                                 u32_ServerTimeoutS(60),
                                 u32_ServerRetryWaitS(300),
                                 u32_ServerClientUpdateS(300)
{
    try
    {
        MRH_BlockFile c_File(MRH_USER_CONFIGURATION_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_SOURCE]) == 0)
            {
                s_SourceDirPath = Block.GetValue(p_Identifier[SOURCE_DIR_PATH]);
                
                if (s_SourceDirPath.size() > 0 && *(--(s_SourceDirPath.end())) != '/')
                {
                    s_SourceDirPath += "/";
                }
            }
            else if (Block.GetName().compare(p_Identifier[BLOCK_DESTINATION]) == 0)
            {
                s_ContentLinkDirPath = Block.GetValue(p_Identifier[LINK_CONTENT_DIR_PATH]);
                
                if (s_ContentLinkDirPath.size() > 0 && *(--(s_ContentLinkDirPath.end())) != '/')
                {
                    s_ContentLinkDirPath += "/";
                }
                
                // End of link dir path should be the directory
                s_PackageLinkDirPath = Block.GetValue(p_Identifier[LINK_PACKAGE_DIR_PATH]);
                
                if (s_PackageLinkDirPath.size() > 0 && *(--(s_PackageLinkDirPath.end())) == '/')
                {
                    s_PackageLinkDirPath.erase(--(s_PackageLinkDirPath.end()));
                }
            }
            else if (Block.GetName().compare(p_Identifier[BLOCK_USER_CONTENT]) == 0)
            {
                s_DocumentsDir = Block.GetValue(p_Identifier[USER_CONTENT_DOCUMENTS]);
                s_PicturesDir = Block.GetValue(p_Identifier[USER_CONTENT_PICTURES]);
                s_MusicDir = Block.GetValue(p_Identifier[USER_CONTENT_MUSIC]);
                s_VideosDir = Block.GetValue(p_Identifier[USER_CONTENT_VIDEOS]);
                s_DownloadsDir = Block.GetValue(p_Identifier[USER_CONTENT_DOWNLOADS]);
                s_ClipboardFile = Block.GetValue(p_Identifier[USER_CONTENT_CLIPBOARD]);
                s_InfoPersonFile = Block.GetValue(p_Identifier[USER_CONTENT_INFO_PERSON]);
                s_InfoResidenceFile = Block.GetValue(p_Identifier[USER_CONTENT_INFO_RESIDENCE]);
            }
            else if (Block.GetName().compare(p_Identifier[BLOCK_SERVER]) == 0)
            {
                s_ServerAccountMail = Block.GetValue(p_Identifier[SERVER_ACCOUNT_MAIL]);
                s_ServerAccountPassword = Block.GetValue(p_Identifier[SERVER_ACCOUNT_PASSWORD]);
                s_ServerDeviceKey = Block.GetValue(p_Identifier[SERVER_DEVICE_KEY]);
                s_ServerDevicePassword = Block.GetValue(p_Identifier[SERVER_DEVICE_PASSWORD]);
                s_ServerAddress = Block.GetValue(p_Identifier[SERVER_ADDRESS]);
                i_ServerPort = std::stoi(Block.GetValue(p_Identifier[SERVER_PORT]));
                u32_ServerTimeoutS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SERVER_TIMEOUT_S])));
                u32_ServerRetryWaitS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SERVER_RETRY_WAIT_S])));
                u32_ServerClientUpdateS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SERVER_CLIENT_UPDATE_S])));
            }
        }
    }
    catch (std::exception& e)
    {
        throw Exception("Could not read configuration: " + std::string(e.what()));
    }
}

Configuration::~Configuration() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Configuration::GetSourceDirectoryPath() const noexcept
{
    return s_SourceDirPath;
}

std::string Configuration::GetContentLinkDirectoryPath() const noexcept
{
    return s_ContentLinkDirPath;
}

std::string Configuration::GetPackageLinkDirectoryPath() const noexcept
{
    return s_PackageLinkDirPath;
}

std::string Configuration::GetDocumentsDirectory() const noexcept
{
    return s_DocumentsDir;
}

std::string Configuration::GetPicturesDirectory() const noexcept
{
    return s_PicturesDir;
}

std::string Configuration::GetMusicDirectory() const noexcept
{
    return s_MusicDir;
}

std::string Configuration::GetVideosDirectory() const noexcept
{
    return s_VideosDir;
}

std::string Configuration::GetDownloadsDirectory() const noexcept
{
    return s_DownloadsDir;
}

std::string Configuration::GetClipboardFile() const noexcept
{
    return s_ClipboardFile;
}

std::string Configuration::GetInfoPersonFile() const noexcept
{
    return s_InfoPersonFile;
}

std::string Configuration::GetInfoResidenceFile() const noexcept
{
    return s_InfoResidenceFile;
}

std::string Configuration::GetServerAccountMail() const noexcept
{
    return s_ServerAccountMail;
}

std::string Configuration::GetServerAccountPassword() const noexcept
{
    return s_ServerAccountPassword;
}

std::string Configuration::GetServerDeviceKey() const noexcept
{
    return s_ServerDeviceKey;
}

std::string Configuration::GetServerDevicePassword() const noexcept
{
    return s_ServerDevicePassword;
}

std::string Configuration::GetServerAddress() const noexcept
{
    return s_ServerAddress;
}

int Configuration::GetServerPort() const noexcept
{
    return i_ServerPort;
}

MRH_Uint32 Configuration::GetServerTimeoutS() const noexcept
{
    return u32_ServerTimeoutS;
}

MRH_Uint32 Configuration::GetServerRetryWaitS() const noexcept
{
    return u32_ServerRetryWaitS;
}

MRH_Uint32 Configuration::GetServerClientUpdateS() const noexcept
{
    return u32_ServerClientUpdateS;
}

