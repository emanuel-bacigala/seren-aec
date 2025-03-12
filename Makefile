CC              = gcc
CPP             = g++
DEFS           += -DHAVE_LIBNCURSESW -DHAVE_NCURSES_H -DDEFAULT_PULSE -DWEB_RTC_API_VERION=0
CFLAGS          = -O2 -Wall
IDIR            = -I. -I/usr/include/webrtc_audio_processing
LIBS            = $$(pkg-config --libs speexdsp) $$(pkg-config --libs webrtc-audio-processing) -lncursesw -lgmp -logg -lopus -lasound -lm
#LIBS            = -lspeexdsp -lwebrtc_audio_processing -lncursesw -lgmp -logg -lopus -lasound -lm
LDFLAGS         = -s


objects = common.o rw.o adsp.o tones.o slist.o msgbook.o md5.o xtea.o cast-128.o \
          blowfish.o camellia.o twofish.o random.o dhm.o recording.o udp.o audio.o nc.o \
          input.o stun.o pc-engine.o seren.o webRtc_aec.o speex_aec.o

all: seren

seren: $(objects)
	$(CPP) $(LDFLAGS) -o seren $(objects) $(LIBS)

seren.o: seren.c config.h pc-engine.h udp.h audio.h slist.h dhm.h \
 common.h msgbook.h input.h nc.h stun.h random.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c seren.c

pc-engine.o: pc-engine.c pc-engine.h udp.h audio.h tones.h slist.h dhm.h \
 pc-engine-internal.h common.h msgbook.h rw.h adsp.h random.h xtea.h \
 cast-128.h blowfish.h camellia.h twofish.h recording.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c pc-engine.c

stun.o: stun.c stun.h rw.h udp.h msgbook.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c stun.c

nc.o: nc.c nc.h config.h common.h msgbook.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c nc.c

input.o: input.c input.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c input.c

audio.o: audio.c audio.h common.h msgbook.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c audio.c

udp.o: udp.c udp.h common.h msgbook.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c udp.c

recording.o: recording.c recording.h rw.h common.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c recording.c

dhm.o: dhm.c dhm.h rw.h md5.h random.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c dhm.c

random.o: random.c random.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c random.c

twofish.o: twofish.c twofish.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c twofish.c

camellia.o: camellia.c camellia.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c camellia.c

blowfish.o: blowfish.c blowfish.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c blowfish.c

cast-128.o: cast-128.c cast-128.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c cast-128.c

xtea.o: xtea.c xtea.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c xtea.c

md5.o: md5.c md5.h rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c md5.c

msgbook.o: msgbook.c msgbook.h common.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c msgbook.c

slist.o: slist.c slist.h common.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c slist.c

tones.o: tones.c tones.h common.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c tones.c

adsp.o: adsp.c adsp.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c adsp.c

rw.o: rw.c rw.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c rw.c

common.o: common.c common.h
	$(CC) $(CFLAGS) $(DEFS) $(IDIR) -c common.c

webRtc_aec.o: webRtc_aec.cpp webRtc_aec.h
	$(CPP) $(CFLAGS) -DWEBRTC_POSIX -DWEBRTC_AUDIO_PROCESSING_ONLY_BUILD $(IDIR) -c webRtc_aec.cpp

speex_aec.o: speex_aec.c speex_aec.h
	$(CC) $(CFLAGS) $(IDIR) -c speex_aec.c

clean:
	rm -f seren $(objects) *~

.PHONY: all clean distclean install uninstall
