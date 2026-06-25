    global _start
_start:
	push rbp
	mov rbp, rsp
	push r12
	push r13

	mov r12, 0x40404040		; size to mprotect - will be set by woody_woodpacker
	mov r13, 0x41414141		; text section size - ill be set by woody_woodpacker
	lea rdi, [ rel _start ] ; Get payload address
	sub rdi, r12			; Get text section address (adjusted for padding)
	mov rax, 10				; Mprotect syscall nb
	mov rsi, r12			; Set the size
	mov rdx, 7				; RWE permissions
	syscall					; Call mprotect
	jmp _start_decrypt

_start_decrypt:
	call _init_vars
	db `_key_placeholder`	; XTEA key - Will be set by woody_woodpacker

_init_vars:
	pop rdx					; Get key from stack
	lea rdi, [ rel _start ] ; Get payload start address
	sub rdi, r13			; Get text section start address
	mov rsi, 0x9E3779B9		; Set delta
	jmp _check_finished		; Don't decrypt if size < 8

_decrypt:
	mov rcx, 32	    		; Get num_rounds
	mov rax, rsi			; Get delta
	imul rax, rcx			; Get sum (delta * num_rounds)

	mov r8, [rdi]			; Copy the first 8 bytes of rdi
	mov r9, [rdi]			; Copy the first 8 bytes of rdi
	mov r10, 0xffffffff		; Set bitmask of 4 bytes
	and r8, r10				; Get 4 least signifact bytes
	shr r9, 32				; Get 4 most significant bytes

_loop:
	mov r11, r8             ; Get v0
	shr r11, 5				; v0 >> 5
	mov r10, r8				; Get v0
	shl r10, 4              ; v0 << 4
	xor r10, r11			; ((v0 << 4) ^ (v0 >> 5))
	add r10, r8             ; ((v0 << 4) ^ (v0 >> 5)) + v0

	mov r11, rax			; Get sum
	shr r11, 11				; sum >> 11
	and r11, 3				; (sum >> 11) & 3

	shl r11, 2				; Shift index for 32 bit array
	add r11, rdx 			; key + index
	mov r11d, DWORD [r11]	; key[(sum>>11) & 3]]

	add r11, rax			; sum + key[(sum >> 11) & 3]
	xor r10, r11			; r10 ^ r11
	sub r9d, r10d			; v1 -= (r10 ^ r11)

	sub rax, rsi			; sum -= delta

	mov r10, r9				; Get v1
	shl r10, 4				; v1 << 4
	mov r11, r9				; Get v1
	shr r11, 5				; v1 >> 5
	xor r10, r11			; ((v1 << 4) ^ (v1 >> 5))
	add r10, r9				; ((v1 << 4) ^ (v1 >> 5)) + v1

	mov r11, rax			; Get sum
	and r11, 3				; sum & 3

	shl r11, 2				; Shift index for 32 bit array
	add r11, rdx			; key + index
	mov r11d, DWORD [r11]	; key[sum & 3]

	add r11, rax			; sum + key[sum & 3]
	xor r10, r11			; r10 ^ r11
	sub r8d, r10d			; v0 -= (r10 ^ r11)

	dec rcx					; count--
	cmp rcx, 0				; count == 0 ?
	jne _loop				; Loop if count != 0

	mov rax, r9				; Get v1
	shl rax, 32				; v1 << 32
	mov rcx, 0xffffffff     ; Set bitmask of 4 bytes
	and r8, rcx				; Get 4 least significant bytes of v0
	or rax, r8              ; v1 | v0
	mov [rdi], rax          ; Update 8 data bytes

	add rdi, 8				; Increase data pointer
	sub r13, 8				; Decrease size left to decrypt

_check_finished:
	cmp r13, 8				; size < 8 ?
	jge _decrypt			; If not, decrypt more bytes

_reset_protection:
	lea rdi, [ rel _start ] ; Get payload address
	sub rdi, r12			; Get text section address (adjusted for padding)
	mov rax, 10				; Mprotect syscall nb
	mov rsi, r12			; Set the size
	mov rdx, 5				; R.E permissions
	syscall					; Call mprotect

    jmp _woody				; Decryption finished, print woody and exit

_write:
	mov rax, 1
	mov rdi, 1
	pop rsi
	mov rdx, 16
	syscall
	jmp _end

_woody:
    call _write
    db `....WOODY.....\n\0`

_end:
	pop r12
	pop r13
	pop rdx
	jmp 0x42424242
