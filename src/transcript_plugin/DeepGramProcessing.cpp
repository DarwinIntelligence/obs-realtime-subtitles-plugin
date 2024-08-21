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

        size_t input_buf_size = 0;
        {
            std::lock_guard<std::mutex> lock(dg->deepgram_buf_mutex);
            input_buf_size = dg->input_buffers[0].size;
        }
        if(dg->input_buffers[0].size > 0){
            info_log("Anything in here?");
            dg->processed_successfully = deepgram_process_audio(dg);
            if (dg->processed_successfully == true){
                // info_log("Do we get here after true?");
            }
        }
        
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
        info_log("resampled: %d channels, %d frames, %f ms", (int)dfd->channels, (int)resampled_16khz_frames, (float)resampled_16khz_frames / SAMPLE_RATE * 1000.0f);
        // info_log("Printing the size of len %ld", dfd->resampled_buffer.size);
        deque_push_back(&dfd->resampled_buffer, resampled_16khz[0],
				    resampled_16khz_frames * sizeof(float));

    }
    size_t len = dfd->resampled_buffer.size;
    std::vector<float> deepgram_input;
    deepgram_input.resize(len);
    deque_pop_front(&dfd->resampled_buffer, deepgram_input.data(), deepgram_input.size());
    info_log("sending %ld frames to deepgram", deepgram_input.size());
    FILE *audiofile;
    audiofile=fopen("writingData", "ab");
    fwrite(deepgram_input.data(), sizeof(deepgram_input[0]), deepgram_input.size(), audiofile);
    fclose(audiofile);

    //code to send frames to deepgram API
    return true;
    
}

// void send_to_deepgram(transcript_data *dgd){
//     dgd->buffered_output_num_chars = dgd->buffered_output_num_chars;
//     break;
// }