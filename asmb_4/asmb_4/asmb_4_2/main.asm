.586P
.MODEL FLAT, STDCALL

.stack 4096

ExitProcess PROTO, dwExitCode:DWORD

.data
    var_int1 dd 123456789
    var_str2 db 'Hello, ASM!', 0

.code
main PROC
    mov eax, var_int1
    lea ebx, var_str2
    
    add eax, 1

    invoke ExitProcess, 0
main ENDP
END main