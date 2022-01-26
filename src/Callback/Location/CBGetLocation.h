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

#define MRH_USER_LOCATION_USE_SERVER 1

// C / C++
#if MRH_USER_LOCATION_USE_SERVER > 0
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#endif

// External
#include <libmrhpsb/MRH_Callback.h>
#if MRH_USER_LOCATION_USE_SERVER > 0
#include <libmrhsrv.h>
#endif

// Project
#include "../../Configuration.h"


class CBGetLocation : public MRH_Callback
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
#if MRH_USER_LOCATION_USE_SERVER > 0
    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to construct with.
     */
    
    CBGetLocation(Configuration const& c_Configuration) noexcept;
#else
    /**
     *  Default constructor.
     */
    
    CBGetLocation() noexcept;
#endif
    
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
    // Types
    //*************************************************************************************

#if MRH_USER_LOCATION_USE_SERVER > 0
    enum ConnectionState
    {
        // Server Connection
        CONNECT_CONNECTION = 0,
        CONNECT_COMMUNICATION = 1,
        
        // Authentication
        AUTH_SEND_REQUEST_CONNECTION = 2,
        AUTH_SEND_REQUEST_COMMUNICATION = 3,
        AUTH_RECIEVE_CHALLENGE_CONNECTION = 4,
        AUTH_RECIEVE_CHALLENGE_COMMUNICATION = 5,
        AUTH_SEND_PROOF_CONNECTION = 6,
        AUTH_SEND_PROOF_COMMUNICATION = 7,
        AUTH_RECIEVE_RESULT_CONNECTION,
        AUTH_RECIEVE_RESULT_COMMUNICATION,
        
        // Channel
        CHANNEL_SEND_REQUEST,
        CHANNEL_RECIEVE_RESPONSE,
        
        // Client Pairing
        CLIENT_RECIEVE_REQUEST,
        CLIENT_SEND_CHALLENGE,
        CLIENT_RECIEVE_PROOF,
        CLIENT_SEND_RESULT,
        
        // Recieve Location
        LOCATION_RECIEVE_FIRST_LOCATION, // @NOTE: Used for connected, but not yet recieved (callback = false)
        LOCATION_RECIEVE_CURRENT_LOCATION,
        
        // Bounds
        CONNECTION_STATE_MAX = LOCATION_RECIEVE_CURRENT_LOCATION,
        
        CONNECTION_STATE_COUNT = CONNECTION_STATE_MAX + 1
    };
    
    //*************************************************************************************
    // Client
    //*************************************************************************************
    
    /**
     *  Update the location platform client.
     *
     *  \param p_Instance The instance to update with.
     */
    
    static void ClientUpdate(CBGetLocation* p_Instance) noexcept;
    
    /**
     *  Get the next connection state.
     *
     *  \param e_State The current connection state.
     *  \param b_Failed If the current state failed or not.
     *
     *  \return The next connection state.
     */
    
    static ConnectionState NextState(ConnectionState e_State, bool b_Failed) noexcept;
    
    /**
     *  Recieve a message from a server.
     *
     *  \param p_Server The server to recieve from.
     *  \param v_Message The wanted messages.
     *  \param p_Buffer The buffer used to store a wanted message.
     *  \param p_Password The password to use for message decryption.
     *
     *  \return The first matching recieved message on success, MRH_SRV_CS_MSG_UNK on failure.
     */
    
    MRH_Srv_NetMessage RecieveServerMessage(MRH_Srv_Server* p_Server, std::vector<MRH_Srv_NetMessage> v_Message, uint8_t* p_Buffer, const char* p_Password) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Thread
    std::thread c_Thread;
    std::atomic<bool> b_RunThread;
    
    // Connection
    std::atomic<ConnectionState> e_ConnectionState;
    
    char p_AccountMail[MRH_SRV_SIZE_ACCOUNT_MAIL];
    char p_AccountPassword[MRH_SRV_SIZE_ACCOUNT_PASSWORD];
    
    char p_DeviceKey[MRH_SRV_SIZE_DEVICE_KEY];
    char p_DevicePassword[MRH_SRV_SIZE_DEVICE_PASSWORD];
    
    char p_ConServerAddress[MRH_SRV_SIZE_SERVER_ADDRESS];
    int i_ConServerPort;
    
    char p_ComServerChannel[MRH_SRV_SIZE_SERVER_CHANNEL];
    char p_ComServerAddress[MRH_SRV_SIZE_SERVER_ADDRESS];
    int i_ComServerPort;
    
    MRH_Uint32 u32_TimeoutS;
    MRH_Uint32 u32_ClientUpdateS;

    // Location
    std::mutex c_LocationMutex;
#endif
    MRH_Sfloat64 f64_Latitude;
    MRH_Sfloat64 f64_Longtitude;
    MRH_Sfloat64 f64_Elevation;
    MRH_Sfloat64 f64_Facing;
    
protected:

};

#endif /* CBGetLocation_h */
