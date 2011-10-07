;****************************************************************************
; SetDelay* [event $FE]
; Adds a delay in playback
;****************************************************************************

SetDelaySFX:
    call SetDelay               ; We're just a wrapper
    jp DoTick_SFXSkip           ; End of subroutine

SetDelayBGM:
    call SetDelay               ; We're just a wrapper
    jp DoTick_BGMSkip           ; End of subroutine

SetDelay:
    PollPCM
    call GetParam               ; Get delay
    PollPCM

    ex de, hl
    ld (hl), d                  ; Store new address
    dec l
    ld (hl), e
    dec l
    ld (hl), c

    PollPCM

    dec l                       ; Store new delay
    ld (hl), b

    ret                         ; End of subroutine
