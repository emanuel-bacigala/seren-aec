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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if (WEB_RTC_API_VERION == 0)
  #include <webrtc/modules/audio_processing/include/audio_processing.h>
  #include <webrtc/modules/interface/module_common_types.h>
  webrtc::AudioProcessing* apm;
  webrtc::AudioFrame near_frame;
  webrtc::AudioFrame far_frame;
#elif (WEB_RTC_API_VERION == 2)
  #include "modules/audio_processing/include/audio_processing.h"
  static rtc::scoped_refptr<webrtc::AudioProcessing> apm;
  static webrtc::StreamConfig input_config;
  static int16_t* farData_  = NULL;
  static int16_t* nearData_ = NULL;
#else
  #error "Only WEB_RTC_API_VERION 0 and 2 are supported"
#endif

extern "C" int webRTC_aecInit(unsigned samplerate, int16_t** farData, int16_t** nearData) {
#if (WEB_RTC_API_VERION == 2)
    if (!farData_ && !nearData_) {
        farData_  = (int16_t*)malloc(2*samplerate/100); // 10ms frames
        nearData_ = (int16_t*)malloc(2*samplerate/100);
        memset(farData_, 0, 2*samplerate/100);
        *farData  = farData_;
        *nearData = nearData_;
    }
#endif
    if (!apm) {
#if (WEB_RTC_API_VERION == 0)
        apm = webrtc::AudioProcessing::Create();
#elif (WEB_RTC_API_VERION == 2)
        apm = webrtc::AudioProcessingBuilder().Create();
#endif
        if (!apm) {
            fprintf(stderr, "webRTC AEC init failed\n");
            return 1;
        }
#if (WEB_RTC_API_VERION == 0)
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
#elif (WEB_RTC_API_VERION == 2)
        webrtc::AudioProcessing::Config config;
        config.echo_canceller.enabled = true;
        config.echo_canceller.mobile_mode = false;

        //config.gain_controller1.enabled = true;
        //config.gain_controller1.mode = webrtc::AudioProcessing::Config::GainController1::kAdaptiveAnalog;
        //config.gain_controller2.enabled = true;
        //config.high_pass_filter.enabled = true;

        apm->ApplyConfig(config);
        input_config.set_sample_rate_hz(samplerate);
        input_config.set_num_channels(1);
#endif
    } else {
        fprintf(stderr, "webRTC AEC already initialized\n");
    }
    return 0;
}

extern "C" int webRTC_aecProcessFarEnd() {
    int status;
#if (WEB_RTC_API_VERION == 0)
    if ((status = apm->ProcessReverseStream(&far_frame)) != webrtc::AudioProcessing::kNoError) {
#elif (WEB_RTC_API_VERION == 2)
    if ((status = apm->ProcessReverseStream(farData_, input_config, input_config, farData_)) != webrtc::AudioProcessing::kNoError) {
#endif
        fprintf(stderr, "ProcessReverseStream() failed: %d\n", status);
        return 1;
    }

    return 0;
}

extern "C" int webRTC_aecProcessNearEnd(int delay) {
    int status;
    apm->set_stream_delay_ms(delay);
#if (WEB_RTC_API_VERION == 0)
    if ((status = apm->ProcessStream(&near_frame)) != webrtc::AudioProcessing::kNoError) {
#elif (WEB_RTC_API_VERION == 2)
    if ((status = apm->ProcessStream(nearData_, input_config, input_config, nearData_)) != webrtc::AudioProcessing::kNoError) {
#endif
        fprintf(stderr, "ProcessStream() failed: %d\n", status);
        return 1;
    }

    return 0;
}

extern "C" int webRTC_aecDeinit(void) {
#if (WEB_RTC_API_VERION == 0)
    delete apm;
    apm = NULL;
#elif (WEB_RTC_API_VERION == 2)
    apm = nullptr;
    if (farData_)  free(farData_);
    if (nearData_) free(nearData_);
    farData_ = nearData_ = NULL;
#endif
    return 0;
}
