## X86-X64 Assembly Cheet Sheet

### General-Purpose Registers (64-bit)

| 64-bit | 32-bit | 16-bit | 8-bit | Purpose |
| --- | --- | --- | --- | --- |
| RAX | EAX | AX | AL | Return value / accumulator |
| RBX | EBX	| BX | BL | Callee-saved base |
| RCX | ECX	| CX | CL | 4th argument / counter |
| RDX | EDX	| DX | DL | 3rd argument / I/O |
| RSI | ESI	| SI | SIL | 2nd argument / source index |
| RDI | EDI	| DI | DIL | 1st argument / dest index |
| RBP | EBP	| BP | BPL | Callee-saved frame pointer |
| RSP | ESP	| SP | SPL | Stack pointer |
| R8 | R8D | R8W | R8B | 5th argument |
| R9 | R9D | R9W | R9B | 6th argument |
| R10 | R10D | R10W | R10B | Temporary |
| R11 | R11D | R11W | R11B | Temporary |
| R12-R15 | R12D-R15D | R12W-R15W | R12B-R15B | Callee-saved |

### Common Instructions

**Data Movement**

```assembly
mov rax, rbx        ; reg ← reg
mov rax, [rsp+8]    ; load from memory
mov [rsp+8], rax    ; store to memory
lea rax, [rsp+8]    ; load effective address
push rax            ; push to stack (rsp -= 8)
pop rax             ; pop from stack (rsp += 8)
```

**Arithmetic**
```assembly
add rax, rbx      ; rax += rbx
sub rax, rbx      ; rax -= rbx
imul rax, rbx     ; rax *= rbx (signed)
idiv rbx          ; rdx:rax ÷ rbx → rax=quotient, rdx=remainder
inc / dec rax     ; ++ / --
neg rax           ; rax = -rax
```

**Bitwise & Logic**
```assembly
and rax, rbx      ; bitwise AND
or  rax, rbx      ; bitwise OR
xor rax, rax      ; zero register (common idiom)
not rax           ; bitwise NOT
shl rax, cl       ; left shift
shr rax, cl       ; logical right shift
sar rax, cl       ; arithmetic right shift
```

**Control Flow**
```assembly
cmp rax, rbx      ; set flags
jz / je label     ; jump if zero / equal
jnz / jne label   ; jump if not zero / not equal
jg label          ; jump if greater (signed)
jl label          ; jump if less (signed)
call func         ; push rip; jmp func
ret               ; pop rip
jmp label         ; unconditional jump
```

**System Calls (Linux)**
```assembly
mov rax, 1      ; numer syscalla: write
mov rdi, 1      ; file descriptor: stdout
mov rsi, msg    ; adres bufora
mov rdx, len    ; długość bufora
syscall
```