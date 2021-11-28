global print_char
global print_newline
global print_string
global exit
global print_uint
global print_int
global read_char
global read_word
global parse_uint
global parse_int
global string_equals
global string_copy
global string_length



; Принимает код возврата и завершает текущий процесс
exit:
	mov rax, 60
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
	xor rax, rax
	.loop:
		cmp byte [rdi + rax], 0
		je .end
		inc rax
		jmp .loop
	.end:
		ret

; Принимает указатель на нуль-терминированную строку и id потока вывода, выводит её в выбранный поток
print_string:
	mov r9, rsi
	push rdi
	call string_length
	pop rsi
	mov rdx, rax
	mov rax, 1
	mov rdi, r9
	syscall
	ret

; Принимает код символа и выводит его в stdout
print_char:
	push rdi
	mov rdi, rsp
	call print_string
	pop rdi
	ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
	xor rax, rax
	mov rdi, 0xA
	call print_char
	ret

; Выводит беззнаковое 8-байтовое число в десятичном формате
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
	mov r9, 10
	mov rax, rdi
	mov rdi, rsp
	dec rdi
	push 0
	sub rsp, 16

	.loop:
		xor rdx, rdx
		div r9
		add rdx, '0'
		dec rdi
		mov [rdi], dl
		cmp rax, 0
		je .end
		jmp .loop

	.end:
		call print_string
		add rsp, 24
		ret

; Выводит знаковое 8-байтовое число в десятичном формате
print_int:
	xor rax, rax
	cmp rdi, 0
	jl .minus
	.plus:
		call print_uint
		jmp .end
	.minus:
		push rdi
		mov rdi, '-'
		call print_char
		pop rdi
		neg rdi
		call print_uint
	.end:
		ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
	xor rax, rax
	xor rcx, rcx
	xor r9, r9
	xor r10, r10
	.loop:
		mov r9b, byte [rdi + rcx]
		mov r10b, byte [rsi + rcx]
		cmp r9b, r10b
		jne .neq
		cmp r9b, 0
		je .eq
		inc rcx
		jmp .loop
	.eq:
		mov rax, 1
		ret
	.neq:
		mov rax, 0
		ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
	xor rax, rax
	xor rdi, rdi
	push 0
	mov rsi, rsp
	mov rdx, 1
	syscall
	pop rax
	ret

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
	xor rcx, rcx
	xor r9, r9
	mov r10, rdi
	.loop:
		push rdi
		push rsi
		call read_char
		pop rsi
		pop rdi
		test r9, r9
		je .tab_check
	.sym:
		cmp r9, rsi
		jae .err
		cmp rax, 0x20
		je .add_null
		cmp rax, 0x9
		je .add_null
		cmp rax, 0xA
		je .add_null
		mov [rdi + r9], rax
		test rax, rax
		je .end
		inc r9
		jmp .loop

	.tab_check:
		cmp rax, 0x20
		je .loop
		cmp rax, 0x9
		je .loop
		cmp rax, 0xA
		je .loop
		jmp .sym

	.add_null:
		xor rax, rax
		mov [rdi + r9], rax
		jmp .end

	.err:
		xor rax, rax
		ret
	.end:
		mov rax, r10
		mov rdx, r9
		ret

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
	parse_uint:
	mov r8, 10
	xor rax, rax
	xor rcx, rcx
	.loop:
		movzx r9, byte [rdi + rcx]
		cmp r9b, '0'
		jb .end
		cmp r9b, '9'
		ja .end
		xor rdx, rdx
		mul r8
		sub r9b, '0'
		add rax, r9
		inc rcx
		jmp .loop
	.end:
		mov rdx, rcx
		ret

; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был)
; rdx = 0 если число прочитать не удалось
parse_int:
	mov r8, 10
	xor rax, rax
	xor rcx, rcx
	push 1
	.loop:
		movzx r9, byte [rdi + rcx]
		cmp r9b, '-'
		je .minus
		cmp r9b, '0'
		jb .check
		cmp r9b, '9'
		ja .check
		xor rdx, rdx
		mul r8
		sub r9b, '0'
		add rax, r9
	.continue:
		inc rcx
		jmp .loop

	.minus:
		mov r10, 0
		mov [rsp], r10
		jmp .continue

	.check:
		pop r10
		cmp r10, 0
		je .neg
		jmp .end

	.neg:
		neg rax
		jmp .end

	.end:
		mov rdx, rcx
		ret

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
	call string_length
	cmp rax, rdx
	jae .long
	xor rax, rax
	.loop:
		mov rcx, [rdi + rax]
		mov [rsi+rax], rcx
		cmp byte [rdi + rax], 0
		je .end
		inc rax
		jmp .loop
	.end:
		inc rax
		ret
	.long:
		xor rax, rax
		ret