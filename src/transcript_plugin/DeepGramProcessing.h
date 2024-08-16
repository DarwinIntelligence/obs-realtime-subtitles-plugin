#ifndef DEEP_GRAM_PROCESSING_H
#define DEEP_GRAM_PROCESSING_H


#include <obs-module.h>


#include "TranscriptPlugin.h"
#include "TranscriptPluginData.h"




void deepgram_loop(void *data);
bool deepgram_process_audio(transcript_data *dgd);

#endif //DEEP_GRAM_PROCESSING_H