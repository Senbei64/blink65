;- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

.export _variant_isr, _variant_irq_vec
.import _variant_system_isr, _variant_time_ms
.include "c64.inc"


;- GLOBAL VARIABLES ----------------------------------------------------------
_variant_irq_vec := IRQVec


;- GLOBAL FUNCTIONS ----------------------------------------------------------
.code

.proc _variant_isr
    ;
    ; CIA 1 TIMER B
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

    ; epilogue of system ISR: clear irq requests from CIA 1, restore registers
    jmp $EA7E

system_irq:
    ; jump to system ISR
    jmp (_variant_system_isr)
.endproc

;-----------------------------------------------------------------------------
