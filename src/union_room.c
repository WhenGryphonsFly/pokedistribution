//[[!!!]]
#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "berry_crush.h"
#include "cable_club.h"
#include "data.h"
#include "decompress.h"
#include "dodrio_berry_picking.h"
#include "dynamic_placeholder_text_util.h"
#include "easy_chat.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "field_control_avatar.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "link.h"
#include "link_rfu.h"
#include "list_menu.h"
#include "load_save.h"
#include "menu.h"
#include "mevent.h"
#include "mystery_gift_menu.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon_jump.h"
#include "quest_log.h"
#include "random.h"
#include "save_location.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "start_menu.h"
#include "strings.h"
#include "task.h"
#include "trade.h"
#include "trade_scene.h"
#include "trainer_card.h"
#include "union_room.h"
#include "union_room_battle.h"
#include "union_room_chat.h"
#include "rfu_union_tool.h"
#include "union_room_message.h"
#include "constants/songs.h"
#include "constants/maps.h"
#include "constants/cable_club.h"
#include "constants/field_weather.h"
#include "constants/trainer_card.h"
#include "constants/union_room.h"

static EWRAM_DATA u8 sUnionRoomPlayerName[12] = {};
static EWRAM_DATA u8 sPlayerCurrActivity = 0;
static EWRAM_DATA u8 sPlayerActivityGroupSize = 0;
static EWRAM_DATA union UnkUnion_Main sUnionRoomMain = {};
static EWRAM_DATA u32 sUnref_203B060 = 0;
EWRAM_DATA struct GFtgtGnameSub gPartnerTgtGnameSub = {};
EWRAM_DATA u16 gUnionRoomOfferedSpecies = SPECIES_NONE;
EWRAM_DATA u8 gUnionRoomRequestedMonType = TYPE_NORMAL;
static EWRAM_DATA struct UnionRoomTrade sUnionRoomTrade = {};

static struct UnkStruct_Leader * sLeader;
static struct UnkStruct_Group * sGroup;
static struct UnkStruct_URoom * sURoom;

static void Task_TryBecomeLinkLeader(u8 taskId);
static void Leader_DestroyResources(struct UnkStruct_Leader * leader);
static bool8 Leader_SetStateIfMemberListChanged(struct UnkStruct_Leader * leader, u32 state1, u32 state2);
static void ItemPrintFunc_PossibleGroupMembers(u8 windowId, u32 itemId, u8 y);
static u8 LeaderUpdateGroupMembership(struct UnkStruct_Main0 * main0);
static u8 UnionRoomLeaderField0CompactionAndCount(struct UnkStruct_Main0 * main0);
static void Task_TryJoinLinkGroup(u8 taskId);
static u32 IsTryingToTradeWithHoennTooSoon(struct UnkStruct_Group * group, s32 id);
static void AskToJoinRfuGroup(struct UnkStruct_Group * group, s32 id);
static void Task_ListenToWireless(u8 taskId);
static void ListMenuItemPrintFunc_UnionRoomGroups(u8 windowId, u32 itemId, u8 y);
static u8 GetNewLeaderCandidate(void);
static void CreateTask_RunScriptAndFadeToActivity(void);
static void CreateTask_StartActivity(void);
static void Task_MEvent_Leader(u8 taskId);
static void Task_CardOrNewsWithFriend(u8 taskId);
static void Task_CardOrNewsOverWireless(u8 taskId);
static void Task_RunUnionRoom(u8 taskId);
static u16 ReadAsU16(const u8 *data);
static void ReceiveUnionRoomActivityPacket(struct UnkStruct_URoom * uRoom);
static bool32 UnionRoom_HandleContactFromOtherPlayer(struct UnkStruct_URoom * uRoom);
static void Task_InitUnionRoom(u8 taskId);
static u8 HandlePlayerListUpdate(void);
static u8 CreateTask_SearchForChildOrParent(struct UnkStruct_Main4 * main4, struct UnkStruct_Main4 * arg1, u32 arg2);
static bool32 GetGnameWonderFlagByLinkGroup(struct GFtgtGname * gname, s16 linkGroup);
static u8 CreateTask_ListenForPartnersWithCompatibleSerialNos(struct UnkStruct_Main4 * main4, u32 linkGroup);
static u8 CreateTask_ListenForPartnersWithSerial7F7D(struct UnkStruct_Main4 * main4, u32 linkGroup);
static bool32 UR_PrintFieldMessage(const u8 * str);
static bool32 UR_RunTextPrinters_CheckPrinter0Active(void);
static bool8 PrintOnTextbox(u8 *textState, const u8 *str);
static s8 UnionRoomHandleYesNo(u8 *state_p, bool32 no_draw);
static s32 ListMenuHandler_AllItemsAvailable(u8 *state_p, u8 *win_id_p, u8 *list_menu_id_p, const struct WindowTemplate * winTemplate, const struct ListMenuTemplate * menuTemplate);
static s32 TradeBoardMenuHandler(u8 *state_p, u8 *win_id_p, u8 *list_menu_id_p, u8 *trade_board_win_id_p, const struct WindowTemplate * winTemplate, const struct ListMenuTemplate * menuTemplate, struct UnkStruct_Main0 * traders);
static void UR_BlankBg0(void);
static void JoinGroup_BlankBg0AndEnableScriptContexts(void);
static void UR_AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 colorIdx);
static void BlankUnkStruct_x20Array(struct UnkStruct_x20 * x20, u8 count);
static void BlankUnkStruct_x1CArray(struct UnkStruct_x1C * x1C, u8 count);
static bool8 AreGnameUnameDifferent(struct UnionGnameUnamePair * arg0, const struct UnionGnameUnamePair * arg1);
static bool32 AreUnionRoomPlayerGnamesDifferent(struct UnionGnameUnamePair * arg0, struct UnionGnameUnamePair * arg1);
static u32 Findx20Inx1CArray(struct UnkStruct_x20 * x20, struct UnkStruct_x1C * x1Carr);
static u8 Appendx1Ctox20(struct UnkStruct_x20 * x20arr, struct UnkStruct_x1C * x1C, u8 count);
static void PrintUnionRoomGroupOnWindow(u8 windowId, u8 x, u8 y, struct UnkStruct_x20 * group, u8 colorIdx, u8 id);
static void PrintGroupMemberCandidateOnWindowWithColor(u8 windowId, u8 x, u8 y, struct UnkStruct_x20 * group, u8 colorIdx, u8 id);
static bool32 PlayerIsTalkingToUnionRoomAide(void);
static u32 GetResponseIdx_InviteToURoomActivity(s32 activity);
static u32 ConvPartnerUnameAndGetWhetherMetAlready(struct UnkStruct_x20 * unkX20);
static s32 UnionRoomGetPlayerInteractionResponse(struct UnkStruct_Main0 * main0, u8 overrideGender, u8 playerIdx, u32 playerGender);
static void nullsub_92(u8 windowId, s32 itemId, u8 y);
static void TradeBoardListMenuItemPrintFunc(u8 windowId, s32 itemId, u8 y);
static s32 GetIndexOfNthTradeBoardOffer(struct UnkStruct_x20 * x20, s32 n);
static s32 GetUnionRoomPlayerGender(s32 playerIdx, struct UnkStruct_Main0 * main0);
static s32 IsRequestedTypeAndSpeciesInPlayerParty(u32 type, u32 species);
static void GetURoomActivityRejectMsg(u8 *dst, s32 activity, u32 playerGender);
static void GetURoomActivityStartMsg(u8 *dst, u8 activity);
static s32 GetChatLeaderActionRequestMessage(u8 *dst, u32 gender, u16 *activity_p, struct UnkStruct_URoom * uroom);
static bool32 PollPartnerYesNoResponse(struct UnkStruct_URoom * uroom);
static bool32 HasAtLeastTwoMonsOfLevel30OrLower(void);
static void ResetUnionRoomTrade(struct UnionRoomTrade * trade);
static bool32 RegisterTradeMonAndGetIsEgg(u32 monId, struct UnionRoomTrade * trade);
static void RegisterTradeMon(u32 monId, struct UnionRoomTrade * trade);
static u32 GetPartyPositionOfRegisteredMon(struct UnionRoomTrade * trade, u8 mpId);
static void HandleCancelTrade(bool32 unlockObjs);
static void UR_EnableScriptContext2AndFreezeObjectEvents(void);
static u8 GetSinglePartnerSpriteGenderParam(s32 linkPlayer);
static u8 GetActivePartnerSpriteGenderParam(struct UnkStruct_URoom * uroom);
static void ViewURoomPartnerTrainerCard(u8 *dest, struct UnkStruct_URoom * uRoom, bool8 parent_child);

#define _8456CD8(a, b) ((a) | ((b) << 8))

static const u8 *const sUnionRoomActivityStringPtrs[] = {
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_SingleBattle,
    gUnionRoomActivity_DoubleBattle,
    gUnionRoomActivity_MultiBattle,
    gUnionRoomActivity_PokemonTrades,
    gUnionRoomActivity_Chat,
    gUnionRoomActivity_WonderCards,
    gunionRoomActivity_WonderNews,
    gUnionRoomActivity_Cards,
    gUnionRoomActivity_PokemonJump,
    gUnionRoomActivity_BerryCrush,
    gUnionRoomActivity_BerryPicking,
    gUnionRoomActivity_Search,
    gUnionRoomActivity_SpinTradeJP,
    gUnionRoomActivity_ItemTradeJP,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_Blank,
    gUnionRoomActivity_WonderCards,
    gunionRoomActivity_WonderNews
};

static const struct WindowTemplate sWindowTemplate_BButtonCancel = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 30,
    .height = 2,
    .paletteNum = 0xF,
    .baseBlock = 0x008
};

static const u32 sLinkGroupToActivityAndCapacity[] = {
    [LINK_GROUP_SINGLE_BATTLE] = _8456CD8(ACTIVITY_BATTLE,    2),
    [LINK_GROUP_DOUBLE_BATTLE] = _8456CD8(ACTIVITY_DBLBATTLE, 2),
    [LINK_GROUP_MULTI_BATTLE]  = _8456CD8(ACTIVITY_MLTBATTLE, 4),
    [LINK_GROUP_TRADE]         = _8456CD8(ACTIVITY_TRADE,     2),
    [LINK_GROUP_POKEMON_JUMP]  = _8456CD8(ACTIVITY_PJUMP,     5 | 0x20),
    [LINK_GROUP_BERRY_CRUSH]   = _8456CD8(ACTIVITY_BCRUSH,    5 | 0x20),
    [LINK_GROUP_BERRY_PICKING] = _8456CD8(ACTIVITY_BPICK,     5 | 0x30),
    [LINK_GROUP_WONDER_CARD]   = _8456CD8(ACTIVITY_SPINTRADE, 5 | 0x30),
    [LINK_GROUP_WONDER_NEWS]   = _8456CD8(ACTIVITY_ITEMTRADE, 5 | 0x30)
};

static const struct WindowTemplate sWindowTemplate_List_PossibleGroupMembers = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 13,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 0x044
};

static const struct WindowTemplate sWindowTemplate_NumPlayerMode = {
    .bg = 0,
    .tilemapLeft = 16,
    .tilemapTop = 3,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x0C6
};

const struct ListMenuItem sListMenuItems_PossibleGroupMembers[] = {
    {gUnionRoomActivity_Blank, 0},
    {gUnionRoomActivity_Blank, 1},
    {gUnionRoomActivity_Blank, 2},
    {gUnionRoomActivity_Blank, 3},
    {gUnionRoomActivity_Blank, 4}
};

static const struct ListMenuTemplate sListMenuTemplate_PossibleGroupMembers = {
    .items = sListMenuItems_PossibleGroupMembers,
    .moveCursorFunc = NULL,
    .itemPrintFunc = ItemPrintFunc_PossibleGroupMembers,
    .totalItems = 5,
    .maxShowed = 5,
    .windowId = 0,
    .header_X = 0,
    .item_X = 1,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 2,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 2,
    .cursorKind = 1
};

static const struct WindowTemplate sWindowTemplate_MysteryGiftList = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 17,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 0x044
};

static const struct WindowTemplate sWindowTemplate_MysteryGiftPlayerNameAndId = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 3,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x0EE
};

static const struct ListMenuItem sListMenuItems_UnionRoomGroups[] = {
    {gUnionRoomActivity_Blank,  0},
    {gUnionRoomActivity_Blank,  1},
    {gUnionRoomActivity_Blank,  2},
    {gUnionRoomActivity_Blank,  3},
    {gUnionRoomActivity_Blank,  4},
    {gUnionRoomActivity_Blank,  5},
    {gUnionRoomActivity_Blank,  6},
    {gUnionRoomActivity_Blank,  7},
    {gUnionRoomActivity_Blank,  8},
    {gUnionRoomActivity_Blank,  9},
    {gUnionRoomActivity_Blank, 10},
    {gUnionRoomActivity_Blank, 11},
    {gUnionRoomActivity_Blank, 12},
    {gUnionRoomActivity_Blank, 13},
    {gUnionRoomActivity_Blank, 14},
    {gUnionRoomActivity_Blank, 15}
};

static const struct ListMenuTemplate sListMenuTemplate_UnionRoomGroups = {
    .items = sListMenuItems_UnionRoomGroups,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = ListMenuItemPrintFunc_UnionRoomGroups,
    .totalItems = 16,
    .maxShowed = 5,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 2,
    .scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD,
    .fontId = 2,
    .cursorKind = 0
};

static const struct WindowTemplate sWindowTemplate_InviteToActivity = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 6,
    .width = 8,
    .height = 7,
    .paletteNum = 15,
    .baseBlock = 0x001
};

static const struct ListMenuItem sListMenuItems_InviteToActivity[] = {
    {gUnknown_8459354,  _8456CD8( ACTIVITY_CARD, 2)},
    {gUnknown_8459344,  _8456CD8(ACTIVITY_BATTLE | IN_UNION_ROOM, 2)},
    {gUnknown_845934C,  _8456CD8(ACTIVITY_CHAT | IN_UNION_ROOM, 2)},
    {gUnknown_8459360,  _8456CD8(ACTIVITY_NONE | IN_UNION_ROOM, 0)}
};

static const struct ListMenuTemplate sListMenuTemplate_InviteToActivity = {
    .items = sListMenuItems_InviteToActivity,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 4,
    .maxShowed = 4,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 2,
    .cursorKind = 0
};

static const struct WindowTemplate sWindowTemplate_TradeBoardRegisterInfoExit = {
    .bg = 0,
    .tilemapLeft = 18,
    .tilemapTop = 8,
    .width = 11,
    .height = 5,
    .paletteNum = 15,
    .baseBlock = 0x001
};

static const struct ListMenuItem gUnknown_8456E3C[] = {
    {gText_Register,   1},
    {gUnknown_8459370, 2},
    {gUnknown_8459360, 3}
};

static const struct ListMenuTemplate sListMenuTemplate_TradeBoardRegisterInfoExit = {
    .items = gUnknown_8456E3C,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 3,
    .maxShowed = 3,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 2,
    .cursorKind = 0
};

static const struct WindowTemplate sWindowTemplate_TypeNames = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 2,
    .width = 9,
    .height = 11,
    .paletteNum = 15,
    .baseBlock = 0x001
};

static const struct WindowTemplate sTradeBoardWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 28,
    .height = 2,
    .paletteNum = 13,
    .baseBlock = 0x001
};

static const struct WindowTemplate gUnknown_8456F24 = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 5,
    .width = 28,
    .height = 10,
    .paletteNum = 13,
    .baseBlock = 0x039
};

static const struct ListMenuItem sTradeBoardListMenuItems[] = {
    {gUnionRoomActivity_Blank, -3},
    {gUnionRoomActivity_Blank, 0},
    {gUnionRoomActivity_Blank, 1},
    {gUnionRoomActivity_Blank, 2},
    {gUnionRoomActivity_Blank, 3},
    {gUnionRoomActivity_Blank, 4},
    {gUnionRoomActivity_Blank, 5},
    {gUnionRoomActivity_Blank, 6},
    {gUnionRoomActivity_Blank, 7},
    {gUnknown_8459368, 8}
};

static const struct WindowTemplate gUnknown_8456F94 = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 5,
    .width = 28,
    .height = 10,
    .paletteNum = 13,
    .baseBlock = 0x039
};

static const struct ListMenuItem gUnknown_8456F9C[] = {
    {gUnionRoomActivity_Blank,  0},
    {gUnionRoomActivity_Blank,  1},
    {gUnionRoomActivity_Blank,  2},
    {gUnionRoomActivity_Blank,  3},
    {gUnionRoomActivity_Blank,  4},
    {gUnionRoomActivity_Blank,  5},
    {gUnionRoomActivity_Blank,  6},
    {gUnionRoomActivity_Blank,  7},
    {gUnionRoomActivity_Blank,  8},
    {gUnionRoomActivity_Blank,  9},
    {gUnionRoomActivity_Blank, 10},
    {gUnionRoomActivity_Blank, 11},
    {gUnionRoomActivity_Blank, 12},
    {gUnionRoomActivity_Blank, 13},
    {gUnionRoomActivity_Blank, 14},
    {gUnionRoomActivity_Blank, 15}
};

static const struct UnionGnameUnamePair sUnionGnameUnamePair_Dummy = {};

// starts at gUnknown_082F0474 in pokeemerald, union link groups

ALIGNED(4) static const u8 gUnknown_845704C[] = {
    ACTIVITY_BATTLE,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457050[] = {
    ACTIVITY_DBLBATTLE,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457054[] = {
    ACTIVITY_MLTBATTLE,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457058[] = {
    ACTIVITY_TRADE,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_845705C[] = {
    ACTIVITY_PJUMP,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457060[] = {
    ACTIVITY_BCRUSH,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457064[] = {
    ACTIVITY_BPICK,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457068[] = {
    ACTIVITY_WCARD2,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_845706C[] = {
    ACTIVITY_WNEWS2,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457070[] = {
    ACTIVITY_NONE | IN_UNION_ROOM,
    ACTIVITY_BATTLE | IN_UNION_ROOM,
    ACTIVITY_TRADE | IN_UNION_ROOM,
    ACTIVITY_CHAT | IN_UNION_ROOM,
    ACTIVITY_CARD | IN_UNION_ROOM,
    ACTIVITY_ACCEPT | IN_UNION_ROOM,
    ACTIVITY_DECLINE | IN_UNION_ROOM,
    ACTIVITY_NPCTALK | IN_UNION_ROOM,
    ACTIVITY_PLYRTALK | IN_UNION_ROOM,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_845707C[] = {
    ACTIVITY_SEARCH,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_8457080[] = {
    ACTIVITY_BATTLE,
    ACTIVITY_DBLBATTLE,
    ACTIVITY_MLTBATTLE,
    ACTIVITY_TRADE,
    ACTIVITY_PJUMP,
    ACTIVITY_BCRUSH,
    ACTIVITY_BPICK,
    ACTIVITY_WCARD2,
    ACTIVITY_WNEWS2,
    ACTIVITY_SPINTRADE,
    0xFF
};

ALIGNED(4) static const u8 gUnknown_845708C[] = {
    ACTIVITY_BATTLE,
    ACTIVITY_DBLBATTLE,
    ACTIVITY_MLTBATTLE,
    ACTIVITY_TRADE,
    ACTIVITY_BCRUSH,
    0xFF
};

static const u8 *const sAcceptedActivityIds[] = {
    [LINK_GROUP_SINGLE_BATTLE] = gUnknown_845704C,
    [LINK_GROUP_DOUBLE_BATTLE] = gUnknown_8457050,
    [LINK_GROUP_MULTI_BATTLE]  = gUnknown_8457054,
    [LINK_GROUP_TRADE]         = gUnknown_8457058,
    [LINK_GROUP_POKEMON_JUMP]  = gUnknown_845705C,
    [LINK_GROUP_BERRY_CRUSH]   = gUnknown_8457060,
    [LINK_GROUP_BERRY_PICKING] = gUnknown_8457064,
    [LINK_GROUP_WONDER_CARD]   = gUnknown_8457068,
    [LINK_GROUP_WONDER_NEWS]   = gUnknown_845706C,
    [9]                        = gUnknown_8457070,
    [10]                       = gUnknown_845707C,
    [11]                       = gUnknown_8457080,
    [12]                       = gUnknown_845708C
};

static const u8 sLinkGroupToURoomActivity[] = {
    [LINK_GROUP_SINGLE_BATTLE] = ACTIVITY_BATTLE,
    [LINK_GROUP_DOUBLE_BATTLE] = ACTIVITY_DBLBATTLE,
    [LINK_GROUP_MULTI_BATTLE]  = ACTIVITY_MLTBATTLE,
    [LINK_GROUP_TRADE]         = ACTIVITY_TRADE,
    [LINK_GROUP_POKEMON_JUMP]  = ACTIVITY_PJUMP,
    [LINK_GROUP_BERRY_CRUSH]   = ACTIVITY_BCRUSH,
    [LINK_GROUP_BERRY_PICKING] = ACTIVITY_BPICK,
    [LINK_GROUP_WONDER_CARD]   = ACTIVITY_WCARD2,
    [LINK_GROUP_WONDER_NEWS]   = ACTIVITY_WNEWS2
};

static const u8 sUnref_84570D1[] = _("{DYNAMIC 00}·{DYNAMIC 01}");

#undef _8456CD8

// These are functions in Emerald but inlined in FireRed

#define IntlConvPartnerUname7(dest, arg1) ({                            \
    StringCopy7(dest, (arg1).gname_uname.uname);                           \
    ConvertInternationalString(dest, (arg1).gname_uname.gname.unk_00.language); \
})

#define IntlConvPartnerUname(dest, arg1) ({                             \
    StringCopy(dest, (arg1).gname_uname.uname);                            \
    ConvertInternationalString(dest, (arg1).gname_uname.gname.unk_00.language); \
})

#define CopyTrainerCardData(dest, src, _version) ({ \
    (dest) = *((struct TrainerCard * )(src));       \
    (dest).version = _version;                      \
})

#define GetStringRightAlignXOffset(_fontId, _string, _maxWidth) ({ \
    u16 strWidth = GetStringWidth(_fontId, _string, 0);            \
    _maxWidth - strWidth;                                          \
})

static void PrintPlayerNameAndIdOnWindow(u8 windowId)
{
    u8 text[12];
    u8 text2[12];

    UR_AddTextPrinterParameterized(windowId, 2, gSaveBlock2Ptr->playerName, 0, 2, UR_COLOR_DKE_WHT_LTE);
    StringCopy(text2, gUnknown_84571B4);
    ConvertIntToDecimalStringN(text, ReadAsU16(gSaveBlock2Ptr->playerTrainerId), STR_CONV_MODE_LEADING_ZEROS, 5);
    StringAppend(text2, text);
    UR_AddTextPrinterParameterized(windowId, 0, text2, 0, 0x10, UR_COLOR_DKE_WHT_LTE);
}

static void StringExpandPlaceholders_AwaitingCommFromAnother(u8 *dst, u8 caseId)
{
    switch (caseId)
    {
    case ACTIVITY_BATTLE:
    case ACTIVITY_DBLBATTLE:
    case ACTIVITY_MLTBATTLE:
    case ACTIVITY_TRADE:
    case ACTIVITY_PJUMP:
    case ACTIVITY_BCRUSH:
    case ACTIVITY_BPICK:
    case ACTIVITY_WCARD2:
    case ACTIVITY_WNEWS2:
        // UB: argument *dst isn't used, instead it always prints to gStringVar4
        StringExpandPlaceholders(gStringVar4, gUnknown_8457234);
        break;
    }
}

static void Leader_GetAcceptNewMemberPrompt(u8 *dst, u8 activity)
{
    switch (activity)
    {
    case ACTIVITY_BATTLE:
    case ACTIVITY_DBLBATTLE:
    case ACTIVITY_TRADE:
        StringExpandPlaceholders(dst, gUnknown_84574A0);
        break;
    case ACTIVITY_WCARD2:
    case ACTIVITY_WNEWS2:
        StringExpandPlaceholders(dst, gUnknown_84574C4);
        break;
    case ACTIVITY_MLTBATTLE:
    case ACTIVITY_PJUMP:
    case ACTIVITY_BCRUSH:
    case ACTIVITY_BPICK:
        StringExpandPlaceholders(dst, gUnknown_84574EC);
        break;
    }
}

static bool8 Leader_SetStateIfMemberListChanged(struct UnkStruct_Leader * data, u32 state1, u32 state2)
{
    switch (LeaderUpdateGroupMembership(data->field_0))
    {
    case UNION_ROOM_SPAWN_IN:
        PlaySE(SE_PC_LOGIN);
        RedrawListMenu(data->listTaskId);
        IntlConvPartnerUname7(gStringVar2, data->field_0->arr[data->playerCount]);
        Leader_GetAcceptNewMemberPrompt(gStringVar4, sPlayerCurrActivity);
        data->state = state1;
        break;
    case UNION_ROOM_SPAWN_OUT:
        RfuSetStatus(0, 0);
        RedrawListMenu(data->listTaskId);
        data->state = state2;
        return TRUE;
    }

    return FALSE;
}

static void ItemPrintFunc_PossibleGroupMembers(u8 windowId, u32 itemId, u8 y)
{
    struct UnkStruct_Leader * data = sUnionRoomMain.leader;
    u8 var = 0;

    switch (data->field_0->arr[itemId].groupScheduledAnim)
    {
    case UNION_ROOM_SPAWN_IN:
        if (data->field_0->arr[itemId].field_1B != 0)
            var = UR_COLOR_GRN_WHT_LTG;
        break;
    case UNION_ROOM_SPAWN_OUT:
        var = UR_COLOR_RED_WHT_LTR;
        break;
    }

    PrintGroupMemberCandidateOnWindowWithColor(windowId, 0, y, &data->field_0->arr[itemId], var, itemId);
}

static u8 LeaderUpdateGroupMembership(struct UnkStruct_Main0 * arg0)
{
    struct UnkStruct_Leader * data = sUnionRoomMain.leader;
    u8 ret = UNION_ROOM_SPAWN_NONE;
    u8 i;
    s32 id;

    for (i = 1; i < UROOM_MAX_PARTY_SIZE; i++)
    {
        u16 var = data->field_0->arr[i].groupScheduledAnim;
        if (var == UNION_ROOM_SPAWN_IN)
        {
            id = Findx20Inx1CArray(&data->field_0->arr[i], data->field_4->arr);
            if (id != 0xFF)
            {
                data->field_0->arr[i].gname_uname = data->field_4->arr[id].gname_uname;
                data->field_0->arr[i].field_18 = 1;
            }
            else
            {
                data->field_0->arr[i].groupScheduledAnim = UNION_ROOM_SPAWN_OUT;
                ret = UNION_ROOM_SPAWN_OUT;
            }
        }
    }

    for (id = 0; id < RFU_CHILD_MAX; id++)
        Appendx1Ctox20(data->field_0->arr, &data->field_4->arr[id], UROOM_MAX_PARTY_SIZE);

    if (ret != UNION_ROOM_SPAWN_OUT)
    {
        for (id = 0; id < UROOM_MAX_PARTY_SIZE; id++)
        {
            if (data->field_0->arr[id].field_1B != 0)
                ret = UNION_ROOM_SPAWN_IN;
        }
    }

    return ret;
}

static u8 UnionRoomLeaderField0CompactionAndCount(struct UnkStruct_Main0 * arg0)
{
    struct UnkStruct_Leader * data = sUnionRoomMain.leader;
    u8 copiedCount;
    s32 i;
    u8 ret;

    for (i = 0; i < UROOM_MAX_PARTY_SIZE; i++)
        data->field_8->arr[i] = data->field_0->arr[i];

    copiedCount = 0;
    for (i = 0; i < UROOM_MAX_PARTY_SIZE; i++)
    {
        if (data->field_8->arr[i].groupScheduledAnim == UNION_ROOM_SPAWN_IN)
        {
            data->field_0->arr[copiedCount] = data->field_8->arr[i];
            copiedCount++;
        }
    }

    ret = copiedCount;
    for (; copiedCount < UROOM_MAX_PARTY_SIZE; copiedCount++)
    {
        data->field_0->arr[copiedCount].gname_uname = sUnionGnameUnamePair_Dummy;
        data->field_0->arr[copiedCount].field_18 = 0;
        data->field_0->arr[copiedCount].groupScheduledAnim = UNION_ROOM_SPAWN_NONE;
        data->field_0->arr[copiedCount].field_1A_1 = FALSE;
        data->field_0->arr[copiedCount].field_1B = 0;
    }

    for (i = 0; i < UROOM_MAX_PARTY_SIZE; i++)
    {
        if (data->field_0->arr[i].groupScheduledAnim != UNION_ROOM_SPAWN_IN)
            continue;
        if (data->field_0->arr[i].field_1B != 0x40)
            continue;

        ret = i;
        break;
    }

    return ret;
}

static bool32 IsPartnerActivityAcceptable(u32 activity, u32 group)
{
    if (group == 0xFF)
        return TRUE;

    if (group <= NELEMS(sAcceptedActivityIds)) // UB: <= may access data outside the array
    {
        const u8 *bytes = sAcceptedActivityIds[group];

        while ((*(bytes) != 0xFF))
        {
            if ((*bytes) == activity)
                return TRUE;
            bytes++;
        }
    }

    return FALSE;
}

static u8 URoomGroupListGetTextColor(struct UnkStruct_Group * data, u32 id)
{
    if (data->field_0->arr[id].groupScheduledAnim == UNION_ROOM_SPAWN_IN)
    {
        if (data->field_0->arr[id].gname_uname.gname.started)
            return UR_COLOR_WHT_WHT_LTE;
        else if (data->field_0->arr[id].field_1A_1)
            return UR_COLOR_RED_WHT_LTR;
        else if (data->field_0->arr[id].field_1B != 0)
            return UR_COLOR_GRN_WHT_LTG;
    }

    return UR_COLOR_DKE_WHT_LTE;
}

static void ListMenuItemPrintFunc_UnionRoomGroups(u8 windowId, u32 itemId, u8 y)
{
    struct UnkStruct_Group * data = sUnionRoomMain.group;
    u8 color_idx = URoomGroupListGetTextColor(data, itemId);

    PrintUnionRoomGroupOnWindow(windowId, 8, y, &data->field_0->arr[itemId], color_idx, itemId);
}

static u8 GetNewLeaderCandidate(void)
{
    struct UnkStruct_Group * data = sUnionRoomMain.group;
    u8 ret = 0;
    u8 i;
    s32 id;

    for (i = 0; i < 16; i++)
    {
        if (data->field_0->arr[i].groupScheduledAnim != UNION_ROOM_SPAWN_NONE)
        {
            id = Findx20Inx1CArray(&data->field_0->arr[i], data->field_4->arr);
            if (id != 0xFF)
            {
                if (data->field_0->arr[i].groupScheduledAnim == UNION_ROOM_SPAWN_IN)
                {
                    if (AreUnionRoomPlayerGnamesDifferent(&data->field_0->arr[i].gname_uname, &data->field_4->arr[id].gname_uname))
                    {
                        data->field_0->arr[i].gname_uname = data->field_4->arr[id].gname_uname;
                        data->field_0->arr[i].field_1B = 64;
                        ret = 1;
                    }
                    else
                    {
                        if (data->field_0->arr[i].field_1B != 0)
                        {
                            data->field_0->arr[i].field_1B--;
                            if (data->field_0->arr[i].field_1B == 0)
                                ret = 2;
                        }
                    }
                }
                else
                {
                    data->field_0->arr[i].groupScheduledAnim = UNION_ROOM_SPAWN_IN;
                    data->field_0->arr[i].field_1B = 64;
                    ret = 1;
                }

                data->field_0->arr[i].field_18 = 0;
            }
            else
            {
                if (data->field_0->arr[i].groupScheduledAnim != UNION_ROOM_SPAWN_OUT)
                {
                    data->field_0->arr[i].field_18++;
                    if (data->field_0->arr[i].field_18 >= 300)
                    {
                        data->field_0->arr[i].groupScheduledAnim = UNION_ROOM_SPAWN_OUT;
                        ret = 2;
                    }
                }
            }
        }
    }

    for (id = 0; id < RFU_CHILD_MAX; id++)
    {
        if (Appendx1Ctox20(data->field_0->arr, &data->field_4->arr[id], 16) != 0xFF)
            ret = 1;
    }

    return ret;
}

void MEvent_CreateTask_Leader(u32 activity)
{
    u8 taskId;
    struct UnkStruct_Leader * dataPtr;

    taskId = CreateTask(Task_MEvent_Leader, 0);
    sUnionRoomMain.leader = dataPtr = (void*)(gTasks[taskId].data);

    dataPtr->state = 0;
    dataPtr->textState = 0;
    dataPtr->activity = activity;
    gSpecialVar_Result = 0;
}

static void Task_MEvent_Leader(u8 taskId)
{
    struct UnkStruct_Leader * data = sUnionRoomMain.leader;
    struct WindowTemplate winTemplate;
    s32 val;

    switch (data->state)
    {
    case 0:
        sPlayerCurrActivity = data->activity;
        sPlayerActivityGroupSize = 2;
        SetHostRFUtgtGname(data->activity, 0, 0);
        SetGnameBufferWonderFlags(FALSE, FALSE);
        SetWirelessCommType1();
        OpenLink();
        InitializeRfuLinkManager_LinkLeader(2);
        data->state = 1;
        break;
    case 1:
        data->field_4 = AllocZeroed(4 * sizeof(struct UnkStruct_x1C));
        data->field_0 = AllocZeroed(UROOM_MAX_PARTY_SIZE * sizeof(struct UnkStruct_x20));
        data->field_8 = AllocZeroed(UROOM_MAX_PARTY_SIZE * sizeof(struct UnkStruct_x20));
        BlankUnkStruct_x1CArray(data->field_4->arr, 4);
        BlankUnkStruct_x20Array(data->field_0->arr, UROOM_MAX_PARTY_SIZE);
        LinkRfu3_SetGnameUnameFromStaticBuffers(&data->field_0->arr[0].gname_uname.gname, data->field_0->arr[0].gname_uname.uname);
        data->field_0->arr[0].field_18 = 0;
        data->field_0->arr[0].groupScheduledAnim = UNION_ROOM_SPAWN_IN;
        data->field_0->arr[0].field_1A_1 = FALSE;
        data->field_0->arr[0].field_1B = 0;
        data->listenTaskId = CreateTask_ListenForPartnersWithCompatibleSerialNos(data->field_4, 0xFF);

        winTemplate = sWindowTemplate_List_PossibleGroupMembers;
        winTemplate.baseBlock = GetMysteryGiftBaseBlock();
        data->listWindowId = AddWindow(&winTemplate);
        MG_DrawTextBorder(data->listWindowId);
        gMultiuseListMenuTemplate = sListMenuTemplate_PossibleGroupMembers;
        gMultiuseListMenuTemplate.windowId = data->listWindowId;
        data->listTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);

        CopyBgTilemapBufferToVram(0);
        data->playerCount = 1;
        data->state = 2;
        break;
    case 2:
        StringCopy(gStringVar1, sUnionRoomActivityStringPtrs[sPlayerCurrActivity]);
        StringExpandPlaceholders_AwaitingCommFromAnother(gStringVar4, sPlayerCurrActivity);
        data->state = 3;
        break;
    case 3:
        AddTextPrinterToWindow1(gStringVar4);
        data->state = 4;
        break;
    case 4:
        Leader_SetStateIfMemberListChanged(data, 5, 6);
        if (JOY_NEW(B_BUTTON))
        {
            data->state = 13;
            DestroyWirelessStatusIndicatorSprite();
        }
        break;
    case 6:
        // The link with your friend has been dropped...
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_84577F8))
        {
            data->playerCount = UnionRoomLeaderField0CompactionAndCount(data->field_0);
            RedrawListMenu(data->listTaskId);
            data->state = 2;
        }
        break;
    case 5:
        data->state = 7;
        break;
    case 7:
        switch (mevent_message_print_and_prompt_yes_no(&data->textState, (u16 *)&data->messageWindowId, FALSE, gStringVar4))
        {
        case 0:
            LoadWirelessStatusIndicatorSpriteGfx();
            CreateWirelessStatusIndicatorSprite(0, 0);
            data->field_0->arr[data->playerCount].field_1B = 0;
            RedrawListMenu(data->listTaskId);
            data->field_19 = 5;
            SendByteToPartnerByIdAndName(5, ReadAsU16(data->field_0->arr[data->playerCount].gname_uname.gname.unk_00.playerTrainerId), data->field_0->arr[data->playerCount].gname_uname.uname);
            data->state = 8;
            break;
        case 1:
        case -1:
            data->field_19 = 6;
            SendByteToPartnerByIdAndName(6, ReadAsU16(data->field_0->arr[data->playerCount].gname_uname.gname.unk_00.playerTrainerId), data->field_0->arr[data->playerCount].gname_uname.uname);
            data->state = 8;
            break;
        }
        break;
    case 8:
        val = WaitSendByteToPartnerByIdAndName(ReadAsU16(data->field_0->arr[data->playerCount].gname_uname.gname.unk_00.playerTrainerId), data->field_0->arr[data->playerCount].gname_uname.uname);
        if (val == 1)
        {
            if (data->field_19 == 5)
            {
                data->field_0->arr[data->playerCount].field_1B = 0;
                RedrawListMenu(data->listTaskId);
                data->playerCount++;
                IntlConvPartnerUname7(gStringVar1, data->field_0->arr[data->playerCount - 1]);
                StringExpandPlaceholders(gStringVar4, gUnknown_8457554);
                data->state = 9;
                LinkRfu_StopManagerAndFinalizeSlots();
            }
            else
            {
                RequestDisconnectSlotByTrainerNameAndId(data->field_0->arr[data->playerCount].gname_uname.uname, ReadAsU16(data->field_0->arr[data->playerCount].gname_uname.gname.unk_00.playerTrainerId));
                data->field_0->arr[data->playerCount].groupScheduledAnim = UNION_ROOM_SPAWN_NONE;
                UnionRoomLeaderField0CompactionAndCount(data->field_0);
                RedrawListMenu(data->listTaskId);
                data->state = 2;
            }

            data->field_19 = 0;
        }
        else if (val == 2)
        {
            RfuSetStatus(0, 0);
            data->state = 2;
        }
        break;
    case 9:
        AddTextPrinterToWindow1(gStringVar4);
        data->state = 10;
        break;
    case 10:
        if (++data->delayTimerAfterOk > 120)
            data->state = 11;
        break;
    case 11:
        if (!Leader_SetStateIfMemberListChanged(data, 5, 6))
            data->state = 12;
        break;
    case 12:
        if (LmanAcceptSlotFlagIsNotZero())
        {
            WaitRfuState(FALSE);
            data->state = 15;
        }
        else
        {
            data->state = 6;
        }
        break;
    case 13:
        DestroyWirelessStatusIndicatorSprite();
        LinkRfu_Shutdown();
        DestroyListMenuTask(data->listTaskId, 0, 0);
        CopyBgTilemapBufferToVram(0);
        RemoveWindow(data->listWindowId);
        DestroyTask(data->listenTaskId);
        Free(data->field_8);
        Free(data->field_0);
        Free(data->field_4);
        data->state++;
        break;
    case 14:
        // Please start over from the beginning.
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_84571B8))
        {
            DestroyTask(taskId);
            gSpecialVar_Result = 5;
        }
        break;
    case 15:
        if (RfuGetStatus() == 1 || RfuGetStatus() == 2)
        {
            data->state = 13;
        }
        else if (gReceivedRemoteLinkPlayers)
        {
            UpdateGameData_GroupLockedIn(TRUE);
            data->state++;
        }
        break;
    case 16:
        DestroyListMenuTask(data->listTaskId, 0, 0);
        CopyBgTilemapBufferToVram(0);
        RemoveWindow(data->listWindowId);
        DestroyTask(data->listenTaskId);
        Free(data->field_8);
        Free(data->field_0);
        Free(data->field_4);
        SetLinkStandbyCallback();
        data->state++;
        break;
    case 17:
        if (IsLinkTaskFinished())
            DestroyTask(taskId);
        break;
    }
}

void MEvent_CreateTask_CardOrNewsWithFriend(u32 activity)
{
    u8 taskId;
    struct UnkStruct_Group * dataPtr;

    taskId = CreateTask(Task_CardOrNewsWithFriend, 0);
    sUnionRoomMain.group = dataPtr = (void*)(gTasks[taskId].data);
    sGroup = dataPtr;

    dataPtr->state = 0;
    dataPtr->textState = 0;
    dataPtr->cardOrNews = activity - ACTIVITY_WCARD2; // 0: Card; 1: News
    gSpecialVar_Result = 0;
}

static void Task_CardOrNewsWithFriend(u8 taskId)
{
    s32 id;
    struct WindowTemplate winTemplate1, winTemplate2;
    struct UnkStruct_Group * data = sUnionRoomMain.group;

    switch (data->state)
    {
    case 0:
        SetHostRFUtgtGname(data->cardOrNews + ACTIVITY_WCARD2, 0, 0);
        SetWirelessCommType1();
        OpenLink();
        InitializeRfuLinkManager_JoinGroup();
        data->field_4 = AllocZeroed(4 * sizeof(struct UnkStruct_x1C));
        data->field_0 = AllocZeroed(16 * sizeof(struct UnkStruct_x20));
        data->state = 1;
        break;
    case 1:
        AddTextPrinterToWindow1(gUnknown_8458FE4);
        data->state = 2;
        break;
    case 2:
        BlankUnkStruct_x1CArray(data->field_4->arr, 4);
        BlankUnkStruct_x20Array(data->field_0->arr, 16);
        data->listenTaskId = CreateTask_ListenForPartnersWithCompatibleSerialNos(data->field_4, data->cardOrNews + LINK_GROUP_WONDER_CARD);

        winTemplate1 = sWindowTemplate_MysteryGiftList;
        winTemplate1.baseBlock = GetMysteryGiftBaseBlock();
        data->listWindowId = AddWindow(&winTemplate1);

        data->playerNameAndIdWindowId = AddWindow(&sWindowTemplate_MysteryGiftPlayerNameAndId);

        MG_DrawTextBorder(data->listWindowId);
        gMultiuseListMenuTemplate = sListMenuTemplate_UnionRoomGroups;
        gMultiuseListMenuTemplate.windowId = data->listWindowId;
        data->listTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);

        MG_DrawTextBorder(data->playerNameAndIdWindowId);
        FillWindowPixelBuffer(data->playerNameAndIdWindowId, PIXEL_FILL(1));
        PutWindowTilemap(data->playerNameAndIdWindowId);
        PrintPlayerNameAndIdOnWindow(data->playerNameAndIdWindowId);
        CopyWindowToVram(data->playerNameAndIdWindowId, COPYWIN_GFX);

        CopyBgTilemapBufferToVram(0);
        data->leaderId = 0;
        data->state = 3;
        break;
    case 3:
        id = GetNewLeaderCandidate();
        switch (id)
        {
        case 1:
            PlaySE(SE_PC_LOGIN);
        default:
            RedrawListMenu(data->listTaskId);
            break;
        case 0:
            id = ListMenu_ProcessInput(data->listTaskId);
            if (JOY_NEW(A_BUTTON) && id != -1)
            {
                // this unused variable along with the assignment is needed to match
                u32 unusedVar;
                unusedVar  = data->field_0->arr[id].gname_uname.gname.activity;

                if (data->field_0->arr[id].groupScheduledAnim == UNION_ROOM_SPAWN_IN && !data->field_0->arr[id].gname_uname.gname.started)
                {
                    data->leaderId = id;
                    LoadWirelessStatusIndicatorSpriteGfx();
                    CreateWirelessStatusIndicatorSprite(0, 0);
                    RedrawListMenu(data->listTaskId);
                    IntlConvPartnerUname(gStringVar1, data->field_0->arr[data->leaderId]);
                    CreateTask_RfuReconnectWithParent(data->field_0->arr[data->leaderId].gname_uname.uname, ReadAsU16(data->field_0->arr[data->leaderId].gname_uname.gname.unk_00.playerTrainerId));
                    PlaySE(SE_POKENAV_ON);
                    data->state = 4;
                }
                else
                {
                    PlaySE(SE_WALL_HIT);
                }
            }
            else if (JOY_NEW(B_BUTTON))
            {
                data->state = 6;
            }
            break;
        }
        break;
    case 4:
        AddTextPrinterToWindow1(gUnknown_8459238);
        IntlConvPartnerUname(gStringVar1, data->field_0->arr[data->leaderId]);
        data->state = 5;
        break;
    case 5:
        if (gReceivedRemoteLinkPlayers)
        {
            sPlayerCurrActivity = data->field_0->arr[data->leaderId].gname_uname.gname.activity;
            data->state = 10;
        }

        switch (RfuGetStatus())
        {
        case 1:
        case 2:
        case 6:
            data->state = 8;
            break;
        case 5:
            AddTextPrinterToWindow1(gUnknown_84576AC);
            RfuSetStatus(0, 0);
            break;
        }
        break;
    case 6:
    case 8:
    case 10:
        DestroyListMenuTask(data->listTaskId, 0, 0);
        CopyBgTilemapBufferToVram(0);
        RemoveWindow(data->playerNameAndIdWindowId);
        RemoveWindow(data->listWindowId);
        DestroyTask(data->listenTaskId);
        Free(data->field_0);
        Free(data->field_4);
        data->state++;
        break;
    case 9:
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_8457838[RfuGetStatus()]))
        {
            DestroyWirelessStatusIndicatorSprite();
            DestroyTask(taskId);
            LinkRfu_Shutdown();
            gSpecialVar_Result = 5;
        }
        break;
    case 7:
        DestroyWirelessStatusIndicatorSprite();
        AddTextPrinterToWindow1(gUnknown_84571B8);
        DestroyTask(taskId);
        LinkRfu_Shutdown();
        gSpecialVar_Result = 5;
        break;
    case 11:
        data->state++;
        SetLinkStandbyCallback();
        break;
    case 12:
        if (IsLinkTaskFinished())
            DestroyTask(taskId);
        break;
    }
}

void MEvent_CreateTask_CardOrNewsOverWireless(u32 activity)
{
    u8 taskId;
    struct UnkStruct_Group * dataPtr;

    taskId = CreateTask(Task_CardOrNewsOverWireless, 0);
    sUnionRoomMain.group = dataPtr = (void*)(gTasks[taskId].data);
    sGroup = dataPtr;

    dataPtr->state = 0;
    dataPtr->textState = 0;
    dataPtr->cardOrNews = activity - ACTIVITY_WCARD2; // 0: Card; 1: News
    gSpecialVar_Result = 0;
}

static void Task_CardOrNewsOverWireless(u8 taskId)
{
    s32 id;
    struct WindowTemplate winTemplate;
    struct UnkStruct_Group * data = sUnionRoomMain.group;

    switch (data->state)
    {
    case 0:
        SetHostRFUtgtGname(0, 0, 0);
        SetWirelessCommType1();
        OpenLink();
        InitializeRfuLinkManager_JoinGroup();
        data->field_4 = AllocZeroed(4 * sizeof(struct UnkStruct_x1C));
        data->field_0 = AllocZeroed(16 * sizeof(struct UnkStruct_x20));
        data->state = 1;
        break;
    case 1:
        AddTextPrinterToWindow1(gUnknown_84591DC);
        data->state = 2;
        break;
    case 2:
        BlankUnkStruct_x1CArray(data->field_4->arr, 4);
        BlankUnkStruct_x20Array(data->field_0->arr, 16);
        data->listenTaskId = CreateTask_ListenForPartnersWithSerial7F7D(data->field_4, data->cardOrNews + LINK_GROUP_WONDER_CARD);

        if (data->field_13 != 0)
        {
            winTemplate = sWindowTemplate_MysteryGiftList;
            winTemplate.baseBlock = GetMysteryGiftBaseBlock();
            data->listWindowId = AddWindow(&winTemplate);

            MG_DrawTextBorder(data->listWindowId);
            gMultiuseListMenuTemplate = sListMenuTemplate_UnionRoomGroups;
            gMultiuseListMenuTemplate.windowId = data->listWindowId;
            data->listTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);

            CopyBgTilemapBufferToVram(0);
        }

        data->leaderId = 0;
        data->state = 3;
        break;
    case 3:
        id = GetNewLeaderCandidate();
        switch (id)
        {
        case 1:
            PlaySE(SE_PC_LOGIN);
        default:
            if (data->field_13 != 0)
                RedrawListMenu(data->listTaskId);
            break;
        case 0:
            if (data->field_13 != 0)
                id = ListMenu_ProcessInput(data->listTaskId);
            if (data->refreshTimer > 120)
            {
                if (data->field_0->arr[0].groupScheduledAnim == UNION_ROOM_SPAWN_IN && !data->field_0->arr[0].gname_uname.gname.started)
                {
                    if (GetGnameWonderFlagByLinkGroup(&data->field_0->arr[0].gname_uname.gname, data->cardOrNews + LINK_GROUP_WONDER_CARD))
                    {
                        data->leaderId = 0;
                        data->refreshTimer = 0;
                        LoadWirelessStatusIndicatorSpriteGfx();
                        CreateWirelessStatusIndicatorSprite(0, 0);
                        CreateTask_RfuReconnectWithParent(data->field_0->arr[0].gname_uname.uname, ReadAsU16(data->field_0->arr[0].gname_uname.gname.unk_00.playerTrainerId));
                        PlaySE(SE_POKENAV_ON);
                        data->state = 4;
                    }
                    else
                    {
                        PlaySE(SE_BOO);
                        data->state = 10;
                    }
                }
            }
            else if (JOY_NEW(B_BUTTON))
            {
                data->state = 6;
                data->refreshTimer = 0;
            }
            data->refreshTimer++;
            break;
        }
        break;
    case 4:
        AddTextPrinterToWindow1(gUnknown_845928C);
        IntlConvPartnerUname(gStringVar1, data->field_0->arr[data->leaderId]);
        data->state = 5;
        break;
    case 5:
        if (gReceivedRemoteLinkPlayers)
        {
            sPlayerCurrActivity = data->field_0->arr[data->leaderId].gname_uname.gname.activity;
            data->state = 12;
        }

        switch (RfuGetStatus())
        {
        case 1:
        case 2:
        case 6:
            data->state = 8;
            break;
        case 5:
            AddTextPrinterToWindow1(gUnknown_845777C);
            RfuSetStatus(0, 0);
            break;
        }
        break;
    case 6:
    case 8:
    case 10:
    case 12:
        if (data->field_13 != 0)
        {
            DestroyListMenuTask(data->listTaskId, 0, 0);
            CopyBgTilemapBufferToVram(0);
            RemoveWindow(data->listWindowId);
        }
        DestroyTask(data->listenTaskId);
        Free(data->field_0);
        Free(data->field_4);
        data->state++;
        break;
    case 9:
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_84577BC))
        {
            DestroyWirelessStatusIndicatorSprite();
            DestroyTask(taskId);
            LinkRfu_Shutdown();
            gSpecialVar_Result = 5;
        }
        break;
    case 7:
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_84571E0))
        {
            DestroyWirelessStatusIndicatorSprite();
            DestroyTask(taskId);
            LinkRfu_Shutdown();
            gSpecialVar_Result = 5;
        }
        break;
    case 11:
        if (MG_PrintTextOnWindow1AndWaitButton(&data->textState, gUnknown_845933C[data->cardOrNews]))
        {
            DestroyWirelessStatusIndicatorSprite();
            DestroyTask(taskId);
            LinkRfu_Shutdown();
            gSpecialVar_Result = 5;
        }
        break;
    case 13:
        data->state++;
        SetLinkStandbyCallback();
        break;
    case 14:
        if (IsLinkTaskFinished())
            DestroyTask(taskId);
        break;
    }
}

static u16 ReadAsU16(const u8 *ptr)
{
    return (ptr[1] << 8) | (ptr[0]);
}

static void Task_ListenForPartnersWithCompatibleSerialNos(u8 taskId)
{
    s32 i, j;
    struct UnkStruct_Main4 ** ptr = (void*) gTasks[taskId].data;

    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        LinkRfu_GetNameIfCompatible(&ptr[0]->arr[i].gname_uname.gname, ptr[0]->arr[i].gname_uname.uname, i);
        if (!IsPartnerActivityAcceptable(ptr[0]->arr[i].gname_uname.gname.activity, gTasks[taskId].data[2]))
        {
            ptr[0]->arr[i].gname_uname = sUnionGnameUnamePair_Dummy;
        }
        for (j = 0; j < i; j++)
        {
            if (!AreGnameUnameDifferent(&ptr[0]->arr[j].gname_uname, &ptr[0]->arr[i].gname_uname))
            {
                ptr[0]->arr[i].gname_uname = sUnionGnameUnamePair_Dummy;
            }
        }
        ptr[0]->arr[i].active = AreGnameUnameDifferent(&ptr[0]->arr[i].gname_uname, &sUnionGnameUnamePair_Dummy);
    }
}

static bool32 GetGnameWonderFlagByLinkGroup(struct GFtgtGname * gname, s16 linkGroup)
{
    if (linkGroup == LINK_GROUP_WONDER_CARD)
    {
        if (!gname->unk_00.hasCard)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else if (linkGroup == LINK_GROUP_WONDER_NEWS)
    {
        if (!gname->unk_00.hasNews)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}

static void Task_ListenForPartnersWithSerial7F7D(u8 taskId)
{
    s32 i;
    struct UnkStruct_Main4 ** ptr = (void*) gTasks[taskId].data;

    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        if (LinkRfu_GetNameIfSerial7F7D(&ptr[0]->arr[i].gname_uname.gname, ptr[0]->arr[i].gname_uname.uname, i))
        {
            GetGnameWonderFlagByLinkGroup(&ptr[0]->arr[i].gname_uname.gname, gTasks[taskId].data[2]);
        }
        ptr[0]->arr[i].active = AreGnameUnameDifferent(&ptr[0]->arr[i].gname_uname, &sUnionGnameUnamePair_Dummy);
    }
}

static u8 CreateTask_ListenForPartnersWithCompatibleSerialNos(struct UnkStruct_Main4 * main4, u32 linkGroup)
{
    u8 taskId = CreateTask(Task_ListenForPartnersWithCompatibleSerialNos, 0);
    struct UnkStruct_Main4 ** ptr = (void*) gTasks[taskId].data;
    ptr[0] = main4;
    gTasks[taskId].data[2] = linkGroup;
    return taskId;
}

static u8 CreateTask_ListenForPartnersWithSerial7F7D(struct UnkStruct_Main4 * main4, u32 linkGroup)
{
    u8 taskId = CreateTask(Task_ListenForPartnersWithSerial7F7D, 0);
    struct UnkStruct_Main4 ** ptr = (void*) gTasks[taskId].data;
    ptr[0] = main4;
    gTasks[taskId].data[2] = linkGroup;
    return taskId;
}

static void UR_AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 colorIdx)
{
    struct TextPrinterTemplate printerTemplate;

    printerTemplate.currentChar = str;
    printerTemplate.windowId = windowId;
    printerTemplate.fontId = fontId;
    printerTemplate.x = x;
    printerTemplate.y = y;
    printerTemplate.currentX = x;
    printerTemplate.currentY = y;
    printerTemplate.unk = 0;

    gTextFlags.useAlternateDownArrow = FALSE;
    switch (colorIdx)
    {
    case UR_COLOR_DKE_WHT_LTE:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_DARK_GRAY;
        printerTemplate.bgColor = TEXT_COLOR_WHITE;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_GRAY;
        break;
    case UR_COLOR_RED_WHT_LTR:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_RED;
        printerTemplate.bgColor = TEXT_COLOR_WHITE;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_RED;
        break;
    case UR_COLOR_GRN_WHT_LTG:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_GREEN;
        printerTemplate.bgColor = TEXT_COLOR_WHITE;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_GREEN;
        break;
    case UR_COLOR_WHT_WHT_LTE:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_WHITE;
        printerTemplate.bgColor = TEXT_COLOR_WHITE;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_GRAY;
        break;
    case UR_COLOR_WHT_DKE_LTE:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_WHITE;
        printerTemplate.bgColor = TEXT_COLOR_DARK_GRAY;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_GRAY;
        break;
    case UR_COLOR_GRN_DN6_LTB:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_COLOR_LIGHT_GREEN;
        printerTemplate.bgColor = TEXT_DYNAMIC_COLOR_6;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_BLUE;
        break;
    case UR_COLOR_DN5_DN6_LTB:
        printerTemplate.letterSpacing = 0;
        printerTemplate.lineSpacing = 0;
        printerTemplate.fgColor = TEXT_DYNAMIC_COLOR_5;
        printerTemplate.bgColor = TEXT_DYNAMIC_COLOR_6;
        printerTemplate.shadowColor = TEXT_COLOR_LIGHT_BLUE;
        break;
    }

    AddTextPrinter(&printerTemplate, 0xFF, NULL);
}

static void BlankUnkStruct_x20Array(struct UnkStruct_x20 * x20arr, u8 count)
{
    s32 i;

    for (i = 0; i < count; i++)
    {
        x20arr[i].gname_uname = sUnionGnameUnamePair_Dummy;
        x20arr[i].field_18 = 0xFF;
        x20arr[i].groupScheduledAnim = UNION_ROOM_SPAWN_NONE;
        x20arr[i].field_1A_1 = FALSE;
        x20arr[i].field_1B = 0;
    }
}

static void BlankUnkStruct_x1CArray(struct UnkStruct_x1C * x1Carr, u8 count)
{
    s32 i;

    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        x1Carr[i].gname_uname = sUnionGnameUnamePair_Dummy;
        x1Carr[i].active = FALSE;
    }
}

static bool8 AreGnameUnameDifferent(struct UnionGnameUnamePair * left, const struct UnionGnameUnamePair * right)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (left->gname.unk_00.playerTrainerId[i] != right->gname.unk_00.playerTrainerId[i])
        {
            return TRUE;
        }
    }

    for (i = 0; i < RFU_USER_NAME_LENGTH; i++)
    {
        if (left->uname[i] != right->uname[i])
        {
            return TRUE;
        }
    }

    return FALSE;
}

static bool32 AreUnionRoomPlayerGnamesDifferent(struct UnionGnameUnamePair * left, struct UnionGnameUnamePair * right)
{
    s32 i;

    if (left->gname.activity != right->gname.activity)
    {
        return TRUE;
    }

    if (left->gname.started != right->gname.started)
    {
        return TRUE;
    }

    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        if (left->gname.child_sprite_gender[i] != right->gname.child_sprite_gender[i])
        {
            return TRUE;
        }
    }

    if (left->gname.species != right->gname.species)
    {
        return TRUE;
    }

    if (left->gname.type != right->gname.type)
    {
        return TRUE;
    }

    return FALSE;
}

static u32 Findx20Inx1CArray(struct UnkStruct_x20 * x20, struct UnkStruct_x1C * x1Carr)
{
    u8 result = 0xFF;
    s32 i;

    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        if (x1Carr[i].active && !AreGnameUnameDifferent(&x20->gname_uname, &x1Carr[i].gname_uname))
        {
            result = i;
            x1Carr[i].active = FALSE;
        }
    }

    return result;
}

static u8 Appendx1Ctox20(struct UnkStruct_x20 * x20arr, struct UnkStruct_x1C * x1C, u8 count)
{
    s32 i;

    if (x1C->active)
    {
        for (i = 0; i < count; i++)
        {
            if (x20arr[i].groupScheduledAnim == UNION_ROOM_SPAWN_NONE)
            {
                x20arr[i].gname_uname = x1C->gname_uname;
                x20arr[i].field_18 = 0;
                x20arr[i].groupScheduledAnim = UNION_ROOM_SPAWN_IN;
                x20arr[i].field_1B = 0x40;
                x1C->active = FALSE;
                return i;
            }
        }
    }

    return 0xFF;
}

static void PrintUnionRoomGroupOnWindow(u8 windowId, u8 x, u8 y, struct UnkStruct_x20 * group, u8 colorIdx, u8 id)
{
    u8 activity;
    u8 id_str[6];
    u8 uname[30];

    ConvertIntToDecimalStringN(gStringVar4, id + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringAppend(gStringVar4, gUnknown_84571B0);
    UR_AddTextPrinterParameterized(windowId, 0, gStringVar4, x, y, UR_COLOR_DKE_WHT_LTE);
    x += 18;
    activity = group->gname_uname.gname.activity;
    if (group->groupScheduledAnim == UNION_ROOM_SPAWN_IN && !(activity & IN_UNION_ROOM))
    {
        IntlConvPartnerUname(uname, *group);
        UR_AddTextPrinterParameterized(windowId, 2, uname, x, y, colorIdx);
        ConvertIntToDecimalStringN(id_str, group->gname_uname.gname.unk_00.playerTrainerId[0] | (group->gname_uname.gname.unk_00.playerTrainerId[1] << 8), STR_CONV_MODE_LEADING_ZEROS, 5);
        StringCopy(gStringVar4, gUnknown_84571B4);
        StringAppend(gStringVar4, id_str);
        x += 77;
        UR_AddTextPrinterParameterized(windowId, 0, gStringVar4, x, y, colorIdx);
    }
}

static void PrintGroupMemberCandidateOnWindowWithColor(u8 windowId, u8 x, u8 y, struct UnkStruct_x20 * group, u8 colorIdx, u8 id)
{
    u8 id_str[6];
    u8 uname[30];

    if (group->groupScheduledAnim == UNION_ROOM_SPAWN_IN)
    {
        IntlConvPartnerUname(uname, *group);
        UR_AddTextPrinterParameterized(windowId, 2, uname, x, y, colorIdx);
        ConvertIntToDecimalStringN(id_str, group->gname_uname.gname.unk_00.playerTrainerId[0] | (group->gname_uname.gname.unk_00.playerTrainerId[1] << 8), STR_CONV_MODE_LEADING_ZEROS, 5);
        StringCopy(gStringVar4, gUnknown_84571B4);
        StringAppend(gStringVar4, id_str);
        x += 71;
        UR_AddTextPrinterParameterized(windowId, 0, gStringVar4, x, y, colorIdx);
    }
}

