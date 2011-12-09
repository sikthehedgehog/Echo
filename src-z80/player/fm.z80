;****************************************************************************
; NoteOnFM
; Does a "note on" for a FM channel
;****************************************************************************

NoteOnFMSFX:
    call NoteOnFM               ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

NoteOnFMBGM:
    ld b, a
    PollPCM

    push hl
    ld a, b
    and $07                     ; Check if channel is free
    ld hl, RAM_Locked
    add l
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip1      ; Don't play if locked

    ld a, b
    call NoteOnFM               ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

NoteOnFM:
    and $07                     ; Get channel ID
    ld (ix+0), $28              ; Note off
    ld (ix+1), a

    ld b, a
    ex af, af'
    PollPCM

    ld a, b                     ; Determine which port to write
    and $04
    rrca
    ld iyl, a

    PollPCM
    call GetParam               ; Get note
    PollPCM

    ex af, af'
    push de
    push hl

    ld d, a
    and $03                     ; Index of first frequency register
    add $A4
    ld e, a

    PollPCM

    ld h, FMFreqTable>>8        ; Get address of note
    ld a, b
    and $1F
    add FMFreqTable&$FF
    ld l, a

    PollPCM

    ld a, b                     ; Set new frequency
    and $E0
    rrca
    rrca
    ld b, a
    ld a, (hl)
    or b
    ld (iy+0), e
    ld (iy+1), a

    PollPCM

    ld a, e
    sub 4
    ld e, a
    dec l
    ld a, (hl)
    ld (iy+0), e
    ld (iy+1), a

    PollPCM

    ld a, d                     ; Note on!
    or $F0
    ld (ix+0), $28
    ld (ix+1), a

    pop hl
    pop de
    ret                         ; End of subroutine

;****************************************************************************
; NoteOffFM
; Does a "note off" for a FM channel
;****************************************************************************

NoteOffFMSFX:
    call NoteOffFM              ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

NoteOffFMBGM:
    ld b, a
    PollPCM
    ld a, b

    push hl
    and $07                     ; Check if channel is free
    ld hl, RAM_Locked
    add l
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip       ; Don't stop if locked

    ld a, b
    call NoteOffFM              ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

NoteOffFM:
    and $07                     ; Get channel ID
    ld (ix+0), $28              ; Note off
    ld (ix+1), a
    ret                         ; End of subroutine

;****************************************************************************
; SetFMNote*
; Sets the note of a FM channel without "note on"
;****************************************************************************

SetNoteFMSFX:
    call SetNoteFM              ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

SetNoteFMBGM:
    ld b, a
    PollPCM
    ld a, b

    push hl
    and $07                     ; Check if channel is free
    ld hl, RAM_Locked
    add l
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip2      ; Don't change if locked

    ld a, b
    call SetNoteFM              ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

SetNoteFM:
    push af
    ld b, a
    PollPCM

    ld a, b                     ; Determine which port to write
    and $04
    rrca
    ld iyl, a

    PollPCM
    call GetParam               ; Get high byte
    PollPCM

    pop af
    push de

    and $07
    ld d, a
    and $03                     ; Index of first frequency register
    add $A4
    ld e, a

    PollPCM

    ld (iy+0), e                ; Load high byte
    ld (iy+1), b

    PollPCM                     ; Load low byte
    call GetParam
    PollPCM

    ld a, e
    sub 4
    ld e, a

    ld (iy+0), e
    ld (iy+1), b

    PollPCM
    pop de
    ret                         ; End of subroutine

;****************************************************************************
; LoadFM*
; Loads a FM instrument
;****************************************************************************

LoadFMSFX:
    call LoadFMEvent            ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

LoadFMBGM:
    and $07                     ; Get channel ID
    ld b, a
    PollPCM

    push de
    push bc
    ld a, b
    ld de, RAM_BGMFMInstr       ; Store instrument ID
    add e
    ld e, a
    PollPCM
    call GetParam
    PollPCM
    ld a, b
    ld (de), a
    
    ld a, e                     ; Reset volume
    add 8
    ld e, a
    xor a
    ld (de), a
    
    ld e, c
    pop bc
    ld c, e
    pop de

    PollPCM

    push hl                     ; Check if channel is free
    ld a, b
    ld h, RAM_Locked>>8
    add RAM_Locked&$FF
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip       ; Don't load if locked

    PollPCM

    ld a, b                     ; Get back instrument ID
    push hl
    ld hl, RAM_BGMFMInstr
    add l
    ld l, a
    ld b, (hl)
    pop hl

    ex af, af'
    PollPCM
    ex af, af'

    call LoadFMDirect           ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

LoadFMEvent:
    and $07                     ; Get channel ID

    ex af, af'
    PollPCM
    call GetParam               ; Get instrument ID
    PollPCM
    ex af, af'
LoadFMDirect:

    push af
    and $04                     ; Determine which port to write
    rrca
    ld iyl, a
    PollPCM
    pop af

    push bc
    push de
    push hl

    ld h, RAM_PointerList>>8    ; Get instrument address
    ld l, b
    ld d, (hl)
    inc h
    ld e, (hl)
    inc h
    ld c, (hl)
    ex de, hl

    push af
    PollPCM
    
    ld b, 28/4                  ; Load FM instrument into our scratch buffer,
    ld de, RAM_Scratch            ; so we don't conflict with PCM bank
.getinstr:                        ; switch (which would slow down A LOT)
    ld a, b
    ex af, af'
    call GetParam
    ex de, hl
    ld (hl), b
    ex de, hl
    inc e
    call GetParam
    ex de, hl
    ld (hl), b
    ex de, hl
    inc e
    call GetParam
    ex de, hl
    ld (hl), b
    ex de, hl
    inc e
    call GetParam
    ex de, hl
    ld (hl), b
    ex de, hl
    inc e
    ex af, af'
    ld b, a
    djnz .getinstr
    call GetParam
    ex de, hl
    ld (hl), b
    ex de, hl

    PollPCM
    
    pop af                      ; Kill ADSR
    ld b, a
    call KillFM
    ld a, b

    ld de, RAM_FMData           ; Get address of FM data
    and $07
    add e
    ld e, a

    push af
    and $03                     ; Register index for operator
    add $B0
    
    ld hl, RAM_Scratch          ; Get address of buffer with the instrument
                                  ; data
    ex af, af'
    PollPCM                     ; Write operator
    ex af, af'
    ld (iy+0), a
    ld b, (hl)
    inc l
    ld (iy+1), b

    ex af, af'                  ; Store $B0 register in FM data buffer
    ld a, b
    ld (de), a
    ld a, e
    add 8
    ld e, a
    PollPCM
    ex af, af'

    sub $B0-$30

    ex af, af'                  ; Write registers $30-$3C
    PollPCM
    ex af, af'
    
    ld (iy+0), a
    ld b, (hl)
    nop
    ld (iy+1), b
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld b, (hl)
    nop
    ld (iy+1), b
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld b, (hl)
    nop
    ld (iy+1), b
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld b, (hl)
    nop
    ld (iy+1), b
    add 4
    inc l
    nop

    ld b, 4                     ; Write registers $40-$4C
.reg40:
    ld c, a
    push af
    PollPCM
    
    ld (iy+0), c
    ld a, (hl)
    ld (de), a
    ld (iy+1), a
    ld a, e
    add 8
    ld e, a
    
    pop af
    add 4
    inc l
    djnz .reg40

    ld b, 20/4                  ; Write registers $50-$5C
.reg50:
    ex af, af'
    PollPCM
    ex af, af'
    
    ld (iy+0), a
    ld c, (hl)
    nop
    ld (iy+1), c
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld c, (hl)
    nop
    ld (iy+1), c
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld c, (hl)
    nop
    ld (iy+1), c
    add 4
    inc l
    nop
    
    ld (iy+0), a
    ld c, (hl)
    nop
    ld (iy+1), c
    add 4
    inc l
    
    djnz .reg50

    PollPCM
    
    ex de, hl
    ld a, l
    sub 8*6
    ld l, a
    ld b, (hl)
    add 8
    ld l, a
    
    PollPCM

    pop af
    pop hl
    pop de
    pop bc
    ret

;****************************************************************************
; SetFMVol*
; Sets the volume of a FM channel
;****************************************************************************

SetFMVolSFX:
    call SetFMVolSFXEvent       ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

SetFMVolBGM:
    and $07
    ld b, a

    PollPCM

    push de
    push bc
    ld a, b
    ld d, RAM_BGMFMVol>>8        ; Store BGM volume
    add RAM_BGMFMVol&$FF
    ld e, a
    PollPCM
    call GetParam
    PollPCM
    ex de, hl
    ld (hl), b
    ex de, hl
    ld e, c
    pop bc
    ld c, e
    pop de

    PollPCM

    push hl
    ld a, b                     ; Check if channel is free
    ld hl, RAM_Locked
    add l
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip       ; Don't change if locked

    PollPCM

    ld a, b
    push hl
    ld hl, RAM_BGMFMVol
    add l
    ld l, a
    ld b, (hl)
    pop hl

    push af
    PollPCM
    pop af

    call SetFMVolLoad           ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

SetFMVolSFXEvent:
    push af
    PollPCM
    call GetParam               ; Get new volume
    PollPCM
    pop af

SetFMVolLoad:
    push bc
    push de
    push hl
    
    and $07
    
    push af
    ld h, RAM_FMData>>8         ; Get address of FM data
    add RAM_FMData&$FF
    ld l, a
    
    ex af, af'
    PollPCM
    ex af, af'

    and $04                     ; Determine which port to write
    rrca
    ld iyl, a
    PollPCM
    pop af

    and $03                     ; Index of first volume register
    add $40
    ld c, a

    ld a, (hl)                  ; Get algorithm
    and $07
    ld e, a

    PollPCM

    ld a, l
    add 8
    ld l, a
    ld a, e                     ; Process operator #1
    cp $07
    jr c, .noop1
    ld (iy+0), c
    ld a, (hl)
    add b
    cp $7F
    jr c, .notooloud1
    ld a, $7F
.notooloud1:
    ld (iy+1), a
.noop1:
    ld a, c
    add 4
    ld c, a

    PollPCM

    ld a, l
    add 8
    ld l, a
    ld a, e                     ; Process operator #2
    cp $05
    jr c, .noop2
    ld (iy+0), c
    ld a, (hl)
    add b
    cp $7F
    jr c, .notooloud2
    ld a, $7F
.notooloud2:
    ld (iy+1), a
.noop2:
    ld a, c
    add 4
    ld c, a

    PollPCM

    ld a, l
    add 8
    ld l, a
    ld a, e                     ; Process operator #3
    cp $04
    jr c, .noop3
    ld (iy+0), c
    ld a, (hl)
    add b
    cp $7F
    jr c, .notooloud3
    ld a, $7F
.notooloud3:
    ld (iy+1), a
.noop3:
    ld a, c
    add 4
    ld c, a

    PollPCM

    ld a, l
    add 8
    ld l, a
    ld (iy+0), c
    ld a, (hl)                  ; Process operator #4
    add b
    cp $7F
    jr c, .notooloud4
    ld a, $7F
.notooloud4:
    ld (iy+1), a

    PollPCM

    pop hl
    pop de
    pop bc
    ret                         ; End of subroutine

;****************************************************************************
; SetFMParam*
; Sets the different parameters of a FM channel
;****************************************************************************

SetFMParamSFX:
    call SetFMParam             ; We're just a wrapper
    jp ProcessSFXRun            ; End of subroutine

SetFMParamBGM:
    ld b, a
    PollPCM
    ld a, b

    push hl
    and $07                     ; Check if channel is free
    ld hl, RAM_Locked
    add l
    ld l, a
    ld a, (hl)
    pop hl
    or a
    jp nz, ProcessBGMSkip1      ; Don't modify if locked

    ld a, b
    call SetFMParam             ; We're just a wrapper
    jp ProcessBGMRun            ; End of subroutine

SetFMParam:
    ld b, a
    PollPCM

    ld a, b                     ; Determine which port to write
    and $04
    rrca
    ld iyl, a

    ld a, b
    ex af, af'
    PollPCM
    call GetParam               ; Get parameters
    PollPCM
    ex af, af'

    and $03                     ; Get channel ID
    add $B4
    ld (iy+0), a                ; Set new parameters
    ld (iy+1), b

    ret                         ; End of subroutine

;****************************************************************************
; LockChannelFM [events $E0-$E7]
; Locks a FM channel
;****************************************************************************

LockChannelFM:
    and $07
    ld b, a
    PollPCM
    
    push hl
    ld h, RAM_Locked>>8         ; Get address of channel to lock
    ld a, b
    add RAM_Locked&$FF
    ld l, a
    ld (hl), $01                ; Lock channel
    pop hl
    
    PollPCM
    
    ld a, b                     ; Determine which port to write
    and $04
    rrca
    ld iyl, a
    
    ld a, b                     ; Reset FM parameters
    and $03
    add a
    add a
    add $B4
    ld (iy+0), a
    ld (iy+1), $C0
    
    jp ProcessSFXRun            ; End of subroutine

;****************************************************************************
; KillFM
; Kills a FM channel
;****************************************************************************

KillFM:
    and $07                     ; Get channel ID
    
    push af
    push de
    push hl
    
    ld c, a                     ; Determine to which port to write
    and $04
    rrca
    ld iyl, a
    ld a, c
    
    ld (ix+0), $28              ; Stop current note (if any)
    ld (ix+1), a
    
    and $03                     ; Load dummy FM instrument
    add $40
    ld c, 6
    ld hl, DummyFMInstr
.loaddummy:
    ex af, af'
    PollPCM
    ex af, af'
    ld e, (hl)
    
    ld (iy+0), a
    add 4
    ld (iy+1), e
    nop
    ld (iy+0), a
    add 4
    ld (iy+1), e
    nop
    ld (iy+0), a
    add 4
    ld (iy+1), e
    nop
    ld (iy+0), a
    add 4
    ld (iy+1), e
    
    inc l
    dec c
    jp nz, .loaddummy
    
    pop hl
    pop de
    PollPCM
    pop af

    ld c, a                     ; Reset ADSR
    or $F0
    ld (ix+0), $28
    ld (ix+1), a
    ld (ix+0), $28
    ld (ix+1), c

    PollPCM
    ret                         ; End of subroutine
