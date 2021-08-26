//[[!!!]]
// decoration.c
#include "global.h"
#include "decoration.h"
#include "constants/decorations.h"
#include "data/decoration/tiles.h"
#include "data/decoration/description.h"
#include "data/decoration/header.h"

// battle_records.h
#include "battle_records.h"
void TryRecordLinkBattleOutcome(s32 battlerId){}

// specials
void AnimateElevator(void){}
void AnimatePcTurnOff(void){}
void AnimatePcTurnOn(void){}
void AnimateTeleporterCable(void){}
void AnimateTeleporterHousing(void){}
void AreLeadMonEVsMaxedOut(void){}
void BackupHelpContext(void){}
void BattleCardAction(void){}
void BedroomPC(void){}
void BrailleCursorToggle(void){}
void BufferBigGuyOrBigGirlString(void){}
void BufferEReaderTrainerName(void){}
void BufferMonNickname(void){}
void BufferMoveDeleterNicknameAndMove(void){}
void BufferSonOrDaughterString(void){}
void BufferTMHMMoveName(void){}
void CallTrainerTowerFunc(void){}
void CapeBrinkGetMoveToTeachLeadPokemon(void){}
void ChangeBoxPokemonNickname(void){}
void ChangePokemonNickname(void){}
void CheckAddCoins(void){}
void ChooseMonForMoveTutor(void){}
void ChooseMonForWirelessMinigame(void){}
void ChoosePartyMon(void){}
void ChooseSendDaycareMon(void){}
void CloseElevatorCurrentFloorWindow(void){}
void CompareHeracrossSize(void){}
void CompareMagikarpSize(void){}
void CountPartyAliveNonEggMons_IgnoreVar0x8004Slot(void){}
void CountPartyNonEggMons(void){}
void CreateInGameTradePokemon(void){}
void DaisyMassageServices(void){}
void DaycareMonReceivedMail(void){}
void DisplayBerryPowderVendorMenu(void){}
void DisplayMoveTutorMenu(void){}
void DoCableClubWarp(void){}
void DoCredits(void){}
void DoDeoxysTriangleInteraction(void){}
void DoDiveWarp(void){}
void DoesPlayerPartyContainSpecies(void){}
void DoFallWarp(void){}
void DoInGameTradeScene(void){}
void DoPicboxCancel(void){}
void DoPokemonLeagueLightingEffect(void){}
void DoSeagallopFerryScene(void){}
void DoSSAnneDepartureCutscene(void){}
void DoWateringBerryTreeAnim(void){}
void DrawElevatorCurrentFloorWindow(void){}
void DrawWholeMapView(void){}
void EggHatch(void){}
void EnterHallOfFame(void){}
void EnterSafariMode(void){}
void ExitSafariMode(void){}
void ForcePlayerOntoBike(void){}
void ForcePlayerToStartSurfing(void){}
void GetBattleOutcome(void){}
void GetCostToWithdrawRoute5DaycareMon(void){}
void GetDaycareCost(void){}
void GetDaycareMonNicknames(void){}
void GetDaycarePokemonCount(void){}
void GetDaycareState(void){}
void GetElevatorFloor(void){}
void GetHeracrossSizeRecordInfo(void){}
void GetInGameTradeSpeciesInfo(void){}
void GetLeadMonFriendship(void){}
void GetMagikarpSizeRecordInfo(void){}
void GetMartClerkObjectId(void){}
void GetNumLevelsGainedForRoute5DaycareMon(void){}
void GetNumLevelsGainedFromDaycare(void){}
void GetNumMovesSelectedMonHas(void){}
void GetPartyMonSpecies(void){}
void GetPCBoxToSendMon(void){}
void GetPlayerAvatarBike(void){}
void GetPlayerTrainerIdOnesDigit(void){}
void GetPlayerXY(void){}
void GetPokedexCount(void){}
void GetProfOaksRatingMessage(void){}
void GetQuestLogState(void){}
void GetRandomSlotMachineId(void){}
void GetSeagallopNumber(void){}
void GetSelectedMonNicknameAndSpecies(void){}
void GetStarterSpecies(void){}
void GetTradeSpecies(void){}
void GetTrainerBattleMode(void){}
void GiveEggFromDaycare(void){}
void GiveLeadMonEffortRibbon(void){}
void HallOfFamePCBeginFade(void){}
void HasLeadMonBeenRenamed(void){}
void HasLearnedAllMovesFromCapeBrinkTutor(void){}
void HelpSystem_Disable(void){}
void HelpSystem_Enable(void){}
void InitElevatorFloorSelectMenuPos(void){}
void InitRoamer(void){}
void IsBadEggInParty(void){}
void IsDodrioInParty(void){}
void IsMonOTIDNotPlayers(void){}
void IsMonOTNameNotPlayers(void){}
void IsPlayerLeftOfVermilionSailor(void){}
void IsPlayerNotInTrainerTowerLobby(void){}
void IsPokemonJumpSpeciesInParty(void){}
void IsPokerusInParty(void){}
void IsSelectedMonEgg(void){}
void IsStarterFirstStageInParty(void){}
void IsThereMonInRoute5Daycare(void){}
void IsThereRoomInAnyBoxForMorePokemon(void){}
void IsTrainerReadyForRematch(void){}
void LeadMonHasEffortRibbon(void){}
void ListMenu(void){}
void LookThroughPorthole(void){}
void LoopWingFlapSound(void){}
void MoveDeleterForgetMove(void){}
void NameRaterWasNicknameChanged(void){}
void NullFieldSpecial(void){}
void PlayerHasGrassPokemonInParty(void){}
void PlayerPartyContainsSpeciesWithPlayerID(void){}
void PlayerPC(void){}
void PlayTrainerEncounterMusic(void){}
void PrintPlayerBerryPowderAmount(void){}
void PutMonInRoute5Daycare(void){}
void QuestLog_CutRecording(void){}
void QuestLog_StartRecordingInputsAfterDeferredEvent(void){}
void RejectEggFromDayCare(void){}
void RemoveBerryPowderVendorMenu(void){}
void RemoveCameraObject(void){}
void RestoreHelpContext(void){}
void ReturnFromLinkRoom(void){}
void ReturnToListMenu(void){}
void SampleResortGorgeousMonAndReward(void){}
void Script_BufferFanClubTrainerName(void){}
void Script_ClearHeldMovement(void){}
void Script_FacePlayer(void){}
void Script_FadeOutMapMusic(void){}
void Script_GetNumFansOfPlayerInTrainerFanClub(void){}
void Script_HasEnoughBerryPowder(void){}
void Script_HasTrainerBeenFought(void){}
void ScriptHatchMon(void){}
void Script_IsFanClubMemberFanOfPlayer(void){}
void Script_SetHelpContext(void){}
void Script_SetPlayerGotFirstFans(void){}
void Script_TakeBerryPowder(void){}
void Script_TryGainNewFanFromCounter(void){}
void Script_TryLoseFansFromPlayTimeAfterLinkBattle(void){}
void Script_TryLoseFansFromPlayTime(void){}
void Script_UpdateTrainerFanClubGameClear(void){}
void SeafoamIslandsB4F_CurrentDumpsPlayerOnLand(void){}
void SelectMoveDeleterMove(void){}
void SelectMoveTutorMon(void){}
void SetBattledTrainerFlag(void){}
void SetCableClubWarp(void){}
void SetCB2WhiteOut(void){}
void SetDaycareCompatibilityString(void){}
void SetDeoxysTrianglePalette(void){}
void SetFlavorTextFlagFromSpecialVars(void){}
void SetHelpContextForMap(void){}
void SetHiddenItemFlag(void){}
void SetIcefallCaveCrackedIceMetatiles(void){}
void SetSeenMon(void){}
void SetUpTrainerMovement(void){}
void SetUsedPkmnCenterQuestLogEvent(void){}
void SetVermilionTrashCans(void){}
void ShakeScreen(void){}
void ShouldShowBoxWasFullMessage(void){}
void ShouldTryRematchBattle(void){}
void ShowBattleRecords(void){}
void ShowBerryCrushRankings(void){}
void ShowDaycareLevelMenu(void){}
void ShowDiploma(void){}
void ShowDodrioBerryPickingRecords(void){}
void ShowFieldMessageStringVar4(void){}
void ShowPokemonJumpRecords(void){}
void ShowPokemonStorageSystemPC(void){}
void ShowTownMap(void){}
void ShowTrainerCantBattleSpeech(void){}
void ShowTrainerIntroSpeech(void){}
void ShowWirelessCommunicationScreen(void){}
void SpawnCameraObject(void){}
void StartDroughtWeatherBlend(void){}
void StartGroudonKyogreBattle(void){}
void StartLegendaryBattle(void){}
void StartMarowakBattle(void){}
void StartOldManTutorialBattle(void){}
void StartRegiBattle(void){}
void StartRematchBattle(void){}
void StartSouthernIslandBattle(void){}
void StickerManGetBragFlags(void){}
void StoreSelectedPokemonInDaycare(void){}
void TakePokemonFromDaycare(void){}
void TakePokemonFromRoute5Daycare(void){}
void TryFieldPoisonWhiteOut(void){}
void UpdateLoreleiDollCollection(void){}
void UpdatePickStateFromSpecialVar8005(void){}
void UpdateTrainerCardPhotoIcons(void){}
void VsSeekerFreezeObjectsAfterChargeComplete(void){}
void VsSeekerResetObjectMovementAfterChargeComplete(void){}
void AwardBattleTowerRibbons(void){}
void BattleTowerMapScript2(void){}
void BattleTower_SoftReset(void){}
void BattleTowerUtil(void){}
void BufferEReaderTrainerGreeting(void){}
void BufferRandomHobbyOrLifestyleString(void){}
void BufferUnionRoomPlayerName(void){}
void CableClub_AskSaveTheGame(void){}
void CalculatePlayerPartyCount(void){}
void CheckPartyBattleTowerBanlist(void){}
void ChooseBattleTowerPlayerParty(void){}
void ChooseHalfPartyForBattle(void){}
void ChooseNextBattleTowerTrainer(void){}
void CleanupLinkRoomState(void){}
void CloseMuseumFossilPic(void){}
void CreateEventLegalEnemyMon(void){}
void CreatePCMenu(void){}
void DetermineBattleTowerPrize(void){}
void DoesPartyHaveEnigmaBerry(void){}
void DrawSeagallopDestinationMenu(void){}
void Dummy_TryEnableBravoTrainerBattleTower(void){}
void EndTrainerApproach(void){}
void EnterColosseumPlayerSpot(void){}
void EnterTradeSeat(void){}
void ExitLinkRoom(void){}
void Field_AskSaveTheGame(void){}
void GetPlayerFacingDirection(void){}
void GetSelectedSeagallopDestination(void){}
void GiveBattleTowerPrize(void){}
void HasAtLeastOneBerry(void){}
void HasEnoughMonsForDoubleBattle(void){}
void HealPlayerParty(void){}
void InitUnionRoom(void){}
void OpenMuseumFossilPic(void){}
void Overworld_PlaySpecialMapMusic(void){}
void OverworldWhiteOutGetMoneyLoss(void){}
void PrintBattleTowerTrainerGreeting(void){}
void ReducePlayerPartyToThree(void){}
void RegisteredItemHandleBikeSwap(void){}
void RockSmashWildEncounter(void){}
void SaveBattleTowerProgress(void){}
void Script_ResetUnionRoomTrade(void){}
void Script_ShowLinkTrainerCard(void){}
void SetBattleTowerParty(void){}
void SetBattleTowerProperty(void){}
void SetEReaderTrainerGfxId(void){}
void ShowEasyChatMessage(void){}
void ShowEasyChatScreen(void){}
void StartSpecialBattle(void){}
void StartWiredCableClubTrade(void){}
void TryBattleLinkup(void){}
void TryBecomeLinkLeader(void){}
void TryContestLinkup(void){}
void TryJoinLinkGroup(void){}
void TryRecordMixLinkup(void){}
void TryTradeLinkup(void){}
void UnionRoomSpecial(void){}



// item use
#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "berry_pouch.h"
#include "berry_powder.h"
#include "bike.h"
#include "coins.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "item.h"
#include "item_menu.h"
#include "item_use.h"
#include "itemfinder.h"
#include "mail.h"
#include "event_object_lock.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "quest_log.h"
#include "region_map.h"
#include "script.h"
#include "strings.h"
#include "task.h"
#include "teachy_tv.h"
#include "tm_case.h"
#include "vs_seeker.h"
#include "constants/fanfares.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/field_weather.h"

void BattleUseFunc_BerryPouch(u8 taskId){}
void BattleUseFunc_Ether(u8 taskId){}
void BattleUseFunc_GuardSpec(u8 taskId){}
void BattleUseFunc_Medicine(u8 taskId){}
void BattleUseFunc_PokeBallEtc(u8 taskId){}
void BattleUseFunc_PokeDoll(u8 taskId){}
void BattleUseFunc_PokeFlute(u8 taskId){}
void FieldUseFunc_BerryPouch(u8 taskId){}
void FieldUseFunc_BlackFlute(u8 taskId){}
void FieldUseFunc_CoinCase(u8 taskId){}
void FieldUseFunc_Ether(u8 taskId){}
void FieldUseFunc_EvoItem(u8 taskId){}
void FieldUseFunc_FameChecker(u8 taskId){}
void FieldUseFunc_MachBike(u8 taskId){}
void FieldUseFunc_Medicine(u8 taskId){}
void FieldUseFunc_OakStopsYou(u8 taskId){}
void FieldUseFunc_OldRod(u8 taskId){}
void FieldUseFunc_OrangeMail(u8 taskId){}
void FieldUseFunc_PokeFlute(u8 taskId){}
void FieldUseFunc_PowderJar(u8 taskId){}
void FieldUseFunc_PpUp(u8 taskId){}
void FieldUseFunc_RareCandy(u8 taskId){}
void FieldUseFunc_SacredAsh(u8 taskId){}
void FieldUseFunc_SuperRepel(u8 taskId){}
void FieldUseFunc_TeachyTv(u8 taskId){}
void FieldUseFunc_TmCase(u8 taskId){}
void FieldUseFunc_TownMap(u8 taskId){}
void FieldUseFunc_VsSeeker(u8 taskId){}
void ItemUseInBattle_EnigmaBerry(u8 taskId){}
void ItemUseOutOfBattle_EnigmaBerry(u8 taskId){}
void ItemUseOutOfBattle_EscapeRope(u8 taskId){}
void ItemUseOutOfBattle_Itemfinder(u8 taskId){}

// Originally in scrcmd.c
#include "gba/gba.h"
#include <string.h>
#include "constants/global.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/species.h"
void *const gNullScriptPtr = NULL;
