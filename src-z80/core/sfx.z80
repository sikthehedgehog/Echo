;****************************************************************************
; PlaySFX [command $02]
; Plays a SFX
;****************************************************************************

PlaySFX:
    PollPCM
    call ClearSFX               ; Clear SFX resources
    PollPCM

    ld a, (RAM_Status)          ; Show SFX playback in Echo's status
    or $01
    ld (RAM_Status), a

    PollPCM

    ld hl, RAM_ComBank          ; Get command parameters
    ld c, (hl)
    inc l
    ld e, (hl)
    inc l
    ld d, (hl)

    PollPCM

    xor a                       ; Command parsed
    ld (RAM_Command), a

    ld hl, RAM_SFXData          ; Set SFX as playing
    ld (hl), $01
    inc l                       ; No delays!
    ld (hl), $01
    inc l                       ; Store SFX start bank
    ld (hl), c
    inc l                       ; Store SFX start address (low)
    ld (hl), e
    inc l                       ; Store SFX start address (high)
    ld (hl), d

    PollPCM

    ld hl, ProcessSFX           ; Tell Echo to process SFX
    ld (DoTick_SFX+1), hl

    PollPCM
    jp IdleLoop                 ; End of subroutine

;****************************************************************************
; ProcessSFX
; Processes a tick for a SFX
;****************************************************************************

ProcessSFX:
    PollPCM

    ld hl, RAM_SFXData+1        ; SFX data address

    ld a, (hl)                  ; Delaying?
    dec a
    jp z, .nodelay
    ld (hl), a

    jp DoTick_SFXSkip           ; End of subroutine

.nodelay:
    PollPCM

    inc l                       ; Get current address
    ld c, (hl)
    inc l
    ld e, (hl)
    inc l
    ld d, (hl)
    ex de, hl

ProcessSFXRun:
    PollPCM                     ; Fetch next event
    call GetParam
    PollPCM

    ld a, b                     ; Parse byte

    cp $08
    jp c, NoteOnFMSFX           ; Events $00-$07: note on FM
    cp $0B
    jp c, NoteOnPSGSFX          ; Events $08-$0A: note on PSG (square)
    jp z, NoteOnNoiseSFX        ; Event $0B: note on PSG (noise)
    cp $0C
    jp z, PlayPCMSFX            ; Event $0C: note on PCM

    PollPCM
    ld a, b

    cp $18
    jp c, NoteOffFMSFX          ; Events $10-$17: note off FM
    cp $1C
    jp c, NoteOffPSGSFX         ; Events $18-$1B: note off PSG
    jp z, StopPCMSFX            ; Event $1C: note off PCM

    PollPCM
    ld a, b

    cp $FE
    jp z, SetDelaySFX           ; Event $FE: set delay
    cp $FF
    jp z, StopSFXEvent          ; Event $FF: stop SFX

    PollPCM
    ld a, b

    cp $28
    jp c, SetFMVolSFX           ; Events $28-$2B: set FM volume
    cp $2C
    jp c, SetPSGVolSFX          ; Events $28-$2B: set PSG volume

    PollPCM
    ld a, b

    cp $38
    jp c, SetNoteFMSFX          ; Events $30-$37: set FM note
    cp $3B
    jp c, SetNotePSGSFX         ; Events $38-$3A: set PSG note (square)
    jp z, SetNoteNoiseSFX       ; Event $3B: set PSG note (noise)

    PollPCM
    ld a, b

    cp $48
    jp c, LoadFMSFX             ; Events $40-$47: load FM instrument
    cp $4C
    jp c, LoadPSGSFX            ; Events $48-$4B: load PSG instrument

    PollPCM
    ld a, b

    cp $E8
    jp c, LockChannelFM         ; Events $E0-$E7: lock FM channel
    cp $EC
    jp c, LockChannelPSG        ; Events $E8-$EB: lock PSG channel
    jp z, LockChannelPCM        ; Event $EC: lock PCM channel

    PollPCM
    ld a, b

    cp $F8                      ; Events $F0-$F7: set FM parameters
    jp c, SetFMParamSFX

;****************************************************************************
; StopSFX* [command $03, event $FF]
; Stops SFX playback
;****************************************************************************

StopSFXEvent:
    call StopSFX                ; We're just a wrapper
    jp DoTick_SFXSkip           ; End of subroutine

StopSFXCmd:
    xor a                       ; Command parsed
    ld (RAM_Command), a
    call StopSFX                ; We're just a wrapper
    jp IdleLoop                 ; End of subroutine

StopSFX:
    PollPCM

    ld a, (RAM_Status)          ; Hide SFX playback in Echo's status
    and $FE
    ld (RAM_Status), a

    PollPCM

    xor a                       ; Stop playback
    ld (RAM_SFXPlaying), a
    ld hl, DoTick_SFXSkip
    ld (DoTick_SFX+1), hl

    PollPCM
    call ClearSFX               ; Clear SFX resources

    PollPCM
    ret                         ; End of subroutine

;****************************************************************************
; ClearSFX
; Clears SFX resources
;****************************************************************************

ClearSFX:
    ld a, (RAM_Locked+6)        ; Stop PCM playback if needed
    or a
    call nz, StopPCM

;----------------------------------------------------------------------------

    ld b, 4                     ; Look for locked PSG channels
    ld de, RAM_Locked+11
.unlockpsg:

    PollPCM

    ld a, (de)                    ; Check if this channel needs unlocking
    or a
    jr z, .psgfree
    xor a
    ld (de), a

    PollPCM

    ld a, b                       ; Restore BGM volume
    rrca
    rrca
    rrca
    rrca
    dec a
    ld h, RAM_PSGData>>8
    ld l, a
    ld c, (hl)
    sub 15
    ld l, a
    ld (hl), c

    PollPCM
    push de

    ld a, l                       ; Restore BGM envelope
    add 8
    ld l, a
    add 12-8
    ld e, a
    ld d, h
    
    PollPCM

    ld a, (de)
    ld (hl), a
    inc l
    inc e
    ld a, (de)
    ld (hl), a
    inc l
    inc e
    ld a, (de)
    ld (hl), a

    pop de
    PollPCM

.psgfree:
    dec e                         ; Go for next PSG channel to unlock
    djnz .unlockpsg

;----------------------------------------------------------------------------

    ld b, 8                     ; Look for locked FM channels
.unlockfm:

    PollPCM

    ld a, (de)                    ; Check if this channel needs unlocking
    or a
    jp z, .fmfree
    xor a
    ld (de), a

    dec b                         ; Mute FM channel
    ld (ix+0), $28
    ld (ix+1), b

    PollPCM
    
    ld a, b
    and $04                       ; Determine which port to write
    rrca
    ld iyl, a
    
    ld a, b                       ; Kill ADSR
    call KillFM

    PollPCM
    
    ld a, b                       ; Restore BGM FM instrument
    ld h, RAM_BGMFMInstr>>8
    add RAM_BGMFMInstr&$FF
    ld l, a
    
    push bc
    ld a, b
    ld b, (hl)
    call LoadFMDirect
    pop bc
    
    PollPCM
    
    push bc
    ld a, b                       ; Restore BGM FM volume
    add RAM_BGMFMVol&$FF
    ld l, a
    ld a, b
    ld b, (hl)
    call SetFMVolLoad
    pop bc

    PollPCM
    inc b
.fmfree:
    PollPCM
    dec e                         ; Go for next FM channel to unlock
    dec b
    jp nz, .unlockfm

;----------------------------------------------------------------------------

    ret                         ; End of subroutine
