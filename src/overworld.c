//[[!!!]]
#include "global.h"
#include "gflib.h"
#include "bg_regs.h"
#include "cable_club.h"
#include "credits.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_camera.h"
#include "field_control_avatar.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_specials.h"
#include "field_tasks.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "heal_location.h"
#include "help_system.h"
#include "link.h"
#include "link_rfu.h"
#include "load_save.h"
#include "m4a.h"
#include "map_name_popup.h"
#include "map_preview_screen.h"
#include "metatile_behavior.h"
#include "money.h"
#include "new_game.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "play_time.h"
#include "quest_log.h"
#include "quest_log_objects.h"
#include "random.h"
#include "renewable_hidden_items.h"
#include "roamer.h"
#include "safari_zone.h"
#include "save_location.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "start_menu.h"
#include "tileset_anims.h"
#include "trainer_pokemon_sprites.h"
#include "vs_seeker.h"
#include "wild_encounter.h"
#include "constants/maps.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"

#define PLAYER_TRADING_STATE_IDLE 0x80
#define PLAYER_TRADING_STATE_BUSY 0x81
#define PLAYER_TRADING_STATE_UNK_2 0x82
#define PLAYER_TRADING_STATE_EXITING_ROOM 0x83

#define FACING_NONE 0
#define FACING_UP 1
#define FACING_DOWN 2
#define FACING_LEFT 3
#define FACING_RIGHT 4
#define FACING_FORCED_UP 7
#define FACING_FORCED_DOWN 8
#define FACING_FORCED_LEFT 9
#define FACING_FORCED_RIGHT 10

typedef u16 (*KeyInterCB)(u32 key);

struct InitialPlayerAvatarState
{
    u8 transitionFlags;
    u8 direction;
    bool8 unk2;
};

struct TradeRoomPlayer
{
    u8 playerId;
    bool8 isLocalPlayer;
    u8 c;
    u8 facing;
    struct MapPosition pos;
    u16 field_C;
};

EWRAM_DATA struct WarpData gLastUsedWarp = {};
static EWRAM_DATA struct WarpData sWarpDestination = {};
static EWRAM_DATA struct WarpData sFixedDiveWarp = {};
static EWRAM_DATA struct WarpData sFixedHoleWarp = {};

// File boundary perhaps?
static EWRAM_DATA struct InitialPlayerAvatarState sInitialPlayerAvatarState = {};

// File boundary perhaps?
EWRAM_DATA bool8 gDisableMapMusicChangeOnMapLoad = FALSE;
static EWRAM_DATA u16 sAmbientCrySpecies = SPECIES_NONE;
static EWRAM_DATA bool8 sIsAmbientCryWaterMon = FALSE;

// File boundary perhaps?
ALIGNED(4) EWRAM_DATA bool8 gUnknown_2031DE0 = FALSE;
static EWRAM_DATA const struct CreditsOverworldCmd *sCreditsOverworld_Script = NULL;
static EWRAM_DATA s16 sCreditsOverworld_CmdLength = 0;
static EWRAM_DATA s16 sCreditsOverworld_CmdIndex = 0;

// File boundary perhaps?
EWRAM_DATA struct LinkPlayerObjectEvent gLinkPlayerObjectEvents[4] = {};

u16 *gBGTilemapBuffers1;
u16 *gBGTilemapBuffers2;
u16 *gBGTilemapBuffers3;
void (*gFieldCallback)(void);
bool8 (*gFieldCallback2)(void);
u16 gHeldKeyCodeToSend;
u8 gLocalLinkPlayerId;
u8 gFieldLinkPlayerCount;

static u8 sPlayerTradingStates[4];
static KeyInterCB sPlayerKeyInterceptCallback;
static bool8 sReceivingFromLink;
static u8 sRfuKeepAliveTimer;

static u8 CountBadgesForOverworldWhiteOutLossCalculation(void);
static void Overworld_ResetStateAfterWhitingOut(void);
static void Overworld_SetWhiteoutRespawnPoint(void);
static u8 GetAdjustedInitialTransitionFlags(struct InitialPlayerAvatarState *playerStruct, u16 metatileBehavior, u8 mapType);
static u8 GetAdjustedInitialDirection(struct InitialPlayerAvatarState *playerStruct, u8 transitionFlags, u16 metatileBehavior, u8 mapType);
static u16 GetCenterScreenMetatileBehavior(void);
static void SetDefaultFlashLevel(void);
static void Overworld_TryMapConnectionMusicTransition(void);
static void ChooseAmbientCrySpecies(void);

static void CB2_Overworld(void);
static void CB2_LoadMap2(void);
static void c2_80567AC(void);
static void CB2_ReturnToFieldLocal(void);
static void CB2_ReturnToFieldLink(void);
static void FieldClearVBlankHBlankCallbacks(void);
static void SetFieldVBlankCallback(void);
static void VBlankCB_Field(void);

static bool32 map_loading_iteration_3(u8 *state);
static bool32 sub_8056CD8(u8 *state);
static bool32 map_loading_iteration_2_link(u8 *state);
static void do_load_map_stuff_loop(u8 *state);
static void MoveSaveBlocks_ResetHeap_(void);
static void sub_8056E80(void);
static void sub_8056F08(void);
static void InitOverworldGraphicsRegisters(void);
static void sub_8057024(bool32 a0);
static void sub_8057074(void);
static void mli4_mapscripts_and_other(void);
static void ReloadObjectsAndRunReturnToFieldMapScript(void);
static void sub_8057114(void);
static void SetCameraToTrackGuestPlayer(void);
static void SetCameraToTrackGuestPlayer_2(void);
static void sub_8057178(void);
static void sub_80571A8(void);
static void CreateLinkPlayerSprites(void);
static void CB2_LoadMapForQLPlayback(void);
static void DoLoadMap_QLPlayback(u8 *state);
static bool32 LoadMap_QLPlayback(u8 *state);
static bool32 SetUpScrollSceneForCredits(u8 *state, u8 unused);
static bool8 MapLdr_Credits(void);
static void CameraCB_CreditsPan(struct CameraObject * camera);
static void Task_OvwldCredits_FadeOut(u8 taskId);
static void Task_OvwldCredits_WaitFade(u8 taskId);

static void CB1_UpdateLinkState(void);
static void ResetAllMultiplayerState(void);
static void ClearAllPlayerKeys(void);
static void SetKeyInterceptCallback(KeyInterCB callback);
static void ResetAllTradingStates(void);
static void UpdateAllLinkPlayers(u16 *linkKeys, s32 selfId);
static void UpdateHeldKeyCode(u16 interceptedKeys);
static u32 GetLinkSendQueueLength(void);
static u16 GetDirectionForDpadKey(u16 key);
static void SetPlayerFacingDirection(u8 linkPlayerId, u8 setFacing);
static void ResetPlayerHeldKeys(u16 *linkKeys);
static u16 KeyInterCB_SelfIdle(u32 linkPlayerId);
static u16 KeyInterCB_DeferToEventScript(u32 linkPlayerId);
static u16 KeyInterCB_DeferToRecvQueue(u32 linkPlayerId);
static u16 KeyInterCB_DeferToSendQueue(u32 linkPlayerId);
static void LoadTradeRoomPlayer(s32 i, s32 selfId, struct TradeRoomPlayer * trainer);
static bool32 PlayerIsAtSouthExit(struct TradeRoomPlayer * player);
static const u8 *TryGetTileEventScript(struct TradeRoomPlayer * player);
static const u8 *TryInteractWithPlayer(struct TradeRoomPlayer * player);
static bool32 sub_8057FEC(struct TradeRoomPlayer * player);
static bool32 sub_8058004(struct TradeRoomPlayer * player);
static u16 GetDirectionForEventScript(const u8 *script);
static void sub_80581BC(void);
static void CreateConfirmLeaveTradeRoomPrompt(void);
static void InitLinkRoomStartMenuScript(void);
static void InitMenuBasedScript(const u8 *script);
static void sub_80581DC(const u8 *script);
static void RunTerminateLinkScript(void);
static void SpawnLinkPlayerObjectEvent(u8 i, s16 x, s16 y, u8 gender);
static void InitLinkPlayerObjectEventPos(struct ObjectEvent *objEvent, s16 x, s16 y);
static u8 GetSpriteForLinkedPlayer(u8 linkPlayerId);
static void GetLinkPlayerCoords(u8 linkPlayerId, u16 *x, u16 *y);
static u8 GetLinkPlayerFacingDirection(u8 linkPlayerId);
static u8 GetLinkPlayerElevation(u8 linkPlayerId);
static u8 GetLinkPlayerIdAt(s16 x, s16 y);
static void CreateLinkPlayerSprite(u8 i, u8 version);
static u8 MovementEventModeCB_Normal(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static u8 MovementEventModeCB_Ignored(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static u8 MovementEventModeCB_Normal_2(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static u8 FacingHandler_DoNothing(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static u8 FacingHandler_DpadMovement(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static u8 FacingHandler_ForcedFacingChange(struct LinkPlayerObjectEvent *, struct ObjectEvent *, u8);
static void MovementStatusHandler_EnterFreeMode(struct LinkPlayerObjectEvent *, struct ObjectEvent *);
static void MovementStatusHandler_TryAdvanceScript(struct LinkPlayerObjectEvent *, struct ObjectEvent *);
static u8 FlipVerticalAndClearForced(u8 newFacing, u8 oldFacing);
static u8 LinkPlayerDetectCollision(u8 selfObjEventId, u8 a2, s16 x, s16 y);
static void SpriteCB_LinkPlayer(struct Sprite * sprite);

extern const struct MapLayout * gMapLayouts[];
extern const struct MapHeader *const *gMapGroups[];

// Routines related to game state on warping in

static const u8 sWhiteOutMoneyLossMultipliers[] = {
     2,
     4,
     6,
     9,
    12,
    16,
    20,
    25,
    30
};

static const u16 sWhiteOutMoneyLossBadgeFlagIDs[] = {
    FLAG_BADGE01_GET,
    FLAG_BADGE02_GET,
    FLAG_BADGE03_GET,
    FLAG_BADGE04_GET,
    FLAG_BADGE05_GET,
    FLAG_BADGE06_GET,
    FLAG_BADGE07_GET,
    FLAG_BADGE08_GET
};

void IncrementGameStat(u8 statId)
{
    u32 statVal;
    if (statId >= NUM_USED_GAME_STATS)
        return;
    statVal = GetGameStat(statId);
    if (statVal < 0xFFFFFF)
        statVal++;
    else
        statVal = 0xFFFFFF;
    SetGameStat(statId, statVal);
}

u32 GetGameStat(u8 statId)
{
	return 0;
}

void SetGameStat(u8 statId, u32 statVal)
{
	return;
}

bool32 IsUpdateLinkStateCBActive(void)
{
    return FALSE;
}

static void SetKeyInterceptCallback(KeyInterCB func)
{
    sRfuKeepAliveTimer = 0;
    sPlayerKeyInterceptCallback = func;
}

// Once every ~60 frames, if the link state hasn't changed (timer reset by calls
// to SetKeyInterceptCallback), it does a bunch of sanity checks on the connection.
// I'm not sure if sRfuKeepAliveTimer is reset in the process, though; rfu stuff is
// still undocumented.
static void CheckRfuKeepAliveTimer(void)
{
    if (gWirelessCommType != 0 && ++sRfuKeepAliveTimer > 60)
        LinkRfu_FatalError();
}

static u16 sub_8057D98(u32 key)
{
    CheckRfuKeepAliveTimer();
    return LINK_KEY_CODE_EMPTY;
}

// Ignore the player's inputs as long as there is an event script
// in ScriptContext2.
static u16 KeyInterCB_DeferToEventScript(u32 key)
{
    u16 retVal;
    if (ScriptContext2_IsEnabled() == TRUE)
    {
        retVal = LINK_KEY_CODE_EMPTY;
    }
    else
    {
        retVal = LINK_KEY_CODE_UNK_4;
        SetKeyInterceptCallback(sub_8057D98);
    }
    return retVal;
}

// Ignore the player's inputs as long as there are events being recived.
static u16 KeyInterCB_DeferToRecvQueue(u32 key)
{
    u16 retVal;
    if (GetLinkRecvQueueLength() > 2)
    {
        retVal = LINK_KEY_CODE_EMPTY;
    }
    else
    {
        retVal = LINK_KEY_CODE_UNK_4;
        ScriptContext2_Disable();
        SetKeyInterceptCallback(sub_8057D98);
    }
    return retVal;
}

// Ignore the player's inputs as long as there are events being sent.
static u16 KeyInterCB_DeferToSendQueue(u32 key)
{
    u16 retVal;
    if (GetLinkSendQueueLength() > 2)
    {
        retVal = LINK_KEY_CODE_EMPTY;
    }
    else
    {
        retVal = LINK_KEY_CODE_UNK_4;
        ScriptContext2_Disable();
        SetKeyInterceptCallback(sub_8057D98);
    }
    return retVal;
}

bool32 Overworld_RecvKeysFromLinkIsRunning(void)
{
    u8 temp;

    if (GetLinkRecvQueueLength() < 2)
        return FALSE;
    else if (IsUpdateLinkStateCBActive() != TRUE)
        return FALSE;
    else if (IsSendingKeysToLink() != TRUE)
        return FALSE;
    else if (sPlayerKeyInterceptCallback == KeyInterCB_DeferToRecvQueue)
        return TRUE;
    else if (sPlayerKeyInterceptCallback != KeyInterCB_DeferToEventScript)
        return FALSE;

    temp = sReceivingFromLink;
    sReceivingFromLink = FALSE;

    if (temp == TRUE)
        return TRUE;
    else if (gPaletteFade.active && gPaletteFade.softwareFadeFinishing)
        return TRUE;
    else
        return FALSE;
}

bool32 Overworld_SendKeysToLinkIsRunning(void)
{
    if (GetLinkSendQueueLength() < 2)
        return FALSE;
    else if (IsUpdateLinkStateCBActive() != TRUE)
        return FALSE;
    else if (IsSendingKeysToLink() != TRUE)
        return FALSE;
    else if (sPlayerKeyInterceptCallback == KeyInterCB_DeferToSendQueue)
        return TRUE;
    else
        return FALSE;
}

bool32 IsSendingKeysOverCable(void)
{
    if (gWirelessCommType != 0)
        return FALSE;
    else if (!IsSendingKeysToLink())
        return FALSE;
    else
        return TRUE;
}

static u32 GetLinkSendQueueLength(void)
{
    if (gWirelessCommType != 0)
        return Rfu.sendQueue.count;
    else
        return gLink.sendQueue.count;
}

