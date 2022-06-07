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

#ifndef CBGetLocation_h
#define CBGetLocation_h

// C / C++
#include <thread>
#include <mutex>
#include <atomic>

// External
#include <libmrhpsb/MRH_Callback.h>

// Project
#include "../../Configuration.h"


class CBGetLocation : public MRH_Callback
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to construct with.
     */
    
    CBGetLocation(Configuration const& c_Configuration);
    
    /**
     *  Default destructor.
     */
    
    ~CBGetLocation() noexcept;
    
    //*************************************************************************************
    // Callback
    //*************************************************************************************
    
    /**
     *  Perform a callback with a recieved get location event.
     *
     *  \param p_Event The recieved get location event.
     *  \param u32_GroupID The event group id for the user event.
     */
    
    void Callback(const MRH_Event* p_Event, MRH_Uint32 u32_GroupID) noexcept override;
    
private:
    
    //*************************************************************************************
    // Stream
    //*************************************************************************************
    
    /**
     *  Update the location local stream.
     *  
     *  \param p_Instance The callback instance to update with.
     *  \param s_FilePath The full path to the stream socket file.
     */
    
    static void UpdateStream(CBGetLocation* p_Instance, std::string s_FilePath) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::thread c_Thread;
    std::mutex c_Mutex;
    std::atomic<bool> b_Update;
    
    bool b_LocationRecieved;
    MRH_Sfloat64 f64_Latitude;
    MRH_Sfloat64 f64_Longtitude;
    MRH_Sfloat64 f64_Elevation;
    MRH_Sfloat64 f64_Facing;
    
protected:

};

#endif /* CBGetLocation_h */
