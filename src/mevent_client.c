#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "overworld.h"
#include "script.h"
#include "battle_tower.h"
#include "mystery_event_script.h"
#include "mevent.h"
#include "mevent_server.h"

static EWRAM_DATA struct mevent_client * s_mevent_client_ptr = NULL;

static void mevent_client_init(struct mevent_client *, u32, u32);
static u32 mevent_client_exec(struct mevent_client *);
static void mevent_client_free_resources(struct mevent_client *);

extern const struct mevent_client_cmd gMEventClientScript_InitialListen[];

void mevent_client_do_init(void)
{
    s_mevent_client_ptr = AllocZeroed(sizeof(struct mevent_client));
    mevent_client_init(s_mevent_client_ptr, 1, 0);
}

u32 mevent_client_do_exec(u16 * a0)
{
    u32 result;
    if (s_mevent_client_ptr == NULL)
        return 6;
    result = mevent_client_exec(s_mevent_client_ptr);
    if (result == 6)
    {
        *a0 = s_mevent_client_ptr->param;
        mevent_client_free_resources(s_mevent_client_ptr);
        Free(s_mevent_client_ptr);
        s_mevent_client_ptr = NULL;
    }
    return result;
}

void mevent_client_inc_flag(void)
{
    s_mevent_client_ptr->flag++;
}

void * mevent_client_get_buffer(void)
{
    return s_mevent_client_ptr->buffer;
}

void mevent_client_set_param(u32 a0)
{
    s_mevent_client_ptr->param = a0;
}

static void mevent_client_init(struct mevent_client * svr, u32 sendPlayerNo, u32 recvPlayerNo)
{
    svr->unk_00 = 0;
    svr->mainseqno = 0;
    svr->flag = 0;
    svr->sendBuffer = AllocZeroed(ME_SEND_BUF_SIZE);
    svr->recvBuffer = AllocZeroed(ME_SEND_BUF_SIZE);
    svr->cmdBuffer = AllocZeroed(ME_SEND_BUF_SIZE);
    svr->buffer = AllocZeroed(0x40);
    mevent_srv_sub_init(&svr->manager, sendPlayerNo, recvPlayerNo);
}

static void mevent_client_free_resources(struct mevent_client * svr)
{
    Free(svr->sendBuffer);
    Free(svr->recvBuffer);
    Free(svr->cmdBuffer);
    Free(svr->buffer);
}

static void mevent_client_jmp_buffer(struct mevent_client * svr)
{
    memcpy(svr->cmdBuffer, svr->recvBuffer, ME_SEND_BUF_SIZE);
    svr->cmdidx = 0;
}

static void mevent_client_send_word(struct mevent_client * svr, u32 ident, u32 word)
{
    CpuFill32(0, svr->sendBuffer, ME_SEND_BUF_SIZE);
    *(u32 *)svr->sendBuffer = word;
    mevent_srv_sub_init_send(&svr->manager, ident, svr->sendBuffer, sizeof(u32));
}

static u32 client_mainseq_0(struct mevent_client * svr)
{
    // init
    memcpy(svr->cmdBuffer, gMEventClientScript_InitialListen, ME_SEND_BUF_SIZE);
    svr->cmdidx = 0;
    svr->mainseqno = 4;
    svr->flag = 0;
    return 0;
}

static u32 client_mainseq_1(struct mevent_client * svr)
{
    // done
    return 6;
}

static u32 client_mainseq_2(struct mevent_client * svr)
{
    // do recv
    if (mevent_srv_sub_recv(&svr->manager))
    {
        svr->mainseqno = 4;
        svr->flag = 0;
    }
    return 1;
}

static u32 client_mainseq_3(struct mevent_client * svr)
{
    // do send
    if (mevent_srv_sub_send(&svr->manager))
    {
        svr->mainseqno = 4;
        svr->flag = 0;
    }
    return 1;
}

static u32 client_mainseq_4(struct mevent_client * svr)
{
    // process command
    struct mevent_client_cmd * cmd = &svr->cmdBuffer[svr->cmdidx];
    ++svr->cmdidx;
    switch (cmd->instr)
    {
    case 0: // nop
        break;
    case 1: // Return (parameter)
        svr->param = cmd->parameter;
        svr->mainseqno = 1;
        svr->flag = 0;
        break;
    case 2: // Wait for receive with expected magic number (parameter)
        mevent_srv_sub_init_recv(&svr->manager, cmd->parameter, svr->recvBuffer);
        svr->mainseqno = 2;
        svr->flag = 0;
        break;
    case 3: // Wait for send to finish
        svr->mainseqno = 3;
        svr->flag = 0;
        break;
    case 20: // Send entire buffer; use magic number 0x14 (server doesn't use information, as it immediately returns)
        mevent_srv_sub_init_send(&svr->manager, 0x14, svr->sendBuffer, 0);
        svr->mainseqno = 3;
        svr->flag = 0;
        break;
    case 19: // Send game stat (parameter); use magic number 0x12 (unused)
        mevent_client_send_word(svr, 0x12, GetGameStat(cmd->parameter));
        svr->mainseqno = 3;
        svr->flag = 0;
        break;
    case 6: // If (result) is 0, write recvBuffer to cmdBuffer
        if (svr->param == 0)
            mevent_client_jmp_buffer(svr);
        break;
    case 7: // If (result) is 1, write recvBuffer to cmdBuffer
        if (svr->param == 1)
            mevent_client_jmp_buffer(svr);
        break;
    case 4: // Write recvBuffer to cmdBuffer
        mevent_client_jmp_buffer(svr);
        break;
    case 5: // Display message and yes/no prompt, and write to (result)
        memcpy(svr->buffer, svr->recvBuffer, 0x40);
        svr->mainseqno = 5;
        svr->flag = 0;
        return 2;
    case 11: // Display message
        memcpy(svr->buffer, svr->recvBuffer, 0x40);
        svr->mainseqno = 5;
        svr->flag = 0;
        return 3;
    case 12: // [[???]] Elevates recvBuffer higher
        memcpy(svr->buffer, svr->recvBuffer, 0x40);
        svr->mainseqno = 5;
        svr->flag = 0;
        return 5;
    case 13: // Prompt to replace Wonder Card, and write to (result)
        svr->mainseqno = 5;
        svr->flag = 0;
        return 4;
    case 8: // Begin sending client's header information; use magic number 0x11 (server expects client header information)
        BuildMEventClientHeader(svr->sendBuffer);
        mevent_srv_sub_init_send(&svr->manager, 0x11, svr->sendBuffer, sizeof(struct MEventClientHeaderStruct));
        break;
    case 14: // Begin sending (result); use magic number 0x13 (server expects 32 bits)
        mevent_client_send_word(svr, 0x13, svr->param);
        break;
    case 10: // Replace Wonder Card with the one from the server
        OverwriteSavedWonderCardWithReceivedCard(svr->recvBuffer);
        break;
    case 9: // Replace Wonder News with the one from the server if the ID is different, and begin sending whether the operation occurred or not; use magic number 0x13 (server expects 32 bits)
        if (!MEvent_HaveAlreadyReceivedWonderNews(svr->recvBuffer))
        {
            OverwriteSavedWonderNewsWithReceivedNews(svr->recvBuffer);
            mevent_client_send_word(svr, 0x13, 0);
        }
        else
            // Other trainer already has news
            mevent_client_send_word(svr, 0x13, 1);
        break;
    case 15: // Treat recvBuffer as Mystery Event rather than Mystery Gift, and set (result) to data[2] or last call to SetMysteryEventScriptStatus (Immediate Execution Script cmd 0x0E)
        svr->mainseqno = 6;
        svr->flag = 0;
        break;
    case 16: // Add PKMN icon in recvBuffer to bottom of Wonder Card if possible
        MEvent_ReceiveDistributionMon(svr->recvBuffer);
        break;
    case 17: // Copy recvBuffer to RAM Script
        MEventSetRamScript(svr->recvBuffer, 1000);
        break;
    case 18: // Copy recvBuffer to EReader trainer (use magic number 0x1a?)
        memcpy(&gSaveBlock2Ptr->battleTower.ereaderTrainer, svr->recvBuffer, sizeof(struct BattleTowerEReaderTrainer));
        ValidateEReaderTrainer();
        break;
    case 21: // Arbitrary code execution of recvBuffer; set (result)
        memcpy(gDecompressionBuffer, svr->recvBuffer, ME_SEND_BUF_SIZE);
        svr->mainseqno = 7;
        svr->flag = 0;
        break; 
    }

    return 1;
}

static u32 client_mainseq_5(struct mevent_client * svr)
{
    // wait flag
    if (svr->flag)
    {
        svr->mainseqno = 4;
        svr->flag = 0;
    }
    return 1;
}

static u32 client_mainseq_6(struct mevent_client * svr)
{
    // Run mevent buffer script
    switch (svr->flag)
    {
    case 0:
        MEventScript_InitContext(svr->recvBuffer);
        ++svr->flag;
        break;
    case 1:
        if (!MEventScript_Run(&svr->param))
        {
            svr->mainseqno = 4;
            svr->flag = 0;
        }
        break;
    }
    return 1;
}

static u32 client_mainseq_7(struct mevent_client * svr)
{
    // exec arbitrary code
    u32 (*func)(u32 *, struct SaveBlock2 *, struct SaveBlock1 *) = (void *)gDecompressionBuffer;
    if (func(&svr->param, gSaveBlock2Ptr, gSaveBlock1Ptr) == 1)
    {
        svr->mainseqno = 4;
        svr->flag = 0;
    }
    return 1;
}

static u32 mevent_client_exec(struct mevent_client * svr)
{
    u32 (*funcs[])(struct mevent_client *) = {
        client_mainseq_0,
        client_mainseq_1,
        client_mainseq_2,
        client_mainseq_3,
        client_mainseq_4,
        client_mainseq_5,
        client_mainseq_6,
        client_mainseq_7
    };
    return funcs[svr->mainseqno](svr);
}
