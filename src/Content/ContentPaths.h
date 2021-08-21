/**
 *  ContentPaths.h
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

#ifndef ContentPaths_h
#define ContentPaths_h

// C / C++

// External

// Project


//*************************************************************************************
// Source Paths
//*************************************************************************************

#ifndef MRH_PACKAGE_USER_DIRECTORY_SOURCE
    #define MRH_PACKAGE_USER_DIRECTORY_SOURCE "/var/mrhpsuser/_User"
#endif
#ifndef MRH_USER_DOCUMENTS_SOURCE_PATH
    #define MRH_USER_DOCUMENTS_SOURCE_PATH "/home/owner/Documents"
#endif
#ifndef MRH_USER_PICTURES_SOURCE_PATH
    #define MRH_USER_PICTURES_SOURCE_PATH "/home/owner/Pictures"
#endif
#ifndef MRH_USER_VIDEOS_SOURCE_PATH
    #define MRH_USER_VIDEOS_SOURCE_PATH "/home/owner/Videos"
#endif
#ifndef MRH_USER_MUSIC_SOURCE_PATH
    #define MRH_USER_MUSIC_SOURCE_PATH "/home/owner/Music"
#endif
#ifndef MRH_USER_DOWNLOADS_SOURCE_PATH
    #define MRH_USER_DOWNLOADS_SOURCE_PATH "/home/owner/Downloads"
#endif
#ifndef MRH_USER_CLIPBOARD_SOURCE_PATH
    #define MRH_USER_CLIPBOARD_SOURCE_PATH "/home/owner/Clipboard/Clipboard.txt"
#endif
#ifndef MRH_USER_INFO_PERSON_SOURCE_PATH
    #define MRH_USER_INFO_PERSON_SOURCE_PATH "/etc/MRH_User/MRH_UserPerson.conf"
#endif
#ifndef MRH_USER_INFO_RESIDENCE_SOURCE_PATH
    #define MRH_USER_INFO_RESIDENCE_SOURCE_PATH "/etc/MRH_User/MRH_UserResidence.conf"
#endif

//*************************************************************************************
// Link Paths
//*************************************************************************************

#define MRH_PACKAGE_USER_DIRECTORY_LINK "FSRoot/_User" // Relative, combined with package path

#ifndef MRH_USER_DOCUMENTS_LINK_PATH
    #define MRH_USER_DOCUMENTS_LINK_PATH "/var/mrhpsuser/_User/Documents"
#endif
#ifndef MRH_USER_PICTURES_LINK_PATH
    #define MRH_USER_PICTURES_LINK_PATH "/var/mrhpsuser/_User/Pictures"
#endif
#ifndef MRH_USER_VIDEOS_LINK_PATH
    #define MRH_USER_VIDEOS_LINK_PATH "/var/mrhpsuser/_User/Videos"
#endif
#ifndef MRH_USER_MUSIC_LINK_PATH
    #define MRH_USER_MUSIC_LINK_PATH "/var/mrhpsuser/_User/Music"
#endif
#ifndef MRH_USER_DOWNLOADS_LINK_PATH
    #define MRH_USER_DOWNLOADS_LINK_PATH "/var/mrhpsuser/_User/Downloads"
#endif
#ifndef MRH_USER_CLIPBOARD_LINK_PATH
    #define MRH_USER_CLIPBOARD_LINK_PATH "/var/mrhpsuser/_User/Clipboard.txt"
#endif
#ifndef MRH_USER_INFO_PERSON_LINK_PATH
    #define MRH_USER_INFO_PERSON_LINK_PATH "/var/mrhpsuser/_User/MRH_UserPerson.conf"
#endif
#ifndef MRH_USER_INFO_RESIDENCE_LINK_PATH
    #define MRH_USER_INFO_RESIDENCE_LINK_PATH "/var/mrhpsuser/_User/MRH_UserResidence.conf"
#endif


#endif /* ContentPaths_h */
