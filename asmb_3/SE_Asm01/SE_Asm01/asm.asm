.586P
.MODEL FLAT, STDCALL

includelib kernel32.lib

ExitProcess PROTO :DWORD
MessageBoxA PROTO :DWORD, :DWORD, :DWORD, :DWORD

.STACK 4096

.CONST
    MB_OK EQU 0 

.DATA
    MsgCaption DB "Антипов Алексей Романович, 2 курс, 9 группа", 0
    MsgText DB "AH=XXh, AL=YYh, Сумма EAX: XX, Флаг EBX: X", 0 
    
    myBytes BYTE 10h, 20h, 30h, 40h
    myWords WORD 8Ah, 3Bh, 44h, 5Fh, 99h
    myDoubles DWORD 1, 2, 3, 4, 5, 6
    IntArray32 DWORD 3, 13, 9, 1, 2, 17, 32 
    myPointer DWORD myDoubles
    
    AH_Offset EQU 3         
    AL_Offset EQU 11        
    SumOffset EQU 27        
    FlagOffset EQU 41       
    
    AH_Value BYTE ?
    AL_Value BYTE ?
    ZeroFoundFlag DWORD 1

.CODE

DecToTwoDigit PROC
    push EAX
    push EBX
    push EDX
    
    
    mov BL, 10            
    mov AH, 0             
    
    div BL                
    
    add AL, '0'           
    mov [ESI], AL         
    
    mov AL, AH            
    add AL, '0'           
    mov [ESI + 1], AL     
    
    pop EDX
    pop EBX
    pop EAX
    ret
DecToTwoDigit ENDP

ByteToHex PROC
    push EAX
    push EBX

    mov BL, AL
    shr AL, 4
    call ConvertNibble
    mov [ESI], AL

    mov AL, BL
    and AL, 0Fh
    call ConvertNibble
    mov [ESI + 1], AL

    pop EBX
    pop EAX
    ret
ByteToHex ENDP

ConvertNibble PROC
    cmp AL, 9
    jle IsDigit
    add AL, 'A' - 10
    jmp Done
IsDigit:
    add AL, '0'
Done:
    ret
ConvertNibble ENDP


main PROC
    mov EAX, 0                  
    mov ESI, OFFSET IntArray32
    mov ECX, 7
    mov [ZeroFoundFlag], 1      
    
LOOP_START:
    add EAX, [ESI]
    
    cmp DWORD PTR [ESI], 0      
    je ZERO_FOUND_AND_CONT      
    
    add ESI, 4                  
    loop LOOP_START
    
    jmp AFTER_LOOP              

ZERO_FOUND_AND_CONT:
    mov [ZeroFoundFlag], 0      
    
    add ESI, 4
    loop LOOP_START             
    
AFTER_LOOP:
    mov EBX, [ZeroFoundFlag]    
    
    push EAX                   
    push EBX                   
    
    mov ESI, OFFSET myBytes
    mov AH, [ESI]
    mov AL, [ESI + 3]
    
    mov AH_Value, AH
    mov AL_Value, AL

    mov AL, AH_Value
    mov ESI, OFFSET MsgText
    add ESI, AH_Offset
    call ByteToHex

    mov AL, AL_Value
    mov ESI, OFFSET MsgText
    add ESI, AL_Offset
    call ByteToHex
    
    
    pop EBX                    
    pop EAX                    

    mov ESI, OFFSET MsgText
    add ESI, SumOffset         
    call DecToTwoDigit         

    mov ESI, OFFSET MsgText
    add ESI, FlagOffset        
    
    mov AL, BL                 
    call ConvertNibble         
    mov [ESI], AL               
    
    
    INVOKE MessageBoxA, 0, OFFSET MsgText, OFFSET MsgCaption, MB_OK
    
    INVOKE ExitProcess, 0
main ENDP
end main