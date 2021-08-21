/**
 *  Main.cpp
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
#include <cstdlib>

// External
#include <libmrhpsb.h>

// Project
#include "./Callback/Service/CBAvail.h"
#include "./Callback/Service/CBReset.h"
#include "./Callback/Service/CBCustomCommand.h"
#include "./Callback/Content/CBAccessContent.h"
#include "./Callback/Content/CBAccessClear.h"
#include "./Callback/Location/CBGetLocation.h"
#include "./Content/Content.h"
#include "./Revision.h"

// Pre-defined
#ifndef MRH_USER_SERVICE_THREAD_COUNT
    #define MRH_USER_SERVICE_THREAD_COUNT 2
#endif


//*************************************************************************************
// Exit
//*************************************************************************************

static int Exit(libmrhpsb* p_Context, const char* p_Exception, int i_Result)
{
    if (p_Context != NULL)
    {
        delete p_Context;
    }
    
    if (p_Exception != NULL)
    {
        MRH_PSBLogger::Singleton().Log(MRH_PSBLogger::ERROR, p_Exception,
                                       "Main.cpp", __LINE__);
    }
    
    return i_Result;
}

//*************************************************************************************
// Main
//*************************************************************************************

int main(int argc, const char* argv[])
{
    // Setup service base
    MRH_PSBLogger& c_Logger = MRH_PSBLogger::Singleton();
    libmrhpsb* p_Context;
    
    try
    {
        p_Context = new libmrhpsb("mrhpsuser",
                                  argc,
                                  argv,
                                  MRH_USER_SERVICE_THREAD_COUNT,
                                  true);
    }
    catch (MRH_PSBException& e)
    {
        return Exit(NULL, e.what(), EXIT_FAILURE);
    }
    catch (std::exception& e)
    {
        return Exit(NULL, e.what(), EXIT_FAILURE);
    }
    
    try
    {
        // Create the user content
        std::shared_ptr<Content> p_Content(new Content());
        
        // Add callbacks
        std::shared_ptr<MRH_Callback> p_CBAvail(new CBAvail(p_Content));
        std::shared_ptr<MRH_Callback> p_CBReset(new CBReset(p_Content));
        std::shared_ptr<MRH_Callback> p_CBCustomCommand(new CBCustomCommand());
        
        std::shared_ptr<MRH_Callback> p_CBAccessContent(new CBAccessContent(p_Content));
        std::shared_ptr<MRH_Callback> p_CBAccessClear(new CBAccessClear(p_Content));
        
        std::shared_ptr<MRH_Callback> p_CBGetLocation(new CBGetLocation());
        
        p_Context->AddCallback(p_CBAvail, MRH_EVENT_USER_AVAIL_U);
        p_Context->AddCallback(p_CBReset, MRH_EVENT_PS_RESET_REQUEST_U);
        p_Context->AddCallback(p_CBCustomCommand, MRH_EVENT_USER_CUSTOM_COMMAND_U);
        
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_DOCUMENTS_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_PICTURES_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_MUSIC_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_VIDEOS_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_DOWNLOADS_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_CLIPBOARD_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_INFO_PERSON_U);
        p_Context->AddCallback(p_CBAccessContent, MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U);
        p_Context->AddCallback(p_CBAccessClear, MRH_EVENT_USER_ACCESS_CLEAR_U);
        
        p_Context->AddCallback(p_CBGetLocation, MRH_EVENT_USER_GET_LOCATION_U);
    }
    catch (MRH_PSBException& e)
    {
        return Exit(p_Context, e.what(), EXIT_FAILURE);
    }
    catch (Exception& e)
    {
        return Exit(p_Context, e.what(), EXIT_FAILURE);
    }
    catch (std::exception& e)
    {
        return Exit(p_Context, e.what(), EXIT_FAILURE);
    }
    
    c_Logger.Log(MRH_PSBLogger::INFO, "mrhpsuser setup successfull.",
                 "Main.cpp", __LINE__);
    
    // Update service until termination
    p_Context->Update();
    
    // Exit
    c_Logger.Log(MRH_PSBLogger::INFO, "Terminating service.",
                 "Main.cpp", __LINE__);
    delete p_Context;
    return EXIT_SUCCESS;
}
