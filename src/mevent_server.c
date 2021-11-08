#include "global.h"
#include "gflib.h"
#include "script.h"
#include "mevent.h"
#include "mevent_server.h"

EWRAM_DATA struct mevent_srv_common * s_mevent_srv_common_ptr = NULL;

static void mevent_srv_init_common(struct mevent_srv_common *, const void *, u32, u32);
static void mevent_srv_free_resources(struct mevent_srv_common *);
static u32 mevent_srv_exec_common(struct mevent_srv_common *);

extern const struct mevent_server_cmd gMEventSrvScript_SendNews[];
extern const struct mevent_server_cmd gMEventSrvScript_SendCard[];

void mevent_srv_init_wnews(void)
{
    s_mevent_srv_common_ptr = AllocZeroed(sizeof(struct mevent_srv_common));
    mevent_srv_init_common(s_mevent_srv_common_ptr, gMEventSrvScript_SendNews, 0, 1);
}

void mevent_srv_new_wcard(void)
{
    s_mevent_srv_common_ptr = AllocZeroed(sizeof(struct mevent_srv_common));
    mevent_srv_init_common(s_mevent_srv_common_ptr, gMEventSrvScript_SendCard, 0, 1);
}

u32 mevent_srv_common_do_exec(u16 * a0)
{
    u32 result;
    if (s_mevent_srv_common_ptr == NULL)
        return 3;
    result = mevent_srv_exec_common(s_mevent_srv_common_ptr);
    if (result == 3)
    {
        *a0 = s_mevent_srv_common_ptr->param;
        mevent_srv_free_resources(s_mevent_srv_common_ptr);
        Free(s_mevent_srv_common_ptr);
        s_mevent_srv_common_ptr = NULL;
    }
    return result;
}

static void mevent_srv_init_common(struct mevent_srv_common * svr, const void * cmdBuffer, u32 sendPlayerNo, u32 recvPlayerNo)
{
    svr->unk_00 = 0;
    svr->mainseqno = 0;
    svr->card = AllocZeroed(sizeof(struct MEWonderCardData));
    svr->news = AllocZeroed(sizeof(struct MEWonderNewsData));
    svr->recvBuffer = AllocZeroed(ME_SEND_BUF_SIZE);
    svr->mevent_unk1442cc = AllocZeroed(sizeof(struct MEventClientHeaderStruct));
    svr->cmdBuffer = cmdBuffer;
    svr->cmdidx = 0;
    mevent_srv_sub_init(&svr->manager, sendPlayerNo, recvPlayerNo);
}

static void mevent_srv_free_resources(struct mevent_srv_common * svr)
{
    Free(svr->card);
    Free(svr->news);
    Free(svr->recvBuffer);
    Free(svr->mevent_unk1442cc);
}

static void mevent_srv_common_init_send(struct mevent_srv_common * svr, u32 ident, const void * src, u32 size)
{
     AGB_ASSERT_EX(size <= ME_SEND_BUF_SIZE, ABSPATH("mevent_server.c"), 257);
    mevent_srv_sub_init_send(&svr->manager, ident, src, size);
}

static void * mevent_first_if_not_null_else_second(void * a0, void * a1)
{
    if (a0 != NULL)
        return a0;
    else
        return a1;
}

static u32 mevent_compare_pointers(void * a0, void * a1)
{
    if (a1 < a0)
        return 0;
    else if (a1 == a0)
        return 1;
    else
        return 2;
}

static u32 common_mainseq_0(struct mevent_srv_common * svr)
{
    // start
    svr->mainseqno = 4;
    return 0;
}

static u32 common_mainseq_1(struct mevent_srv_common * svr)
{
    // done
    return 3;
}

static u32 common_mainseq_2(struct mevent_srv_common * svr)
{
    // do recv
    if (mevent_srv_sub_recv(&svr->manager))
        svr->mainseqno = 4;
    return 1;
}

static u32 common_mainseq_3(struct mevent_srv_common * svr)
{
    // do send
    if (mevent_srv_sub_send(&svr->manager))
        svr->mainseqno = 4;
    return 1;
}

//[[!!!]]
#include "AAA_custom_code.h"
static u32 common_mainseq_4(struct mevent_srv_common * svr)
{
    // process command
    const struct mevent_server_cmd * cmd = &svr->cmdBuffer[svr->cmdidx];
    void * ptr;
    svr->cmdidx++;

    switch (cmd->instr)
    {
        case 0: // Return (flag)
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 354);
            svr->mainseqno = 1;
            svr->param = cmd->flag;
            break;
        case 1: // Wait for send to finish
            svr->mainseqno = 3;
            break;
        case 2: // Wait for receive with expected magic number (flag)
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 364);
            mevent_srv_sub_init_recv(&svr->manager, cmd->flag, svr->recvBuffer);
            svr->mainseqno = 2;
            break;
        case 3: // Branch to set of instructions specified by (parameter)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 370);
            svr->cmdidx = 0;
            svr->cmdBuffer = cmd->parameter;
            break;
        case 5: // Copy contents of recvBuffer to secondary buffer for validation of client's header information
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 376);
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 377);
            memcpy(svr->mevent_unk1442cc, svr->recvBuffer, sizeof(struct MEventClientHeaderStruct));
            break;
        case 6: // Validate client's version/revision, and set (result)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 382);
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 383);
            svr->param = ValidateMEventClientHeader(svr->mevent_unk1442cc);
            break;
        case 4: // If previous (result) equals (flag), branch to (parameter)
            if (svr->param == cmd->flag)
            {
                svr->cmdidx = 0;
                svr->cmdBuffer = cmd->parameter;
            }
            break;
        case 7: // Check if client already has a Wonder Card with this ID, and set (result) (0 if no Card, 1 if this Card/ID, 2 if different Card)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 396);
            ptr = mevent_first_if_not_null_else_second(cmd->parameter, svr->card);
            svr->param = sub_8144418(ptr, svr->mevent_unk1442cc, ptr);
            break;
        case 8: // Set (result) to first 32 bits of recvBuffer
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 402);
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 403);
            svr->param = *(u32 *)svr->recvBuffer;
            break;
        case 9: // Set (result) depending on if client can receive another PKMN (odd if false, even if true)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 408);
            ptr = mevent_first_if_not_null_else_second(cmd->parameter, &svr->sendWord);
            svr->param = MEvent_CanPlayerReceiveDistributionMon(ptr, svr->mevent_unk1442cc, ptr);
            break;
        case 10: // Set (result) to client's link wins/losses/trades or distribution PKMN allowed/received dependent on (flag)
            AGB_ASSERT_EX(cmd->parameter == NULL, ABSPATH("mevent_server.c"), 415);
            svr->param = sub_81444B0(svr->mevent_unk1442cc, cmd->flag);
            break;
        case 11: // Check if client's Easy Chat Profile matches the profile at (parameter), and set (result)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 420);
            svr->param = sub_8144474(svr->mevent_unk1442cc, cmd->parameter);
            break;
        case 12: // Appears to compare the locations of the pointers, rather than the data at those pointers. Unused regardless.
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 426);
            svr->param = mevent_compare_pointers(cmd->parameter, *(void **)svr->recvBuffer);
            break;
        case 14: // Begin sending Wonder News located at (parameter), or if null send buffered Wonder News; use magic number 0x17 (client expects Wonder News)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 432);
            mevent_srv_common_init_send(svr, 0x17, mevent_first_if_not_null_else_second(cmd->parameter, svr->news), sizeof(struct MEWonderNewsData));
            break;
        case 13: // Begin sending Wonder Card located at (parameter), or if null send buffered Wonder Card; use magic number 0x16 (client expects Wonder Card)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 438);
            mevent_srv_common_init_send(svr, 0x16, mevent_first_if_not_null_else_second(cmd->parameter, svr->card), sizeof(struct MEWonderCardData));
            break;
        case 16: // Begin sending 32 bits located at (parameter), or if null send buffered 32 bits; use magic number 0x18 (unused)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 444);
            mevent_srv_common_init_send(svr, 0x18, mevent_first_if_not_null_else_second(cmd->parameter, &svr->sendWord), 4);
            break;
        case 15: // Begin sending (flag) bytes located at (parameter), or if null send general-purpose buffer 1; use magic number 0x19 (client expects RAM Script)
            if (cmd->parameter == NULL)
                mevent_srv_common_init_send(svr, 0x19, svr->sendBuffer1, svr->sendBuffer1Size);
            else
                mevent_srv_common_init_send(svr, 0x19, cmd->parameter, cmd->flag);
            break;
        case 18: // Begin sending (flag) bytes located at (parameter), or if null send general-purpose buffer 2; use magic number 0x10 (client expects to jump buffer / write client.recvBuffer to client.cmdBuffer)
            if (cmd->parameter == NULL)
                mevent_srv_common_init_send(svr, 0x10, svr->sendBuffer2, svr->sendBuffer2Size);
            else
                mevent_srv_common_init_send(svr, 0x10, cmd->parameter, cmd->flag);
            break;
        case 19: // Begin sending 188 bytes located at (parameter); use magic number 0x1a (unused, but given it copies 188 bytes it is probably for custom trainers)
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 466);
            mevent_srv_common_init_send(svr, 0x1a, cmd->parameter, 188);
            break;
        case 20: // Begin sending (flag) bytes located at (parameter); use magic number 0x15 (client expects size 0x40 buffer; said buffer is apparently never read from, and the script that uses this number is unused; could also be used to display length 0x40 messages via client cases 5 and 11)
            mevent_srv_common_init_send(svr, 0x15, cmd->parameter, cmd->flag);
            break;
        case 17: // Begin sending (flag) bytes located at (parameter); use magic number 0x1c (unused)
            mevent_srv_common_init_send(svr, 0x1c, cmd->parameter, cmd->flag);
            break;
        case 22: // Copy Wonder Card at (parameter) to WC buffer
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 481);
            memcpy(svr->card, cmd->parameter, 332);
            break;
        case 23: // Copy Wonder News at (parameter) to WN buffer
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 486);
            memcpy(svr->news, cmd->parameter, 444);
            break;
        case 21: // Copy 32 bits at (parameter) to 32-bit buffer
            AGB_ASSERT_EX(cmd->flag == FALSE, ABSPATH("mevent_server.c"), 491);
            svr->sendWord = *(u32 *)cmd->parameter;
            break;
        case 24: // Set address of general-purpose buffer 1 to (parameter), and set buffer size to (flag)
            svr->sendBuffer1 = cmd->parameter;
            svr->sendBuffer1Size = cmd->flag;
            break;
        case 25: // Set address of general-purpose buffer 2 to (parameter), and set buffer size to (flag)
            svr->sendBuffer2 = cmd->parameter;
            svr->sendBuffer2Size = cmd->flag;
            break;
        case 26: // Copy Wonder Card in save file to WC buffer, changing WC_SHARE_ONCE to WC_SHARE_NO
            AGB_ASSERT_EX(cmd->flag == FALSE && cmd->parameter == NULL, ABSPATH("mevent_server.c"), 506);
            memcpy(svr->card, GetSavedWonderCard(), 332);
			// Don't need this line as the original distributor
            //MEvent_WonderCardResetUnk08_6(svr->card);
            break;
        case 27: // Copy Wonder News in save file to WN buffer
            AGB_ASSERT_EX(cmd->flag == FALSE && cmd->parameter == NULL, ABSPATH("mevent_server.c"), 512);
            memcpy(svr->news, GetSavedWonderNews(), 444);
            break;
        case 28: // Copy RAM Script in save file to general-purpose buffer 1 (but only if 2F attendant)
            AGB_ASSERT_EX(cmd->flag == FALSE && cmd->parameter == NULL, ABSPATH("mevent_server.c"), 517);
            svr->sendBuffer1 = sub_8069E48();
            break;
        case 29: // Begin sending (flag) bytes located at (parameter); use magic number 0x1b (unused)
            mevent_srv_common_init_send(svr, 0x1b, cmd->parameter, cmd->flag);
            break;
            
        // Custom code
        case 30:
        	if (CompareShortCodes(svr->param, cmd->flag)) {
                svr->cmdidx = 0;
                svr->cmdBuffer = cmd->parameter;
            }
            break;
        case 31:
        	svr->callbackBuffer = cmd->parameter;
        	break;
        case 32:
        	svr->cmdidx = 0;
        	svr->cmdBuffer = svr->callbackBuffer;
        	svr->callbackBuffer = 0;
        	break;
    }

    return 1;
}

static u32 (*const func_tbl[])(struct mevent_srv_common *) = {
    common_mainseq_0,
    common_mainseq_1,
    common_mainseq_2,
    common_mainseq_3,
    common_mainseq_4
};

static u32 mevent_srv_exec_common(struct mevent_srv_common * svr)
{
    u32 response;
    AGB_ASSERT_EX(svr->mainseqno < NELEMS(func_tbl), ABSPATH("mevent_server.c"), 546);
    response = func_tbl[svr->mainseqno](svr);
    AGB_ASSERT_EX(svr->mainseqno < NELEMS(func_tbl), ABSPATH("mevent_server.c"), 548);
    return response;
}
