const struct MEWonderCardData custom_cGift_1_wc = {
	.cardId=1,
	.icon=WC_ICON_HO_OH,
	.headerNumber=1,
	.type=WC_TYPE_STAMP,
	.color=WX_COLOR_BLUE,
	.shareState=WC_SHARE_YES,
	.recvMonCapacity=7,
	
	.headerA=_("OMNI-TICKET$"),
	.headerB=_("Custom Distribution PoC$"),
	.contents={
		_("Go to the second floor of the POKéMON$"),
		_("CENTER and meet the delivery person.$"),
		_("Receive your gifts and then save!!$"),
		_("Look out for additional surprises…$")
	},
	.footerA=_("Do not toss before receiving gift!!$"),
	.footerB=_("Press {A_BUTTON} for a cookie.$")
};

const struct MEWonderNewsData custom_cGift_1_wn = {
	.newsId=1,
	.shareState=WN_SHARE_YES,
	.color=WX_COLOR_BLUE,
	
	.title=_("It's a whole new world we live in…$"),
	.contents={
		_("I've worked on this off-and-on for$"),
		_("a few years, but I've finally done it:$"),
		_("an easily modifiable custom$"),
		_("distribution. I wish to extend my$"),
		_("thanks to ajxpk and all of the members$"),
		_("of pret. I look forward to working on$"),
		_("this project for years to come.$"),
		_("$"),
		_("- WhenGryphonsFly{PLAY_BGM}{MUS_SLOW_PALLET}$"),
		_("P.S. Shoutouts to SimpleFlips$")
	}
};

const struct mevent_server_cmd custom_cGift_1_FireRed[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT
	SERVER_BEGIN_SENDING_CARD_AT
	SERVER_WAIT_FOR_SEND_TO_FINISH
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER
	SERVER_BEGIN_SENDING_NEWS_AT
	SERVER_WAIT_FOR_SEND_TO_FINISH
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER
	SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES(SHORT_CODE("BPE\0", 0xFF), custom_cGift_1_Emerald),
	// 606
	// 608-614
};

const struct mevent_server_cmd custom_cGift_1_CompatibilityCheck[] = {
	SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES(SHORT_CODE("\0\0\0J", 0xFF), custom_sharedServerScript_Incompatible),
	SERVER_COPY_TO_BUFFER_CALLBACK(custom_cGift_1_FireRed),
	SERVER_COPY_TO_BUFFER_CARD_AT(&custom_cGift_1_wc),
	SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES(SHORT_CODE("BP\0\0", 0xFF), custom_sharedServerScript_ReplaceCardPrompt),
	SERVER_BRANCH_ALWAYS(custom_sharedServerScript_Incompatible)
};

const struct mevent_server_cmd custom_cGift_1_sCmdInit[] = {
	SERVER_COPY_TO_BUFFER_CALLBACK(custom_cGift_1_CompatibilityCheck),
	SERVER_BRANCH_ALWAYS(custom_sharedServerScript_GetVersion)
};

const struct MysteryGiftDistributionData custom_cGift_1 = {
	.name=_("Omni-Ticket$"),
	.hasBothCardAndNews=TRUE,
	.showNews=TRUE,
	.pkmnStamps={SPECIES_MEW, SPECIES_LUGIA, SPECIES_HO_OH, 0, SPECIES_DEOXYS, SPECIES_LATIOS, SPECIES_LATIAS},

	.wonderCard = custom_cGift_1_wc,
	.wonderNews = custom_cGift_1_wn,
	.serverCommands = custom_cGift_1_sCmdInit
};
