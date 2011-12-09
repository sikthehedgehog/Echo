;****************************************************************************
; Player variables
;****************************************************************************

    ds $100-($&$FF), $FF
RAM_PSGData:        ds 4*16         ; PSG envelope data
                                      ; ds 1 ... Channel volume
                                      ; ds 1 ... Global volume
                                      ; ds 3 ... Current address
                                      ; ds 3 ... Looping address
                                      ; ds 3 ... Start address
                                      ; ds 1 ... Padding
                                      ; ds 3 ... BGM instrument address
                                      ; ds 1 ... BGM channel volume

RAM_BGMFMInstr:     ds 8            ; FM instruments used by BGM
RAM_BGMFMVol:       ds 8            ; FM volumes used by BGM
;RAM_FMVolume:       ds 8            ; Volume of each FM channel
RAM_FMData:         ds 8*5          ; FM info (for volume handling)
                                      ; ds 8*1 ... Register $B0
                                      ; ds 8*1 ... Register $40
                                      ; ds 8*1 ... Register $44
                                      ; ds 8*1 ... Register $48
                                      ; ds 8*1 ... Register $4C

RAM_LastBank:       ds 1            ; Last accessed bank

RAM_BGMData:                        ; Where BGM data starts
RAM_BGMPlaying:     ds 1              ; Set if a BGM is playing
RAM_BGMDelay:       ds 1              ; How many ticks to wait
RAM_BGMBank:        ds 1              ; Current BGM bank
RAM_BGMAddress:     ds 2              ; Current BGM address
RAM_BGMLoopPoint:   ds 3              ; BGM loop point

RAM_SFXData:                        ; Where SFX data starts
RAM_SFXPlaying:     ds 1              ; Set if a SFX is playing
RAM_SFXDelay:       ds 1              ; How many ticks to wait
RAM_SFXBank:        ds 1              ; Current SFX bank
RAM_SFXAddress:     ds 2              ; Current SFX address

RAM_Locked:         ds 12           ; Locked channels

RAM_Scratch:        ds 32           ; Scratch bytes, may be useful when
                                      ; buffering to speed up to avoid bank
                                      ; switching conflicts

;****************************************************************************
; Pointer list starts being stored from here
; $300 (768) bytes are needed to store the pointer list
;
; Format for a pointer list entry is as follows:
;   RAM_PointerList[$000+n] = address high
;   RAM_PointerList[$100+n] = address low
;   RAM_PointerList[$200+n] = bank
;****************************************************************************

    ds $100-($&$FF), $FF

RAM_PointerList: equ $1C00

;****************************************************************************
; 68000 communication variables
;****************************************************************************

RAM_Stack:          equ $1FF0       ; Where stack starts

RAM_Status:         equ $1FF0       ; Current playback status
RAM_Command:        equ $1FFF       ; Command type
RAM_ComAddr:        equ $1FFD       ; Command address parameter
RAM_ComBank:        equ $1FFC       ; Command bank parameter
