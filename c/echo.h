#ifndef ECHO_H
#define ECHO_H

/* Required headers */
#include <stdint.h>

/* Echo commands */
enum {
   ECHO_CMD_NONE,          /* 0x00 - No command */
   ECHO_CMD_LOADLIST,      /* 0x01 - Load instrument list */
   ECHO_CMD_PLAYSFX,       /* 0x02 - Play a SFX */
   ECHO_CMD_STOPSFX,       /* 0x03 - Stop SFX playback */
   ECHO_CMD_PLAYBGM,       /* 0x04 - Play a BGM */
   ECHO_CMD_STOPBGM,       /* 0x05 - Stop BGM playback */
   ECHO_CMD_RESUMEBGM,     /* 0x06 - Resume BGM playback */
   ECHO_CMD_SETPCMRATE,    /* 0x07 - Set PCM rate */
};

/* Echo status flags */
#define ECHO_STAT_BGM      0x0002   /* Background music is playing */
#define ECHO_STAT_SFX      0x0001   /* Sound effect is playing */
#define ECHO_STAT_BUSY     0x8000   /* Echo still didn't parse command */

/* Function prototypes */
void echo_init(const void **);
void echo_play_bgm(const void *);
void echo_stop_bgm(void);
void echo_resume_bgm(void);
void echo_play_sfx(const void *);
void echo_stop_sfx(void);
void echo_set_pcm_rate(uint8_t);
uint16_t echo_get_status(void);
void echo_send_command(uint8_t);
void echo_send_command_addr(uint8_t, const void *);
void echo_send_command_byte(uint8_t, uint8_t);

/* Deprecated functions */
static void (* const echo_send_command_ex)(uint8_t, const void *) =
   echo_send_command_addr;

#endif
