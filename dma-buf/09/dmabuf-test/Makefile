# SPDX-License-Identifier: GPL-2.0

CROSS_COMPILE := arm-linux-gnueabi-

CC := $(CROSS_COMPILE)gcc
CFLAGS := -I../../usr/include -I../../drivers/staging/android/uapi

PROGS := ion_test

all: $(PROGS)

clean:
	rm -fr $(PROGS)
