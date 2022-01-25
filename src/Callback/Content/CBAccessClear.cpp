/**
 *  CBAccessClear.cpp
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
#include "./CBAccessClear.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBAccessClear::CBAccessClear(std::shared_ptr<Content>& p_Content) noexcept : p_Content(p_Content)
{}

CBAccessClear::~CBAccessClear() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBAccessClear::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    MRH_EvD_U_AccessClear_S c_Data;
    c_Data.u8_Result = MRH_EVD_BASE_RESULT_FAILED;
    
    // Reset happened? Needed for package info
    if (p_Content->GetReset() == true)
    {
        try
        {
            p_Content->ClearAccess();
            c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
        }
        catch (Exception& e)
        {
            MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                           "CBAccessClear.cpp", __LINE__);
        }
    }
    
    
    MRH_Event* p_Result = MRH_EVD_CreateSetEvent(MRH_EVENT_USER_ACCESS_CLEAR_S, &c_Data);
    
    if (p_Result == NULL)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to create response event!",
                                       "CBAccessClear.cpp", __LINE__);
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
                                       "CBAccessClear.cpp", __LINE__);
        MRH_EVD_DestroyEvent(p_Result);
    }
}
