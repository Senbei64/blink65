;- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

.export _variant_isr, _variant_irq_vec
.import _variant_system_isr, _variant_time_ms
.include "vic20.inc"


;- GLOBAL VARIABLES ----------------------------------------------------------
_variant_irq_vec = IRQVec


;- GLOBAL FUNCTIONS ----------------------------------------------------------
.code

.proc _variant_isr
    ;
    ; VIA 2 TIMER 1
    ;

    ; TODO: if doing PWM trigger a oneshot SR out

    ; keep track of passing time
    inc _variant_time_ms
    .repeat 3, i
    bne check_system_isr
    inc _variant_time_ms+i+1
    .endrepeat

check_system_isr:
    ; jump to system ISR once every 16 milliseconds
    lda _variant_time_ms
    and #$0F
    beq system_irq

    ; clear all pending irq requests from VIA 2
    lda #$7F
    sta VIA2_IFR

    ; epilogue of system ISR
    jmp $EB18

system_irq:
    ; jump to system ISR
    jmp (_variant_system_isr)
.endproc

;-----------------------------------------------------------------------------
