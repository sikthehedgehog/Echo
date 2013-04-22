// Required headers
#include <stdint.h>
#include "echoblob.h"
#include "echo.h"

// Z80 addresses
static volatile uint8_t*  const z80_ram    = (uint8_t *)  0xA00000;
static volatile uint16_t* const z80_busreq = (uint16_t *) 0xA11100;
static volatile uint16_t* const z80_reset  = (uint16_t *) 0xA11200;

// Macros to control the Z80
#define Z80_REQUEST() \
   { *z80_busreq = 0x100; while (*z80_busreq & 0x100); }
#define Z80_RELEASE() \
   { *z80_busreq = 0; }
#define Z80_RESET() \
   { *z80_reset = 0; \
     int16_t i; for (i = 8; i >= 0; i--); \
     *z80_reset = 0x100; }

//***************************************************************************
// echo_init
// Initializes Echo and gets it running.
//---------------------------------------------------------------------------
// param list: pointer to instrument list
//***************************************************************************

void echo_init(const void **list) {
   // Take over the Z80
   Z80_RESET();
   Z80_REQUEST();
   
   // Tell Echo to not run any commands by default (the assembly counterpart
   // would tell it to load the instrument list, but we can't do that here
   // due to linker shenanigans)
   z80_ram[0x1FFF] = 0x00;
   
   // Load the instrument list manually, since thanks to linker shenanigans
   // we can't implement the list properly in ROM :/
   volatile uint8_t *dest = &z80_ram[0x1C00];
   while (*list) {
      // Retrieve pointer to next instrument
      // Cast it to an integer since we need to treat it as such
      // This should be considered bad C, but since this is hardware-specific
      // code this should be fine to do (portability is not expected)
      uint32_t ptr = (uint32_t) *list;
      
      // Turn the pointer into the base+address notation Echo wants and store
      // it in Z80 RAM directly (where the list would go)
      dest[0x000] = (ptr >> 8 & 0x7F) | 0x80;
      dest[0x100] = (ptr & 0xFF);
      dest[0x200] = (ptr >> 15 & 0x7F) | (ptr >> 16 & 0x80);
      
      // Go for next pointer
      list++;
      dest++;
   }
   
   // Copy the Echo blob into Z80 RAM
   // No, memcpy() won't do here since we must ensure accesses are byte-sized
   // (memcpy() may not know this and try word or long accesses)
   const uint8_t *src = echo_blob;
   dest = z80_ram;
   int16_t count = sizeof(echo_blob)-1;
   while (count-- >= 0)
      *dest++ = *src++;
   
   // Let Echo start running!
   Z80_RESET();
   Z80_RELEASE();
}

//***************************************************************************
// echo_send_command
// Sends a raw command to Echo. No parameters are taken.
//---------------------------------------------------------------------------
// param cmd: command to send
//***************************************************************************

void echo_send_command(uint8_t cmd) {
   // We need access to Z80 bus
   Z80_REQUEST();
   
   // Is Echo busy yet?
   while (z80_ram[0x1FFF] != 0x00) {
      Z80_RELEASE();
      int16_t i;
      for (i = 0x3FF; i >= 0; i--);
      Z80_REQUEST();
   }
   
   // Write the command
   z80_ram[0x1FFF] = cmd;

   // Done with the Z80
   Z80_RELEASE();
}

//***************************************************************************
// echo_send_command_addr
// Sends a raw command to Echo. An address parameter is taken.
//---------------------------------------------------------------------------
// param cmd: command to send
// param addr: address parameter
//***************************************************************************

void echo_send_command_addr(uint8_t cmd, const void *addr) {
   // Since we need to split the address into multiple bytes we put it in an
   // integer. This is a bad practice in general, period, but since we don't
   // care about portability here we can afford to do it this time.
   uint32_t param = (uint32_t) addr;
   
   // We need access to Z80 bus
   Z80_REQUEST();
   
   // Is Echo busy yet?
   while (z80_ram[0x1FFF] != 0x00) {
      Z80_RELEASE();
      int16_t i;
      for (i = 0x3FF; i >= 0; i--);
      Z80_REQUEST();
   }
   
   // Write the command
   z80_ram[0x1FFF] = cmd;
   z80_ram[0x1FFD] = param;
   param >>= 8;
   z80_ram[0x1FFE] = param | 0x80;
   param >>= 7;
   param = (param & 0x7F) | (param >> 1 & 0x80);
   z80_ram[0x1FFC] = param;

   // Done with the Z80
   Z80_RELEASE();
}

//***************************************************************************
// echo_send_command_byte
// Sends a raw command to Echo. A byte parameter is taken.
//---------------------------------------------------------------------------
// param cmd: command to send
// param byte: parameter
//***************************************************************************

void echo_send_command_byte(uint8_t cmd, uint8_t byte) {
   // We need access to Z80 bus
   Z80_REQUEST();
   
   // Is Echo busy yet?
   while (z80_ram[0x1FFF] != 0x00) {
      Z80_RELEASE();
      int16_t i;
      for (i = 0x3FF; i >= 0; i--);
      Z80_REQUEST();
   }
   
   // Write the command
   z80_ram[0x1FFF] = cmd;
   z80_ram[0x1FFC] = byte;

   // Done with the Z80
   Z80_RELEASE();
}

//***************************************************************************
// echo_play_bgm
// Starts playing background music.
//---------------------------------------------------------------------------
// param ptr: pointer to BGM stream
//***************************************************************************

void echo_play_bgm(const void *ptr) {
   echo_send_command_addr(ECHO_CMD_PLAYBGM, ptr);
}

//***************************************************************************
// echo_stop_bgm
// Stops background music playback.
//***************************************************************************

void echo_stop_bgm(void) {
   echo_send_command(ECHO_CMD_STOPBGM);
}

//***************************************************************************
// echo_resume_bgm
// Resumes background music playback.
//***************************************************************************

void echo_resume_bgm(void) {
   echo_send_command(ECHO_CMD_RESUMEBGM);
}

//***************************************************************************
// echo_play_sfx
// Starts playing a sound effect.
//---------------------------------------------------------------------------
// param ptr: pointer to SFX stream
//***************************************************************************

void echo_play_sfx(const void *ptr) {
   echo_send_command_addr(ECHO_CMD_PLAYSFX, ptr);
}

//***************************************************************************
// echo_stop_sfx
// Stops sound effect playback.
//***************************************************************************

void echo_stop_sfx(void) {
   echo_send_command(ECHO_CMD_STOPSFX);
}

//***************************************************************************
// echo_set_pcm_rate
// Changes the playback rate of PCM.
//---------------------------------------------------------------------------
// param rate: new rate (timer A value)
//***************************************************************************

void echo_set_pcm_rate(uint8_t rate) {
   echo_send_command_byte(ECHO_CMD_SETPCMRATE, rate);
}

//***************************************************************************
// echo_get_status
// Retrieves Echo's current status.
//---------------------------------------------------------------------------
// return: status flags (see ECHO_STAT_*)
//***************************************************************************

uint16_t echo_get_status(void) {
   // We need access to the Z80
   Z80_REQUEST();
   
   // Retrieve status from Z80 RAM
   uint16_t status = 0;
   status = z80_ram[0x1FF0];
   if (z80_ram[0x1FFF] != 0)
      status |= ECHO_STAT_BUSY;
   
   // Done with the Z80
   Z80_RELEASE();
   
   // Return status
   return status;
}
