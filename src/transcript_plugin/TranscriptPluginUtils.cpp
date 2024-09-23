#include "TranscriptPluginUtils.h"

#include <obs-module.h>
#include <obs.h>
#include <obs-frontend-api.h>


/// @brief set up the text source, including the format of the text itself as it will appear on the screen.
std::string create_obs_text_source_if_needed()
{
	// check if a source called "Darwin Realtime Subtitles" exists
	for (int i = 1; i < 5; i++){
		std::string sub_name= "Darwin Realtime Subtitles " + std::to_string(i); 
		obs_source_t *source = obs_get_source_by_name(sub_name.c_str());
		if (!source) {
			// create a new OBS text source called "Darwin Realtime Subtitles"
			obs_source_t *scene_as_source = obs_frontend_get_current_scene();
			obs_scene_t *scene = obs_scene_from_source(scene_as_source);
		#ifdef _WIN32
			source = obs_source_create("text_gdiplus_v3", sub_name.c_str(), nullptr, nullptr);
		#else
			source = obs_source_create("text_ft2_source_v2", sub_name.c_str(), nullptr, nullptr);
		#endif
			if (source) {
				// add source to the current scene
				obs_scene_add(scene, source);
				// set source settings
				obs_data_t *source_settings = obs_source_get_settings(source);
				obs_data_set_bool(source_settings, "word_wrap", true);
				obs_data_set_int(source_settings, "custom_width", 1760);
				obs_data_t *font_data = obs_data_create();
				obs_data_set_string(font_data, "face", "Noto Sans CJK KR");
				obs_data_set_string(font_data, "style", "Light");
				obs_data_set_int(font_data, "size", 40);
				obs_data_set_int(font_data, "flags", 0);
				obs_data_set_obj(source_settings, "font", font_data);
				obs_data_release(font_data);
				obs_source_update(source, source_settings);
				obs_data_release(source_settings);

				// set transform settings
				obs_transform_info transform_info;
				transform_info.pos.x = 962.0;
				transform_info.pos.y = 959.0;
				transform_info.bounds.x = 1769.0;
				transform_info.bounds.y = 145.0;
				transform_info.bounds_type = obs_bounds_type::OBS_BOUNDS_SCALE_INNER;
				transform_info.bounds_alignment = OBS_ALIGN_CENTER;
				transform_info.alignment = OBS_ALIGN_CENTER;
				transform_info.scale.x = 1.0;
				transform_info.scale.y = 1.0;
				transform_info.rot = 0.0;
				obs_sceneitem_t *source_sceneitem = obs_scene_sceneitem_from_source(scene, source);
				obs_sceneitem_set_info2(source_sceneitem, &transform_info);
				obs_sceneitem_release(source_sceneitem);

				obs_source_release(source);
				obs_source_release(scene_as_source); //if you don't release, OBS can't shut down gracefully when closed.

				return sub_name;
			}

		}
		

		
		

	}
	
	return "Darwin Realtime Subtitles 4"; //If there are already 4, just return the 4th one. You will have to manually delete them for now.
	
	
}

bool add_sources_to_list(void *list_property, obs_source_t *source)
{
	auto source_id = obs_source_get_id(source);
	if (strcmp(source_id, "text_ft2_source_v2") != 0 &&
	    strcmp(source_id, "text_gdiplus_v3") != 0 &&
	    strcmp(source_id, "text_gdiplus_v2") != 0) {
		return true;
	}

	obs_property_t *sources = (obs_property_t *)list_property;
	const char *name = obs_source_get_name(source);
	obs_property_list_add_string(sources, name, name);
	return true;
}

void remove_sources()
{
	for (int i = 1; i < 5; i++){
		std::string sub_name= "Darwin Realtime Subtitles " + std::to_string(i); 
		obs_source_t *source = obs_get_source_by_name(sub_name.c_str());
		if(source){
			obs_source_t *scene_as_source = obs_frontend_get_current_scene();
			obs_scene_t *scene = obs_scene_from_source(scene_as_source);
			obs_sceneitem_t *source_sceneitem = obs_scene_sceneitem_from_source(scene, source);
			obs_sceneitem_release(source_sceneitem);

			obs_source_release(source);
		}
	}

}