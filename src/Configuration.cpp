/**
 *  Copyright (C) 2021 - 2022 The MRH Project Authors.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
        SERVER_SOCKET_PATH,
        
        // Bounds
        IDENTIFIER_MAX = SERVER_SOCKET_PATH,

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
        "SocketPath"
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
                                 s_ServerSocketPath("/tmp/mrh/mrhpsuser_location.sock")
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
                s_ServerSocketPath = Block.GetValue(p_Identifier[SERVER_SOCKET_PATH]);
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

std::string Configuration::GetServerSocketPath() const noexcept
{
    return s_ServerSocketPath;
}
