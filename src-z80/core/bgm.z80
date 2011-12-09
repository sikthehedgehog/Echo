;****************************************************************************
; PlayBGM [command $04]
; Plays a BGM
;****************************************************************************

PlayBGM:
    PollPCM
    call ClearBGM               ; Clear BGM resources
    PollPCM

    ld a, (RAM_Status)          ; Show BGM playback in Echo's status
    or $02
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

    ld hl, RAM_BGMData          ; Set BGM as playing
    ld (hl), $01
    inc l                       ; No delays!
    ld (hl), $01
    inc l                       ; Store BGM start bank
    ld (hl), c
    inc l                       ; Store BGM start address (low)
    ld (hl), e
    inc l                       ; Store BGM start address (high)
    ld (hl), d

    PollPCM

    ld hl, ProcessBGM           ; Tell Echo to process BGM
    ld (DoTick_BGM+1), hl
    
;    ld b, 8                     ; Kill all FM channels explicitly
;.resetallfm:
;    ld a, b
;    and $04
;    rrca
;    ld iyl, a
;    ld a, b
;    call KillFM
;    djnz .resetallfm

    jp IdleLoop                 ; End of subroutine

;****************************************************************************
; ProcessBGM
; Processes a tick for a BGM
;****************************************************************************

ProcessBGM:
    PollPCM

    ld hl, RAM_BGMData+1        ; BGM data address

    ld a, (hl)                  ; Delaying?
    dec a
    jp z, .nodelay
    ld (hl), a

    jp DoTick_BGMSkip           ; End of subroutine

.nodelay:
    PollPCM

    inc l
    ld c, (hl)                  ; Get current address
    inc l
    ld e, (hl)
    inc l
    ld d, (hl)
    ex de, hl

ProcessBGMRun:
ProcessBGMSkip:
    PollPCM                     ; Fetch next event
    call GetParam
    PollPCM

    ld a, b                     ; Parse byte

    cp $08
    jp c, NoteOnFMBGM           ; Events $00-$07: note on FM
    cp $0B
    jp c, NoteOnPSGBGM          ; Events $08-$0A: note on PSG (square)
    jp z, NoteOnNoiseBGM        ; Event $0B: note on PSG (noise)
    cp $0C
    jp z, PlayPCMBGM            ; Event $0C: play PCM

    PollPCM
    ld a, b

    cp $18
    jp c, NoteOffFMBGM          ; Events $10-$17: note off FM
    cp $1C
    jp c, NoteOffPSGBGM         ; Events $18-$1B: note off PSG
    jp z, StopPCMBGM            ; Event $1C: note off PCM

    PollPCM
    ld a, b

    cp $FE
    jp z, SetDelayBGM           ; Event $FE: set delay
    cp $FF
    jp z, StopBGMEvent          ; Event $FF: stop BGM
    cp $FC
    jp z, LoopBGM               ; Event $FC: loop BGM
    cp $FD
    jp z, SetLoopBGM            ; Event $FD: set loop point

    PollPCM
    ld a, b

    cp $28
    jp c, SetFMVolBGM           ; Events $28-$2B: set FM volume
    cp $2C
    jp c, SetPSGVolBGM          ; Events $28-$2B: set PSG volume

    PollPCM
    ld a, b

    cp $38
    jp c, SetNoteFMBGM          ; Events $30-$37: set FM note
    cp $3B
    jp c, SetNotePSGBGM         ; Events $38-$3A: set PSG note (square)
    jp z, SetNoteNoiseBGM       ; Event $3B: set PSG note (noise)

    PollPCM
    ld a, b

    cp $48
    jp c, LoadFMBGM             ; Events $40-$47: load FM instrument
    cp $4C
    jp c, LoadPSGBGM            ; Events $48-$4B: load PSG instrument

    PollPCM
    ld a, b

    cp $F8                      ; Events $F0-$F7: set FM parameters
    jp c, SetFMParamBGM

    PollPCM                     ; FFFFFFFFF bad event >:(
    jp StopBGMEvent             ; End of subroutine

ProcessBGMSkip2:                ; This is where we land after a locked event
    PollPCM                       ; that had two bytes for the parameter
    inc l                         ; Skip first byte
    jp nz, .nobankskip2
    inc h
    jp nz, .nobankskip2
    ld h, $80
    inc c
.nobankskip2:

ProcessBGMSkip1:                ; This is where we land after a locked event
    PollPCM                       ; that had one byte for the parameter
    inc l                         ; Skip byte
    jp nz, .nobankskip1
    inc h
    jp nz, .nobankskip1
    ld h, $80
    inc c
.nobankskip1:

    jp ProcessBGMRun            ; Keep processing

;****************************************************************************
; StopBGM* [command $05, event $FF]
; Stops BGM playback
;****************************************************************************

StopBGMEvent:
    call StopBGM                ; We're just a wrapper
    jp DoTick_BGMSkip           ; End of subroutine

StopBGMCmd:
    xor a                       ; Command parsed
    ld (RAM_Command), a
    call StopBGM                ; We're just a wrapper
    jp IdleLoop                 ; End of subroutine

StopBGM:
    ld a, (RAM_Status)          ; Hide BGM playback in Echo's status
    and $FD
    ld (RAM_Status), a

    PollPCM

    call ClearBGM               ; Clear BGM resources
    PollPCM

    xor a                       ; Stop playback
    ld (RAM_BGMPlaying), a
    ld hl, DoTick_BGMSkip
    ld (DoTick_BGM+1), hl

    ret                         ; End of subroutine

;****************************************************************************
; ClearBGM
; Clears BGM resources
;****************************************************************************

ClearBGM:
    ld a, (RAM_Locked+6)        ; Stop PCM playback if needed
    or a
    call z, StopPCM

;----------------------------------------------------------------------------

    ld b, 4                     ; Reset all PSG channels
    ld de, RAM_PSGData+48+15
    ld hl, RAM_Locked+11
.mutepsg:
    PollPCM
    
    ld (hl), $00                ; Reset BGM volume
    
    ld a, e
    sub 15
    ld e, a
    
    ld a, (hl)                  ; Mute PSG channel if it isn't locked
    or a
    jr nz, .nopsgmute
    xor a
    ld (de), a
    
.nopsgmute:
    PollPCM
    
    dec e
    dec l
    djnz .mutepsg

;----------------------------------------------------------------------------

    ld b, 8                     ; Reset all FM channels
    ld de, RAM_BGMFMVol+7
.resetfm:
    PollPCM
    
    ;xor a                       ; Reset BGM volume
    ld a, $7F
    ld (de), a
    dec e
    
    ld a, (hl)                  ; Kill FM channel if not locked
    or a
    jp nz, .nofmkill
    
    PollPCM
    
    ld a, b
    dec a
    call KillFM
    
.nofmkill:
    dec l
    djnz .resetfm

;----------------------------------------------------------------------------

    ret                         ; End of subroutine

;****************************************************************************
; LoopBGM [event $FC]
; Makes a BGM loop
;****************************************************************************

LoopBGM:
    PollPCM

    ex de, hl                   ; Get looping address
    inc l
    ld c, (hl)
    inc l
    ld e, (hl)
    inc l
    ld d, (hl)
    dec l
    dec l
    dec l
    ex de, hl

    jp ProcessBGMRun            ; End of subroutine

;****************************************************************************
; SetLoopBGM [event $FD]
; Sets the BGM loop point
;****************************************************************************

SetLoopBGM:
    PollPCM

    ex de, hl
    inc l                       ; Store loop point address
    ld (hl), c
    inc l
    ld (hl), e
    inc l
    ld (hl), d
    dec l
    dec l
    dec l
    ex de, hl

    jp ProcessBGMRun            ; End of subroutine
