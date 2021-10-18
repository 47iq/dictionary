%define MAX_CAPACITY 256
%define STDOUT 1
%define STDERR 2

section .text

%include 'colon.inc'

extern find_word
extern read_word
extern print_string
extern print_newline
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
	mov rdi, hello_msg
    mov rsi, STDOUT
    call print_string
	sub rsp, MAX_CAPACITY		; Аллоцируем буффер 
    mov rsi, MAX_CAPACITY		; 
    mov rdi, rsp				; 
    call read_word 				; Считываем слово для поиска
    cmp rax, 0 					;
    jz .err_read 				; Проверка на считывание
    mov rsi, current			; Начало linked list в rsi
    mov rdi, rax 				; Указатель на строку-ключ в rdi
    call find_word				;
    cmp rax, 0 					; Проверка на наличие в словаре
    je .not_found 				;

.found:
	push rax					;
	mov rdi, found				; Если нашли значение по ключу
	mov rsi, STDOUT				; то выводим вхождение
	call print_string 			; 
	pop rdi 					;
	mov rsi, STDOUT				; 
	call print_string 			; 
	call print_newline 			;
	jmp .end					;

.err_read:
	mov rdi, read_error			; При ошибке чтения 
	mov rsi, STDERR				;
	call print_string			;
	jmp .end					;

.not_found:
	mov rdi, not_found			; При отутствии в словаре
	mov rsi, STDERR				;
	call print_string			;
	jmp .end					;

.end:
	add rsp, MAX_CAPACITY		; Завершение программы
	call exit    				; 