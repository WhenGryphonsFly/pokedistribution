const struct MysteryGiftDistributionData custom_cGift_1 = {
	.name=_("Omni-Ticket$"), .hasBothCardAndNews=TRUE, .showNews=TRUE, .pkmn={SPECIES_MEW, SPECIES_LUGIA, SPECIES_HO_OH, SPECIES_NONE, SPECIES_DEOXYS, SPECIES_LATIOS, SPECIES_LATIAS},
	.wonderCard = {
		.cardId=1, .icon=WC_ICON_QUESTION_MARK, .headerNumber=1, .type=WC_TYPE_POKEMON, .color=WX_COLOR_BLUE, .shareState=WC_SHARE_YES, .recvMonCapacity=7,
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
	},
	.wonderNews = {
		.newsId=1, .shareState=WN_SHARE_YES, .color=WX_COLOR_BLUE,
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
	},
};
