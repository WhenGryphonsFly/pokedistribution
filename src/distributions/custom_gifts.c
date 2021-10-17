#ifdef GUARD_CUSTOM_CODE_C

const u8 custom_gText_distCg_Ultima[] = _("Ultima");
const struct MysteryGiftDistributionData custom_mgdd_Ultima = {
	{1008, WC_ICON_UNOWN_QM, 1, WC_TYPE_STANDARD, WC_COLOR_SILVER, WC_SHARE_YES, 1, _("Test 1"), _("Test 2WWWW"), {_("Test 3"), _("Test 4"), _("Test 5"), _("Test 6")}, _("Test 7"), _("Test 8")},
	{},
	{ {}, {}, {} }
};


const struct ListMenuItem custom_sListMenuItems_CgMenu[] = {
	{ custom_gText_distCg_Ultima, 0 }
};
const struct MysteryGiftDistributionData custom_mgdd_all[] = {
	custom_mgdd_Ultima
};

#endif
