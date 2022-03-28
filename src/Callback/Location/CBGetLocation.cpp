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
#include <libmrhls.h>

// Project
#include "./CBGetLocation.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CBGetLocation::CBGetLocation(Configuration const& c_Configuration) : b_Update(true),
                                                                     b_LocationRecieved(false),
                                                                     f64_Latitude(0.f),
                                                                     f64_Longtitude(0.f),
                                                                     f64_Elevation(0.f),
                                                                     f64_Facing(0.f)
{
    try
    {
        c_Thread = std::thread(UpdateStream, 
                               this, 
                               c_Configuration.GetServerSocketPath());
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}

CBGetLocation::~CBGetLocation() noexcept
{
    b_Update = false;
    c_Thread.join();
}

//*************************************************************************************
// Callback
//*************************************************************************************

void CBGetLocation::Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept
{
    // Grab location data and availability
    MRH_EvD_U_GetLocation_S c_Data;
    
    c_Mutex.lock();
    
    if (b_LocationRecieved == true)
    {
        c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
    }
    else
    {
        c_Data.u8_Result = MRH_EVD_BASE_RESULT_SUCCESS;
    }
    
    c_Data.f64_Latitude = f64_Latitude;
    c_Data.f64_Longtitude = f64_Longtitude;
    c_Data.f64_Elevation = f64_Elevation;
    c_Data.f64_Facing = f64_Facing;
    
    c_Mutex.unlock();
    
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
// Stream
//*************************************************************************************

void CBGetLocation::UpdateStream(CBGetLocation* p_Instance, std::string s_FilePath) noexcept
{
    MRH_PSBLogger& c_Logger = MRH_PSBLogger::Singleton();
    c_Logger.Log(MRH_PSBLogger::INFO, "Opening local stream: " +
                                      s_FilePath,
                 "CBGetLocation.cpp", __LINE__);
    
    // Build stream first
    MRH_LocalStream* p_Stream = MRH_LS_Open(s_FilePath.c_str(), 0);
    
    if (p_Stream == NULL)
    {
        c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to create local stream: " +
                                           std::string(MRH_ERR_GetLocalStreamErrorString()),
                     "CBGetLocation.cpp", __LINE__);
        return;
    }
    
    // Built now wait for connection
    while (MRH_LS_Connect(p_Stream) < 0)
    {
        // Wait before retry if connection error
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    MRH_LSM_Location_Data c_Location;
    int i_Result;
    
    while (p_Instance->b_Update == true)
    {
        // Read data until a full message was read
        while ((i_Result = MRH_LS_Read(p_Stream, 100)) != 0)
        {
            if (i_Result < 0)
            {
                c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to read local stream: " +
                             std::string(MRH_ERR_GetLocalStreamErrorString()),
                             "CBGetLocation.cpp", __LINE__);
            }
        }
        
        // Check message and get message data
        if (MRH_LS_GetLastMessage(p_Stream) != MRH_LSM_LOCATION)
        {
            c_Logger.Log(MRH_PSBLogger::ERROR, "Recieved local stream message " +
                                               std::to_string(MRH_LS_GetLastMessage(p_Stream)) +
                                               ", but wanted " +
                                               std::to_string(MRH_LSM_LOCATION),
                         "CBGetLocation.cpp", __LINE__);
            continue;
        }
        else if (MRH_LS_GetLastMessageData(p_Stream, &c_Location) < 0)
        {
            c_Logger.Log(MRH_PSBLogger::ERROR, "Failed get local stream message data: " +
                                               std::string(MRH_ERR_GetLocalStreamErrorString()),
                         "CBGetLocation.cpp", __LINE__);
            continue;
        }
        
        // Got data, update location
        std::lock_guard<std::mutex> c_Guard(p_Instance->c_Mutex);
        
        p_Instance->b_LocationRecieved = true;
        p_Instance->f64_Latitude = c_Location.f64_Latitude;
        p_Instance->f64_Longtitude = c_Location.f64_Longtitude;
        p_Instance->f64_Elevation = c_Location.f64_Elevation;
        p_Instance->f64_Facing = c_Location.f64_Facing;
    }
    
    // Termination, close stream
    MRH_LS_Close(p_Stream);
}
