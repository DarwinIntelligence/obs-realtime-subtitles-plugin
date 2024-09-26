#include "DeepGramProcessing.h"


#include <curl/curl.h>
#include "TranscriptPluginCallbacks.h"
#include <iostream>
#include <fstream>
#include <chrono>


void deepgram_loop(void *data){
    info_log("Here is the new thread id: from deepgram loop %zu", std::hash<std::thread::id>{}(std::this_thread::get_id()));
    int keep_alive = 0;

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
				// "wss://translate.darwinai.link/listen?client_id=1111", "12345678@"); //Darwins
                dg->url, dg->api_key);
    
    if(dg->endpoint == NULL){
        info_log("Didn't connect. Exiting deepgram loop");
        return;

    }

    //get all the info from the info buffer. We will finally use it here
    while (dg->continue_deepgram_loop){
        // info_log("In the loop");
        if(dg->endpoint != NULL){
            std::lock_guard<std::mutex> lock(dg->endpoint_mutex);//lock the use of the endpoint until the end of this if statement. lock_guard is destroyed when end of scope is reached
            // info_log("Before we get the message");
            std::vector<std::string> messages =
			dg->endpoint->get_messages(dg->endpoint_id);
            // info_log("After we get the message");
            for (auto message : messages) {
                auto json_message = nlohmann::json::parse(message);
                if(dg->transcribe){
                    // info_log("Transcribe? Nah, Yea");

                    std::string json_transcript =
                        json_message["channel"]["alternatives"][0]
                                ["transcript"];
                    // info_log("%s", json_transcript.c_str());
                    if (json_message["is_final"]) {
                        if (dg->transcript != "" &&
                            json_transcript != "") {
                            dg->transcript += " ";
                        }
                        if (json_transcript != "") {
                            std::string combined =
                                dg->transcript + json_transcript;
                            if (combined.length() <
                                50) { //This is arbitrary I need to change this to a real number.
                                dg->transcript = combined;
                            } else {
                                dg->transcript = json_transcript;
                            }
                            info_log("Here is the lang we are printing in %s", dg->source_lang.c_str());
                            send_caption_to_source(dg->text_source_name, dg->transcript.c_str());
                            
                        }
                    }
                }
                if(dg->translate){
                    
                    //info_log("Translate? Nah, Yea");

                    std::string json_translation =
                        json_message["channel"]["alternatives"][0]
                                ["translate"];
                    // info_log("%s", json_translation.c_str());
                    if (json_message["is_final"]) {
                        if (dg->translation != "" &&
                            json_translation != "") {
                            dg->translation += " ";
                        }
                        if (json_translation != "") {
                            std::string combined =
                                dg->translation + json_translation;
                            if (combined.length() <
                                50) { //This is arbitrary I need to change this to a real number.
                                dg->translation = combined;
                            } else {
                                dg->translation = json_translation;
                            }
                            // info_log("Here is the lang we are printing in %s", dg->source_lang.c_str());
                            send_caption_to_source(dg->text_source_name, dg->translation.c_str());
                            
                        }
                    }
                }

            }
            // info_log("Transcript from deepgram_loop %s", dg->transcript.c_str());
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));//adding a print stops seg faults, so I think we are checking the socket too often.
        keep_alive++;
        if(keep_alive == 100){//so the websocket never closes unless we want it to
            if (dg->endpoint != NULL){
                // info_log("sending Ping");
                dg->endpoint->send_text(dg->endpoint_id, "PING");//send a keep alive PING
                keep_alive = 0;
            }
        }
        
    }
    info_log("Trying to delete the endpoint from deepgram loop");
    if(dg->endpoint != NULL){
        delete dg->endpoint;
    }
    dg->continue_deepgram_loop = true; //so we can start the loop.
    info_log("Exiting the deepgram loop");


}




