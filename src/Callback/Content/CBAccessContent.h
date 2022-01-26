/**
 *  CBAccessContent.h
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

#ifndef CBAccessContent_h
#define CBAccessContent_h

// C / C++
#include <memory>

// External
#include <libmrhpsb/MRH_Callback.h>

// Project
#include "../../Content/Content.h"


class CBAccessContent : public MRH_Callback
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param p_Content The content information to create on callback.
     */
    
    CBAccessContent(std::shared_ptr<Content>& p_Content) noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~CBAccessContent() noexcept;
    
    //*************************************************************************************
    // Callback
    //*************************************************************************************
    
    /**
     *  Perform a callback with a recieved content access event.
     *
     *  \param p_Event The recieved content access event.
     *  \param u32_GroupID The event group id for the user event.  
     */
    
    void Callback(const MRH_EVBase* p_Event, MRH_Uint32 u32_GroupID) noexcept override;
    
private:
    
    //*************************************************************************************
    // Callback
    //*************************************************************************************
    
    /**
     *  Create content access.
     *
     *  \param u32_Type The content access event type.
     *
     *  \return The access service event.
     */
    
    bool CreateAccess(MRH_Uint32 u32_Type) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::shared_ptr<Content> p_Content;
    
protected:

};

#endif /* CBAccessContent_h */
