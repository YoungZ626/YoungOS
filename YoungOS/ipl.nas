; Young-ipl
; TAB=4

CYLS	EQU		20				; 声明CYLS=20, 柱面数量

		ORG		0x7c00			; 指明程序装载地址

; 标准FAT12格式软盘专用的代码 Stand FAT12 format floppy code

		JMP		entry
		DB		0x90			; nop指令的机器码(与跳转指令共占3字节)
		DB		"YOUNG OS"		; 启动扇区名称（8字节）
		DW		512				; 每个扇区（sector）大小（必须512字节）
		DB		1				; 簇（cluster）大小（必须为1个扇区）
		DW		1				; FAT起始位置（一般为第一个扇区）
		DB		2				; FAT个数（必须为2）
		DW		224				; 根目录大小（一般为224项）
		DW		2880			; 该磁盘大小（必须为2880扇区1440*1024/512）
		DB		0xf0			; 磁盘类型（必须为0xf0）
		DW		9				; FAT的长度（必须9扇区）
		DW		18				; 一个磁道（track）有几个扇区（必须为18）
		DW		2				; 磁头数（必须为2）
		DD		0				; 不使用分区，必须是0
		DD		2880			; 重写一次磁盘大小
		DB		0,0,0x29		; 意义不明（固定）
		DD		0xffffffff		; （可能是）卷标号码
		DB		"YOUNGOS    "	; 磁盘的名称（必须为11字节，不足填空格）
		DB		"FAT12   "		; 磁盘格式名称（必须8字节，不足填空格）
		RESB	18				; 先空出18字节

; 程序主体

entry:
		MOV		AX,0			; 初始化寄存器，设置引导程序数据段和栈段基址(寄存器)为0
		MOV		SS,AX
		MOV		SP,0x7c00		;栈顶基址为0x7c00
		MOV		DS,AX

; 读取磁盘

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; 柱面0
		MOV		DH,0			; 磁头0
		MOV		CL,2			; 扇区2

readloop:
		MOV		SI,0			; 记录失败次数寄存器

retry:
		MOV		AH,0x02			; AH=0x02 : 读入磁盘
		MOV		AL,1			; 1个扇区
		MOV		BX,0
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JNC		next			; 没出错则跳转到fin
		ADD		SI,1			; 往SI加1
		CMP		SI,5			; 比较SI与5
		JAE		error			; SI >= 5 跳转到error
		MOV		AH,0x00
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 重置驱动器
		JMP		retry
next:
		MOV		AX,ES			; 把内存地址后移0x200（512/16十六进制转换）
		ADD		AX,0x0020
		MOV		ES,AX			; ADD ES,0x020因为没有ADD ES，只能通过AX进行
		ADD		CL,1			; 往CL里面加1
		CMP		CL,18			; 比较CL与18
		JBE		readloop		; CL <= 18 跳转到readloop
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		readloop		; DH < 2 跳转到readloop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		; CH < CYLS 跳转到readloop
; YoungOS内核程序在内存中的地址=
; 0x08200 + (33 - 1(除去引导区)) * 512 = 0x0c200,即0:0xc200.
; 将YoungOS柱面数保存在ds:[0x0ff0]处,即0:0x0ff0处,供后续程序使用。
; 读取YoungOS到[0x08200, 0x30800)内存中后, 跳转到0xc200处执行YoungOS
		MOV		[0x0ff0],CH		; IPL
		JMP		0xc200

error:
		MOV		SI,msg			; msg段内偏移地址

putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 给SI加1，指向下一个字符
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字， 显示AL上的ASCII字符到屏幕上
		MOV		BX,15			; 指定字符颜色
		INT		0x10			; 调用显卡BIOS， 10h显示入参指定的字符
		JMP		putloop

fin:
		HLT						; 让CPU停止，等待指令
		JMP		fin				; 无限循环

msg:
		DB		0x0a, 0x0a		; 换行两次
		DB		"load error"
		DB		0x0a			; 换行
		DB		0

		RESB	0x7dfe-$		; 填写0x00直到0x001fe

		DB		0x55, 0xaa
