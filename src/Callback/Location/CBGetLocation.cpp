/**
 *  CBGetLocation.cpp
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
#include <libmrhmstream/MRH_MessageOpCode.h>

// Project
#include "./CBGetLocation.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBGetLocation::CBGetLocation(Configuration const& c_Configuration) noexcept : c_Stream(c_Configuration.GetServerSocketPath(),
                                                                                       true),
                                                                              f64_Latitude(0.f),
                                                                              f64_Longtitude(0.f),
                                                                              f64_Elevation(0.f),
                                                                              f64_Facing(0.f)
{}

CBGetLocation::~CBGetLocation() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBGetLocation::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    // Grab location data and availability
    MRH_EvD_U_GetLocation_S c_Data;
    
    // Set initial failed result
    c_Data.u8_Result = MRH_EVD_BASE_RESULT_FAILED;
    
    // Read all location messages
    if (c_Stream.GetConnected() == true)
    {
        // Recieved data
        MRH_MessageOpCode::LOCATION_C_LOCATION_DATA c_OpCode(0.f,
                                                             0.f,
                                                             0.f,
                                                             0.f);
        
        // Recieve as many as possible!
        while (c_Stream.Recieve(c_OpCode.v_Data) == true)
        {
            // Is this a usable opcode?
            if (c_OpCode.GetOpCode() != MRH_MessageOpCode::LOCATION_C_LOCATION)
            {
                continue;
            }
            
            // Update current
            f64_Latitude = c_OpCode.GetLatitude();
            f64_Longtitude = c_OpCode.GetLongtitude();
            f64_Elevation = c_OpCode.GetFacing();
            f64_Facing = c_OpCode.GetFacing();
            
            // We have valid data
            if (c_Data.u8_Result == MRH_EVD_BASE_RESULT_FAILED)
            {
                c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
            }
        }
    }
    
    c_Data.f64_Latitude = f64_Latitude;
    c_Data.f64_Longtitude = f64_Longtitude;
    c_Data.f64_Elevation = f64_Elevation;
    c_Data.f64_Facing = f64_Facing;
    
    // Got location data, now create event
    MRH_Event* p_Result = MRH_EVD_CreateSetEvent(MRH_EVENT_USER_GET_LOCATION_S, &c_Data);
    
    if (p_Result == NULL)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to create response event!",
                                       "CBGetLocation.cpp", __LINE__);
        return;
    }
    
    p_Result->u32_GroupID = u32_GroupID;
    
    // Add created event
    try
    {
        MRH_EventStorage::Singleton().Add(p_Result);
    }
    catch (MRH_PSBException& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBGetLocation.cpp", __LINE__);
    }
}
