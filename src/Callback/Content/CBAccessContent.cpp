/**
 *  CBAccessContent.cpp
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

// External
#include <libmrhpsb/MRH_PSBLogger.h>

// Project
#include "./CBAccessContent.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBAccessContent::CBAccessContent(std::shared_ptr<Content>& p_Content) noexcept : p_Content(p_Content)
{}

CBAccessContent::~CBAccessContent() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

bool CBAccessContent::CreateAccess(MRH_Uint32 u32_Type) noexcept
{
    bool b_Result = true;
    
    try
    {
        switch (u32_Type)
        {
            case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
                p_Content->AllowAccess(Content::DOCUMENTS);
                break;
            case MRH_EVENT_USER_ACCESS_PICTURES_U:
                p_Content->AllowAccess(Content::PICTURES);
                break;
            case MRH_EVENT_USER_ACCESS_MUSIC_U:
                p_Content->AllowAccess(Content::MUSIC);
                break;
            case MRH_EVENT_USER_ACCESS_VIDEOS_U:
                p_Content->AllowAccess(Content::VIDEOS);
                break;
            case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
                p_Content->AllowAccess(Content::DOWNLOADS);
                break;
            case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
                p_Content->AllowAccess(Content::CLIPBOARD);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
                p_Content->AllowAccess(Content::INFO_PERSON);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
                p_Content->AllowAccess(Content::INFO_RESIDENCE);
                break;
                
            default:
                throw Exception("Wrong content access type event " +
                                std::to_string(u32_Type) +
                                " to create access for!");
        }
        
        b_Result = true;
    }
    catch (std::exception& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::WARNING, e.what(),
                                       "CBAccessContent.cpp", __LINE__);
        b_Result = false;
    }
    
    return b_Result;
}

void CBAccessContent::Callback(const MRH_EVBase* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    MRH_Uint32 u32_Type = p_Event->GetType();
    
    try
    {
        switch (u32_Type)
        {
            case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_DOCUMENTS_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_PICTURES_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_PICTURES_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_MUSIC_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_MUSIC_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_VIDEOS_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_VIDEOS_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_DOWNLOADS_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_CLIPBOARD_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_INFO_PERSON_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
                MRH_EventStorage::Singleton().Add(MRH_U_ACCESS_INFO_RESIDENCE_S(CreateAccess(u32_Type)), u32_GroupID);
                break;
            
            default:
                throw Exception("Wrong content access type event" +
                                std::to_string(u32_Type) +
                                " to create response event for!");
        }
    }
    catch (MRH_PSBException& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBAccessContent.cpp", __LINE__);
    }
    catch (Exception& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBAccessContent.cpp", __LINE__);
    }
    catch (std::exception& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "General exception: " + std::string(e.what()),
                                       "CBAccessContent.cpp", __LINE__);
    }
}
