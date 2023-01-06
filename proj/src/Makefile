# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c utils.c video/video.c keyboard/kbc.c timer/timer.c rtc/rtc.c serialport/serialport.c game/handlers.c game/images.c game/model.c game/view.c

CFLAGS += -pedantic #-D__LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

video/video.o: video/video.c video/video.h
		cc $(CFLAGS) -o video/video.o -c video/video.c 
keyboard/kbc.o: keyboard/kbc.c keyboard/kbc.h
		cc $(CFLAGS) -o keyboard/kbc.o -c keyboard/kbc.c 
timer/timer.o: timer/timer.c timer/timer.h
		cc $(CFLAGS) -o timer/timer.o -c timer/timer.c
rtc/rtc.o: rtc/rtc.c rtc/rtc.h
		cc $(CFLAGS) -o rtc/rtc.o -c rtc/rtc.c 
serialport/serialport.o: serialport/serialport.c serialport/serialport.h
		cc $(CFLAGS) -o serialport/serialport.o -c serialport/serialport.c 
game/handlers.o: game/handlers.c game/handlers.h
		cc $(CFLAGS) -o game/handlers.o -c game/handlers.c 
game/images.o: game/images.c game/images.h
		cc $(CFLAGS) -o game/images.o -c game/images.c 
game/model.o: game/model.c game/model.h
		cc $(CFLAGS) -o game/model.o -c game/model.c 
game/view.o: game/view.c game/view.h
		cc $(CFLAGS) -o game/view.o -c game/view.c 

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
