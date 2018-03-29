TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# デフォルト動作

default :
	$(MAKE) haribote.img

# ファイル生成規則

haribote.img : haribote/ipl10.bin haribote/haribote.sys Makefile \
		hello3/hello3.hrb freeMem/freeMem.hrb firstfit/firstfit.hrb \
		bestfit/bestfit.hrb worstfit/worstfit.hrb
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:haribote/ipl10.bin len:512 from:0 to:0 \
		copy from:haribote/haribote.sys to:@: \
		copy from:haribote/ipl10.nas to:@: \
		copy from:make.bat to:@: \
		copy from:hello3/hello3.hrb to:@: \
		copy from:freeMem/freeMem.hrb to:@: \
		copy from:firstfit/firstfit.hrb to:@: \
		copy from:bestfit/bestfit.hrb to:@: \
		copy from:worstfit/worstfit.hrb to:@: \
		imgout:haribote.img

# コマンド

run :
	$(MAKE) haribote.img
	$(COPY) haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) haribote.img
	$(IMGTOL) w a: haribote.img

full :
	$(MAKE) -C haribote
	$(MAKE) -C apilib
	$(MAKE) -C hello3
	$(MAKE) -C freeMem
	$(MAKE) -C firstfit
	$(MAKE) -C bestfit
	$(MAKE) -C worstfit
	$(MAKE) haribote.img

run_full :
	$(MAKE) full
	$(COPY) haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install_full :
	$(MAKE) full
	$(IMGTOL) w a: haribote.img

run_os :
	$(MAKE) -C haribote
	$(MAKE) run

clean :
# 何もしない

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img

clean_full :
	$(MAKE) -C haribote		clean
	$(MAKE) -C apilib		clean
	$(MAKE) -C hello3		clean
	$(MAKE) -C freeMem		clean
	$(MAKE) -C firstfit   clean
	$(MAKE) -C bestfit    clean
	$(MAKE) -C worstfit   clean

src_only_full :
	$(MAKE) -C haribote		src_only
	$(MAKE) -C apilib		src_only
	$(MAKE) -C hello3		src_only
	$(MAKE) -C freeMem		src_only
	$(MAKE) -C firstfit   src_only
	$(MAKE) -C bestfit    src_only
	$(MAKE) -C worstfit   src_only
	-$(DEL) haribote.img

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) haribote.img
