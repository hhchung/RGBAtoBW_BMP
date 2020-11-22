
SRCS   := 
CFLAGS := -Ofast -MMD 
EXE_NAME := rgbtobw
QEMU_RUN :=

SRCS    := bw.c bmp.c main.c
OBJS    := $(SRCS:.c=.o)
DEPS    := $(SRCS:.c=.d)


ifeq ($(ARCH),aarch64)
	QEMU_RUN := qemu-aarch64 -L ./gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc
	CC := aarch64-linux-gnu-gcc
	#CFLAGS += -DNEON_ASM
	OBJS += ./aarch64/bw.o
endif


default: build

build: $(EXE_NAME)

$(EXE_NAME): $(OBJS)
	$(CC)  $^  -pg -g -lm -o $(EXE_NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.s: %.S
	$(CC) -S $< -o $@

%.o: %.s
	$(CC) -c $< -o $@ 


run: $(EXE_NAME)
	$(QEMU_RUN) ./$(EXE_NAME) pictures/01.bmp pictures/01_out.bmp

clean:
	rm -f $(EXE_NAME) *.o perf.* *.d ./pictures/01_*.bmp

-include $(DEPS)

