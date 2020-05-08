#pragma once

#include <tello/response.hpp>

namespace tello {

    class StatusResponse : public Response {

    public:
        explicit StatusResponse(const string& response);

        string get_mpry();

        int get_x();
        int get_y();
        int get_z();

        int get_vgx();
        int get_vgy();
        int get_vgz();
    	
        float get_agx();
        float get_agy();
        float get_agz();

        int get_pitch();
        int get_roll();
        int get_bat();
        int get_yaw();
    	
        int get_templ();
        int get_temph();

        int get_tof();
        int get_h();
        float get_baro();
        int get_time();

    private:
        unordered_map<string, string> values;
        float readFloat(string&& key, float defaultValue);
        int readInt(string&& key, int defaultValue);

    };
}