%define current 0				; В current всегда адрес начала списка


; Аргументы:
; %1 - строка ключа
; %2 - имя метки для значения

%macro colon 2					; 
%%next: dq current				; Связываем элемент с предком
%define current %%next 			; Обновляем current
db %1, 0						; Здесь храним ключ
%2: 							; Генерируем метку по второму арг.
%endmacro						;