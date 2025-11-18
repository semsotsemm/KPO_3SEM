.586P
.MODEL FLAT, STDCALL

includelib kernel32.lib

ExitProcess PROTO :DWORD
MessageBoxA PROTO :DWORD, :DWORD, :DWORD, :DWORD

.STACK 4096

.CONST
    MB_OK EQU 6

.DATA
    MsgCaption DB "Лабораторная работа MASM", 0
    MsgText    DB "Результат сложения = X", 0
    
.CODE
    main PROC
        START:
            MOV EAX, 3d
            MOV EBX, 4d
    
            ADD EAX, EBX
            ADD EAX, '0'

            MOV [MsgText + 21], AL 

            PUSH MB_OK              
            PUSH OFFSET MsgCaption  
            PUSH OFFSET MsgText     
            PUSH 0                  
            CALL MessageBoxA        
    
            PUSH 0  
            CALL ExitProcess
    
        main ENDP                                   
    end main