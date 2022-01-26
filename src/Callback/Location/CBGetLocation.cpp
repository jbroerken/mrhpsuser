/**
 *  CBGetLocationN.cpp
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
CBGetLocation::CBGetLocation(Configuration const& c_Configuration) noexcept : e_ConnectionState(CONNECTION_STATE_MAX),
                                                                              b_RunThread(true),
                                                                              f64_Latitude(0.f),
                                                                              f64_Longtitude(0.f),
                                                                              f64_Elevation(0.f),
                                                                              f64_Facing(0.f)
{
    // Grab connection info
    memset(p_AccountMail, '\0', MRH_SRV_SIZE_ACCOUNT_MAIL);
    strncpy(p_AccountMail,
            c_Configuration.GetServerAccountMail().c_str(),
            MRH_SRV_SIZE_ACCOUNT_MAIL);
    
    memset(p_AccountPassword, '\0', MRH_SRV_SIZE_ACCOUNT_PASSWORD);
    strncpy(p_AccountPassword,
            c_Configuration.GetServerAccountPassword().c_str(),
            MRH_SRV_SIZE_ACCOUNT_PASSWORD);
    
    memset(p_DeviceKey, '\0', MRH_SRV_SIZE_DEVICE_KEY);
    strncpy(p_DeviceKey,
            c_Configuration.GetServerDeviceKey().c_str(),
            MRH_SRV_SIZE_DEVICE_KEY);
    
    memset(p_DevicePassword, '\0', MRH_SRV_SIZE_DEVICE_PASSWORD);
    strncpy(p_DevicePassword,
            c_Configuration.GetServerDevicePassword().c_str(),
            MRH_SRV_SIZE_DEVICE_PASSWORD);
    
    memset(p_ConServerAddress, '\0', MRH_SRV_SIZE_SERVER_ADDRESS);
    strncpy(p_ConServerAddress,
            c_Configuration.GetServerConnectionAddress().c_str(),
            MRH_SRV_SIZE_SERVER_ADDRESS);
    i_ConServerPort = c_Configuration.GetServerConnectionPort();
    
    memset(p_ComServerChannel, '\0', MRH_SRV_SIZE_SERVER_CHANNEL);
    strncpy(p_ComServerChannel,
            c_Configuration.GetServerCommunicationChannel().c_str(),
            MRH_SRV_SIZE_SERVER_CHANNEL);
    
    memset(p_ComServerAddress, '\0', MRH_SRV_SIZE_SERVER_CHANNEL);
    i_ComServerPort = -1;
    
    // Got connection info, now start updating client
    try
    {
        c_Thread = std::thread(ClientUpdate, this);
    }
    catch (std::exception& e)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, "Failed to start server thread: " +
                                                             std::string(e.what()),
                                       "CBGetLocation.cpp", __LINE__);
    }
}
#else
CBGetLocation::CBGetLocation() noexcept: f64_Latitude(0.f),
                                         f64_Longtitude(0.f),
                                         f64_Elevation(0.f),
                                         f64_Facing(0.f)
{}
#endif

CBGetLocation::~CBGetLocation() noexcept
{
#if MRH_USER_LOCATION_USE_SERVER > 0
    b_RunThread = false;
    c_Thread.join();
#endif
}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBGetLocation::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    // Grab location data and availability
    MRH_EvD_U_GetLocation_S c_Data;
    
#if MRH_USER_LOCATION_USE_SERVER > 0
    if (e_ConnectionState != 0) // @TODO: Paired State
    {
        c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
    }
    else
    {
        c_Data.u8_Result = MRH_EVD_BASE_RESULT_FAILED;
    }
    
    // Grab location data
    c_LocationMutex.lock();
    
    c_Data.f64_Latitude = f64_Latitude;
    c_Data.f64_Longtitude = f64_Longtitude;
    c_Data.f64_Elevation = f64_Elevation;
    c_Data.f64_Facing = f64_Facing;
    
    c_LocationMutex.unlock();
#else
    c_Data.u8_Result = MRH_EVD_BASE_RESULT_FAILED;
    
    c_Data.f64_Latitude = f64_Latitude;
    c_Data.f64_Longtitude = f64_Longtitude;
    c_Data.f64_Elevation = f64_Elevation;
    c_Data.f64_Facing = f64_Facing;
#endif
    
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

//*************************************************************************************
// Client
//*************************************************************************************

#if MRH_USER_LOCATION_USE_SERVER > 0
void CBGetLocation::ClientUpdate(CBGetLocation* p_Instance) noexcept
{
    while (p_Instance->b_RunThread == true)
    {
        
    }
}
#endif
