/**
 *  CBAvail.cpp
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
