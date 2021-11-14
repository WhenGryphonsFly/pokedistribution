const struct mevent_client_cmd custom_sharedClientScript_SendVersion[] = {
	CLIENT_BEGIN_SENDING_CLIENT_VERSION,
	CLIENT_WAIT_FOR_SEND_TO_FINISH,
	CLIENT_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_COMMANDS),
	CLIENT_EXECUTE_COMMANDS_FROM_SERVER
};

const struct mevent_client_cmd custom_sharedClientScript_Incompatible[] = {
	CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT,
	CLIENT_RETURN(RETURN_INCOMPATIBLE)
};

const struct mevent_client_cmd custom_sharedClientScript_ClientAlreadyHasSameCard[] = {
	CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT,
	CLIENT_RETURN(RETURN_SAME_CARD)
};

const struct mevent_client_cmd custom_sharedClientScript_ReplaceCardPrompt[] = {
	CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD,
	CLIENT_BEGIN_SENDING_RESULT,
	CLIENT_WAIT_FOR_SEND_TO_FINISH,
	CLIENT_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_COMMANDS),
	CLIENT_EXECUTE_COMMANDS_FROM_SERVER
};

const struct mevent_client_cmd custom_sharedClientScript_ClientRefusedOverwrite[] = {
	CLIENT_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_SERVER_SENT_STRING),
	{.instr = 12}, // [[!!!]]
	CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT,
	CLIENT_RETURN(0x0e) // [[!!!]]
};

//===============//

const struct mevent_server_cmd custom_sharedServerScript_GetVersion[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(4*CLIENT_COMMAND_SIZE, custom_sharedClientScript_SendVersion),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_VERSION),
	SERVER_PREPARE_TO_GET_CLIENT_VERSION,
	SERVER_GET_CLIENT_VERSION,
	SERVER_BRANCH_TO_CALLBACK
};

const struct mevent_server_cmd custom_sharedServerScript_Incompatible[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(2*CLIENT_COMMAND_SIZE, custom_sharedClientScript_Incompatible),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_SENT_BUFFER),
	SERVER_RETURN(RETURN_INCOMPATIBLE)
};

const struct mevent_server_cmd custom_sharedServerScript_ClientAlreadyHasSameCard[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(2*CLIENT_COMMAND_SIZE, custom_sharedClientScript_ClientAlreadyHasSameCard),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_SENT_BUFFER),
	SERVER_RETURN(RETURN_SAME_CARD)
};


ALIGNED(4) const u8 custom_copy_gUnknown_84595B0[] = _("Canceled reading the Card.");
const struct mevent_server_cmd custom_sharedServerScript_ClientRefusedOverwrite[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(4*CLIENT_COMMAND_SIZE, custom_sharedClientScript_ClientRefusedOverwrite),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT(0x1b, custom_copy_gUnknown_84595B0),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_SENT_BUFFER),
	SERVER_RETURN(RETURN_COMMUNICATION_CANCELED)
};

const struct mevent_server_cmd custom_sharedServerScript_ClientHasDifferentCard[] = {
	SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(5*CLIENT_COMMAND_SIZE, custom_sharedClientScript_ReplaceCardPrompt),
	SERVER_WAIT_FOR_SEND_TO_FINISH,
	SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(MAGIC_CLIENT_SENT_WORD),
	SERVER_READ_WORD_FROM_CLIENT,
	SERVER_BRANCH_IF_RESULT_WAS(1, custom_sharedServerScript_ClientRefusedOverwrite),
	SERVER_BRANCH_TO_CALLBACK
};

const struct mevent_server_cmd custom_sharedServerScript_ReplaceCardPrompt[] = {
	SERVER_CHECK_IF_CLIENT_HAS_BUFFERED_CARD,
	SERVER_BRANCH_IF_RESULT_WAS(1, custom_sharedServerScript_ClientAlreadyHasSameCard),
	SERVER_BRANCH_IF_RESULT_WAS(2, custom_sharedServerScript_ClientHasDifferentCard),
	SERVER_BRANCH_TO_CALLBACK
};
