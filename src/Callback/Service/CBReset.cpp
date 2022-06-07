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
#include "./CBReset.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBReset::CBReset(std::shared_ptr<Content>& p_Content) noexcept : p_Content(p_Content)
{}

CBReset::~CBReset() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBReset::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    // Get package path
    MRH_EvD_Sys_ResetRequest_U c_Data;
    
    if (MRH_EVD_ReadEvent(&c_Data, MRH_EVENT_PS_RESET_REQUEST_U, p_Event) < 0)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to read event data!",
                                       "CBReset.cpp", __LINE__);
        return;
    }
    
    // Reset in individual try-catch blocks so both will be performed
    try
    {
        try
        {
            p_Content->Reset(c_Data.p_PackagePath);
        }
        catch (Exception& e)
        {
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                           "CBSReset.cpp", __LINE__);
            
            // Invalid path, which will fail but remove links
            p_Content->Reset("");
        }
    }
    catch (...)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Content reset failed!",
                                       "CBReset.cpp", __LINE__);
    }
}
