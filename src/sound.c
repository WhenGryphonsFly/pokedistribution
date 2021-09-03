//[[!!!]]
#include "global.h"
#include "gba/m4a_internal.h"
#include "gflib.h"
#include "battle.h"
#include "quest_log.h"
#include "m4a.h"
#include "constants/songs.h"
#include "constants/fanfares.h"
#include "task.h"

struct Fanfare
{
    u16 songNum;
    u16 duration;
};

// TODO: what are these
extern u8 gDisableMapMusicChangeOnMapLoad;
extern u8 gDisableHelpSystemVolumeReduce;

// ewram
EWRAM_DATA struct MusicPlayerInfo* gMPlay_PokemonCry = NULL;
EWRAM_DATA u8 gPokemonCryBGMDuckingCounter = 0;

// iwram bss
static u16 sCurrentMapMusic;
static u16 sNextMapMusic;
static u8 sMapMusicState;
static u8 sMapMusicFadeInSpeed;
static u16 sFanfareCounter;

// iwram common
bool8 gDisableMusic;

extern u32 gBattleTypeFlags;
extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;
extern struct MusicPlayerInfo gMPlayInfo_SE3;
extern struct ToneData gCryTable[];
extern struct ToneData gCryTable2[];

static const struct Fanfare sFanfares[] = {
    [FANFARE_00]        = { MUS_LEVEL_UP,         80 },
    [FANFARE_01]        = { MUS_OBTAIN_ITEM,     160 },
    [FANFARE_02]        = { MUS_EVOLVED,         220 },
    [FANFARE_03]        = { MUS_OBTAIN_TMHM,     220 },
    [FANFARE_04]        = { MUS_HEAL,            160 },
    [FANFARE_05]        = { MUS_OBTAIN_BADGE,    340 },
    [FANFARE_06]        = { MUS_MOVE_DELETED,    180 },
    [FANFARE_07]        = { MUS_OBTAIN_BERRY,    120 },
    [FANFARE_08]        = { MUS_SLOTS_JACKPOT,   250 },
    [FANFARE_09]        = { MUS_SLOTS_WIN,       150 },
    [FANFARE_10]        = { MUS_TOO_BAD,         160 },
    [FANFARE_POKEFLUTE] = { MUS_POKE_FLUTE,      450 },
    [FANFARE_KEY_ITEM]  = { MUS_OBTAIN_KEY_ITEM, 170 },
    [FANFARE_DEX_EVAL]  = { MUS_DEX_RATING,      196 }
};

extern u16 SpeciesToCryId(u16);

static void Task_Fanfare(u8 taskId);
static void CreateFanfareTask(void);
static void Task_DuckBGMForPokemonCry(u8 taskId);
static void RestoreBGMVolumeAfterPokemonCry(void);

#define CRY_VOLUME  120 // was 125 in R/S

void InitMapMusic(void)
{
    gDisableMusic = FALSE;
    ResetMapMusic();
}

void MapMusicMain(void)
{
    switch (sMapMusicState)
    {
    case 0:
        break;
    case 1:
        sMapMusicState = 2;
        PlayBGM(sCurrentMapMusic);
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (IsBGMStopped())
        {
            sNextMapMusic = 0;
            sMapMusicState = 0;
        }
        break;
    case 6:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            PlayBGM(sCurrentMapMusic);
        }
        break;
    case 7:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            FadeInNewBGM(sNextMapMusic, sMapMusicFadeInSpeed);
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            sMapMusicFadeInSpeed = 0;
        }
        break;
    }
}

void ResetMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 0;
    sMapMusicFadeInSpeed = 0;
}

void StopMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void PlayFanfareByFanfareNum(u8 fanfareNum)
{
    u16 songNum;

    {
        m4aMPlayStop(&gMPlayInfo_BGM);
        songNum = sFanfares[fanfareNum].songNum;
        sFanfareCounter = sFanfares[fanfareNum].duration;
        m4aSongNumStart(songNum);
    }
}

void PlayFanfare(u16 songNum)
{
    s32 i;
    for (i = 0; (u32)i < 14; i++)
    {
        if (sFanfares[i].songNum == songNum)
        {
            PlayFanfareByFanfareNum(i);
            CreateFanfareTask();
            return;
        }
    }

    PlayFanfareByFanfareNum(0);
    CreateFanfareTask();
}

bool8 IsFanfareTaskInactive(void)
{
    if (FuncIsActiveTask(Task_Fanfare) == TRUE)
        return FALSE;
    return TRUE;
}

static void Task_Fanfare(u8 taskId)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
    }
    else
    {
        m4aMPlayContinue(&gMPlayInfo_BGM);
        DestroyTask(taskId);
    }
}

static void CreateFanfareTask(void)
{
    if (FuncIsActiveTask(Task_Fanfare) != TRUE)
        CreateTask(Task_Fanfare, 80);
}

void FadeInNewBGM(u16 songNum, u8 speed)
{
    if (gDisableMusic)
        songNum = 0;
    if (songNum == 0xFFFF)
        songNum = 0;
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_BGM);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0);
    m4aSongNumStop(songNum);
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

bool8 IsBGMStopped(void)
{
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

void PlayBGM(u16 songNum)
{
    if (gDisableMusic)
        songNum = 0;
    if (songNum == 0xFFFF)
        songNum = 0;
    m4aSongNumStart(songNum);
}

void PlaySE(u16 songNum)
{
    if(gDisableMapMusicChangeOnMapLoad == 0)
        m4aSongNumStart(songNum);
}

bool8 IsSEPlaying(void)
{
    if ((gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_PAUSE) && (gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_PAUSE))
        return FALSE;
    if (!(gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_TRACK) && !(gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

