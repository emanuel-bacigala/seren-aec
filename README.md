This is a fork of Seren simple VoIP program http://holdenc.altervista.org/seren/ made by Giorgio Vazzana
with added Speex and webRTC Acoustic Echo Cancellation. 

required build packages:
  libncurses-dev libgmp-dev libwebrtc-audio-processing-dev libspeexdsp-dev libopus-dev libogg-dev libasound2-dev

compile steps:
  1. adjust DEFS = -DHAVE_LIBNCURSESW -DHAVE_NCURSES_H -DDEFAULT_PULSE in Makefile to your needs
  2. make

The following cmdline options has been added:
  -E algo        set AEC algorithm
                 0-off, 1:speex, 2: webRTC
  -e param       set AEC parameter
                 speex-filterLen, webRTC-latency

Command added:
  /E type param   set AEC type and param     [0,2] [0,1000000]
                  0:disabled, 1:speex param=filterLength, 2: webRTC param=latency

Internal audio format changed to : 16000Hz, 2 channels, s16_le
Initial cipher type changed to   : twofish 256bit (pc-engine.c @L1539)

The Speex filter length should be set to around 200-500ms. It is necessary to estimate the webRTC MIC-SPEAKER latency.
Seren loopback mode (command /l) can help.

Enjoy the echo free voice chat:)
