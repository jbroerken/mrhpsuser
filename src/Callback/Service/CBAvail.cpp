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
#include "./CBAvail.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBAvail::CBAvail(std::shared_ptr<Content>& p_Content) noexcept : p_Content(p_Content)
{}

CBAvail::~CBAvail() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBAvail::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    MRH_EvD_U_ServiceAvail_S c_Data;
    c_Data.u8_Available = MRH_EVD_BASE_RESULT_SUCCESS;
    c_Data.u32_SupplierID = 0x4d524800;
    c_Data.u32_BinaryID = 0x55534552;
    c_Data.u32_Version = 1;
    
    if (p_Content->GetReset() == false)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::INFO, "Content was not reset!",
                                       "CBAvail.cpp", __LINE__);
        c_Data.u8_Available = MRH_EVD_BASE_RESULT_FAILED;
    }
    
    MRH_Event* p_Result = MRH_EVD_CreateSetEvent(MRH_EVENT_USER_AVAIL_S, &c_Data);
    
    if (p_Result == NULL)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to create response event!",
                                       "CBAvail.cpp", __LINE__);
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
                                       "CBAvail.cpp", __LINE__);
        MRH_EVD_DestroyEvent(p_Result);
    }
}
