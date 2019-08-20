;------------------------------------------------------------------------------
;       CSTARTUP source for RL78
;
;       This module contains the code executed before the C/C++ "main"
;       function is called.
;
;       The code usually must be tailored to suit a specific hardware
;       configuration.
;
;       Assembler options:
;
;       -D__NEAR_MODEL__        To assemble for use with compiler near
;                               code model.
;
;       -D__FAR_MODEL__         To assemble for use with compiler far
;                               code model.
;
;------------------------------------------------------------------------------
;       Copyright 2011 IAR Systems AB.
;       $Revision: 4194 $
;------------------------------------------------------------------------------

#if !defined(__NEAR_MODEL__) && !defined(__FAR_MODEL__)
  #error One of the macros __NEAR_MODEL__ or __FAR_MODEL__ must be defined !
#endif

#include "cfi.m"

;------------------------------------------------------------------------------
;       The stack segment.
;       The stack size is defined in the linker command file
;------------------------------------------------------------------------------

        MODULE  ?CSTARTUP

        SECTION    CSTACK:DATA(1)


;------------------------------------------------------------------------------
;       The interrupt vector segment.
;       Interrupt functions with defined vectors will reserve
;       space in this area as well as conformingly written assembly
;       language interrupt handlers
;------------------------------------------------------------------------------

;        SECTION  .intvec:CODE:ROOT(1)
;        DATA16
;        DC16    __iar_program_start                 ; Reset vector
; hurui add 2017
#if (1)
        ASEGN  .intvec:CODE, 0x0000
        DW      __iar_program_start         ;RST_vect                 (0x00)
        DW      0FFFFH;INTDBG_vect              (0x02)
        DW      02020H;INTWDTI_vect             (0x04)
        DW      02030H;INTLVI_vect              (0x06)
        DW      02040H;INTP0_vect               (0x08)
        DW      02050H;INTP1_vect               (0x0A)
        DW      02060H;INTP2_vect               (0x0C)
        DW      02070H;INTP3_vect               (0x0E)
        DW      02080H;INTP4_vect               (0x10)
        DW      020A0H;INTP5_vect               (0x12)
        
        DW      020C0H;INTST2_vect               (0x14)
        DW      020E0H;INTSR2_vect               (0x16)
        DW      02100H;INTSRE2_vect               (0x18)
        
        DW      02110H;INTCR_vect               (0x1A)
        DW      02120H;INTAES_vect/INTAESF       (0x1C)
        DW      02130H;INTST0                    (0x1E)
        DW     02140H;INTIICA0                   (0x20)
        DW     02150H;INTSR0                  (0x22)
        
        DW     02160H;INTSRE0                 (0x24)
        DW     02170H;INTST1               (0x26)
        DW     02180H;INTSR1                (0x28)
        DW     02190H;INTSRE1              (0x2A)
        DW     021A0H;INTTM00             (0x2C)
        
#endif
 
;------------------------------------------------------------------------------
;       The actual startup code
;
;       Entry:  __program_start
;------------------------------------------------------------------------------

        SECTION    .text:CODE:ROOT(0)

        PUBLIC  `@cstart`             ; Renesas debugger specific
        PUBLIC  __iar_program_start
        
        EXTERN  ___low_level_init
        EXTERN  _NEAR_CONST_LOCATION
PMC     DEFINE  0xFFFFE

        REQUIRE __MAIN_CALL

          CFI Block cfiBlock0 Using cfiCommon2
          CFI ?RET Undefined
          ;CFI CFA SP
          CFI Function __iar_program_start
          CALL_GRAPH_ROOT __iar_program_start, "Reset"
;------------------------------------------------------------------------------
;       Perform the run-time initialization.
;------------------------------------------------------------------------------

__iar_program_start:
`@cstart`:
        CODE
        DI

        MOV     A, #(_NEAR_CONST_LOCATION & 1)  ; Near/Far, set mirror area
        MOV1    CY, A.0
        MOV1    PMC.0, CY
        
        MOVW    SP, #LWRD(sfe(CSTACK))


        ; Init stack segment for as the generated code may sometimes
        ; access the 4th byte of a return address before it is initialized
        MOVW    HL, #LWRD(sfb(CSTACK))
        MOVW    BC, #LWRD(sizeof(CSTACK))
        CMP0    C
        SKZ
        INC     B
        MOV     A, #0xCD        ; 0xCD to fool C-SPY's stack limit check
loop_s:
        MOV     [HL], A
        INCW    HL
        DEC     C
        BNZ     loop_s
        DEC     B
        BNZ     loop_s

        MOV     CS, #0

;------------------------------------------------------------------------------
;       Here is the place to put user initializations.
;------------------------------------------------------------------------------

;       User initialization code

;------------------------------------------------------------------------------
;       Call __low_level_init to perform initialization before initializing
;       segments and calling main.
;       If the function returns 0, no segment initialization should take place.
;
;       Link with your own version of __low_level_init to override the
;       default action: to do nothing but return 1.
;------------------------------------------------------------------------------

        CFI FunCall ___low_level_init
__iar_init$$needed:
#if defined(__USE_FAR_RT_CALLS__) || defined(__FAR_MODEL__)
        CALL    F:___low_level_init
#else
        CALL    ___low_level_init
#endif
        OR      A, X
        CFI TailCall __MAIN_CALL
        BZ      __MAIN_CALL



;------------------------------------------------------------------------------
;       Segment initialization
;
;------------------------------------------------------------------------------

        extern  ___iar_data_init2
        CFI FunCall ___iar_data_init2
        CALL	F:___iar_data_init2
        CFI EndBlock cfiBlock0

;------------------------------------------------------------------------------
;       Segment initialization
;
;       WRKSEG short address work area is filled with zero
;------------------------------------------------------------------------------


        SECTION    .wrkseg:DATA(0)
        SECTION    .text:CODE:NOROOT(0)

        CFI Block cfiBlockW Using cfiCommon2
        CFI Function __iar_program_start
        PUBLIC  __INIT_WRKSEG

__INIT_WRKSEG:

        MOVW    HL, #LWRD(sfb(.wrkseg))
        MOV     B, #sizeof(.wrkseg)
        CLRB    A
loop_w:
        MOV     [HL], A
        INCW    HL
        DEC     B
        BNZ     loop_w
        CFI EndBlock cfiBlockW


;------------------------------------------------------------------------------
;       Initialize constructors
;
;       This segment part is required by the compiler when it is
;       necessary to call constructors of global objects.
;------------------------------------------------------------------------------

        SECTION .preinit_array:CODE(1)
        SECTION .init_array:CODE(1)
        SECTION .text:CODE:NOROOT(0)
        CFI Block cfiBlock1 Using cfiCommon2
        CFI Function __iar_program_start


        EXTERN  ___call_ctors
        PUBLIC  __INIT_CTORS
        
__INIT_CTORS:
__iar_init$$done1:
        MOV     X,  #byte3(sfe(.init_array))
        PUSH    AX
        MOVW    AX, #lwrd(sfe(.init_array))
        PUSH    AX
        MOV     X,  #byte3(sfb(.preinit_array))
        PUSH    AX
        MOVW    AX, #lwrd(sfb(.preinit_array))
        PUSH    AX
        CFI FunCall ___call_ctors
        CALL    F:___call_ctors
        CFI EndBlock cfiBlock1


;------------------------------------------------------------------------------
;       Enter main
;
;       Call the actual "main" function
;------------------------------------------------------------------------------


        SECTION    .text:CODE:NOROOT(0)

        PUBLIC  `@cend`             ; Renesas debugger specific
        PUBLIC  __program_end
        EXTERN  _main
        EXTERN  _exit
        CFI Block cfiBlock2 Using cfiCommon2
        CFI ?RET Undefined
        CFI CFA SP
        CFI Function __iar_program_start

__MAIN_CALL:
__iar_init$$done2:
        CFI FunCall _main
#if defined(__FAR_MODEL__)
        CALL    F:_main
        CFI FunCall _exit
        CALL    F:_exit
#else
        CALL    _main
        CFI FunCall _exit
        CALL    _exit
#endif
__program_end:
`@cend`:
        CFI EndBlock cfiBlock2

;       STOP                            ; Should not return


        END
