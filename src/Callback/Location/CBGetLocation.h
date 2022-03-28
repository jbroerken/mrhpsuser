/**
 *  CBGetLocation.h
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
