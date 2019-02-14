/**
 * This file is part of input-overlay
 * which is licensed under the MPL 2.0 license
 * See LICENSE or mozilla.org/en-US/MPL/2.0/
 * github.com/univrsal/input-overlay
 */

#pragma once

#include <obs-module.h>
#include <string>
#include <uiohook.h>
#include "../util/overlay.hpp"

extern "C" {
#include <graphics/image-file.h>
}

typedef struct obs_data obs_data_t;

namespace sources
{
    class shared_settings
    {
    public:
        std::string image_file;
        std::string layout_file;

        uint32_t cx = 0, cy = 0;                /* Source width/height */
        bool use_center = false;                /* true if monitor center is used for mouse movement */
        uint32_t monitor_w = 0, monitor_h = 0;
        uint8_t mouse_deadzone = 0;
        uint16_t mouse_sens = 0;                /* mouse_delta / mouse_sens = mouse movement */
        uint8_t gamepad = 0;                    /* selected gamepad id */

#ifdef _WIN32
        float left_dz = 0.f, right_dz = 0.f;    /* Windows gamepad analog sticks deadzone  (in %)*/
#endif
		uint8_t selected_source = 0;            /* 0 = Local input */
        uint8_t layout_flags = 0;               /* See overlay_flags in layout_constants.hpp */
    
        obs_source_t* text_source = nullptr; /* contains an instance of freetype/gdi+ source if overlay uses text */
    };

    class input_source
    {
    public:
        obs_source_t* m_source = nullptr;
        obs_data_t* m_source_settings = nullptr;

        uint32_t cx = 0, cy = 0;
        std::unique_ptr<overlay> m_overlay{};
        shared_settings m_settings;
        
        input_source(obs_source_t* source, obs_data_t* settings) :
            m_source(source), m_source_settings(settings)
        {
            m_overlay = std::make_unique<overlay>(&m_settings);
            load_text_source();
            obs_source_update(m_source, settings);
        }

        ~input_source()
        {
            unload_text_source();
        }

        inline void update(obs_data_t* settings);
        inline void tick(float seconds);
        inline void render(gs_effect_t* effect) const;

        void load_text_source();
        void unload_text_source();
    };

    /* Event handlers */
    static bool use_monitor_center_changed(obs_properties_t* props, obs_property_t* p, obs_data_t* data);
	static bool reload_connections(obs_properties_t* props, obs_property_t* property, void* data);
    static bool toggle_font_settings(obs_properties_t* props, obs_property_t* p, obs_data_t* data);

    /* For registering */
    static obs_properties_t* get_properties_for_overlay(void* data);

    void register_overlay_source();
}
