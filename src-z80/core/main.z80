;****************************************************************************
; EntryPoint
; Where the program starts
;****************************************************************************

EntryPoint:
    ld sp, RAM_Stack            ; Init stack

    xor a                       ; Reset Echo status
    ld (RAM_Status), a

    ld hl, $7F11                ; Mute PSG
    ld (hl), $9F
    ld (hl), $BF
    ld (hl), $DF
    ld (hl), $FF
    xor a
    ld (RAM_PSGData), a
    ld (RAM_PSGData+16), a
    ld (RAM_PSGData+32), a
    ld (RAM_PSGData+48), a

    ld hl, $6000                ; Set default bank
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l
    ld (hl), l

    ;ex af, af'                  ; Set 9th bank bit to 0 by default
    ;xor a                       ; (0 = $000000-$7FFFFF range aka MD mode)
    ;ex af, af'                  ; (1 = $800000-$FFFFFF range aka 32X mode)

    ld ix, $4000                ; YM2612 I/O ports base address
    ld iyh, $40

    exx                         ; Init PCM playback status
    ld bc, $0000                ; B = playing, C = bank
    ld de, $0000                ; DE = address
    ld hl, $6000                ; HL = always $6000
    exx

    ld (ix+0), $2B              ; Disable DAC by default
    ld (ix+1), $00

    ld e, $7F                   ; Mute all FM channels
    ld a, $40
    ld b, 4
.mutefm:
    ld (ix+0), a
    ld (ix+1), e
    ld (ix+2), a
    ld (ix+3), e
    inc a
    ld (ix+0), a
    ld (ix+1), e
    ld (ix+2), a
    ld (ix+3), e
    inc a
    ld (ix+0), a
    ld (ix+1), e
    ld (ix+2), a
    ld (ix+3), e
    inc a
    inc a
    djnz .mutefm

    ld (ix+0), $B4              ; Ensure all channels can be heard from both
    ld (ix+1), $C0              ; speakers (by default they're mute!)
    ld (ix+0), $B5
    ld (ix+1), $C0
    ld (ix+0), $B6
    ld (ix+1), $C0
    ld (ix+2), $B4
    ld (ix+3), $C0
    ld (ix+2), $B5
    ld (ix+3), $C0
    ld (ix+2), $B6
    ld (ix+3), $C0

    ld (ix+0), $24              ; Init timers
    ld (ix+1), $FE
    ld (ix+0), $25
    ld (ix+1), $03
    ld (ix+0), $26
    ld (ix+1), $C9
    ld (ix+0), $27
    ld (ix+1), $3F
    
    jp IdleLoop                 ; Go into idle loop

;****************************************************************************
; PollPCM
; Used to update PCM while not idle
;****************************************************************************

PollPCM: macro
    ld a, ($4000)
    bit 0, a
    call nz, UpdatePCM
    endm

;****************************************************************************
; RunCommand
; Checks which command to run
;****************************************************************************

RunCommand:
    dec a                       ; Command $01: load list
    jp z, LoadList
    dec a                       ; Command $02: play SFX
    jp z, PlaySFX
    dec a                       ; Command $03: stop SFX
    jp z, StopSFXCmd
    dec a                       ; Command $04: play BGM
    jp z, PlayBGM
    dec a                       ; Command $05: stop BGM
    jp z, StopBGMCmd

    PollPCM

    xor a                       ; Bad command, ignore >:(
    ld (RAM_Command), a

    PollPCM

;****************************************************************************
; IdleLoop
; Loop that runs when not processing SFX or BGM
;****************************************************************************

IdleLoop:
    ld a, (RAM_Command)         ; Look for commands
    or a
    jr nz, RunCommand

    PollPCM                     ; Poll PCM

    ld a, ($4000)               ; Tick?
    bit 1, a
    jr nz, DoTick
    bit 0, a                    ; Poll PCM again
    call nz, UpdatePCM          ; Not using macro for optimization purposes

    jp IdleLoop                 ; Keep idling

;****************************************************************************
; DoTick
; Called whenever a new tick triggers
;****************************************************************************

DoTick:
    ld a, (ix+0)
    bit 0, a
    call nz, UpdatePCM

    ld (ix+0), $27
    ld (ix+1), $2F

    PollPCM

DoTick_SFX:                     ; Process SFXs
    jp DoTick_SFXSkip
DoTick_SFXSkip:

    PollPCM

DoTick_BGM:                     ; Process BGMs
    jp DoTick_BGMSkip
DoTick_BGMSkip:

    PollPCM

    jp UpdatePSG                ; Update PSG envelopes
DoTick_PSGSkip:

    PollPCM

    jp IdleLoop                 ; End of subroutine

;****************************************************************************
; BankSwitch
; Switches into a new bank (won't update player status!)
;
; input A .... New bank to switch into
; input HL ... Must be $6000
; breaks ..... AF
;****************************************************************************

BankSwitch: macro
    ld (hl), a
    rrca
    ld (hl), a
    rrca
    ld (hl), a
    rrca
    ld (hl), a
    rrca
    ld (hl), a
    rrca
    ld (hl), a
    rrca
    ld (hl), a
    ld (hl), l
    rrca
    ld (hl), a
    endm

;****************************************************************************
; LoadList [command $01]
; Loads the pointer list
;****************************************************************************

LoadList:
    ld hl, RAM_ComBank          ; Get command parameters
    ld c, (hl)
    inc l
    ld e, (hl)
    inc l
    ld d, (hl)
    ex de, hl

    xor a                       ; Command parsed
    ld (RAM_Command), a

    ld a, c                     ; Do initial bank switch
    ld de, $6000
    ex de, hl
    BankSwitch
    ex de, hl

    ld de, RAM_PointerList      ; Where the pointer list starts

.loop:
    ld a, (hl)                  ; Get high address byte
    or a                        ; Is it the end of the list?
    jp z, .end                    ; If so, stop parsing list
    ld (de), a                  ; Nope, store it in list

    inc d                       ; Get address for next byte
    inc l
    jr nz, .noswitch1
    inc h
    jr nz, .noswitch1
    inc c
    ld a, c
    ld h, $60
    BankSwitch
    ld h, $80
.noswitch1:

    ld a, (hl)                  ; Get low address byte
    ld (de), a                  ; Store it in pointer list

    inc d                       ; Get address for next byte
    inc l
    jr nz, .noswitch2
    inc h
    jr nz, .noswitch2
    inc c
    ld a, c
    ld h, $60
    BankSwitch
    ld h, $80
.noswitch2:

    ld a, (hl)                  ; Get bank byte
    ld (de), a                  ; Store it in pointer list

    dec d                       ; Get address for next byte
    dec d
    inc e
    inc l
    jr nz, .noswitch3
    inc h
    jr nz, .noswitch3
    inc c
    ld a, c
    ld h, $60
    BankSwitch
    ld h, $80
.noswitch3:

    jp .loop                    ; Go for next byte

.end:
    ld a, c                     ; Store current bank
    ld (RAM_LastBank), a
    jp IdleLoop                 ; End of subroutine

;****************************************************************************
; GetParam
; Subroutine for getting the parameter byte
;****************************************************************************

GetParam:
    ld a, (RAM_LastBank)        ; Bank switch?
    cp c
    jp z, .noswitchp
    ld a, c
    ld (RAM_LastBank), a
    exx
    BankSwitch
    exx
.noswitchp:
    ld b, (hl)                  ; Get volume

    ;PollPCM

    inc l                       ; Get next address
    jp nz, .nonewbankp
    inc h
    jp nz, .nonewbankp
    ld h, $80
    inc c
.nonewbankp:

    ret                         ; End of subroutine
