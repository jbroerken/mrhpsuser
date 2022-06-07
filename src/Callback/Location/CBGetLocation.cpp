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
    int i_Result;
    
    // Build stream first
    c_Logger.Log(MRH_PSBLogger::INFO, "Opening local stream: " + s_FilePath,
                 "CBGetLocation.cpp", __LINE__);
    
    MRH_LocalStream* p_Stream = MRH_LS_Open(s_FilePath.c_str(), 0);
    
    if (p_Stream == NULL)
    {
        c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetLocalStreamErrorString(),
                     "CBGetLocation.cpp", __LINE__);
        return;
    }
    
    // Now start reading
    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;
    
    MRH_LS_M_Location_Data c_Location;
    MRH_LS_M_Version_Data c_Version;
    
    c_Version.u32_Version = MRH_STREAM_MESSAGE_VERSION;
    
    while (p_Instance->b_Update == true)
    {
        /**
         *  Connect
         */
        
        if (MRH_LS_GetConnected(p_Stream) < 0)
        {
            // Attempt to connect
            if (MRH_LS_Connect(p_Stream) < 0)
            {
                // Wait before retry if connection error
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            
            // Connected, add version info
            if (MRH_LS_MessageToBuffer(p_Buffer, &u32_Size, MRH_LS_M_VERSION, &c_Version) < 0)
            {
                c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetLocalStreamErrorString(),
                             "CBGetLocation.cpp", __LINE__);
            }
            else
            {
                // Continue until fully written
                while ((i_Result = MRH_LS_Write(p_Stream, p_Buffer, u32_Size)) != 0)
                {
                    if (i_Result < 0)
                    {
                        c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetLocalStreamErrorString(),
                                     "CBGetLocation.cpp", __LINE__);
                        break;
                    }
                }
            }
        }
        
        /**
         *  Read
         */
        
        // Read data until a full message was read
        if ((i_Result = MRH_LS_Read(p_Stream, 100, p_Buffer, &u32_Size)) != 0)
        {
            if (i_Result < 0)
            {
                c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetLocalStreamErrorString(),
                             "CBGetLocation.cpp", __LINE__);
                MRH_LS_Disconnect(p_Stream);
            }
            
            // > 0 handled, not finished
            continue;
        }
        
        // Check message and get message data
        if (MRH_LS_GetBufferMessage(p_Buffer) != MRH_LS_M_LOCATION)
        {
            c_Logger.Log(MRH_PSBLogger::ERROR, "Recieved invalid local stream message!",
                         "CBGetLocation.cpp", __LINE__);
            continue;
        }
        else if (MRH_LS_BufferToMessage(&c_Location, p_Buffer, u32_Size) < 0)
        {
            c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetLocalStreamErrorString(),
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
