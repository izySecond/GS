;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;* File Name          : main.s
;* Author             : Silke Behn	
;* Version            : V1.0
;* Date               : 01.06.2021
;* Description        : This is a simple main.
;					  :
;					  : Replace this main with yours.
;
;*******************************************************************************
    EXTERN initITSboard
    EXTERN lcdPrintS            ;Display ausgabe
	EXTERN lcdPrintC	
	EXTERN lcdPrintlnS	
	EXTERN lcdGotoXY	
    EXTERN GUI_init
	EXTERN TP_Init
    EXTERN initTimer



;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	
	AREA MyData, DATA, align = 2
	
	    
DEFAULT_BRIGHTNESS DCW  800

;Adressen aus der CMSIS
;\stm32cubef4\Drivers\CMSIS\Device\ST\STM32F4xx\Include\stm32f429xx.h


; setup the peripherie - Mapping the GPIO
PERIPH_BASE     	equ	0x40000000                 
AHB1PERIPH_BASE 	equ	(PERIPH_BASE + 0x00020000)	
GPIO_D_BASE			equ	(AHB1PERIPH_BASE + 0x0C00)	
GPIO_E_BASE          equ (AHB1PERIPH_BASE + 0x1000)	
GPIO_F_BASE          equ (AHB1PERIPH_BASE + 0x1400)



GPIO_D_PIN			equ	(GPIO_D_BASE + 0x10)
GPIO_D_SET			equ (GPIO_D_BASE + 0x18)
GPIO_D_CLR			equ	(GPIO_D_BASE + 0x1A)	 


GPIO_F_PIN           equ (GPIO_F_BASE + 0x10) 	


APB1PERIPH_BASE     equ  PERIPH_BASE
TIM2_BASE           equ (APB1PERIPH_BASE + 0x0000)

TIMER				equ (TIM2_BASE + 0x24)   ; CNT : current time stamp (32 bit),  resolution
TIM2_CR1            equ (TIM2_BASE)
TIM2_CNT			equ (TIM2_BASE + 0x24)   ; CNT : current time stamp (32 bit),  resolution
TIM2_PSC			equ (TIM2_BASE + 0x28)   ; Prescaler  resolution
TIM2_EGR			equ (TIM2_BASE + 0x14)   ; 16 Bit register, Bit 0 : 1 Restart Timer	
TIM2_ARR            equ (TIM2_BASE + 0x2C)
	
COORDINATEX			equ 10
COORDINATEY			equ 5
	
TEXTRUN				DCB "Running", 0	
TEXTINIT			DCB "00:00:00", 0
TEXTHOLD			DCB "Hold", 0
TEXTTEST			DCB	"test int to start", 0

SHIFTINIT			equ	5
SHIFTHOLD			equ 6
SHIFTRUN			equ 7

INIT				equ	1
RUN					equ 2
HOLD				equ 3

COMPAREONE			equ 1
COMAPREONED			equ 3
	
SHIFTLED			equ 0
TIMMYSFIELD			fill 9*1, 0
DOPPELPUNKT			DCB ':'

HUNDERT	equ 100

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
main	PROC
        BL initITSboard
		ldr r1, =DEFAULT_BRIGHTNESS
		ldrh r0, [r1]
		bl GUI_init
		mov r0, #0x00
		bl TP_Init
		bl def_set_base
		mov r4, #INIT
		ldr r0, =TEXTINIT
		ldr r9, =1
		bl lcdPrintS
		
		
superloop		

check_time						cmp r4, #RUN
								beq print_time
								b continue_superloop
print_time						ldr		R3, =TIMER
								ldr		R1, [R3]
		; R2 die Zeit in hunderstel sekunden 
		; ganzen timer in hunderstel sekunden berechnen
		; R7 zur berchenung der hunderstel, und so 
								ldr		R11, =TIMMYSFIELD
								ldr 	R5, =DOPPELPUNKT
								ldr		R10, [R5]
								strb	R10, [R11, #2]
								strb	R10, [R11, #5]
		;konstanten für die Berechnung von Hunderstelsekunden werden in Register geladen
								ldr		R2, =1000 ;werden die hunderstel sekunden berechnet
								udiv	R2, R1, R2 
		
		;berechnung der sekunden des timers gerundet
								ldr		R10, =HUNDERT
								udiv	R3, R2, R10
		
		; berechnung der hunderstel sekunde der zeit
		;ldr		R8, =HUNDERSTELSEK
								mul		R6, R3, R10
								sub		R7, R2, R6
								bl		hex_to_ascii
								strh	R5, [R11, #6]
		
		; berechnung von min
		;ldr		R8, =MIN
								ldr		R8, =60
								udiv	R7, R3, R8
								bl		hex_to_ascii
								strh	R5, [R11, #0]
		
		; berechnung der sek
		;ldr 	R8, =SEK
								mul		R6, R7, R8
								sub		R7, R3, R6
								bl		hex_to_ascii
								strh	R5, [R11, #3]
		
		; to string
								ldr		R0, =TIMMYSFIELD
								bl 		lcdPrintS
								
continue_superloop				bl def_set_base
								bl def_check_taster
								bl def_set_base
								bl def_update_timmy
								bl def_update_led
								b superloop
				

		ENDP
		
ALIGN
def_update_timmy PROC
	PUSH {r2, LR}


;case INIT
entry_init			mov r5, #INIT
					cmp r4, r5
					beq reset_time
					cmp r9, #1
					beq start_timmy
					b entry_run
reset_time			ldr r0, =TEXTINIT
					bl lcdPrintS
					b entry_is_set
start_timmy			bl def_restart_timer
					b entry_is_set
					
;case RUN
entry_run			cmp r4, #RUN
					beq run_timmy_run
					cmp r4, #HOLD
					beq entry_hold
					cmp r4, #INIT
					beq entry_init
					b entry_is_set
					
run_timmy_run		
					b entry_is_set

;case HOLD
entry_hold			mov r5, #HOLD
					cmp r4, r5
					beq stop_timedisplay
					cmp r4, #RUN
					beq run_timmy_run
					cmp r4, #INIT
					beq entry_init
					b entry_is_set
					
stop_timedisplay	
					b entry_is_set
entry_is_set

	POP {r2, PC}
		ENDP

def_restart_timer		PROC
	PUSH {r1, LR}
		ldr r9, =0
		bl  	initTimer
		ldr 	R1,=TIM2_PSC   			; Set pre scaler such that 1 timer tick represents 10 us
		mov 	R0,#(90*10-1) 
		strh	R0,[R1]
		ldr 	R1,=TIM2_EGR   			; Restart timer	
		mov	    R0,#0x01
        strh    R0, [R1]
		b entry_is_set
	POP{r1, LR}
					ENDP
	
hex_to_ascii 	PROC
	PUSH {R4, R2, R3, LR}

		ldr R2, =10
		udiv R1, R7, R2
		mul R8, R1, R2
		sub	R5, R7, R8
		add R1, #48
		add R5, #48
		lsl R5, #8
		orr R5, R1
		bx lr
		
	pop{R4, R2, R3, PC}
				ENDP
		

def_set_base 	PROC
			PUSH {r0, LR}
					ldr r0, =COORDINATEX
					ldr r1, =COORDINATEY
					bl lcdGotoXY

			POP {r0, PC}
			ENDP


def_check_taster PROC
		PUSH {r1, LR}
load_switch			ldr r0, =GPIO_F_PIN
					ldrb r1, [r0]
check_init 			mov r2, #COMPAREONE << SHIFTINIT
					and r3, r1, r2
					cmp r3, r2
					bne set_register_init
					b check_run
set_register_init	mov r4, #INIT
					mov r9, #1
					b end_check

check_run			mov r2, #COMPAREONE << SHIFTRUN
					and r3, r1, r2
					cmp r3, r2
					bne set_register_run
					b check_hold
set_register_run	mov r4, #RUN
					b end_check
					
check_hold			mov r2, #COMPAREONE << SHIFTHOLD
					and r3, r1, r2
					cmp r3, r2
					bne set_register_hold
					b end_check
set_register_hold	mov r4, #HOLD
					b end_check
end_check

		POP {r1, PC}
				ENDP


def_update_led PROC
		PUSH {r1, LR}
					ldr r1, =GPIO_D_SET
					ldr r2, [r1]
					ldr r5, =GPIO_D_CLR
					mov r6, #COMAPREONED
					str r6, [r5]

check_led_init		
					cmp r4, #INIT
					beq led_init
					b check_led_hold
led_init			
					ldr r3, =0x00000000
					str r3, [r1]
					b end_update_led

check_led_hold		
					cmp r4, #HOLD
					beq led_hold
					b check_led_run
led_hold			
					mov r3, #COMAPREONED << SHIFTLED
					orr r2, r3
					str r2, [r1]
					b end_update_led

check_led_run		
					cmp r4, #RUN
					beq led_run
					b end_update_led
led_run				
					mov r3, #COMPAREONE << SHIFTLED
					orr r2, r3
					str r2, [r1]
					b end_update_led
					
end_update_led
				
		POP {r1, PC}
				ENDP
		

		ALIGN
		END
