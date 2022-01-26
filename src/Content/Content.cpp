/**
 *  Content.cpp
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>

// External
#include <libmrhpsb/MRH_PSBLogger.h>

// Project
#include "./Content.h"

// Pre-defined
namespace
{
#ifdef __APPLE__
    constexpr int i_PackageDirMode = 0666;
#else
    constexpr int i_PackageDirMode = 0600;
#endif
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Content::Content(Configuration const& c_Configuration) : b_Reset(false),
                                                         s_UserDirLinkPath("")
{
    s_ContentLinkDirPath = c_Configuration.GetContentLinkDirectoryPath();
    s_PackageLinkDirPath = c_Configuration.GetPackageLinkDirectoryPath();
    
    std::string s_SourceDirPath(c_Configuration.GetSourceDirectoryPath());
    std::string s_Documents(c_Configuration.GetDocumentsDirectory());
    std::string s_Pictures(c_Configuration.GetPicturesDirectory());
    std::string s_Music(c_Configuration.GetMusicDirectory());
    std::string s_Videos(c_Configuration.GetVideosDirectory());
    std::string s_Downloads(c_Configuration.GetDownloadsDirectory());
    std::string s_Clipboard(c_Configuration.GetClipboardFile());
    std::string s_InfoPerson(c_Configuration.GetInfoPersonFile());
    std::string s_InfoResidence(c_Configuration.GetInfoResidenceFile());
    
    try
    {
        // Make sure the user directory exists
        CheckDir(s_SourceDirPath);
        CheckDir(s_ContentLinkDirPath);
        
        // Make sure stuff exists
        CheckDir(s_SourceDirPath + s_Documents);
        CheckDir(s_SourceDirPath + s_Pictures);
        CheckDir(s_SourceDirPath + s_Music);
        CheckDir(s_SourceDirPath + s_Videos);
        CheckDir(s_SourceDirPath + s_Downloads);
        CheckFile(s_SourceDirPath + s_Clipboard);
        CheckFile(s_SourceDirPath + s_InfoPerson);
        CheckFile(s_SourceDirPath + s_InfoResidence);
        
        // All OK, create
        m_SymLink.emplace(DOCUMENTS, new SymLink(s_SourceDirPath + s_Documents, s_ContentLinkDirPath + s_Documents));
        m_SymLink.emplace(PICTURES, new SymLink(s_SourceDirPath + s_Pictures, s_ContentLinkDirPath + s_Pictures));
        m_SymLink.emplace(MUSIC, new SymLink(s_SourceDirPath + s_Music, s_ContentLinkDirPath + s_Music));
        m_SymLink.emplace(VIDEOS, new SymLink(s_SourceDirPath + s_Videos, s_ContentLinkDirPath + s_Videos));
        m_SymLink.emplace(DOWNLOADS, new SymLink(s_SourceDirPath + s_Downloads, s_ContentLinkDirPath + s_Downloads));
        m_SymLink.emplace(CLIPBOARD, new SymLink(s_SourceDirPath + s_Clipboard, s_ContentLinkDirPath + s_Clipboard));
        m_SymLink.emplace(INFO_PERSON, new SymLink(s_SourceDirPath + s_InfoPerson, s_ContentLinkDirPath + s_InfoPerson));
        m_SymLink.emplace(INFO_RESIDENCE, new SymLink(s_SourceDirPath + s_InfoResidence, s_ContentLinkDirPath + s_InfoResidence));
    }
    catch (Exception& e)
    {
        throw;
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}

Content::~Content() noexcept
{
    for (auto& SymLink : m_SymLink)
    {
        delete SymLink.second;
    }
}

Content::SymLink::SymLink(std::string const& s_SourcePath,
                          std::string const& s_LinkPath) noexcept
{
    this->s_SourcePath = s_SourcePath;
    this->s_LinkPath = s_LinkPath;
}

Content::SymLink::~SymLink() noexcept
{
    ClearAccess();
}

//*************************************************************************************
// Setup
//*************************************************************************************

void Content::CheckDir(std::string s_DirPath)
{
    struct stat s_FileStatus;
    std::string s_CurrentDir;
    bool b_Continue = true;
    size_t s_Pos;
    
    do
    {
        if ((s_Pos = s_DirPath.find_first_of('/')) != std::string::npos)
        {
            s_CurrentDir += s_DirPath.substr(0, s_Pos + 1);
            s_DirPath = s_DirPath.substr(s_Pos + 1);
            
            if (s_DirPath.length() == 0)
            {
                b_Continue = false;
            }
        }
        else
        {
            // Last folder
            s_CurrentDir += s_DirPath;
            b_Continue = false;
        }
        
        if ((stat(s_CurrentDir.c_str(), &s_FileStatus) == 0 && S_ISDIR(s_FileStatus.st_mode)))
        {
            continue;
        }
        
        if (mkdir(s_CurrentDir.c_str(), i_PackageDirMode) < 0)
        {
            throw Exception("Failed to create directory " +
                            s_DirPath +
                            ": " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");
        }
    }
    while (b_Continue == true);
}

void Content::CheckFile(std::string const& s_FilePath)
{
    // Create folders for file
    if (s_FilePath.find_last_of('/') != std::string::npos)
    {
        try
        {
            CheckDir(s_FilePath.substr(0, s_FilePath.find_last_of('/')));
        }
        catch (Exception& e)
        {
            throw Exception(e.what());
        }
    }
    
    // Create file
    struct stat s_FileStatus;
    
    if (stat(s_FilePath.c_str(), &s_FileStatus) == 0 && S_ISREG(s_FileStatus.st_mode))
    {
        return;
    }
    
    std::ofstream s_File(s_FilePath);
    
    if (s_File.is_open() == false)
    {
        throw Exception("Failed to create file: " + s_FilePath);
    }
    
    s_File.close();
}

//*************************************************************************************
// Reset
//*************************************************************************************

std::string Content::GetFullPackageLinkPath(std::string s_PackagePath)
{
    // Check and correct new package path
    if (s_PackagePath.length() == 0)
    {
        throw Exception("Invalid package path!");
    }
    else if (s_PackagePath[s_PackagePath.length() - 1] != '/')
    {
        s_PackagePath += "/";
    }
    
    // @NOTE: Using std::string operations here didn't work while not returning ANY errors.
    //        This convoluted way of doing the string works though.
    size_t us_StringLength = s_PackagePath.length() + s_PackageLinkDirPath.length() + 1;
    char p_CharString[us_StringLength];
    
    std::memset(p_CharString, '\0', us_StringLength);
    std::strcpy(p_CharString, s_PackagePath.c_str());
    std::strcat(p_CharString, s_PackageLinkDirPath.c_str());
    
    return std::string(p_CharString);
}

bool Content::IsSymLink(std::string s_FilePath) noexcept
{
    struct stat s_Status;
    
    if (lstat(s_FilePath.c_str(), &s_Status) == 0 && S_ISLNK(s_Status.st_mode))
    {
        return true;
    }
    
    return false;
}

void Content::Reset(std::string const& s_PackagePath)
{
    // Lock until end for full reset
    std::lock_guard<std::mutex> s_Guard(s_ResetMutex);
    
    // Set default result
    b_Reset = false;
    
    // Unlink all links inside the user dir
    try
    {
        for (auto& SymLink : m_SymLink)
        {
            SymLink.second->ClearAccess();
        }
    }
    catch (Exception& e)
    {
        throw;
    }
    
    // Remove user link from old package
    if (s_UserDirLinkPath.size() > 0)
    {
        // Removal of main user dir link
        if (unlink(s_UserDirLinkPath.c_str()) < 0 && errno != ENOENT)
        {
            throw Exception("Failed to unlink content directory link (" +
                            s_UserDirLinkPath +
                            "): " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");
        }
        
        // Reset link path, no longer in use
        s_UserDirLinkPath = "";
    }
    
    // Create main user dir link
    try
    {
        s_UserDirLinkPath = GetFullPackageLinkPath(s_PackagePath);
    }
    catch (Exception& e)
    {
        throw Exception(e.what());
    }
    
    if (symlink(s_ContentLinkDirPath.c_str(), s_UserDirLinkPath.c_str()) < 0)
    {
        // Already a symlink in place with this name
        if (errno == EEXIST && IsSymLink(s_UserDirLinkPath) == true)
        {
            // Re-link, maybe something wasn't removed correctly (crash)
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::INFO, s_ContentLinkDirPath +
                                                                " directory link to " +
                                                                s_UserDirLinkPath +
                                                                " already exists.",
                                           "Content.cpp", __LINE__);
            
            if (unlink(s_UserDirLinkPath.c_str()) < 0 || symlink(s_ContentLinkDirPath.c_str(), s_UserDirLinkPath.c_str()) < 0)
            {
                throw Exception("Failed to recreate content directory link from " +
                                s_ContentLinkDirPath +
                                " to " +
                                s_UserDirLinkPath +
                                ": " +
                                std::string(std::strerror(errno)) +
                                " (" +
                                std::to_string(errno) +
                                ")!");
            }
        }
        else
        {
            throw Exception("Failed to create content directory link from " +
                            s_ContentLinkDirPath +
                            " to " +
                            s_UserDirLinkPath +
                            ": " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");
        }
    }
    
    // Reset performed
    b_Reset = true;
}

//*************************************************************************************
// Allow Access
//*************************************************************************************

void Content::SymLink::AllowAccess()
{
    std::lock_guard<std::mutex> s_Guard(c_Mutex);
    
    if (symlink(s_SourcePath.c_str(), s_LinkPath.c_str()) < 0)
    {
        if (errno == EEXIST)
        {
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::INFO, "Requested content access link already exists!",
                                           "Content.cpp", __LINE__);
            return;
        }
        else
        {
            throw Exception("Failed to link content " +
                            s_SourcePath +
                            ": " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");
        }
    }
    
    MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::INFO, "Created access link " +
                                                        s_LinkPath +
                                                        " for source " +
                                                        s_SourcePath,
                                   "Content.cpp", __LINE__);
}

void Content::AllowAccess(Type e_Type)
{
    if (e_Type > TYPE_MAX)
    {
        throw Exception("Cannot access content (Uknown type)!");
    }
    else if (b_Reset == false)
    {
        throw Exception("Cannot access content (Reset missing)!");
    }
    
    // Link already existing?
    auto SymLink = m_SymLink.find(e_Type);
    
    if (SymLink == m_SymLink.end())
    {
        throw Exception("Request link is missing!");
    }
    
    try
    {
        SymLink->second->AllowAccess();
    }
    catch (Exception& e)
    {
        throw;
    }
}

//*************************************************************************************
// Clear Access
//*************************************************************************************

void Content::SymLink::ClearAccess()
{
    std::lock_guard<std::mutex> s_Guard(c_Mutex);
    
    if (unlink(s_LinkPath.c_str()) < 0)
    {
        if (errno == ENOENT)
        {
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::INFO, "Requested content access does not exist!",
                                           "Content.cpp", __LINE__);
        }
        else
        {
            throw Exception("Failed to remove content link " +
                            s_LinkPath +
                            ": " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");
        }
    }
}

void Content::ClearAccess()
{
    bool b_Result = true;
    
    for (auto& SymLink : m_SymLink)
    {
        try
        {
            SymLink.second->ClearAccess();
        }
        catch (Exception& e)
        {
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                           "Content.cpp", __LINE__);
            b_Result = false;
        }
    }
    
    if (b_Result == false)
    {
        throw Exception("Failed to reset all content links!");
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Content::SymLink::GetSourcePath() noexcept
{
    return s_SourcePath;
}

std::string Content::SymLink::GetLinkPath() noexcept
{
    return s_LinkPath;
}

bool Content::GetReset() noexcept
{
    return b_Reset;
}
