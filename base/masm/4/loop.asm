
assume cs:abc

abc segment

	mov ax,2
	mov cx,3

s:	add ax,ax
	loop s

	mov ax,4c00H
	int 21H

abc ends
end