/**
 *  Configuration.h
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

#ifndef Configuration_h
#define Configuration_h

// C / C++

// External
#include <MRH_Typedefs.h>

// Project
#include "./Exception.h"


class Configuration
{
public:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_Configuration Configuration class source.
     */

    Configuration(Configuration const& c_Configuration) = delete;
    
    //*************************************************************************************
    // Singleton
    //*************************************************************************************

    /**
     *  Get the class instance.
     *
     *  \return The class instance.
     */

    static Configuration& Singleton() noexcept;
    
    //*************************************************************************************
    // Load
    //*************************************************************************************

    /**
     *  Load the configuration.
     */
    
    void Load();

    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the server account mail.
     *
     *  \return The server account mail.
     */
    
    std::string GetServerAccountMail() noexcept;
    
    /**
     *  Get the server account password.
     *
     *  \return The server account password.
     */
    
    std::string GetServerAccountPassword() noexcept;
    
    /**
     *  Get the server device key.
     *
     *  \return The server device key.
     */
    
    std::string GetServerDeviceKey() noexcept;
    
    /**
     *  Get the server device password.
     *
     *  \return The server device password.
     */
    
    std::string GetServerDevicePassword() noexcept;
    
    /**
     *  Get the server connection server address.
     *
     *  \return The server connection server address.
     */
    
    std::string GetServerConnectionAddress() noexcept;
    
    /**
     *  Get the server connection server port.
     *
     *  \return The server connection server port.
     */
    
    int GetServerConnectionPort() noexcept;
    
    /**
     *  Get the server communication server channel.
     *
     *  \return The server communication server channel.
     */
    
    std::string GetServerCommunicationChannel() noexcept;
    
    /**
     *  Get the server timeout in seconds.
     *
     *  \return The server timeout in seconds.
     */
    
    MRH_Uint32 GetServerTimeoutS() noexcept;
    
private:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    Configuration() noexcept;

    /**
     *  Default destructor.
     */

    ~Configuration() noexcept;

    //*************************************************************************************
    // Data
    //**************************************************************************************

    // Server
    std::string s_ServerAccountMail;
    std::string s_ServerAccountPassword;
    std::string s_ServerDeviceKey;
    std::string s_ServerDevicePassword;
    std::string s_ServerConnectionAddress;
    int i_ServerConnectionPort;
    std::string s_ServerCommunicationChannel;
    MRH_Uint32 u32_ServerTimeoutS;
    
protected:

};

#endif /* Configuration_h */
