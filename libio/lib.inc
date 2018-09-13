section .text

string_length:
    xor rax, rax
	.iterate:
		cmp byte[rdi + rax], 0
		je .end
		inc rax
		jmp .iterate
	.end:
		ret

print_string:
	call string_length
	mov rsi, rdi
	mov rdx, rax
	mov rax, 1
	mov rdi, 1
	syscall
	ret


print_char:
	push rdi ; rdi gets changed by print_string
	mov rdi, rsp
	call print_string
	pop rdi
	ret

print_newline:
    ;xor rax, rax
	mov rax, 1
	mov rdi, 1
	mov rsi, 10
	mov rdx, 1
	syscall
    ret


print_uint:
	mov rax, rdi
	mov rdi, rsp
	push 0
	sub rsp, 16
	dec rdi
	mov r8, 10

	.loop:
		xor rdx, rdx
		div r8
		or dl, 0x30
		dec rdi
		mov [rdi], dl
		test rax, rax
		jnz .loop
	call print_string
	add rsp, 24
	ret
		


print_int:
	test rdi, rdi
	jns .call_uint 
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
	.call_uint:
		jmp print_uint
		ret

string_equals:
    xor rax, rax
	.loop:
		cmp byte[rdi + rax], 0
		je .first_finished
		xor rcx, rcx
		mov cl, byte[rsi + rax]
		cmp byte[rdi + rax], cl
		jne .not_equal
		cmp byte[rsi + rax], 0
		je .not_equal
		inc rax
		jmp .loop
	.not_equal:
		xor rax, rax
		ret
	.first_finished:
		cmp byte[rsi + rax], 0
		je .equal
		jmp .not_equal
	.equal:
		mov rax, 1
		ret


read_char:
	xor rax, rax
	xor rdi, rdi
	push 0
	mov rsi, rsp
	mov rdx, 1
	syscall
	pop rax
	ret

read_word:
	xor r8, r8 ; offset in the destination buffer
	push r9
	push r10
	mov r9, rdi
	mov r10, rsi
	.loop:
		cmp r8, r10
		je .overflow
		call  read_char
		test al, al
		je .end
		cmp al, 0x20
		je .handle_space
		cmp al, 0x9
		je .handle_space
		cmp al, 0x10
		je .handle_space
		cmp al, 13
		je .handle_space
		mov byte [r9 + r8], al
		inc r8
		jmp .loop
	.end:
		mov byte[r9 + r8], 0
		mov rax, r9
		mov rdx, r8
		pop r10
		pop r9
		ret
	.overflow:
		xor rax, rax
		pop r10
		pop r9
		ret
	.handle_space:
		test r8, r8
		jz .loop
		jmp .end

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
	xor rax, rax
	xor rcx, rcx
	mov r8, 10
	.loop:
		movzx r9, byte[rdi + rcx]
		cmp r9, '0'
		jb .end
		cmp r9, '9'
		ja .end
		xor rdx, rdx
		mul r8
		and r9b, 0x0f
		add rax, r9
		inc rcx
		jmp .loop
	.end:
		mov rdx, rcx
		ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
	xor rdx, rdx
	cmp byte[rdi], '-'
	je .neg
	call parse_uint
	ret
	.neg:
		inc rdi
		call parse_uint
		inc rdx
		neg rax
		ret


string_copy:
	xor rax, rax
	.loop:
		cmp rax, rdx
		je .overflow
		cmp byte[rdi + rax], 0
		je .end
		xor r8, r8
		mov r8b, byte[rdi + rax]
		mov [rsi + rax], r8b
		inc rax
		jmp .loop
	.overflow:
		xor rax, rax
		ret
	.end:
		mov byte[rsi + rax], 0
		mov rax, rsi
		ret
