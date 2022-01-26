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

// Pre-defined
#ifndef SERVER_CONNECTION_WAIT_MULTIPLIER
    #define SERVER_CONNECTION_WAIT_MULTIPLIER 10
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#if MRH_USER_LOCATION_USE_SERVER > 0
CBGetLocation::CBGetLocation(Configuration const& c_Configuration) noexcept : e_ConnectionState(CONNECT_CONNECTION),
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
    
    u32_TimeoutS = c_Configuration.GetServerTimeoutS();
    u32_ClientUpdateS = c_Configuration.GetServerClientUpdateS();
    
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
    if (e_ConnectionState == LOCATION_RECIEVE_CURRENT_LOCATION)
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
    MRH_PSBLogger& c_Logger = MRH_PSBLogger::Singleton();
    
    /**
     *  Library Init
     */
    
    MRH_Srv_Context* p_Context = MRH_SRV_Init(MRH_SRV_CLIENT_PLATFORM,
                                              1, /* Connection server is replaced by communication server */
                                              p_Instance->u32_TimeoutS);
    
    if (p_Context == NULL)
    {
        c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetServerErrorString(),
                     "CBGetLocation.cpp", __LINE__);
        return;
    }
    
    MRH_Srv_Server* p_Server = MRH_SRV_CreateServer(p_Context, p_Instance->p_ComServerChannel);
    
    if (p_Server == NULL)
    {
        c_Logger.Log(MRH_PSBLogger::ERROR, MRH_ERR_GetServerErrorString(),
                     "CBGetLocation.cpp", __LINE__);
        
        MRH_SRV_Destroy(p_Context);
        return;
    }
    
    /**
     *  Client Update
     */
    
    // Define locally used connection info
    std::atomic<ConnectionState>& e_State = p_Instance->e_ConnectionState;
    
    uint8_t p_MessageBuffer[MRH_SRV_SIZE_MESSAGE_BUFFER];
    MRH_Srv_NetMessage e_Recieved;
    int i_Result;
    
    char p_Salt[MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT];
    uint32_t u32_Nonce;
    uint8_t u8_HashType;
    
    // Start updating the client
    while (p_Instance->b_RunThread == true)
    {
        // Connection warning and reset
        switch (e_State)
        {
            case CONNECT_CONNECTION:
            case CONNECT_COMMUNICATION:
            {
                break;
            }
                
            default:
            {
                if (MRH_SRV_IsConnected(p_Server) < 0)
                {
                    c_Logger.Log(MRH_PSBLogger::INFO, "Connection lost, reconnecting...",
                                 "CBGetLocation.cpp", __LINE__);
                
                    e_State = CONNECT_CONNECTION;
                }
                break;
            }
        }
        
        // State update for client connection
        switch (e_State)
        {
            /**
             *  Server Connection
             */
                
            case CONNECT_CONNECTION:
            {
                MRH_SRV_Disconnect(p_Server, -1); // Reset server object
                
                i_Result = MRH_SRV_Connect(p_Context,
                                           p_Server,
                                           p_Instance->p_ConServerAddress,
                                           p_Instance->i_ConServerPort,
                                           p_Instance->u32_TimeoutS * SERVER_CONNECTION_WAIT_MULTIPLIER);
                
                if (i_Result < 0)
                {
                    c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to connect to connection server!",
                                 "CBGetLocation.cpp", __LINE__);
                }
                
                e_State = NextState(e_State, (i_Result < 0));
                break;
            }
            case CONNECT_COMMUNICATION:
            {
                MRH_SRV_Disconnect(p_Server, -1);
                
                i_Result = MRH_SRV_Connect(p_Context,
                                           p_Server,
                                           p_Instance->p_ComServerAddress,
                                           p_Instance->i_ComServerPort,
                                           p_Instance->u32_TimeoutS * SERVER_CONNECTION_WAIT_MULTIPLIER);
                
                if (i_Result < 0)
                {
                    c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to connect to comunication server!",
                                 "CBGetLocation.cpp", __LINE__);
                }
                
                e_State = NextState(e_State, (i_Result < 0));
                break;
            }
            
            /**
             *  Authentication
             */
                
            case AUTH_SEND_REQUEST_CONNECTION:
            case AUTH_SEND_REQUEST_COMMUNICATION:
            {
                MRH_SRV_C_MSG_AUTH_REQUEST_DATA c_Request;
                
                strncpy(c_Request.p_Mail, p_Instance->p_AccountMail, MRH_SRV_SIZE_ACCOUNT_MAIL);
                strncpy(c_Request.p_DeviceKey, p_Instance->p_DeviceKey, MRH_SRV_SIZE_DEVICE_KEY);
                c_Request.u8_Actor = MRH_SRV_CLIENT_PLATFORM;
                c_Request.u8_Version = MRH_SRV_NET_MESSAGE_VERSION;
                
                i_Result = MRH_SRV_SendMessage(p_Server, MRH_SRV_C_MSG_AUTH_REQUEST, &c_Request, NULL);
                
                if (i_Result < 0)
                {
                    c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to send auth request net message: " +
                                                       std::string(MRH_ERR_GetServerErrorString()),
                                 "CBGetLocation.cpp", __LINE__);
                }
                
                e_State = NextState(e_State, (i_Result < 0));
                break;
            }
            case AUTH_RECIEVE_CHALLENGE_CONNECTION:
            case AUTH_RECIEVE_CHALLENGE_COMMUNICATION:
            {
                do
                {
                    e_Recieved = RecieveServerMessage(p_Server,
                                                      { MRH_SRV_S_MSG_AUTH_RESULT,
                                                        MRH_SRV_S_MSG_AUTH_CHALLENGE },
                                                      p_MessageBuffer,
                                                      NULL);
                    
                    if (e_Recieved == MRH_SRV_S_MSG_AUTH_CHALLENGE)
                    {
                        MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA c_Challenge;
                        
                        if (MRH_SRV_SetNetMessage(&c_Challenge, p_MessageBuffer) < 0)
                        {
                            c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to set auth challenge message!",
                                         "CBGetLocation.cpp", __LINE__);
                            
                            e_State = NextState(e_State, true);
                        }
                        else
                        {
                            // Copy to use in next step
                            memcpy(p_Salt, c_Challenge.p_Salt, MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT);
                            u32_Nonce = c_Challenge.u32_Nonce;
                            u8_HashType = c_Challenge.u8_HashType;
                            
                            e_State = NextState(e_State, false);
                        }
                        
                        break;
                    }
                    else if (e_Recieved == MRH_SRV_S_MSG_AUTH_RESULT)
                    {
                        c_Logger.Log(MRH_PSBLogger::ERROR, "Failed to authenticate!",
                                     "CBGetLocation.cpp", __LINE__);
                        
                        // Log error if possible
                        MRH_SRV_S_MSG_AUTH_RESULT_DATA c_Result;
                        if (MRH_SRV_SetNetMessage(&c_Result, p_MessageBuffer) == 0)
                        {
                            c_Logger.Log(MRH_PSBLogger::ERROR, "Auth request returned error: " +
                                                               std::to_string(c_Result.u8_Result),
                                         "CBGetLocation.cpp", __LINE__);
                        }
                        
                        // Auth result here always means failure
                        e_State = NextState(e_State, true);
                        break;
                    }
                }
                while (e_Recieved != MRH_SRV_CS_MSG_UNK);
                
                break;
            }
            case AUTH_SEND_PROOF_CONNECTION:
            case AUTH_SEND_PROOF_COMMUNICATION:
            {
                break;
            }
            case AUTH_RECIEVE_RESULT_CONNECTION:
            case AUTH_RECIEVE_RESULT_COMMUNICATION:
            {
                break;
            }
                
            /**
             *  Channel
             */
                
            case CHANNEL_SEND_REQUEST:
            {
                break;
            }
            case CHANNEL_RECIEVE_RESPONSE:
            {
                break;
            }
                
            /**
             *  Client Pairing
             */
                
            case CLIENT_RECIEVE_REQUEST:
            {
                break;
            }
            case CLIENT_SEND_CHALLENGE:
            {
                break;
            }
            case CLIENT_RECIEVE_PROOF:
            {
                break;
            }
            case CLIENT_SEND_RESULT:
            {
                break;
            }
                
            /**
             *  Location
             */
                
            case LOCATION_RECIEVE_FIRST_LOCATION:
            {
                break;
            }
            case LOCATION_RECIEVE_CURRENT_LOCATION:
            {
                break;
            }
                
            /**
             *  Unk
             */
                
            // Failed, unknown, etc
            default:
            {
                c_Logger.Log(MRH_PSBLogger::ERROR, "Unknown client state, stopping client!",
                             "CBGetLocation.cpp", __LINE__);
                
                p_Instance->b_RunThread = false;
                break;
            }
        }
    }
    
    // Clean up
    MRH_SRV_DestroyServer(p_Context, p_Server);
    MRH_SRV_Destroy(p_Context);
}

CBGetLocation::ConnectionState CBGetLocation::NextState(ConnectionState e_State, bool b_Failed) noexcept
{
    switch (e_State)
    {
        // Server Connection
        case CONNECT_CONNECTION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_SEND_REQUEST_CONNECTION;
        case CONNECT_COMMUNICATION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_SEND_REQUEST_COMMUNICATION;
            
        // Authentication
        case AUTH_SEND_REQUEST_CONNECTION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_RECIEVE_CHALLENGE_CONNECTION;
        case AUTH_SEND_REQUEST_COMMUNICATION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_RECIEVE_CHALLENGE_COMMUNICATION;
        case AUTH_RECIEVE_CHALLENGE_CONNECTION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_SEND_PROOF_CONNECTION;
        case AUTH_RECIEVE_CHALLENGE_COMMUNICATION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_SEND_PROOF_COMMUNICATION;
        case AUTH_SEND_PROOF_CONNECTION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_RECIEVE_RESULT_CONNECTION;
        case AUTH_SEND_PROOF_COMMUNICATION:
            return b_Failed ? CONNECT_CONNECTION : AUTH_RECIEVE_RESULT_COMMUNICATION;
        case AUTH_RECIEVE_RESULT_CONNECTION:
            return b_Failed ? CONNECT_CONNECTION : CHANNEL_SEND_REQUEST;
        case AUTH_RECIEVE_RESULT_COMMUNICATION:
            return b_Failed ? CONNECT_CONNECTION : CONNECT_COMMUNICATION;
            
        // Channel
        case CHANNEL_SEND_REQUEST:
            return b_Failed ? CONNECT_CONNECTION : CHANNEL_RECIEVE_RESPONSE;
        case CHANNEL_RECIEVE_RESPONSE:
            return b_Failed ? CONNECT_CONNECTION : CONNECT_COMMUNICATION;
            
        // Client Pairing
        case CLIENT_RECIEVE_REQUEST:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : CLIENT_SEND_CHALLENGE;
        case CLIENT_SEND_CHALLENGE:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : CLIENT_RECIEVE_PROOF;
        case CLIENT_RECIEVE_PROOF:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : CLIENT_SEND_RESULT;
        case CLIENT_SEND_RESULT:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : LOCATION_RECIEVE_FIRST_LOCATION;
            
        // Recieve Location
        case LOCATION_RECIEVE_FIRST_LOCATION:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : LOCATION_RECIEVE_CURRENT_LOCATION;
        case LOCATION_RECIEVE_CURRENT_LOCATION:
            return b_Failed ? CLIENT_RECIEVE_REQUEST : LOCATION_RECIEVE_CURRENT_LOCATION;
            
        // Default
        default:
            return CONNECT_CONNECTION;
    }
}

MRH_Srv_NetMessage CBGetLocation::RecieveServerMessage(MRH_Srv_Server* p_Server, std::vector<MRH_Srv_NetMessage> v_Message, uint8_t* p_Buffer, const char* p_Password) noexcept
{
    MRH_Srv_NetMessage e_Message;
    
    while (true)
    {
        if ((e_Message = MRH_SRV_RecieveMessage(p_Server, p_Buffer, p_Password)) == MRH_SRV_CS_MSG_UNK)
        {
            return e_Message;
        }
        
        for (auto& Message : v_Message)
        {
            if (Message == e_Message)
            {
                return e_Message;
            }
        }
    }
}
#endif
