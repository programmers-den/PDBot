#pragma once

#include <orca/discord.h>

#define BOT_DB "bot.db"
#define MESSAGE_TABLE "messages"
#define HELP_JSON "src/json/help.json"
#define HELP_JSON_DEFAULT "{\"title\":\"[Commands]\",\"description\":\"\",\"color\":49151}"
// #define C_LOG 820803540367704105
#define C_LOG 820803540367704105
// #define C_COUNTER 713233286574637127
#define C_COUNTER 800022514943655936
// #define C_ONE_WORD 825481761189658683
#define C_ONE_WORD 843892866651652136
// #define C_STARBOARD 757072544779206743
#define C_STARBOARD 800022449642012692
// #define C_POLL 748514616966905866
#define C_POLL 800022393937723392
// #define C_WELCOME 668005380630118410
#define C_WELCOME 800748610173272159
// #define C_SERVER_INFO 668011780479647764
#define C_SERVER_INFO 813202256626712616
// #define C_ROLES 668016959836913674
#define C_ROLES 800748663805837353
// #define C_VERIFY 709783674857586759
#define C_VERIFY 844205628775202867
#define VERIFY_SLEEP 6000
// #define R_VC_CHAT_ONE 706860693521039390
// #define VC_CHAT_ONE 668000598834151427
#define VC_CHAT_ONE 856262065042882600
// #define R_VC_CHAT_ONE 845831844367826995
#define R_VC_CHAT_ONE 845832767689850900
// #define VC_CHAT_TWO 839937352193015808
#define VC_CHAT_TWO 856262170663714847
// #define R_VC_CHAT_TWO 845832134110871582
#define R_VC_CHAT_TWO 845832843674124339
// #define VC_MUSIC 668288521219473428
#define VC_MUSIC 856262098686181416
// #define R_VC_MUSIC 706860693521039390
#define R_VC_MUSIC 845832873101885461
// #define R_MEMBER 709780584276295761
#define R_MEMBER 856243637800665108
// #define R_BOT 668008527004041226
#define R_BOT 856242805332115516
// #define R_SPECIAL 856231596961169408
#define R_SPECIAL 856242504424882176
// #define R_PRONOUNS 856229130626269184
#define R_PRONOUNS 856242535234797620
// #define R_LANGS 856230304870957066
#define R_LANGS 856242622011277372
// #define R_LANS_HELP 856232874041409596
#define R_LANS_HELP 856242647684874261
// #define R_FIELDS 856234950424395796
#define R_FIELDS 856242679209001020
// #define R_FUN 856236052732706837
#define R_FUN 856242709563965480
// #define R_OTHER 856235532631408640
#define R_OTHER 856235532631408640
#define STARBOARD_REACTION_COUNT 1
#define MESSAGE_URL_LEN 31+18+18+18+1 // https://discord.com/channels/{guild.id}/{channel.id}/{message.id} url + channel_id + message_id + nullbyte
#define AVATAR_URL_LEN 40+18+1024+1 // https://cdn.discordapp.com/ url + id + avatar length + nullbyte
#define AVATAR_HEIGHT 124
#define AVATAR_WIDTH 124
#define E_STAR "⭐"
#define E_YES_NAME "yes"
#define E_YES_ID 800395480575901706
// #define E_YES_ID 800395872470564904
#define E_NO_NAME "no"
#define E_NO_ID 821218629315723274
// #define E_NO_ID 800395869425762345
#define ID_LEN 18
#define ID_STR_LEN ID_LEN+1
#define MESSAGE_MENTION_LEN MESSAGE_URL_LEN+8+1
#define CHANNEL_MENTiON_LEN ID_LEN+4 // <#ID>
#define USER_MENTION_LEN ID_LEN+4 // <@ID>
#define USER_NICK_MENTION_LEN USER_MENTION_LEN+1 // <@!ID>
#define USER_AND_DESCRIM_LEN MAX_USERNAME_LEN+MAX_DISCRIMINATOR_LEN-1 // username + nullbye + discriminator + nullbyte - nullbyte
#define ROLE_MENTION_LEN ID_LEN+5 // <@&ROLE_ID>
#define TIMESTAMP_LEN 10
#define TIMESTAMP_STR_LEN TIMESTAMP_LEN+1
#define TIMESTAMP_NORMAL_LEN 25
#define TIMESTAMP_NORMAL_STR_LEN TIMESTAMP_NORMAL_LEN+1
#define ID_OWNER 300126997718237195
// #define R_OWNER 668317861743165441
#define R_OWNER 798971183273738260
#define COLOR_RED 16711680
#define COLOR_GREEN 65280
#define COLOR_BLUE 255
#define COLOR_AERO 8174056
#define COLOR_PINK_PANTONE 14108820
#define COLOR_LIGHT_GREEN 9498256
#define COLOR_MINT 10026904
#define COLOR_YELLOW 16776960
#define COLOR_MAGENTA 16711935
