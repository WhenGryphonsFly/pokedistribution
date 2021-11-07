#include "global.h"
#include "gflib.h"
#include "constants/songs.h"
#include "easy_chat.h"
#include "task.h"
#include "decompress.h"
#include "link.h"
#include "link_rfu.h"
#include "ereader_helpers.h"
#include "util.h"
#include "script.h"
#include "event_data.h"
#include "battle_tower.h"
#include "new_game.h"
#include "menews_jisan.h"
#include "cereader_tool.h"
#include "mystery_gift_menu.h"
#include "help_system.h"
#include "mevent.h"
#include "strings.h"

struct MEventTaskData1
{
    u16 stateAdvanceDelay;
    u16 t02;
    u16 t04;
    u16 t06;
    u8 state;
    u8 textOrReceiveState;
    u8 t0A;
    u8 t0B;
    u8 t0C;
    u8 t0D;
    u8 initialSendResult;
    struct MEvent_Str_2 *t10;
};

static void Task_EReaderComm(u8 taskId);
static bool32 IsReceivedWonderNewsHeaderValid(const struct MEWonderNewsData * src);
static void BlankWonderNews(void);
static void BlankMENewsJisan(void);
static bool32 IsReceivedWonderCardHeaderValid(const struct MEWonderCardData * src);
static void BlankSavedWonderCard(void);
static void BlankMEventBuffer2(void);
static void RecordIdOfWonderCardSender(u32 eventId, u32 trainerId, u32 *idsList, s32 count);
static void BlankBuffer344(void);

extern const u8 gUnknownSerialData_Start[];
extern const u8 gUnknownSerialData_End[];

static const u16 sGiftItemFlagIds[] = {
    FLAG_GOT_AURORA_TICKET,
    FLAG_GOT_MYSTIC_TICKET,
    FLAG_0x2A9,
    FLAG_0x2AA,
    FLAG_0x2AB,
    FLAG_0x2AC,
    FLAG_0x2AD,
    FLAG_0x2AE,
    FLAG_0x2AF,
    FLAG_0x2B0,
    FLAG_0x2B1,
    FLAG_0x2B2,
    FLAG_0x2B3,
    FLAG_0x2B4,
    FLAG_0x2B5,
    FLAG_0x2B6,
    FLAG_0x2B7,
    FLAG_0x2B8,
    FLAG_0x2B9,
    FLAG_0x2BA
};

struct MEvent_Str_1 sMEventSendToEReaderManager;

static EWRAM_DATA bool32 sReceivedWonderCardIsValid = FALSE;

static void ResetTTDataBuffer(void)
{
    memset(gDecompressionBuffer, 0, 0x2000);
    gLinkType = 0x5502;
    OpenLink();
    SetSuppressLinkErrorMessage(TRUE);
}

bool32 sub_81436EC(void)
{
    vu16 imeBak = REG_IME;
    u16 data[4];
    REG_IME = 0;
    *(u64 *)data = *(u64 *)gLink.tempRecvBuffer;
    REG_IME = imeBak;
    if (   data[0] == 0xB9A0
        && data[1] == 0xCCD0
        && data[2] == 0xFFFF
        && data[3] == 0xFFFF
    )
        return TRUE;
    return FALSE;
}

static bool32 IsEReaderConnectionSane(void)
{
    if (IsLinkMaster() && GetLinkPlayerCount_2() == 2)
        return TRUE;
    return FALSE;
}

static u32 EReaderReceive(u8 * state_p, u16 * receiveDelay)
{
    if ((*state_p == 3 || *state_p == 4 || *state_p == 5) && HasLinkErrorOccurred())
    {
        *state_p = 0;
        return 3;
    }
    switch (*state_p)
    {
        case 0:
            if (IsLinkMaster() && GetLinkPlayerCount_2() > 1)
            {
                *state_p = 1;
                ;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                *state_p = 0;
                return 1;
            }
            break;
        case 1:
            if (++(*receiveDelay) > 5)
            {
                *receiveDelay = 0;
                *state_p = 2;
            }
            break;
        case 2:
            if (GetLinkPlayerCount_2() == 2)
            {
                PlaySE(SE_DING_DONG);
                CheckShouldAdvanceLinkState();
                *receiveDelay = 0;
                *state_p = 3;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                *state_p = 0;
                return 1;
            }
            break;
        case 3:
            if (++(*receiveDelay) > 30)
            {
                *state_p = 0;
                return 5;
            }
            else if (IsLinkConnectionEstablished())
            {
                if (gReceivedRemoteLinkPlayers)
                {
                    if (IsLinkPlayerDataExchangeComplete())
                    {
                        *state_p = 0;
                        return 2;
                    }
                    else
                        *state_p = 4;
                }
                else
                    *state_p = 3;
            }
            break;
        case 4:
            Link_StartSend5FFFwithParam(0);
            *state_p = 5;
            break;
        case 5:
            if (!gReceivedRemoteLinkPlayers)
            {
                *state_p = 0;
                return 4;
            }
            break;
    }
    return 0;
}

static void ResetDelayTimer(u16 *a0)
{
    *a0 = 0;
}

static bool32 AdvanceDelayTimerCheckTimeout(u16 * a0, u16 a1)
{
    if (++(*a0) > a1)
    {
        *a0 = 0;
        return TRUE;
    }
    return FALSE;
}

void InitMEventData(void)
{
    CpuFill32(0, &gSaveBlock1Ptr->mysteryEventBuffers, sizeof(gSaveBlock1Ptr->mysteryEventBuffers));
    BlankMENewsJisan();
}

struct MEWonderNewsData * GetSavedWonderNews(void)
{
    return &gSaveBlock1Ptr->mysteryEventBuffers.menews.data;
}

struct MEWonderCardData * GetSavedWonderCard(void)
{
    return &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
}

struct MEventBuffer_3430_Sub * sav1_get_mevent_buffer_2(void)
{
    return &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data;
}

struct MENewsJisanStruct * GetMENewsJisanStructPtr(void)
{
    return &gSaveBlock1Ptr->mysteryEventBuffers.me_jisan;
}

u16 * GetMEventProfileECWordsMaybe(void)
{
    return gSaveBlock1Ptr->mysteryEventBuffers.ec_profile_maybe;
}

void DestroyWonderNews(void)
{
    BlankWonderNews();
}

bool32 OverwriteSavedWonderNewsWithReceivedNews(const struct MEWonderNewsData * src)
{
    if (!IsReceivedWonderNewsHeaderValid(src))
        return FALSE;
    BlankWonderNews();
    gSaveBlock1Ptr->mysteryEventBuffers.menews.data = *src;
    gSaveBlock1Ptr->mysteryEventBuffers.menews.crc = CalcCRC16WithTable((void *)&gSaveBlock1Ptr->mysteryEventBuffers.menews.data, sizeof(struct MEWonderNewsData));
    return TRUE;
}

bool32 ValidateReceivedWonderNews(void)
{
    if (CalcCRC16WithTable((void *)&gSaveBlock1Ptr->mysteryEventBuffers.menews.data, sizeof(struct MEWonderNewsData)) != gSaveBlock1Ptr->mysteryEventBuffers.menews.crc)
        return FALSE;
    if (!IsReceivedWonderNewsHeaderValid(&gSaveBlock1Ptr->mysteryEventBuffers.menews.data))
        return FALSE;
    return TRUE;
}

static bool32 IsReceivedWonderNewsHeaderValid(const struct MEWonderNewsData * data)
{
    if (data->newsId == 0)
        return FALSE;
    return TRUE;
}

bool32 WonderNews_Test_Unk_02(void)
{
    const struct MEWonderNewsData * data = &gSaveBlock1Ptr->mysteryEventBuffers.menews.data;
    if (data->shareState == 0)
        return FALSE;
    return TRUE;
}

static void BlankWonderNews(void)
{
    CpuFill32(0, GetSavedWonderNews(), sizeof(gSaveBlock1Ptr->mysteryEventBuffers.menews.data));
    gSaveBlock1Ptr->mysteryEventBuffers.menews.crc = 0;
}

static void BlankMENewsJisan(void)
{
    CpuFill32(0, GetMENewsJisanStructPtr(), sizeof(struct MENewsJisanStruct));
    MENewsJisanReset();
}

bool32 MEvent_HaveAlreadyReceivedWonderNews(const u8 * src)
{
    const u8 * r5 = (const u8 *)&gSaveBlock1Ptr->mysteryEventBuffers.menews.data;
    u32 i;
    if (!ValidateReceivedWonderNews())
        return FALSE;
    for (i = 0; i < sizeof(struct MEWonderNewsData); i++)
    {
        if (r5[i] != src[i])
            return FALSE;
    }
    return TRUE;
}

void DestroyWonderCard(void)
{
    BlankSavedWonderCard();
    BlankMEventBuffer2();
    BlankBuffer344();
    ClearRamScript();
    ResetMysteryEventFlags();
    ResetMysteryEventVars();
    ClearEReaderTrainer(&gSaveBlock2Ptr->battleTower.ereaderTrainer);
}

bool32 OverwriteSavedWonderCardWithReceivedCard(const struct MEWonderCardData * data)
{
    struct MEventBuffer_3430_Sub * r2;
    struct MEWonderCardData * r1;
    if (!IsReceivedWonderCardHeaderValid(data))
        return FALSE;
    DestroyWonderCard();
    memcpy(&gSaveBlock1Ptr->mysteryEventBuffers.mecard.data, data, sizeof(struct MEWonderCardData));
    gSaveBlock1Ptr->mysteryEventBuffers.mecard.crc = CalcCRC16WithTable((void *)&gSaveBlock1Ptr->mysteryEventBuffers.mecard.data, sizeof(struct MEWonderCardData));
    // Annoying hack to match
    r2 = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data;
    r1 = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
    r2->icon = r1->icon;
    return TRUE;
}

bool32 ValidateReceivedWonderCard(void)
{
    if (gSaveBlock1Ptr->mysteryEventBuffers.mecard.crc != CalcCRC16WithTable((void *)&gSaveBlock1Ptr->mysteryEventBuffers.mecard.data, sizeof(struct MEWonderCardData)))
        return FALSE;
    if (!IsReceivedWonderCardHeaderValid(&gSaveBlock1Ptr->mysteryEventBuffers.mecard.data))
        return FALSE;
    if (!ValidateRamScript())
        return FALSE;
    return TRUE;
}

static bool32 IsReceivedWonderCardHeaderValid(const struct MEWonderCardData * data)
{
    if (data->cardId == 0)
        return FALSE;
    if (data->type > 2)
        return FALSE;
    if (!(data->shareState == 0 || data->shareState == 1 || data->shareState == 2))
        return FALSE;
    if (data->color > 7)
        return FALSE;
    if (data->recvMonCapacity > 7)
        return FALSE;
    return TRUE;
}

bool32 WonderCard_Test_Unk_08_6(void)
{
    const struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
    if (data->shareState == 0)
        return FALSE;
    return TRUE;
}

static void BlankSavedWonderCard(void)
{
    CpuFill32(0, &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data, sizeof(struct MEWonderCardData));
    gSaveBlock1Ptr->mysteryEventBuffers.mecard.crc = 0;
}

static void BlankMEventBuffer2(void)
{
    CpuFill32(0, sav1_get_mevent_buffer_2(), 18 * sizeof(u16));
    gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.crc = 0;
}

u16 GetWonderCardFlagId(void)
{
    if (ValidateReceivedWonderCard())
        return gSaveBlock1Ptr->mysteryEventBuffers.mecard.data.cardId;
    return 0;
}

void MEvent_WonderCardResetUnk08_6(struct MEWonderCardData * buffer)
{
    if (buffer->shareState == 1)
        buffer->shareState = 0;
}

static bool32 IsCardIdInValidRange(u16 a0)
{
    if (a0 >= 1000 && a0 < 1020)
        return TRUE;
    return FALSE;
}

bool32 CheckReceivedGiftFromWonderCard(void)
{
    u16 value = GetWonderCardFlagId();
    if (!IsCardIdInValidRange(value))
        return FALSE;
    if (FlagGet(sGiftItemFlagIds[value - 1000]) == TRUE)
        return FALSE;
    return TRUE;
}

static s32 CountReceivedDistributionMons(const struct MEventBuffer_3430_Sub * data, s32 size)
{
    s32 r3 = 0;
    s32 i;
    for (i = 0; i < size; i++)
    {
        if (data->distributedMons[1][i] && data->distributedMons[0][i])
            r3++;
    }
    return r3;
}

static bool32 HasPlayerAlreadyReceivedDistributedMon(const struct MEventBuffer_3430_Sub * data1, const u16 * data2, s32 size)
{
    s32 i;
    for (i = 0; i < size; i++)
    {
        if (data1->distributedMons[1][i] == data2[1])
            return TRUE;
        if (data1->distributedMons[0][i] == data2[0])
            return TRUE;
    }
    return FALSE;
}

static bool32 IsWonderCardSpeciesValid(const u16 * data)
{
    if (data[1] == 0)
        return FALSE;
    if (data[0] == SPECIES_NONE)
        return FALSE;
    if (data[0] >= NUM_SPECIES)
        return FALSE;
    return TRUE;
}

static s32 ValidateCardAndCountMonsReceived(void)
{
    struct MEWonderCardData * data;
    if (!ValidateReceivedWonderCard())
        return 0;
    data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
    if (data->type != 1)
        return 0;
    return CountReceivedDistributionMons(&gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data, data->recvMonCapacity);
}

bool32 MEvent_ReceiveDistributionMon(const u16 * data)
{
    struct MEWonderCardData * buffer = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
    s32 capacity = buffer->recvMonCapacity;
    s32 i;
    if (!IsWonderCardSpeciesValid(data))
        return FALSE;
    if (HasPlayerAlreadyReceivedDistributedMon(&gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data, data, capacity))
        return FALSE;
    for (i = 0; i < capacity; i++)
    {
        if (gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.distributedMons[1][i] == 0 && gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.distributedMons[0][i] == 0)
        {
            gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.distributedMons[1][i] = data[1];
            gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.distributedMons[0][i] = data[0];
            return TRUE;
        }
    }
    return FALSE;
}

#if defined(FIRERED)
#define MEVENT_HEADER_VERSION_CODE 1
#elif defined(LEAFGREEN)
#define MEVENT_HEADER_VERSION_CODE 2
#endif

void BuildMEventClientHeader(struct MEventClientHeaderStruct * data)
{
    s32 i;
    CpuFill32(0, data, sizeof(struct MEventClientHeaderStruct));
    // Magic
    data->unk_00 = 0x101;
    data->unk_04 = 1;
    data->unk_08 = 1;
    data->unk_0C = 1;
    data->unk_10 = MEVENT_HEADER_VERSION_CODE;

    // Check whether a card already exists
    if (ValidateReceivedWonderCard())
    {
        // Populate fields
        data->id = GetSavedWonderCard()->cardId;
        data->unk_20 = *sav1_get_mevent_buffer_2();
        data->maxDistributionMons = GetSavedWonderCard()->recvMonCapacity;
    }
    else
        data->id = 0;

    // Get something
    for (i = 0; i < 4; i++)
        data->unk_16[i] = gSaveBlock1Ptr->mysteryEventBuffers.ec_profile_maybe[i];

    // Get player ID
    CopyTrainerId(data->playerTrainerId, gSaveBlock2Ptr->playerTrainerId);
    StringCopy(data->playerName, gSaveBlock2Ptr->playerName);
    for (i = 0; i < 6; i++)
        data->easyChatProfile[i] = gSaveBlock1Ptr->easyChatProfile[i];
    memcpy(data->gameCode, RomHeaderGameCode, 4);
    data->version = RomHeaderSoftwareVersion;
}

// [[!!!]]
u32 ValidateMEventClientHeader(const struct MEventClientHeaderStruct * data)
{
    if (data->unk_00 != 0x101)
        return FALSE;
    if (!(data->unk_04 & 1))
        return FALSE;
    if (!(data->unk_08 & 1))
        return FALSE;
    if (!(data->unk_0C & 1))
        return FALSE;
    if (!(data->unk_10 & 0x0F))
        return FALSE;
    return TRUE;
}

u32 sub_8144418(const u16 * a0, const struct MEventClientHeaderStruct * a1, void * unused)
{
    if (a1->id == 0)
        return 0;
    if (*a0 == a1->id)
        return 1;
    return 2;
}

u32 MEvent_CanPlayerReceiveDistributionMon(const u16 * a0, const struct MEventClientHeaderStruct * a1, void * unused)
{
    s32 numSpaces = a1->maxDistributionMons - CountReceivedDistributionMons(&a1->unk_20, a1->maxDistributionMons);
    if (numSpaces == 0)
        return 1;
    if (HasPlayerAlreadyReceivedDistributedMon(&a1->unk_20, a0, a1->maxDistributionMons))
        return 3;
    if (numSpaces == 1)
        return 4;
    return 2;
}

bool32 sub_8144474(const struct MEventClientHeaderStruct * a0, const u16 * a1)
{
    s32 i;
    for (i = 0; i < 4; i++)
    {
        if (a0->unk_16[i] != a1[i])
            return FALSE;
    }
    return TRUE;
}

static s32 GetNumReceivedDistributionMons(const struct MEventClientHeaderStruct * a0)
{
    return CountReceivedDistributionMons(&a0->unk_20, a0->maxDistributionMons);
}

u16 sub_81444B0(const struct MEventClientHeaderStruct * a0, u32 command)
{
    switch (command)
    {
        case 0:
            return a0->unk_20.linkWins;
        case 1:
            return a0->unk_20.linkLosses;
        case 2:
            return a0->unk_20.linkTrades;
        case 3:
            return GetNumReceivedDistributionMons(a0);
        case 4:
            return a0->maxDistributionMons;
        default:
             AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 825);
            return 0;
    }
}

// Increments an interaction count in the save block
static void IncrementBattleCardCount(u32 command)
{
    struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
    if (data->type == 2)
    {
        u16 * dest = NULL;
        switch (command)
        {
            case 0:
                dest = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.linkWins;
                break;
            case 1:
                dest = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.linkLosses;
                break;
            case 2:
                dest = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data.linkTrades;
                break;
            case 3:
                break;
            case 4:
                break;
        }
        if (dest == NULL)
        {
             AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 868);
        }
        else if (++(*dest) > 999)
        {
            *dest = 999;
        }
    }
}

u16 MEvent_GetBattleCardCount(u32 command)
{
    switch (command)
    {
        case 0:
        {
            struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
            if (data->type == 2)
            {
                struct MEventBuffer_3430_Sub * buffer = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data;
                return buffer->linkWins;
            }
            break;
        }
        case 1:
        {
            struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
            if (data->type == 2)
            {
                struct MEventBuffer_3430_Sub * buffer = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data;
                return buffer->linkLosses;
            }
            break;
        }
        case 2:
        {
            struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
            if (data->type == 2)
            {
                struct MEventBuffer_3430_Sub * buffer = &gSaveBlock1Ptr->mysteryEventBuffers.buffer_310.data;
                return buffer->linkTrades;
            }
            break;
        }
        case 3:
        {
            struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
            if (data->type == 1)
                return ValidateCardAndCountMonsReceived();
            break;
        }
        case 4:
        {
            struct MEWonderCardData * data = &gSaveBlock1Ptr->mysteryEventBuffers.mecard.data;
            if (data->type == 1)
                return data->recvMonCapacity;
            break;
        }
    }
    AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 913);
    return 0;
}

void ResetReceivedWonderCardFlag(void)
{
    sReceivedWonderCardIsValid = FALSE;
}

bool32 MEventHandleReceivedWonderCard(u16 cardId)
{
    sReceivedWonderCardIsValid = FALSE;
    if (cardId == 0)
        return FALSE;
    if (!ValidateReceivedWonderCard())
        return FALSE;
    if (gSaveBlock1Ptr->mysteryEventBuffers.mecard.data.cardId != cardId)
        return FALSE;
    sReceivedWonderCardIsValid = TRUE;
    return TRUE;
}

void MEvent_RecordIdOfWonderCardSenderByEventType(u32 eventId, u32 trainerId)
{
    if (sReceivedWonderCardIsValid)
    {
        switch (eventId)
        {
            case 2: // trade
                RecordIdOfWonderCardSender(2, trainerId, gSaveBlock1Ptr->mysteryEventBuffers.unk_344[1], 5);
                break;
            case 0: // link win
                RecordIdOfWonderCardSender(0, trainerId, gSaveBlock1Ptr->mysteryEventBuffers.unk_344[0], 5);
                break;
            case 1: // link loss
                RecordIdOfWonderCardSender(1, trainerId, gSaveBlock1Ptr->mysteryEventBuffers.unk_344[0], 5);
                break;
            default:
                 AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 988);
        }
    }
}

static void BlankBuffer344(void)
{
    CpuFill32(0, gSaveBlock1Ptr->mysteryEventBuffers.unk_344, sizeof(gSaveBlock1Ptr->mysteryEventBuffers.unk_344));
}

// Looks up trainerId in an array idsList with count elements.
// If trainerId is found, rearranges idsList to put it in the front.
// Otherwise, drops the last element of the list and inserts
// trainerId at the front.
// Returns TRUE in the latter case.
static bool32 PlaceTrainerIdAtFrontOfList(u32 trainerId, u32 * idsList, s32 count)
{
    s32 i;
    s32 j;

    for (i = 0; i < count; i++)
    {
        if (idsList[i] == trainerId)
            break;
    }
    if (i == count)
    {
        for (j = count - 1; j > 0; j--)
        {
            idsList[j] = idsList[j - 1];
        }
        idsList[0] = trainerId;
        return TRUE;
    }
    else
    {
        for (j = i; j > 0; j--)
        {
            idsList[j] = idsList[j - 1];
        }
        idsList[0] = trainerId;
        return FALSE;
    }
}

static void RecordIdOfWonderCardSender(u32 eventId, u32 trainerId, u32 * idsList, s32 count)
{
    if (PlaceTrainerIdAtFrontOfList(trainerId, idsList, count))
        IncrementBattleCardCount(eventId);
}
