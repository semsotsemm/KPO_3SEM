.586
.MODEL FLAT, stdcall

includelib kernel32.lib

GetStdHandle PROTO : DWORD
WriteConsoleA PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.DATA
    chars_written DWORD ?

.CODE

PUBLIC getmin, getmax, int_to_char, printconsole 
getmin PROC ad : DWORD, len : DWORD
    mov esi, ad
    mov ecx, len
    
    cmp ecx, 0
    jz end_getmin
    
    mov eax, [esi]
    dec ecx
    jz end_getmin
    
    add esi, 4
    
next_element_min:
    mov ebx, [esi]
    cmp ebx, eax
    jl new_min

    skip_min:
    add esi, 4
    dec ecx
    jnz next_element_min
    
    jmp end_getmin

new_min:
    mov eax, ebx
    jmp skip_min

end_getmin:
    ret
getmin ENDP


getmax PROC ad : DWORD, len : DWORD
    mov esi, ad
    mov ecx, len

    cmp ecx, 0
    jz end_getmax

    mov eax, [esi]
    dec ecx
    jz end_getmax
    
    add esi, 4

next_element_max:
    mov ebx, [esi]
    cmp ebx, eax
    jg new_max

    skip_max:
    add esi, 4
    dec ecx
    jnz next_element_max
    
    jmp end_getmax

new_max:
    mov eax, ebx
    jmp skip_max

end_getmax:
    ret
getmax ENDP


int_to_char PROC uses eax ebx ecx edi esi, pstr : dword, intfield : sdword
    mov edi, pstr
    mov esi, 0
    mov eax, intfield
    
    test eax, 80000000h
    jz plus
    
    neg eax
    mov ebx, 10
    mov cl, '-'
    mov byte ptr [edi], cl
    inc edi
    
plus:
    mov ebx, 10
    
convert_loop:
    xor edx, edx
    idiv ebx
    push dx 
    inc esi
    test eax, eax
    jnz convert_loop

fin:
    mov ecx, esi
    
write:
    pop bx
    add bl, '0'
    mov byte ptr [edi], bl
    inc edi
    loop write
    
    mov byte ptr [edi], 0
    ret
int_to_char ENDP


printconsole proc uses eax ebx ecx edi esi, pstr :dword
    push -11
    call GetStdHandle
    mov ebx, eax
    
    mov esi, pstr
    mov edi, 0
    
    count_len:
        cmp byte ptr [esi + edi], 0
        je done_count
        inc edi
        jmp count_len
    done_count:
    
    push 0
    push OFFSET chars_written
    push edi
    push esi
    push ebx
    call WriteConsoleA
    
    ret
printconsole ENDP

END