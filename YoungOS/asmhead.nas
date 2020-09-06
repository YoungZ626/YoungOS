; Young OS boot asm
; TAB=4

[INSTRSET "i486p"]

VBEMODE	EQU		0x103			
; 显示模式
;	0x100 :  640 x  400 x 8bit 彩色
;	0x101 :  640 x  480 x 8bit 彩色
;	0x103 :  800 x  600 x 8bit 彩色
;	0x105 : 1024 x  768 x 8bit 彩色
;	0x107 : 1280 x 1024 x 8bit 彩色

BOTPAK	EQU		0x00280000		; 加载bootpack
DSKCAC	EQU		0x00100000		; 磁盘缓存的位置
DSKCAC0	EQU		0x00008000		; 磁盘缓存的位置（实模式）

; BOOT_INFO相关
CYLS	EQU		0x0ff0			; 引导扇区设置
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 关于颜色的信息
SCRNX	EQU		0x0ff4			; 分辨率X
SCRNY	EQU		0x0ff6			; 分辨率Y
VRAM	EQU		0x0ff8			; 图像缓冲区的起始地址

		ORG		0xc200			;  这个的程序要被装载的内存地址

; 确认VBE是否存在

		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

; 检查VBE的版本

		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320			; if (AX < 0x0200) goto scrn320

; 取得画面模式信息

		MOV		CX,VBEMODE
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

; 画面模式信息的确认
		CMP		BYTE [ES:DI+0x19],8		;颜色数必须为8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4		;颜色的指定方法必须为4(4是调色板模式)
		JNE		scrn320
		MOV		AX,[ES:DI+0x00]				;模式属性bit7不是1就不能加上0x4000
		AND		AX,0x0080
		JZ		scrn320					; 模式属性的bit7是0，所以放弃

;	画面设置

		MOV		BX,VBEMODE+0x4000
		MOV		AX,0x4f02
		INT		0x10
		MOV		BYTE [VMODE],8	; 屏幕的模式（参考C语言的引用）
		MOV		AX,[ES:DI+0x12]
		MOV		[SCRNX],AX
		MOV		AX,[ES:DI+0x14]
		MOV		[SCRNY],AX
		MOV		EAX,[ES:DI+0x28] ;VRAM的地址
		MOV		[VRAM],EAX
		JMP		keystatus

scrn320:
		MOV		AL,0x13						; VGA图、320x200x8bit彩色
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8		; 记下画面模式（参考C语言）
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

;	通过 BIOS 获取指示灯状态

keystatus:
		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

; 	防止PIC接受所有中断
;	AT兼容机的规范、PIC初始化
;	然后之前在CLI不做任何事就挂起
;	PIC在同意后初始化

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; 不断执行OUT指令
		OUT		0xa1,AL

		CLI						; 禁止CPU级别的中断

; 让CPU支持1M以上内存、设置A20GATE

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout		; 等同于wait_KBC_sendready
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 保护模式转换

[INSTRSET "i486p"]				; 说明使用486指令

		LGDT	[GDTR0]			; 设置临时GDT
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; 使用bit31为0（禁用分页）
		OR		EAX,0x00000001	; bit0到1转换（保护模式过渡）
		MOV		CR0,EAX
		JMP		pipelineflush
;进入保护模式，除了CS以外的段寄存器的值都从0x0000变成了0x0008，相当于gdt+1的段
pipelineflush:
		MOV		AX,1*8			; 可读写的段 32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootpack传递
; 相当于memcpy(bootpack,	BOTPAK,	512*1024/4);
; 转送数据以双字为单位，故数据大小用字节数除以4
; 从bootpack的地址开始的512KB内容复制到0x00280000号地址去
		MOV		ESI,bootpack	; 源
		MOV		EDI,BOTPAK		; 目标
		MOV		ECX,512*1024/4
		CALL	memcpy

; 传输磁盘数据

; 从引导区开始
; 相当于memcpy(0x7c00,	DSKCAC,	512/4);
; 将启动扇区复制到1M以后的内存中去——DSKCAC=0x00100000
		MOV		ESI,0x7c00		; 源
		MOV		EDI,DSKCAC		; 目标
		MOV		ECX,512/4
		CALL	memcpy

; 剩余的全部
		MOV		ESI,DSKCAC0+512	; 源
		MOV		EDI,DSKCAC+512	; 目标
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; 除以4得到字节数
		SUB		ECX,512/4		; IPL偏移量
		CALL	memcpy

; 由于还需要asmhead才能完成
; 完成其余的bootpack任务

; bootpack启动

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 传输完成
		MOV		ESI,[EBX+20]	; 源
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 目标
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 堆栈的初始化
		JMP		DWORD 2*8:0x0000001b
;cs=2*8时是GDT[2]的选择符,
;这会使得CPU跳转执行GDT[2].base_addr+eip即0x280000+0x1b处指令,
;即跳转执行0x28001b处指令。
;
;haribote C程序偏移0x1b处为跳转指令的机器码,
;偏移0x1c处存了该跳转指令的目标地址 —— haribote C程序的入口地址。
;
;也就是说, 在执行经作者改编的gcc编译器和链接器所得到的C可执行程序时,
;需跳转到可执行程序0x1b偏移处才能执行到C程序的入口。
waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; AND结果不为0跳转到waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 运算结果不为0跳转到memcpy
		RET
; memcpy地址前缀大小

		ALIGNB	16
GDT0:
		RESB	8				; 初始值
		DW		0xffff,0x0000,0x9200,0x00cf	; 写32bit位段寄存器
		DW		0xffff,0x0000,0x9a28,0x0047	; 可执行的文件的32bit寄存器（bootpack用）

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
