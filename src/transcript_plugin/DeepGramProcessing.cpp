#include "DeepGramProcessing.h"


#include <curl/curl.h>
#include "TranscriptPluginCallbacks.h"


void deepgram_loop(void *data){

    if (data == nullptr){
        info_log("deepgram_loop recieved a null ptr for data");
        return;
    }

    struct transcript_data *dg = static_cast<struct transcript_data *>(data);



    //get all the info from the info buffer. We will finally use it here
    while (true){
        if(dg->endpoint != NULL){
            std::vector<std::string> messages =
			dg->endpoint->get_messages(dg->endpoint_id);
            for (auto message : messages) {
                auto json_message = nlohmann::json::parse(message);
                std::string transcript =
                    json_message["channel"]["alternatives"][0]
                            ["transcript"];
                if (json_message["is_final"]) {
                    if (dg->transcript != "" &&
                        transcript != "") {
                        dg->transcript += " ";
                    }
                    if (transcript != "") {
                        std::string combined =
                            dg->transcript + transcript;
                        if (combined.length() <
                            20) {
                            dg->transcript = combined;
                        } else {
                            dg->transcript = transcript;
                        }
                    }
                }
            }
            send_caption_to_source("Darwin Realtime Subtitles", dg->transcript.c_str());
            // info_log("Transcript from deepgram_loop %s", dg->transcript.c_str());
        }
        // size_t input_buf_size = 0;
        // {
        //     std::lock_guard<std::mutex> lock(dg->deepgram_buf_mutex);
        //     input_buf_size = dg->input_buffers[0].size;
        // }
        // if(dg->input_buffers[0].size > 0){
        //     info_log("Anything in here?");
        //     dg->processed_successfully = deepgram_process_audio(dg);
        //     if (dg->processed_successfully == true){
        //         // info_log("Do we get here after true?");
        //     }
        // }
        
    }

}




// void send_to_deepgram(transcript_data *dgd){
//     dgd->buffered_output_num_chars = dgd->buffered_output_num_chars;
//     break;
// }
