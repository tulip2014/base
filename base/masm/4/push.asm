
assume cs:abc

abc segment

	;mov ax,2000H
	;mov ss,ax
	;mov sp,0H
	mov ax,2H
	push ax
	add ax,ax
	push ax
	
	mov ax,4c00H
	int 21H

abc ends
end