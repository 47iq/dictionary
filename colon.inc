%define current 0										; В current всегда адрес начала списка


; Аргументы:
; %1 - строка ключа
; %2 - имя метки для значения

%macro colon 2											; 
	%ifid %2											;
		%2: dq current									; Связываем элемент с предком
		%define current %2								; Обновляем current
	%else												;
		%fatal "The second argument must be an id" 		;
	%endif												;	
	%ifstr %1											;
		db %1, 0										; Здесь храним ключ
	%else												;
		%faral "The first argument must be a string"	;
	%endif												; 	
%endmacro												;