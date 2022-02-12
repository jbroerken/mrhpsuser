/**
 *  Configuration.h
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

#ifndef Configuration_h
#define Configuration_h

// C / C++

// External
#include <MRH_Typedefs.h>

// Project
#include "./Exception.h"


class Configuration
{
public:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */

    Configuration();

    /**
     *  Default destructor.
     */

    ~Configuration() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the source directory path.
     *
     *  \return The source directory path.
     */
    
    std::string GetSourceDirectoryPath() const noexcept;
    
    /**
     *  Get the content link directory path.
     *
     *  \return The content link directory path.
     */
    
    std::string GetContentLinkDirectoryPath() const noexcept;
    
    /**
     *  Get the package link directory path.
     *
     *  \return The package link directory path.
     */
    
    std::string GetPackageLinkDirectoryPath() const noexcept;
    
    /**
     *  Get the documents directory name.
     *
     *  \return The documents directory name.
     */
    
    std::string GetDocumentsDirectory() const noexcept;
    
    /**
     *  Get the pictures directory name.
     *
     *  \return The pictures directory name.
     */
    
    std::string GetPicturesDirectory() const noexcept;
    
    /**
     *  Get the music directory name.
     *
     *  \return The music directory name.
     */
    
    std::string GetMusicDirectory() const noexcept;
    
    /**
     *  Get the videos directory name.
     *
     *  \return The videos directory name.
     */
    
    std::string GetVideosDirectory() const noexcept;
    
    /**
     *  Get the downloads directory name.
     *
     *  \return The downloads directory name.
     */
    
    std::string GetDownloadsDirectory() const noexcept;
    
    /**
     *  Get the clipboard file name.
     *
     *  \return The clipboard file name.
     */
    
    std::string GetClipboardFile() const noexcept;
    
    /**
     *  Get the person info file name.
     *
     *  \return The person info file name.
     */
    
    std::string GetInfoPersonFile() const noexcept;
    
    /**
     *  Get the residence info file name.
     *
     *  \return The residence info name.
     */
    
    std::string GetInfoResidenceFile() const noexcept;
    
    /**
     *  Get the full server socket path.
     *
     *  \return The full server socket path.
     */
    
    std::string GetServerSocketPath() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //**************************************************************************************

    // Source Paths
    std::string s_SourceDirPath;
    
    // Link Paths
    std::string s_ContentLinkDirPath;
    std::string s_PackageLinkDirPath;
    
    // User Content
    std::string s_DocumentsDir;
    std::string s_PicturesDir;
    std::string s_MusicDir;
    std::string s_VideosDir;
    std::string s_DownloadsDir;
    std::string s_ClipboardFile;
    std::string s_InfoPersonFile;
    std::string s_InfoResidenceFile;
    
    // Server
    std::string s_ServerSocketPath;
    
protected:

};

#endif /* Configuration_h */
