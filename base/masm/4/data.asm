
assume cs:code, ds:data, ss:stack

data segment
	dw 0123h,0456h,0789h,0abch,0111h,0222h,0333h,0444h
data ends

stack segment
	dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
stack ends

code segment
	
start:	mov ax,stack
	mov ss,ax
	mov sp,20h

	mov ax,data
	mov ds,ax
	
	mov ax,2
	add ax,ax
	
	mov ax,4c00H
	int 21H

code ends
end start