#include "DeepGramProcessing.h"


#include <curl/curl.h>
#include "TranscriptPluginCallbacks.h"
#include <iostream>
#include <fstream>
#include <chrono>


void deepgram_loop(void *data){
    info_log("Here is the new thread id: from deepgram loop %zu", std::hash<std::thread::id>{}(std::this_thread::get_id()));


    if (data == nullptr){
        info_log("deepgram_loop recieved a null ptr for data");
        return;
    }

    struct transcript_data *dg = static_cast<struct transcript_data *>(data);
        // info_log("Here is endpoint ID %d", dg->endpoint_id);
    dg->thread_iter++;
        
    dg->endpoint = new WebsocketEndpoint();
	dg->endpoint_id = dg->endpoint->connect(
				// "wss://deepgram.darwinai.link/v1/listen?language=ko&model=nova-2-general&encoding=linear16&sample_rate=44100", ""); //Darwins
				"wss://translate.darwinai.link/listen?client_id=" + std::to_string(dg->thread_iter), "1111"); //Darwins
    
    if(dg->endpoint == NULL){
        info_log("Didn't connect. Exiting deepgram loop");
        return;

    }

    //get all the info from the info buffer. We will finally use it here
    while (dg->continue_deepgram_loop){
        if(dg->endpoint != NULL){
            std::lock_guard<std::mutex> lock(dg->endpoint_mutex);//lock the use of the endpoint until the end of this if statement. lock_guard is destroyed when end of scope is reached
            // info_log("Before we get the message");
            std::vector<std::string> messages =
			dg->endpoint->get_messages(dg->endpoint_id);
            // info_log("After we get the message");
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
                            20) { //This is arbitrary I need to change this to a real number.
                            dg->transcript = combined;
                        } else {
                            dg->transcript = transcript;
                        }
                        info_log("Here is the lang we are printing in %s", dg->source_lang.c_str());
                        send_caption_to_source("Darwin Realtime Subtitles", dg->transcript.c_str());
                        
                    }
                }
            }
            // info_log("Transcript from deepgram_loop %s", dg->transcript.c_str());
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));//adding a print stops seg faults, so I think we are checking the socket too often.
        
    }
    delete dg->endpoint;
    dg->continue_deepgram_loop = true; //so we can start the loop.
    info_log("Exiting the deepgram loop");


}




