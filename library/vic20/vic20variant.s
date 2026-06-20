;- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

.export _variant_isr, _variant_irq_vec
.import _variant_system_isr, _variant_time_ms
.import _vic20_pwm_width, _vic20_pwm_pattern
.include "vic20.inc"


;- GLOBAL VARIABLES ----------------------------------------------------------
_variant_irq_vec = IRQVec


;- GLOBAL FUNCTIONS ----------------------------------------------------------
.code

.proc _variant_isr
    ;
    ; VIA 2 TIMER 1
    ;

    ; if doing PWM on pin L trigger a oneshot pulse
    ldx _vic20_pwm_width+1
    ; sign bit set means no PWM
    bmi end_pwm_l
    ; acknowledge previous one-shot
    ldy #$40
    sty VIA1_IFR
    ; load counter and start new one-shot
    lda _vic20_pwm_width
    sta VIA1_T1CL
    stx VIA1_T1CH
end_pwm_l:

    ; if doing PWM on pin M trigger a oneshot SR out
    lda _vic20_pwm_pattern
    ; zero means no PWM
    beq end_pwm_m
    sta VIA1_SR
end_pwm_m:

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
