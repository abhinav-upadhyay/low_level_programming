%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

global _start

section .data
	fname: db 'test.txt', 0

section .text
	print_string:
		push rdi
		call string_length
		pop rsi
		mov rdx, rax
		mov rax, 1
		mov rdi, 1
		syscall
		ret
	
	string_length:
		xor rax, rax
		.loop:
			cmp byte[rdi + rax], 0
			je .end
			inc rax
			jmp .loop
		.end:
			ret
	
	_start:
		; open the file
		mov rax, 2
		mov rdi, fname
		mov rsi, O_RDONLY
		mov rdx, 0
		syscall

		; mmap the file
		mov r8, rax
		mov rax, 9
		mov rdi, 0 ; we don't have any specific address to map the pages at
		mov rsi, 4096 ; page size
		mov rdx, PROT_READ
		mov r10, MAP_PRIVATE
		mov r9, 0
		syscall
		mov rdi, rax ; rax holds address of the mapped memory pages of the file
		call print_string

		mov rax, 60
		xor rdi, rdi
		syscall
	




