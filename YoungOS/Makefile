OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj int.obj fifo.obj keyboard.obj mouse.obj	\
		memo.obj sheet.obj timer.obj multitask.obj console.obj file.obj window.obj cmos.obj jpeg.obj tek.obj

OBJS_API =	api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj \
			api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj \
			api013.obj api014.obj api015.obj api016.obj api017.obj api018.obj \
			api019.obj api020.obj api021.obj api022.obj api023.obj api024.obj \
			api025.obj api026.obj api027.obj alloca.obj

TOOLPATH  = ../z_tools/
INCPATH1  = ../z_tools/YoungOS
INCPATH2  = inc

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH1) -I$(INCPATH2) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)YoungOS/YoungOS.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
GOLIB    = $(TOOLPATH)golib00.exe 
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) img

# 镜像文件生成

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

YoungOS.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb YoungOS.sys

apilib.lib : Makefile $(OBJS_API)
	$(GOLIB) $(OBJS_API) out:apilib.lib

a.bim : a.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj apilib.lib

a.hrb : a.bim Makefile
	$(BIM2HRB) a.bim a.hrb 0

hello3.bim : hello3.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj apilib.lib

hello3.hrb : hello3.bim Makefile
	$(BIM2HRB) hello3.bim hello3.hrb 0

hello4.bim : hello4.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim stack:1k map:hello4.map \
		hello4.obj apilib.lib

hello4.hrb : hello4.bim Makefile
	$(BIM2HRB) hello4.bim hello4.hrb 0

type.bim : type.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:type.bim stack:1k map:type.map type.obj apilib.lib

type.hrb : type.bim Makefile
	$(BIM2HRB) type.bim type.hrb 48k

tview.bim : tview.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:tview.bim stack:1k map:tview.map tview.obj apilib.lib

tview.hrb : tview.bim Makefile
	$(BIM2HRB) tview.bim tview.hrb 48k

tview.bim : tview.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:tview.bim stack:1k map:tview.map tview.obj apilib.lib

tview.hrb : tview.bim Makefile
	$(BIM2HRB) tview.bim tview.hrb 48k

lineball.bim : lineball.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lineball.bim stack:1k map:lineball.map lineball.obj apilib.lib

lineball.hrb : lineball.bim Makefile
	$(BIM2HRB) lineball.bim lineball.hrb 48k

lines.bim : lines.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lines.bim stack:1k map:lines.map \
		lines.obj apilib.lib

lines.hrb : lines.bim Makefile
	$(BIM2HRB) lines.bim lines.hrb 48k

color.bim : color.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color.bim stack:1k map:color.map \
		color.obj apilib.lib

color.hrb : color.bim Makefile
	$(BIM2HRB) color.bim color.hrb 56k

YoungOS.img : ipl.bin YoungOS.sys Makefile \
	a.hrb hello3.hrb hello4.hrb lines.hrb color.hrb type.hrb tview.hrb lineball.hrb
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:YoungOS.sys to:@: \
		copy from:ipl.nas to:@: \
		copy from:make.bat to:@: \
		copy from:a.hrb to:@: \
		copy from:hello3.hrb to:@: \
		copy from:hello4.hrb to:@: \
		copy from:lines.hrb to:@: \
		copy from:color.hrb to:@: \
		copy from:type.hrb to:@: \
		copy from:tview.hrb to:@: \
		copy from:lineball.hrb to:@: \
		copy from:data/bk1.jpg to:@: \
		copy from:data/HZK16.fnt to:@: \
		imgout:YoungOS.img

# 其他指令

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# 运行程序
img :
	$(MAKE) YoungOS.img

clean :
	-$(DEL) asmhead.bin
	-$(DEL) hankaku.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.hrb

run :
	$(MAKE) img
	$(COPY) YoungOS.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) clean
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: YoungOS.img

src_only :
	$(MAKE) clean
	-$(DEL) YoungOS.img