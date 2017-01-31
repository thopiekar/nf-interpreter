//
// Copyright (c) 2017 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <WireProtocol.h>
#include <WireProtocol_MonitorCommands.h>

static const int AccessMemory_Check    = 0x00;
static const int AccessMemory_Read     = 0x01;
static const int AccessMemory_Write    = 0x02;
static const int AccessMemory_Erase    = 0x03;
static const int AccessMemory_Mask     = 0x0F;

//////////////////////////////////////////////////////////////////////
// helper functions

bool NanoBooter_GetReleaseInfo(ReleaseInfo* releaseInfo)
{
    releaseInfo->version.usMajor = 0;
    releaseInfo->version.usMinor = 0;
    releaseInfo->version.usBuild = 12345;
    releaseInfo->version.usRevision = 1;

    // TODO replace this with string from (possibly...) main config file
    memcpy(&releaseInfo->infoString, ">>>> nanoFramework RULES (nanoBooter here) <<<<", sizeof(releaseInfo->infoString));

    return true;
}

static bool AccessMemory(uint32_t location, uint32_t lengthInBytes, uint8_t* buffer, int mode)
{
    
    return true;
}

////////////////////////////////////////////////////

bool Monitor_Ping(WP_Message* message)
{
    if((message->m_header.m_flags & WP_Flags_c_Reply) == 0)
    {
        Monitor_Ping_Reply cmdReply;
        cmdReply.m_source = Monitor_Ping_c_Ping_Source_NanoBooter;

        ReplyToCommand(message, true, false, &cmdReply, sizeof(cmdReply));
    }

    return true;
}

bool Monitor_OemInfo(WP_Message* message)
{
    if((message->m_header.m_flags & WP_Flags_c_Reply) == 0)
    {
        Monitor_OemInfo_Reply cmdReply;     
        
        bool fOK = NanoBooter_GetReleaseInfo(&cmdReply.m_releaseInfo) == true;
        
        ReplyToCommand(message, fOK, false, &cmdReply, sizeof(cmdReply));
    }

    return true;
}

bool Monitor_WriteMemory(WP_Message* message)
{
    bool ret;
    
    CLR_DBG_Commands_Monitor_WriteMemory* cmd = (CLR_DBG_Commands_Monitor_WriteMemory*)message->m_payload;

    // TODO: not sure if we really need this
    // if(!m_signedDataState.VerifyContiguousData(cmd->m_address, cmd->m_length))
    // {
    //     m_signedDataState.EraseMemoryAndReset();
        
    //     return false;
    // }

    // TODO: not sure if we really need this
    // TinyBooter_OnStateChange(State_MemoryWrite, (void*)cmd->m_address);

    // assume at RAM, directly use the original address 
    ret = AccessMemory(cmd->address, cmd->length, cmd->data, AccessMemory_Write);
  
    ReplyToCommand(message, ret, false, NULL, 0);

    return ret;
}