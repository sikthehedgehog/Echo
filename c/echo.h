#ifndef ECHO_H
#define ECHO_H

/*
 * Macros used to define instrument lists
 * Yeah, this thing is a mess
 */
#define ECHO_LIST_START(name) \
   static const unsigned char name[] = {
#define ECHO_LIST_ENTRY(addr) \
   ((unsigned long)(addr) >> 8 & 0x7F) | 0x80, \
   (unsigned long)(addr) & 0xFF, \
   ((unsigned long)(addr) >> 15 & 0x7F) | \
   ((unsigned long)(addr) >> 16 & 0x80)),
#define ECHO_LIST_END \
   0x00 };

/* Echo commands */
enum {
   ECHO_CMD_NONE,          /* 0x00 - No command */
   ECHO_CMD_LOADLIST,      /* 0x01 - Load instrument list */
   ECHO_CMD_PLAYSFX,       /* 0x02 - Play a SFX */
   ECHO_CMD_STOPSFX,       /* 0x03 - Stop SFX playback */
   ECHO_CMD_PLAYBGM,       /* 0x04 - Play a BGM */
   ECHO_CMD_STOPBGM        /* 0x05 - Stop BGM playback */
};

/* Echo status flags */
#define ECHO_STAT_BGM      0x0001   /* Background music is playing */
#define ECHO_STAT_SFX      0x0002   /* Sound effect is playing */
#define ECHO_STAT_BUSY     0x8000   /* Echo still didn't parse command */

/* Function prototypes */
void echo_play_bgm(const void *);
void echo_stop_bgm(void);
void echo_play_sfx(const void *);
void echo_stop_sfx(void);
unsigned short echo_get_status(void);
void echo_send_command(unsigned char);
void echo_send_command_ex(unsigned char, const void *);

#endif
