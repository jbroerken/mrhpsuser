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

#ifndef Content_h
#define Content_h

// C / C++
#include <mutex>
#include <atomic>
#include <unordered_map>

// External

// Project
#include "../Configuration.h"


class Content
{
public:

    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef enum
    {
        DOCUMENTS = 0,
        PICTURES = 1,
        MUSIC = 2,
        VIDEOS = 3,
        DOWNLOADS = 4,
        CLIPBOARD = 5,
        INFO_PERSON = 6,
        INFO_RESIDENCE = 7,
        
        TYPE_MAX = INFO_RESIDENCE,
        
        TYPE_COUNT = TYPE_MAX + 1
        
    }Type;
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to construct with.
     */
    
    Content(Configuration const& c_Configuration);
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param s_Content Content class source.
     */
    
    Content(Content const& s_Content) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~Content() noexcept;

    //*************************************************************************************
    // Reset
    //*************************************************************************************
    
    /**
     *  Reset setup user content. This function is thread safe.
     *
     *  \param s_PackagePath The full path to the current application package.
     */
    
    void Reset(std::string const& s_PackagePath);
    
    //*************************************************************************************
    // Allow Access
    //*************************************************************************************
    
    /**
     *  Allow access to the requested user content. This function is thread safe.
     *
     *  \param e_Type The content type to allow access for.
     */
    
    void AllowAccess(Type e_Type);
    
    //*************************************************************************************
    // Clear Access
    //*************************************************************************************
    
    /**
     *  Clear all user content access. This function is thread safe.
     */
    
    void ClearAccess();
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if the content was reset. This function is thread safe.
     *
     *  \return true if the content was reset, false if not.
     */
    
    bool GetReset() noexcept;
    
private:

    //*************************************************************************************
    // Content Link
    //*************************************************************************************
    
    class SymLink
    {
    public:
        
        //*************************************************************************************
        // Constructor / Destructor
        //*************************************************************************************
        
        /**
         *  Default constructor.
         *
         *  \param s_SourcePath The full path to the source directory.
         *  \param s_LinkPath The full path to the new link directory.
         */
        
        SymLink(std::string const& s_SourcePath,
                std::string const& s_LinkPath) noexcept;
        
        /**
         *  Default destructor.
         */
        
        ~SymLink() noexcept;
        
        //*************************************************************************************
        // Allow Access
        //*************************************************************************************
        
        /**
         *  Allow access to user content. This function is thread safe.
         */
        
        void AllowAccess();
        
        //*************************************************************************************
        // Clear Access
        //*************************************************************************************
        
        /**
         *  Clear user content access. This function is thread safe.
         */
        
        void ClearAccess();
        
        //*************************************************************************************
        // Getters
        //*************************************************************************************
        
        /**
         *  Get the content source path. This function is thread safe.
         *
         *  \return The full content source path.
         */
        
        std::string GetSourcePath() noexcept;
        
        /**
         *  Get the content link path. This function is thread safe.
         *
         *  \return The full content link path.
         */
        
        std::string GetLinkPath() noexcept;
        
    private:
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
        std::mutex c_Mutex;
        
        std::string s_SourcePath;
        std::string s_LinkPath;
        
    protected:
        
    };
    
    //*************************************************************************************
    // Setup
    //*************************************************************************************
    
    /**
     *  Check and create a directory.
     *
     *  \param s_DirPath The full directory path.
     */
    
    void CheckDir(std::string s_DirPath);
    
    /**
     *  Check and create a file.
     *
     *  \param s_FilePath The full file path.
     */
    
    void CheckFile(std::string const& s_FilePath);
    
    //*************************************************************************************
    // Reset
    //*************************************************************************************
    
    /**
     *  Get the full "_User" link path.
     *
     *  \param s_PackagePath The full path to the application package.
     *
     *  \return The full link path.
     */
    
    std::string GetFullPackageLinkPath(std::string s_PackagePath);
    
    /**
     *  Check if a file is a symbolic link.
     *
     *  \param s_FilePath The full path to the file.
     *
     *  \return true if it is a symbolic link, false if not.
     */
    
    bool IsSymLink(std::string s_FilePath) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // State
    std::mutex s_ResetMutex;
    std::atomic<bool> b_Reset;
    
    // Link directory info
    std::string s_UserDirLinkPath;
    std::string s_PackageLinkDirPath;
    std::string s_ContentLinkDirPath;
    
    // Content links
    std::unordered_map<size_t, SymLink*> m_SymLink;
    
protected:

};

#endif /* Content_h */
