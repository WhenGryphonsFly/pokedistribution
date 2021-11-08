#ifndef GUARD_CUSTOM_SERVER_CMDS_H
#define GUARD_CUSTOM_SERVER_CMDS_H

/* 
	Results of server commands (unlisted if result is unmodified)
	
	SERVER_RETURN(x)
		x
	SERVER_GET_CLIENT_VERSION
		A condensed form of the client's game code and revision, as specified by SHORT_CODE
		Specifically, bit packing is done by eliminating 8 bits from the game code.
			For the first three characters, treat them as ASCII, which allows discarding of the MSB
			For the fourth character, convert to a 3-bit language identifier, a contraction of 5 bits
	SERVER_CHECK_IF_CLIENT_HAS_BUFFERED_CARD
	SERVER_CHECK_IF_CLIENT_HAS_CARD_AT(y)
		0, if the client has no card
		1, if the client has a card with the same ID as the server's card
		2, if the client has a card with a different ID as the server's card
	SERVER_READ_WORD_FROM_CLIENT
		The first 32 bits from the receive buffer
	SERVER_CAN_CLIENT_ACCEPT_STICKER_FROM_BUFFERED_WORD
	SERVER_CAN_CLIENT_ACCEPT_STICKER_AT(y)
		1, if the client has no spaces left on their Wonder Card for stickers
		2, if the client has one space left and can receive the sticker
		3, if the client has at least one space left but already has a sticker with the same icon and ID
		4, if the client has more than one space left and can receive the sticker
	SERVER_GET_CLIENT_LINK_WINS
	SERVER_GET_CLIENT_LINK_LOSSES
	SERVER_GET_CLIENT_LINK_TRADES
		The number of link wins/losses/trades the client has
	SERVER_GET_CLIENT_STICKERS_ACCEPTED
		The number of stickers currently on the client's Wonder Card
	SERVER_GET_CLIENT_STICKERS_MAX
		The maximum number of stickers the client's Wonder Card supports
	SERVER_CHECK_CLIENT_EASY_CHAT_PROFILE_AGAINST(y)
		0, if the client's easy chat profile does not match the profile at y
		1, if the client's easy chat profile does match the profile at y 
*/
#define SERVER_RETURN(x)                                           {.instr =  0, .flag = x}
#define SERVER_WAIT_FOR_SEND_TO_FINISH                             {.instr =  1}
#define SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(x)               {.instr =  2, .flag = x}
#define SERVER_BRANCH_ALWAYS(y)                                    {.instr =  3, .parameter = y}
#define SERVER_BRANCH_IF_RESULT_WAS(x, y)                          {.instr =  4, .flag = x, .parameter = y}
#define SERVER_PREPARE_TO_GET_CLIENT_VERSION                       {.instr =  5}
#define SERVER_GET_CLIENT_VERSION                                  {.instr =  6}
#define SERVER_CHECK_IF_CLIENT_HAS_BUFFERED_CARD                   {.instr =  7, .parameter = 0}
#define SERVER_CHECK_IF_CLIENT_HAS_CARD_AT(y)                      {.instr =  7, .parameter = y}
#define SERVER_READ_WORD_FROM_CLIENT                               {.instr =  8}
#define SERVER_CAN_CLIENT_ACCEPT_STICKER_FROM_BUFFERED_WORD        {.instr =  9, .parameter = 0}
#define SERVER_CAN_CLIENT_ACCEPT_STICKER_AT(y)                     {.instr =  9, .parameter = y}
#define SERVER_GET_CLIENT_LINK_WINS                                {.instr = 10, .flag = 0}
#define SERVER_GET_CLIENT_LINK_LOSSES                              {.instr = 10, .flag = 1}
#define SERVER_GET_CLIENT_LINK_TRADES                              {.instr = 10, .flag = 2}
#define SERVER_GET_CLIENT_STICKERS_ACCEPTED                        {.instr = 10, .flag = 3}
#define SERVER_GET_CLIENT_STICKERS_MAX                             {.instr = 10, .flag = 4}
#define SERVER_CHECK_CLIENT_EASY_CHAT_PROFILE_AGAINST(y)           {.instr = 11, .parameter = y}
//#define SERVER_CASE_12 // Useless; compares locations of pointers rather than values
#define SERVER_BEGIN_SENDING_BUFFERED_CARD                         {.instr = 13, .parameter = 0}
#define SERVER_BEGIN_SENDING_CARD_AT(y)                            {.instr = 13, .parameter = y}
#define SERVER_BEGIN_SENDING_BUFFERED_NEWS                         {.instr = 14, .parameter = 0}
#define SERVER_BEGIN_SENDING_NEWS_AT(y)                            {.instr = 14, .parameter = y}
#define SERVER_BEGIN_SENDING_BUFFERED_RAM_SCRIPT_WITH_SIZE(x)      {.instr = 15, .flag = x, .parameter = 0}
#define SERVER_BEGIN_SENDING_RAM_SCRIPT_WITH_SIZE_AT(x, y)         {.instr = 15, .flag = x, .parameter = y}
#define SERVER_BEGIN_SENDING_BUFFERED_WORD                         {.instr = 16, .parameter = 0}
#define SERVER_BEGIN_SENDING_WORD_AT(y)                            {.instr = 16, .parameter = y}
#define SERVER_BEGIN_SENDING_DATA_WITH_MAGIC_1C_WITH_SIZE_AT(x, y) {.instr = 17, .flag = x, .parameter = y}
#define SERVER_BEGIN_SENDING_BUFFERED_CLIENT_COMMANDS_WITH_SIZE(x) {.instr = 18, .flag = x, .parameter = 0}
#define SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT(x, y)    {.instr = 18, .flag = x, .parameter = y}
#define SERVER_BEGIN_SENDING_CUSTOM_TRAINER_AT(y)                  {.instr = 19, .parameter = y}
#define SERVER_CASE_20 // [[!!!]]
#define SERVER_COPY_TO_BUFFER_WORD_AT(y)                           {.instr = 21, .parameter = y}
#define SERVER_COPY_TO_BUFFER_CARD_AT(y)                           {.instr = 22, .parameter = y}
#define SERVER_COPY_TO_BUFFER_NEWS_AT(y)                           {.instr = 23, .parameter = y}
#define SERVER_COPY_TO_BUFFER_RAM_SCRIPT_WITH_SIZE_AT(x, y)        {.instr = 24, .flag = x, .parameter = y}
#define SERVER_COPY_TO_BUFFER_CLIENT_COMMANDS_WITH_SIZE_AT(x, y)   {.instr = 25, .flag = x, .parameter = y}
//#define SERVER_CASE_26 // Removed; copies the Wonder Card from the save file to the buffer, but save/load functionality has been removed
//#define SERVER_CASE_27 // Removed; copies the Wonder News from the save file to the buffer, but save/load functionality has been removed
//#define SERVER_CASE_28 // Removed; copies the RAM Script from the save file to the buffer, but save/load functionality has been removed
#define SERVER_BEGIN_SENDING_DATA_WITH_MAGIC_1B_WITH_SIZE_AT(x, y) {.instr = 29, .flag = x, .parameter = y}



/*
	Results of client commands (unlisted if result is unmodified)
	
	CLIENT_RETURN(x)
		x
	CLIENT_DISPLAY_MESSAGE_FROM_SERVER_AND_GET_YES_NO
		0, if [[???]]
		1, if ???
		?, if the player selects "Yes"
		?, if the player selects "No"
	CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD
		0, if ???
		1, if ???
		?, if the player selects "Yes"
		?, if the player selects "No"
	CLIENT_ACCEPT_MYSTERY_EVENT
		The contents of data[2] of the Mystery Event upon completion
		data[2] is set by SetMysteryEventScriptStatus in RAM Scripts, among other sources
	CLIENT_ARBITRARY_CODE_EXECUTION
		Arbitrary
*/
#define CLIENT_NOP                                          {.instr =  0}
#define CLIENT_RETURN(x)                                    {.instr =  1, .parameter = x}
#define CLIENT_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(x)        {.instr =  2, .parameter = x}
#define CLIENT_WAIT_FOR_SEND_TO_FINISH                      {.instr =  3}
#define CLIENT_EXECUTE_COMMANDS_FROM_SERVER                 {.instr =  4}
#define CLIENT_DISPLAY_MESSAGE_FROM_SERVER_AND_GET_YES_NO   {.instr =  5}
#define CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_0 {.instr =  6}
#define CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_1 {.instr =  7}
#define CLIENT_BEGIN_SENDING_CLIENT_VERSION                 {.instr =  8}
#define CLIENT_ACCEPT_NEWS_IF_DIFFERENT_AND_BEGIN_SEND      {.instr =  9}
#define CLIENT_ACCEPT_CARD                                  {.instr = 10}
#define CLIENT_DISPLAY_MESSAGE_FROM_SERVER                  {.instr = 11}
#define CLIENT_CASE_12 // [[!!!]]
#define CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD                 {.instr = 13}
#define CLIENT_BEGIN_SENDING_RESULT                         {.instr = 14}
#define CLIENT_ACCEPT_MYSTERY_EVENT                         {.instr = 15}
#define CLIENT_ACCEPT_STICKER_TO_CARD                       {.instr = 16}
#define CLIENT_ACCEPT_RAM_SCRIPT                            {.instr = 17}
#define CLIENT_ACCEPT_CUSTOM_TRAINER                        {.instr = 18}
#define CLIENT_BEGIN_SENDING_GAME_STAT_AND_WAIT(x)          {.instr = 19, .parameter = x}
#define CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT         {.instr = 20}
#define CLIENT_ARBITRARY_CODE_EXECUTION                     {.instr = 21}



/*
    Magic numbers used by server and client commands
	
	0x10  Client Commands  SERVER_BEGIN_SENDING_BUFFERED_CLIENT_COMMANDS_WITH_SIZE(x)/CLIENT_COMMANDS_WITH_SIZE_AT(x, y)
	0x11  Client Version   CLIENT_BEGIN_SENDING_CLIENT_VERSION
	0x12  Game Stat        CLIENT_BEGIN_SENDING_GAME_STAT_AND_WAIT(x)
	0x13  32-bit Word      CLIENT_ACCEPT_NEWS_IF_DIFFERENT_AND_BEGIN_SEND, CLIENT_BEGIN_SENDING_RESULT
	0x14  [[???]]          CLIENT_BEGIN_SENDING_ENTIRE_BUFFER_AND_WAIT
	0x15  [[???]]          SERVER_CASE_20
	0x16  Wonder Card      SERVER_BEGIN_SENDING_BUFFERED_CARD/CARD_AT(y)
	0x17  Wonder News      SERVER_BEGIN_SENDING_BUFFERED_NEWS/NEWS_AT(y)
	0x18  32-bit Word      SERVER_BEGIN_SENDING_BUFFERED_WORD/WORD_AT(y)
	0x19  RAM Script       SERVER_BEGIN_SENDING_BUFFERED_RAM_SCRIPT_WITH_SIZE(x)/RAM_SCRIPT_WITH_SIZE_AT(x, y)
	0x1a  Custom Trainer   SERVER_BEGIN_SENDING_CUSTOM_TRAINER_AT(y)
	0x1b  [[???]]          SERVER_BEGIN_SENDING_DATA_WITH_MAGIC_1B_WITH_SIZE_AT(x, y)
	0x1c  [[???]]          SERVER_BEGIN_SENDING_DATA_WITH_MAGIC_1C_WITH_SIZE_AT(x, y)
*/
#define MAGIC_CLIENT_COMMANDS  0x10
#define MAGIC_CLIENT_VERSION   0x11
#define MAGIC_GAME_STAT        0x12
#define MAGIC_CLIENT_SENT_WORD 0x13
#define MAGIC_WONDER_CARD      0x16
#define MAGIC_WONDER_NEWS      0x17
#define MAGIC_SERVER_SENT_WORD 0x18
#define MAGIC_RAM_SCRIPT       0x19
#define MAGIC_CUSTOM_TRAINER   0x1a



#define CLIENT_COMMAND_SIZE 0x8
#define MAXIMUM_CLIENT_COMMANDS 128 // ME_SEND_BUF_SIZE == 0x400; 0x400 / 0x8 == 0x80 == 128

#endif//GUARD_CUSTOM_SERVER_CMDS_H
