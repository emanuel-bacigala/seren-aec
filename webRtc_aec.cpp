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

#include <webrtc/modules/audio_processing/include/audio_processing.h>
#include <webrtc/modules/interface/module_common_types.h>

webrtc::AudioProcessing* apm;
webrtc::AudioFrame near_frame;
webrtc::AudioFrame far_frame;

extern "C" int webRTC_aecInit(unsigned samplerate, int16_t** farData, int16_t** nearData) {
    if (!apm) {
        apm = webrtc::AudioProcessing::Create();
        if (!apm) {
            fprintf(stderr, "webRTC AEC init failed\n");
            return 1;
        }

        //apm->high_pass_filter()->Enable(true);

        apm->echo_cancellation()->enable_drift_compensation(false);
        apm->echo_cancellation()->Enable(true);

        //apm->noise_reduction()->set_level(kHighSuppression);
        //apm->noise_reduction()->Enable(true);

        far_frame.sample_rate_hz_      = near_frame.sample_rate_hz_      = samplerate;
        far_frame.num_channels_        = near_frame.num_channels_        = 1;
        far_frame.samples_per_channel_ = near_frame.samples_per_channel_ = samplerate/100;
        *farData  = far_frame.data_;
        *nearData = near_frame.data_;
    } else {
        fprintf(stderr, "webRTC AEC already initialized\n");
    }
    return 0;
}

extern "C" int webRTC_aecProcessFarEnd() {
    int status;
    if ((status = apm->ProcessReverseStream(&far_frame)) != webrtc::AudioProcessing::kNoError) {
        fprintf(stderr, "ProcessReverseStream() failed: %d\n", status);
        return 1;
    }

    return 0;
}


extern "C" int webRTC_aecProcessNearEnd(int delay) {
    int status;
    apm->set_stream_delay_ms(delay);
    if ((status = apm->ProcessStream(&near_frame)) != webrtc::AudioProcessing::kNoError) {
        fprintf(stderr, "ProcessStream() failed: %d\n", status);
        return 1;
    }

    return 0;
}

extern "C" int webRTC_aecDeinit(void) {
    delete apm;
    apm = NULL;
    return 0;
}
