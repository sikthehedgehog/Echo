;****************************************************************************
; PlayPCM*
; Plays a PCM sample
;****************************************************************************

PlayPCMSFX:
    call PlayPCM                ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

PlayPCMBGM:
    PollPCM

    ld a, (RAM_Locked+6)        ; Check if channel is free
    or a
    jp nz, ProcessBGMSkip1      ; Don't play sample if locked

    call PlayPCM                ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

PlayPCM:
    call GetParam               ; Get sample ID

    ld a, b
    exx                         ; We'll modify PCM data now
    ld b, $01                   ; Play PCM!

    ld h, RAM_PointerList>>8    ; Get offset in pointer list
    ld l, a

    ld d, (hl)                  ; Get PCM address
    inc h
    ld e, (hl)
    inc h
    ld c, (hl)

    ld hl, $6000                ; Restore $6000 back to HL
    exx                         ; Back to standard variables

    ld (ix+0), $2B              ; Turn on DAC
    ld (ix+1), $80
    ld (ix+0), $2A
    ld (ix+1), $80

    ret                         ; End of subroutine

;****************************************************************************
; UpdatePCM
; Updates PCM output upon a timer event
;****************************************************************************

UpdatePCM:
    ;ld (ix+0), $24              ; Reset timer
    ;ld (ix+1), $FE
    ;ld (ix+0), $25
    ;ld (ix+1), $03
    
    ld (ix+0), $27
    ld (ix+1), $1F

    exx                         ; Switch to PCM registers

    ld a, b                     ; Do any playback?
    or a
    jr z, .nopcm

    ld a, (RAM_LastBank)        ; Bank switch?
    cp c
    jp z, .noswitchu
    ld a, c
    ld (RAM_LastBank), a
    BankSwitch
.noswitchu:

    ld a, (de)                  ; Get sample
    inc a                       ; Is it end of waveform?
    jr z, .stop                   ; If so, stop
    ld (ix+0), $2A              ; Nope, send sample to YM2612
    ld (ix+1), a

    inc e                       ; Get address for next sample
    jr nz, .nopcm
    inc d
    jr nz, .nopcm
    ld d, $80
    inc c

.nopcm:
    exx                         ; Go back to normal registers
    ret                         ; End of subroutine

.stop:
    ld b, $00                   ; Stop playback
    ld (ix+0), $2A              ; Turn off DAC
    ld (ix+1), $80
    ld (ix+0), $2B
    ld (ix+1), $00
    exx                         ; Go back to normal registers
    ret                         ; End of subroutine

;****************************************************************************
; StopPCM*
; Stops a PCM sample
;****************************************************************************

StopPCMSFX:
    call StopPCM                ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

StopPCMBGM:
    PollPCM

    ld a, (RAM_Locked+6)        ; Check if channel is free
    or a
    jp nz, ProcessBGMRun        ; Don't stop sample if locked

    call StopPCM                ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

StopPCM:
    exx                         ; Stop PCM playback
    ld b, $00
    exx

    ld (ix+0), $2B              ; Disable DAC
    ld (ix+1), $00

    ret                         ; End of subroutine


;****************************************************************************
; LockChannelPCM [event $EC]
; Locks the PCM channel
;****************************************************************************

LockChannelPCM:
    ld a, $01                   ; Lock PCM channel
    ld (RAM_Locked+6), a
    
    call StopPCM                ; Stop PCM playback
    jp ProcessSFXRun            ; End of subroutine
