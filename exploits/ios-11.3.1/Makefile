.PHONY: payload/payload.dylib

shellcode.bin: shellcode.s payload/payload.dylib Makefile
	#aarch64-linux-gnu-as -EL -o shellcode.o shellcode.s
	#aarch64-linux-gnu-objcopy -O binary shellcode.o shellcode.bin
	as -arch arm64 shellcode.s -o shellcode.o
	gobjcopy -Obinary shellcode.o shellcode.bin
	rm shellcode.o
	dd if=/dev/zero of=shellcode.bin bs=1 count=1 seek=4095
	cat payload/payload.dylib >> shellcode.bin

shellcode.s: shellcode.in.s payload/payload.dylib Makefile
	python2 gen_shellcode.py

payload/payload.dylib:
	cd payload && make
