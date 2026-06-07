;- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

.export _variant_isr, _variant_irq_vec : absolute
.import _variant_system_isr, _variant_time_ms
.include "pet.inc"


;- GLOBAL VARIABLES ----------------------------------------------------------
_variant_irq_vec = IRQVec


;- GLOBAL FUNCTIONS ----------------------------------------------------------
.code

.proc _variant_isr
    ; determine active VIA interrupts
    lda VIA_IFR
    and VIA_IER

    ; if none, it's a system interrupt, vertical blanking
    beq system_irq

    ; throw master flag, place timer 1 flag in sign
    asl
    bpl clear

    ;
    ; VIA TIMER 1
    ;

    ; TODO: if doing PWM trigger a oneshot SR out

    ; keep track of passing time
    inc _variant_time_ms
    .repeat 3, i
    bne clear
    inc _variant_time_ms+i+1
    .endrepeat
        
clear:
    ; clear all pending irq requests
    lda #$7F
    sta VIA_IFR

    pla
    tay
    pla
    tax
    pla
    rti

system_irq:
    ; Jump to system ISR
    jmp (_variant_system_isr)
.endproc

;-----------------------------------------------------------------------------
