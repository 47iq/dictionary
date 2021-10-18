section .text
global find_word

extern string_length
extern string_equals


; Параметры: 
; rsi - адрес начала linked list
; rdi - указатель на нуль-терминированную строку (ключ)
; Возвращает: 
; rax - адрес найденного элемента, либо 0 при его отсутствии

find_word:
.loop:
	push rsi				; 
	add rsi, 8				; Сравниваем значение 
	call string_equals		; с строкой ключа
	pop rsi					; 
	cmp rax, 0 				; Проверка на соответствие
	jne .found 				; 
	mov rsi, [rsi] 			; Переходим к предку
	cmp rsi, 0 				; Если предка нет,
	je .not_found 			; то завершаем поиск
	jmp .loop 				; 
.found:
    add rsi, 8 				; 
    push rsi 				;
    call string_length 		;  
    pop rsi 				; rsi = len
    add rax, rsi 			;
    inc rax 				; rax = addr
    ret 					;
.not_found:
    xor rax, rax 			; 
    ret						;
