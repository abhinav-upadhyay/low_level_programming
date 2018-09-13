global _start

section .data
	new_line: db 10
	codes: db '0123456789abcdef'
	test: dq -1

section .text

	print_new_line:
		mov rax, 1
		mov rdi, 1
		mov rsi, new_line
		mov rdx, 1
		syscall
		ret
	
	print_hex:
		mov rax, rdi	; copy the argument to rax
		mov rdi, 1
		mov rcx, 64
		mov rdx, 1
		.iterate:
			push rax
			sub rcx, 4
			sar rax, cl
			and rax, 0xf
			lea rsi, [codes + rax]
			mov rax, 1
			push rcx
			syscall
			pop rcx
			pop rax
			test rcx, rcx
			jnz .iterate
		ret
	
_start:
	;mov rdi, 0x1122334455667788
	mov rdi, [test]
	call print_hex
	call print_new_line
	mov byte[test], 1
	mov rdi, [test]
	call print_hex
	call print_new_line
	mov word[test], 1
	mov rdi, [test]
	call print_hex
	call print_new_line
	mov dword[test], 1
	mov rdi, [test]
	call print_hex
	call print_new_line
	mov qword[test], 1
	mov rdi, [test]
	call print_hex
	call print_new_line
	mov rax, 60
	xor rdi, rdi
	syscall


