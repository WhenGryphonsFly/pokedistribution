const struct MEWonderCardData custom_cGift_1_wc = {
	.cardId=1,
	.icon=WC_ICON_QUESTION_MARK,
	.headerNumber=1,
	.type=WC_TYPE_STICKER,
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
	.footerB=_("Press {KEYGFX}  for a cookie.$")
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
		_("- WhenGryphonsFly$"),
		_("P.S. Shoutouts to SimpleFlips$")
	}
};

const struct mevent_client_cmd custom_cGift_1_cCmdInit[] = {
	CLIENT_BEGIN_SENDING_CLIENT_VERSION,
	CLIENT_WAIT_FOR_SEND_TO_FINISH,
	CLIENT_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_COMMANDS),
	CLIENT_EXECUTE_COMMANDS_FROM_SERVER
};

const struct mevent_client_cmd custom_cGift_1_cCmdNotCompatible[] = {
	CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT, // [[!!!]]
	CLIENT_RETURN(0x0a) // [[!!!]]
};

const struct mevent_server_cmd custom_cGift_1_sCmdNotCompatible[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(2*CLIENT_COMMAND_SIZE, custom_cGift_1_cCmdNotCompatible),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(0x14), // [[!!!]]
	SERVER_RETURN(0x0a) // [[!!!]]
};

const struct mevent_server_cmd custom_cGift_1_sCmdX[] = {};

const struct mevent_server_cmd custom_cGift_1_sCmdInit[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(4*CLIENT_COMMAND_SIZE, custom_cGift_1_cCmdInit),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_VERSION),
	SERVER_PREPARE_TO_GET_CLIENT_VERSION,
	SERVER_GET_CLIENT_VERSION,
	SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES(SHORT_CODE("\0\0\0J", 0xFF), custom_cGift_1_sCmdNotCompatible),
	SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES(SHORT_CODE("BP\0\0", 0xFF), custom_cGift_1_sCmdX),
	SERVER_BRANCH_ALWAYS(custom_cGift_1_sCmdNotCompatible)
};

const struct MysteryGiftDistributionData custom_cGift_1 = {
	.name=_("Omni-Ticket$"),
	.hasBothCardAndNews=TRUE,
	.showNews=FALSE,
	.pkmnStickers={SPECIES_MEW, SPECIES_LUGIA, SPECIES_HO_OH, 0, SPECIES_DEOXYS, SPECIES_LATIOS, SPECIES_LATIAS},

	.wonderCard = custom_cGift_1_wc,
	.wonderNews = custom_cGift_1_wn,
	.serverCommands = custom_cGift_1_sCmdInit
};
