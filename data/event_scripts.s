#include "constants/global.h"
#include "constants/flags.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/decorations.h"
#include "constants/items.h"
#include "constants/layouts.h"
#include "constants/maps.h"
#include "constants/metatile_labels.h"
#include "constants/pokemon.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/trainer_classes.h"
#include "constants/vars.h"
#include "constants/battle.h"
#include "constants/heal_locations.h"
#include "constants/field_effects.h"
#include "constants/trainers.h"
#include "constants/trainer_tower.h"
#include "constants/fame_checker.h"
#include "constants/seagallop.h"
#include "constants/game_stat.h"
#include "constants/coins.h"
#include "constants/menu.h"
#include "constants/battle_setup.h"
#include "constants/map_scripts.h"
#include "constants/cable_club.h"
#include "constants/field_tasks.h"
#include "constants/field_weather.h"
#include "constants/weather.h"
#include "constants/union_room.h"
#include "constants/trade.h"
#include "constants/quest_log.h"
#include "constants/daycare.h"
#include "constants/easy_chat.h"
#include "constants/trainer_card.h"
#include "constants/help_system.h"
	.include "asm/macros.inc"
	.include "asm/macros/event.inc"
	.set FALSE, 0
	.set TRUE,  1

	.section script_data, "aw", %progbits

	.include "data/script_cmd_table.inc"

	.align 2
gSpecialVars:: @ 815FD0C
	.4byte gSpecialVar_0x8000
	.4byte gSpecialVar_0x8001
	.4byte gSpecialVar_0x8002
	.4byte gSpecialVar_0x8003
	.4byte gSpecialVar_0x8004
	.4byte gSpecialVar_0x8005
	.4byte gSpecialVar_0x8006
	.4byte gSpecialVar_0x8007
	.4byte gSpecialVar_0x8008
	.4byte gSpecialVar_0x8009
	.4byte gSpecialVar_0x800A
	.4byte gSpecialVar_0x800B
	.4byte gSpecialVar_Facing
	.4byte gSpecialVar_Result
	.4byte gSpecialVar_ItemId
	.4byte gSpecialVar_LastTalked
	.4byte gSpecialVar_MonBoxId
	.4byte gSpecialVar_MonBoxPos
	.4byte gSpecialVar_TextColor
	.4byte gSpecialVar_PrevTextColor
	.4byte gSpecialVar_0x8014

	.include "data/specials.inc"

	.align 2
gStdScripts:: @ 8160450
	.4byte Std_ObtainItem
	.4byte Std_FindItem
	.4byte Std_MsgboxNPC
	.4byte Std_MsgboxSign
	.4byte Std_MsgboxDefault
	.4byte Std_MsgboxYesNo
	.4byte Std_MsgboxAutoclose
	.4byte Std_ObtainDecoration
	.4byte Std_PutItemAway
	.4byte Std_ReceivedItem
gStdScriptsEnd::

	.include "data/scripts/std_msgbox.inc"
	.include "data/scripts/trainer_battle.inc"

@ Unused
Text_WouldYouLikeToMixRecords:: @ 81A5028
	.string "Would you like to mix records with\n"
	.string "other TRAINERS?$"

@ Unused
Text_WeHopeToSeeYouAgain2:: @ 81A505B
	.string "We hope to see you again!$"

	.include "data/text/pc.inc"

@ Unused
Text_WelcomeTradeCenter:: @ 81A5103
	.string "Welcome to the POKéMON CABLE CLUB\n"
	.string "TRADE CENTER.$"

@ Unused
Text_WelcomeColosseum:: @ 81A5133
	.string "Welcome to the POKéMON CABLE CLUB\n"
	.string "COLOSSEUM.$"

@ Unused
Text_WelcomeTimeCapsule:: @ 81A5160
	.string "Welcome to the POKéMON CABLE CLUB\n"
	.string "TIME CAPSULE.$"

Text_PleaseComeAgain:: @ 81A5190
	.string "Please come again!$"

@ Unused
Text_HavingDiscountSaleToday:: @ 81A51A3
	.string "Welcome!\p"
	.string "We're having a discount sale today!$"

@ Unused
Text_PlayerWhatCanIDoForYou:: @ 81A51D0
	.string "{PLAYER}{KUN}, welcome!\p"
	.string "What can I do for you?$"

	.include "data/text/obtain_item.inc"

Text_MakingPreparations:: @ 81A5351
	.string "じゅんびちゅうです！$"

Text_WantWhichFloor:: @ 81A535C
	.string "Which floor do you want?$"

Text_BagItemCanBeRegistered:: @ 81A5375
	.string "An item in the BAG can be\n"
	.string "registered to SELECT for easy use.$"

@ Unused (email from R/S Rivals computer)
Text_TrainerSchoolEmail:: @ 81A53B2
	.string "パソコンに\n"
	.string "ポケモン　トレーナー　こうざの\l"
	.string "メールが　きている！\p"
	.string "‥‥　‥‥　‥‥\p"
	.string "ポケモンが　おぼえられる　わざは　4つ！\p"
	.string "どんな　わざを　おぼえさせるかで\n"
	.string "トレーナーの　じつりょくが　とわれます！\p"
	.string "‥‥　‥‥　‥‥$"

Text_PlayerBootedUpPC:: @ 81A5420
	.string "{PLAYER} booted up the PC.$"

@ Unused
Text_LinkWasCanceled:: @ 81A5435
	.string "つうしんは　キャンセルされました$"

@ Unused
Text_GiveNicknameToReceivedMon:: @ 81A5446
	.string "Want to give a nickname to the\n"
	.string "{STR_VAR_2} you received?$"

gText_PkmnFainted3:: @ 81A5476
	.string "{STR_VAR_1} fainted…\p"
	.string "$"

Text_WelcomeWantToHealPkmn:: @ 81A5483
	.string "Welcome to our POKéMON CENTER!\p"
	.string "Would you like me to heal your\n"
	.string "POKéMON back to perfect health?$"

Text_TakeYourPkmnForFewSeconds:: @ 81A54E1
	.string "Okay, I'll take your POKéMON for a\n"
	.string "few seconds.$"

Text_WeHopeToSeeYouAgain:: @ 81A5511
	.string "We hope to see you again!$"

Text_RestoredPkmnToFullHealth:: @ 81A552B
	.string "Thank you for waiting.\n"
	.string "We've restored your POKéMON to\l"
	.string "full health.$"

	.include "data/text/surf.inc"

@ Unused, from R/S
Text_DoorOpenedFarAway:: @ 81A55EA
	.string "どこか　とおくの　とびらが\n"
	.string "ひらいたような　おとだ‥‥$"

@ Unused, from R/S
Text_BigHoleInTheWall:: @ 81A5606
	.string "かべに\n"
	.string "おおきな　あなが　あいている！$"

Text_WirelessClubUndergoingAdjustments:: @ 81A561A
	.string "I'm terribly sorry.\n"
	.string "The POKéMON WIRELESS CLUB is\l"
	.string "undergoing adjustments now.$"

Text_AppearsToBeUndergoingAdjustments:: @ 81A5667
	.string "It appears to be undergoing\n"
	.string "adjustments…$"

Text_HandedOverItem:: @ 81A5690
	.string "{PLAYER} handed over the\n"
	.string "{STR_VAR_1}.$"

Text_GiveNicknameToThisMon:: @ 81A56A7
	.string "Do you want to give a nickname to\n"
	.string "this {STR_VAR_1}?$"

	.include "data/text/itemfinder.inc"
	.include "data/text/route23.inc"
	.include "data/text/aide.inc"
	.include "data/text/ingame_trade.inc"

Text_CardKeyOpenedDoor:: @ 81A5B88
	.string "Bingo!\n"
	.string "The CARD KEY opened the door!$"

Text_ItNeedsCardKey:: @ 81A5BAD
	.string "No!\n"
	.string "It needs a CARD KEY!$"

Text_AccessedProfOaksPC:: @ 81A5BC6
	.string "Accessed PROF. OAK's PC…\p"
	.string "Accessed the POKéDEX Rating\n"
	.string "System…$"

Text_HavePokedexRated:: @ 81A5C03
	.string "Would you like to have your\n"
	.string "POKéDEX rated?$"

Text_ClosedLinkToProfOaksPC:: @ 81A5C2E
	.string "Closed link to PROF. OAK's PC.$"

Text_VoiceRangOutDontRunAway:: @ 81A5C4D
	.string "Someone's voice rang out,\n"
	.string "“Don't run away!”$"

@ Uncear what the below unused JP texts were for
Text_IdLikeToSeeRequest:: @ 81A5C79
	.string "えっとー\n"
	.string "{STR_VAR_2}が　みてみたいなー\p"
	.string "それも　{STR_VAR_1}{STR_VAR_3}\n"
	.string "{STR_VAR_2}が　みてみたいなー$"

Text_ThankYouForShowingMe:: @ 81A5C9F
	.string "わぁー　すごい！\n"
	.string "{STR_VAR_1}{STR_VAR_3}{STR_VAR_2}だー！\p"
	.string "ありがとー\n"
	.string "また　よろしくねー！$"

Text_ThatsNotRight:: @ 81A5CC3
	.string "それっ　ちがうよー！$"

Text_ISee:: @ 81A5CCE
	.string "そっか‥$"

Text_TheDoorIsClosed:: @ 81A5CD3
	.string "ドアは　しまっている‥$"

Text_TheDoorIsOpen:: @ 81A5CDF
	.string "The door is open…$"

	.include "data/text/pc_transfer.inc"
	.include "data/text/poke_mart.inc"

Text_MonFlewAway:: @ 81A63C4
	.string "The {STR_VAR_1} flew away!$"

Text_TheresBedLetsRest:: @ 81A63D6
	.string "ベッドが　ある‥‥\n"
	.string "やすんでいこう$"

Text_FoundTMHMContainsMove:: @ 81A63E8
	.string "{PLAYER} found a {STR_VAR_2}!\n"
	.string "It contains {STR_VAR_1}.$"

	.include "data/text/seagallop.inc"

@ Call for legendary bird trio
Text_Gyaoo:: @ 81A6448
	.string "Gyaoo!$"

Text_MoveCanOnlyBeLearnedOnce:: @ 81A644F
	.string "This move can be learned only\n"
	.string "once. Is that okay?$"

EventScript_ResetAllMapFlags:: @ 81A6481
	setflag FLAG_HIDE_OAK_IN_HIS_LAB
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	setflag FLAG_HIDE_BILL_HUMAN_SEA_COTTAGE
	setflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	setflag FLAG_HIDE_POKEHOUSE_FUJI
	setflag FLAG_HIDE_LIFT_KEY
	setflag FLAG_HIDE_SILPH_SCOPE
	setflag FLAG_HIDE_CERULEAN_RIVAL
	setflag FLAG_HIDE_SS_ANNE_RIVAL
	setflag FLAG_HIDE_VERMILION_CITY_OAKS_AIDE
	setflag FLAG_HIDE_SAFFRON_CIVILIANS
	setflag FLAG_HIDE_ROUTE_22_RIVAL
	setflag FLAG_HIDE_OAK_IN_CHAMP_ROOM
	setflag FLAG_HIDE_CREDITS_RIVAL
	setflag FLAG_HIDE_CREDITS_OAK
	setflag FLAG_HIDE_CINNABAR_BILL
	setflag FLAG_HIDE_CINNABAR_SEAGALLOP
	setflag FLAG_HIDE_CINNABAR_POKECENTER_BILL
	setflag FLAG_HIDE_LORELEI_IN_HER_HOUSE
	setflag FLAG_HIDE_SAFFRON_FAN_CLUB_BLACKBELT
	setflag FLAG_HIDE_SAFFRON_FAN_CLUB_ROCKER
	setflag FLAG_HIDE_SAFFRON_FAN_CLUB_WOMAN
	setflag FLAG_HIDE_SAFFRON_FAN_CLUB_BEAUTY
	setflag FLAG_HIDE_TWO_ISLAND_GAME_CORNER_LOSTELLE
	setflag FLAG_HIDE_TWO_ISLAND_GAME_CORNER_BIKER
	setflag FLAG_HIDE_TWO_ISLAND_WOMAN
	setflag FLAG_HIDE_TWO_ISLAND_BEAUTY
	setflag FLAG_HIDE_TWO_ISLAND_SUPER_NERD
	setflag FLAG_HIDE_LOSTELLE_IN_HER_HOME
	setflag FLAG_HIDE_THREE_ISLAND_LONE_BIKER
	setflag FLAG_HIDE_FOUR_ISLAND_RIVAL
	setflag FLAG_HIDE_DOTTED_HOLE_SCIENTIST
	setflag FLAG_HIDE_RESORT_GORGEOUS_SELPHY
	setflag FLAG_HIDE_RESORT_GORGEOUS_INSIDE_SELPHY
	setflag FLAG_HIDE_SELPHYS_BUTLER
	setflag FLAG_HIDE_DEOXYS
	setflag FLAG_HIDE_LORELEI_HOUSE_MEOWTH_DOLL
	setflag FLAG_HIDE_LORELEI_HOUSE_CHANSEY_DOLL
	setflag FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_F_DOLL
	setflag FLAG_HIDE_LORELEI_HOUSE_JIGGLYPUFF_DOLL
	setflag FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_M_DOLL
	setflag FLAG_HIDE_LORELEIS_HOUSE_FEAROW_DOLL
	setflag FLAG_HIDE_LORELEIS_HOUSE_PIDGEOT_DOLL
	setflag FLAG_HIDE_LORELEIS_HOUSE_LAPRAS_DOLL
	setflag FLAG_HIDE_POSTGAME_GOSSIPERS
	setflag FLAG_HIDE_FAME_CHECKER_ERIKA_JOURNALS
	setflag FLAG_HIDE_FAME_CHECKER_KOGA_JOURNAL
	setflag FLAG_HIDE_FAME_CHECKER_LT_SURGE_JOURNAL
	setflag FLAG_HIDE_SAFFRON_CITY_POKECENTER_SABRINA_JOURNALS
	setvar VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 500
	end

	.include "data/scripts/hall_of_fame.inc"
	.include "data/scripts/obtain_item.inc"

Common_ShowEasyChatScreen:: @ 81A6AC0
	fadescreen FADE_TO_BLACK
	special ShowEasyChatScreen
	fadescreen FADE_FROM_BLACK
	return

	.include "data/scripts/surf.inc"
	.include "data/scripts/set_gym_trainers.inc"
	.include "data/scripts/bag_full.inc"

@ Unused
EventScript_GymBadgeFanfare:: @ 81A6C21
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	return

EventScript_OutOfCenterPartyHeal:: @ 81A6C26
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	fadescreen FADE_FROM_BLACK
	return

EventScript_WallTownMap:: @ 81A6C32
	lockall
	msgbox Text_ATownMap
	goto_if_questlog EventScript_ReleaseEnd
	fadescreen FADE_TO_BLACK
	special ShowTownMap
	waitstate
	releaseall
	end

	.include "data/text/pokedex_rating.inc"
	.include "data/scripts/pokedex_rating.inc"
	.include "data/scripts/cave_of_origin.inc"

EventScript_ChangePokemonNickname:: @ 81A74EB
	fadescreen FADE_TO_BLACK
	special ChangePokemonNickname
	waitstate
	return

@ Unused
EventScript_HandOverItem:: @ 81A74F2
	getitemname 0, VAR_0x8004
	playfanfare MUS_OBTAIN_TMHM
	message Text_HandedOverItem
	waitmessage
	waitfanfare
	removeitem VAR_0x8004, 1
	return

	.include "data/scripts/pokemon_league.inc"
	.include "data/scripts/movement.inc"	
	.include "data/scripts/flavor_text.inc"
	.include "data/scripts/questionnaire.inc"

EventScript_BagItemCanBeRegistered:: @ 81A77A0
	msgbox Text_BagItemCanBeRegistered, MSGBOX_SIGN
	end

EventScript_Return:: @ 81A77A9
	return

EventScript_SetResultTrue:: @ 81A77AA
	setvar VAR_RESULT, TRUE
	return

EventScript_SetResultFalse:: @ 81A77B0
	setvar VAR_RESULT, FALSE
	return

EventScript_SetExitingCyclingRoad:: @ 81A77B6
	lockall
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	setvar VAR_MAP_SCENE_ROUTE16, 0
	releaseall
	end

EventScript_SetEnteringCyclingRoad:: @ 81A77C1
	lockall
	setvar VAR_MAP_SCENE_ROUTE16, 1
	releaseall
	end

	.include "data/scripts/route23.inc"

EventScript_GetElevatorFloor:: @ 81A7AB9
	special GetElevatorFloor
	return

	.include "data/scripts/aide.inc"

EventScript_CancelMessageBox:: @ 81A7ADB
	special DoPicboxCancel
	release
	end

EventScript_ReleaseEnd:: @ 81A7AE0
	release
	end

@ Unused
EventScript_DelayedLookAround:: @ 81A80FE
	lockall
	applymovement VAR_0x8004, Movement_WalkInPlaceFastestLeft
	waitmovement 0
	delay 20
	applymovement VAR_0x8004, Movement_WalkInPlaceFastestUp
	waitmovement 0
	delay 20
	applymovement VAR_0x8004, Movement_WalkInPlaceFastestRight
	waitmovement 0
	delay 20
	applymovement VAR_0x8004, Movement_WalkInPlaceFastestDown
	waitmovement 0
	delay 20
	releaseall
	end

	.include "data/scripts/silphco_doors.inc"
	.include "data/scripts/pc_transfer.inc"

EventScript_GetInGameTradeSpeciesInfo:: @ 81A8CAD
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	return

EventScript_ChooseMonForInGameTrade:: @ 81A8CBD
	special ChoosePartyMon
	waitstate
	lock
	faceplayer
	copyvar VAR_0x800A, VAR_0x8004
	return

EventScript_GetInGameTradeSpecies:: @ 81A8CC9
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	return

EventScript_DoInGameTrade:: @ 81A8CD9
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	lock
	faceplayer
	return

EventScript_VsSeekerChargingDone:: @ 81A8CED
	special VsSeekerFreezeObjectsAfterChargeComplete
	waitstate
	special VsSeekerResetObjectMovementAfterChargeComplete
	releaseall
	end

	.include "data/scripts/itemfinder.inc"

Std_PutItemAway:: @ 81A8E58
	bufferitemnameplural 1, VAR_0x8000, VAR_0x8001
	checkitemtype VAR_0x8000
	call EventScript_BufferPutAwayPocketName
	msgbox Text_PutItemAway
	return

EventScript_BufferPutAwayPocketName:: @ 81A8E6F
	switch VAR_RESULT
	case POCKET_ITEMS,       EventScript_BufferPutAwayPocketItems
	case POCKET_KEY_ITEMS,   EventScript_BufferPutAwayPocketKeyItems
	case POCKET_POKE_BALLS,  EventScript_BufferPutAwayPocketPokeBalls
	case POCKET_TM_CASE,     EventScript_BufferPutAwayPocketTMCase
	case POCKET_BERRY_POUCH, EventScript_BufferPutAwayPocketBerryPouch
	end

EventScript_BufferPutAwayPocketItems:: @ 81A8EAC
	getstdstring 2, STDSTRING_ITEMS_POCKET
	return

EventScript_BufferPutAwayPocketKeyItems:: @ 81A8EB1
	getstdstring 2, STDSTRING_KEY_ITEMS_POCKET
	return

EventScript_BufferPutAwayPocketPokeBalls:: @ 81A8EB6
	getstdstring 2, STDSTRING_POKEBALLS_POCKET
	return

EventScript_BufferPutAwayPocketTMCase:: @ 81A8EBB
	getstdstring 2, STDSTRING_TM_CASE
	return

EventScript_BufferPutAwayPocketBerryPouch:: @ 81A8EC0
	getstdstring 2, STDSTRING_BERRY_POUCH
	return

EventScript_TryDarkenRuins:: @ 81A925E
	goto_if_set FLAG_SYS_UNLOCKED_TANOBY_RUINS, EventScript_Return
	setweather WEATHER_SHADE
	doweather
	return

EventScript_BrailleCursorWaitButton:: @ 81A926C
	special BrailleCursorToggle
	waitbuttonpress
	playse SE_SELECT
	setvar VAR_0x8006, 1
	special BrailleCursorToggle
	return

EventScript_NoMoreRoomForPokemon:: @ 81A927C
	textcolor 3
	msgbox Text_NoMoreRoomForPokemon
	release
	end

	.include "data/text/braille.inc"

@ Test message!
@ Welcome to the world of Pokémon!
Text_TestMsg:: @ 81ACD45
	.string "テストよう　メッセージです！\n"
	.string "ポケモンの　せかいへ　ようこそ！$"

	.include "data/text/fame_checker.inc"
	.include "data/text/sign_lady.inc"
	.include "data/text/trainer_card.inc"
	.include "data/scripts/trainer_card.inc"
	.include "data/text/help_system.inc"
	.include "data/scripts/field_moves.inc"
	.include "data/scripts/item_ball_scripts.inc"
	.include "data/scripts/mystery_event_club.inc"
	.include "data/text/day_care.inc"
	.include "data/scripts/flash.inc"
	.include "data/scripts/repel.inc"
	.include "data/text/safari_zone.inc"
	.include "data/text/competitive_brothers.inc"
	.include "data/text/eon_ticket.inc"
	.include "data/text/berries.inc"
	.include "data/text/flavor_text.inc"
	.include "data/scripts/hole.inc"
	.include "data/text/trainers.inc"
	.include "data/scripts/test.inc"
	.include "data/text/save.inc"
	.include "data/text/pokedude.inc"
