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
#include "./Configuration.h"
#include "./Revision.h"

// Pre-defined
#ifndef MRH_USER_SERVICE_THREAD_COUNT
    #define MRH_USER_SERVICE_THREAD_COUNT 1
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
    libmrhpsb* p_Context = NULL;
    
    try
    {
        // Build library context first
        p_Context = new libmrhpsb("mrhpsuser",
                                  argc,
                                  argv,
                                  MRH_USER_SERVICE_THREAD_COUNT);
        
        // Next, load config for service data
        Configuration c_Configuration;
        
        // Create the user content
        std::shared_ptr<Content> p_Content(new Content(c_Configuration));
        
        // Create callbacks
        std::shared_ptr<MRH_Callback> p_CBAvail(new CBAvail(p_Content));
        std::shared_ptr<MRH_Callback> p_CBReset(new CBReset(p_Content));
        std::shared_ptr<MRH_Callback> p_CBCustomCommand(new CBCustomCommand());
        
        std::shared_ptr<MRH_Callback> p_CBAccessContent(new CBAccessContent(p_Content));
        std::shared_ptr<MRH_Callback> p_CBAccessClear(new CBAccessClear(p_Content));
        
        std::shared_ptr<MRH_Callback> p_CBGetLocation(new CBGetLocation(c_Configuration));
        
        // Add created callbacks
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
