/*
 * Copyright (C) 2024 Matus Jurecka
 *
 * This file is part of Seren.
 *
 * Seren is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Seren is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speex/speex_echo.h"
#include "speex/speex_preprocess.h"

static SpeexEchoState *st = NULL;
static SpeexPreprocessState *den = NULL;
static int16_t* farData_  = NULL;
static int16_t* nearData_ = NULL;
static int16_t* ecData_   = NULL;


int speex_aecInit(unsigned samplerate, unsigned blockLen, unsigned filterLen, int16_t** farData, int16_t** nearData, int16_t** ecData)
{
    if (!farData_ && !nearData_ && !ecData_) {
        farData_  = (int16_t*)malloc(blockLen*2);
        nearData_ = (int16_t*)malloc(blockLen*2);
        ecData_   = (int16_t*)malloc(blockLen*2);
        memset(farData_, 0, blockLen*2);

        *farData  = farData_;
        *nearData = nearData_;
        *ecData   = ecData_;
    }

    if (!st && !den) {
        st = speex_echo_state_init((int)blockLen, (int)filterLen);
        speex_echo_ctl(st, SPEEX_ECHO_SET_SAMPLING_RATE, (int*)&samplerate);
        den = speex_preprocess_state_init((int)blockLen, (int)samplerate);

        if (!st || !den) {
            fprintf(stderr, "speex AEC init failed\n");
            return 1;
        }
/*
        int tmp = 1;
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_AGC, &tmp);
        tmp = 1;
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_DENOISE, &tmp);
        tmp = 1;
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_DEREVERB, &tmp);

        int echo_suppress_attenuation = 0; // negative value [dB]
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS, &echo_suppress_attenuation);

        int echo_suppress_attenuation_active = 1; // not sure about this
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE, &echo_suppress_attenuation_active);
*/
        speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_STATE, st);
    } else {
        fprintf(stderr, "speex AEC already initialized\n");
    }

    return 0;
}

int speex_aecProcessFarEnd() {
    speex_echo_playback(st, farData_);

    return 0;
}


int speex_aecProcessNearEnd() {
    speex_echo_capture(st, nearData_, ecData_);
    speex_preprocess_run(den, ecData_);

    return 0;
}

int speex_aecDeinit(void) {
    speex_echo_state_destroy(st);
    speex_preprocess_state_destroy(den);
    st = NULL;
    den = NULL;
    if (farData_)  free(farData_);
    if (nearData_) free(nearData_);
    if (ecData_)   free(ecData_);
    farData_ = nearData_ = ecData_ = NULL;

    return 0;
}
