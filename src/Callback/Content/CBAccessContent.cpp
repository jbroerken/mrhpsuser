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

void CBAccessContent::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    MRH_Uint32 u32_ResponseType = MRH_EVENT_UNK;
    
    MRH_EvD_Base_Result_t c_Data;
    c_Data.u8_Result = MRH_EVD_BASE_RESULT_FAILED;
    
    try
    {
        switch (p_Event->u32_Type)
        {
            case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_DOCUMENTS_S;
                p_Content->AllowAccess(Content::DOCUMENTS);
                break;
            case MRH_EVENT_USER_ACCESS_PICTURES_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_PICTURES_S;
                p_Content->AllowAccess(Content::PICTURES);
                break;
            case MRH_EVENT_USER_ACCESS_MUSIC_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_MUSIC_S;
                p_Content->AllowAccess(Content::MUSIC);
                break;
            case MRH_EVENT_USER_ACCESS_VIDEOS_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_VIDEOS_S;
                p_Content->AllowAccess(Content::VIDEOS);
                break;
            case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_DOWNLOADS_S;
                p_Content->AllowAccess(Content::DOWNLOADS);
                break;
            case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_CLIPBOARD_S;
                p_Content->AllowAccess(Content::CLIPBOARD);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_INFO_PERSON_S;
                p_Content->AllowAccess(Content::INFO_PERSON);
                break;
            case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
                u32_ResponseType = MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S;
                p_Content->AllowAccess(Content::INFO_RESIDENCE);
                break;
            
            default:
                throw Exception("Wrong content access type event " +
                                std::to_string(p_Event->u32_Type) +
                                " to create access for!");
        }
        
        c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
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
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBAccessContent.cpp", __LINE__);
    }
    
    // Access handled, send response
    MRH_Event* p_Result = MRH_EVD_CreateSetEvent(u32_ResponseType, &c_Data);
    
    if (p_Result == NULL)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to create response event!",
                                       "CBAccessContent.cpp", __LINE__);
        return;
    }
    
    p_Result->u32_GroupID = u32_GroupID;
    
    try
    {
        MRH_EventStorage::Singleton().Add(p_Result);
    }
    catch (MRH_PSBException& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBAccessContent.cpp", __LINE__);
        MRH_EVD_DestroyEvent(p_Result);
    }
}
