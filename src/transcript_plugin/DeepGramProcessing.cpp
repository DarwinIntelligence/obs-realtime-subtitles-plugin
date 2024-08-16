#include "DeepGramProcessing.h"


#include <curl/curl.h>



void deepgram_loop(void *data){

    if (data == nullptr){
        info_log("deepgram_loop recieved a null ptr for data");
        return;
    }

    struct transcript_data *dg = static_cast<struct transcript_data *>(data);




    //get all the info from the info buffer. We will finally use it here
    while (true){
        dg->processed_succefully = deepgram_process_audio(dg);

    }

}


bool deepgram_process_audio(transcript_data *dfd){

    uint32_t num_frames_from_infos = 0;
	uint64_t start_timestamp_offset_ns = 0;
	// uint64_t end_timestamp_offset_ns = 0;
	size_t overlap_size = 0;

    for (size_t c = 0; c < dfd->channels; c++) {
		// zero the rest of copy_buffers
		memset(dfd->copy_buffers[c] + overlap_size, 0,
		       (dfd->frames - overlap_size) * sizeof(float));
	}
    {
        struct transcript_audio_info info_from_buf = {0};
        const size_t size_of_audio_info = sizeof(transcript_audio_info);

        while (dfd->info_buffer.size >= size_of_audio_info) {
                deque_pop_front(&dfd->info_buffer, &info_from_buf, size_of_audio_info);
                num_frames_from_infos += info_from_buf.frames;
                if (start_timestamp_offset_ns == 0) {
                    start_timestamp_offset_ns = info_from_buf.timestamp_offset_ns;
                }
        }

        for (size_t c = 0; c < dfd->channels; c++) {
			// Push the new data to copy_buffers[c]
			deque_pop_front(&dfd->input_buffers[c],
					    dfd->copy_buffers[c] + overlap_size,
					    num_frames_from_infos * sizeof(float));
		}



    }

    {
        //resample to 16kHz
        float *resampled_16khz[MAX_PREPROC_CHANNELS];
		uint32_t resampled_16khz_frames;
		uint64_t ts_offset;
		{
			audio_resampler_resample(dfd->resampler_to_deepgram,
						 (uint8_t **)resampled_16khz,
						 &resampled_16khz_frames, &ts_offset,
						 (const uint8_t **)dfd->copy_buffers,
						 (uint32_t)num_frames_from_infos);
		}

        deque_push_back(&dfd->resampled_buffer, resampled_16khz[0],
				    resampled_16khz_frames * sizeof(float));
    }
    size_t len = dfd->resampled_buffer.size;
    std::vector<float> deepgram_input;
    deepgram_input.resize(len);
    deque_pop_front(&dfd->resampled_buffer, deepgram_input.data(), deepgram_input.size() * sizeof(float));
    info_log("sending %ld frames to deepgram", deepgram_input.size());
    //code to send frames to deepgram API
    return true;
    
}