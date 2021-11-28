%define MAX_CAPACITY 256
%define STDOUT 1
%define STDERR 2
%define OFFSET 8

section .text

%include 'colon.inc'

extern find_word
extern read_word
extern print_string
extern print_newline
extern string_length
extern exit
global _start

section .data

%include 'words.inc'

read_error: db 'Error. Can not read the word', 10, 0
found: db 'Found an entry in the dictionary: ', 0
not_found: db 'No entries have been found.', 10, 0
hello_msg: db 'Enter the key please: ', 0

section .text

_start:
	mov rdi, hello_msg			; Вывод приглашения к вводу
    mov rsi, STDOUT				;
    call print_string			;
	sub rsp, MAX_CAPACITY		; Аллоцируем буффер 
    mov rsi, MAX_CAPACITY		; 
    mov rdi, rsp				; 
    call read_word 				; Считываем слово для поиска
    cmp rax, 0 					;
    jz .err_read 				; Проверка на ошибку считывания
    mov rsi, current			; Начало linked list в rsi
    mov rdi, rax 				; Указатель на строку-ключ в rdi
    call find_word				;
    cmp rax, 0 					; Проверка на наличие в словаре
    je .not_found 				;

.found:
   	add rax, OFFSET 			; Смещаем на длину key
    push rax 					; Сохраняем addr для вызова string_length
    mov rsi, rax				; rsi = addr
    call string_length 			; rax = len 
    pop rsi 					; rsi = addr
    add rax, rsi				; rax = addr + len
    inc rax 					; rax = addr + len + 1(нуль-терминатор)
	push rax					; Вывод сообщения о найденном вхождении
	mov rdi, found				; 
	mov rsi, STDOUT				; 
	call print_string 			; 
	pop rdi 					;
	mov rsi, STDOUT				; Вывод найденного вхождения
	call print_string 			; 
	call print_newline 			;
	jmp .end					;

.err_read:
	mov rdi, read_error			; Вывод сообщения об ошибке чтения 
	mov rsi, STDERR				;
	call print_string			;
	jmp .end					;

.not_found:
	mov rdi, not_found			; Вывод сообщения об отсутствии вхождения
	mov rsi, STDERR				;
	call print_string			;
	jmp .end					;

.end:
	add rsp, MAX_CAPACITY		; Завершение программы
	call exit    				; 