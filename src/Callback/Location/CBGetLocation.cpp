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

// Project
#include "./CBGetLocation.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#if MRH_USER_LOCATION_USE_SERVER > 0
CBGetLocation::CBGetLocation() : c_Server()
#else
CBGetLocation::CBGetLocation()
#endif
{}

CBGetLocation::~CBGetLocation() noexcept
{}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBGetLocation::Callback(const MRH_EVBase* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    try
    {
#if MRH_USER_LOCATION_USE_SERVER > 0
        MRH_Sfloat32 f32_Latitude;
        MRH_Sfloat32 f32_Longtitude;
        MRH_Sfloat32 f32_Elevation;
        MRH_Sfloat32 f32_Facing;
        
        c_Server.GetLocation(f32_Latitude,
                             f32_Longtitude,
                             f32_Elevation,
                             f32_Facing);
        
        MRH_EventStorage::Singleton().Add(MRH_U_GET_LOCATION_S(true,
                                                               f32_Latitude,
                                                               f32_Longtitude,
                                                               f32_Elevation,
                                                               f32_Facing),
                                          u32_GroupID);
#else
        MRH_EventStorage::Singleton().Add(MRH_U_GET_LOCATION_S(false), u32_GroupID);
#endif
    }
    catch (MRH_PSBException& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, e.what(),
                                       "CBGetLocation.cpp", __LINE__);
    }
}
