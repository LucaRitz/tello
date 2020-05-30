#pragma once

#include <tello/response.hpp>

namespace tello {

    class StatusResponse : public Response {

    public:
        explicit StatusResponse(const string& response);

        string get_mpry() const;

        int get_x() const;
        int get_y() const;
        int get_z() const;

        int get_vgx() const;
        int get_vgy() const;
        int get_vgz() const;

        /**
         * Acceleration on the x-axis
         */
        float get_agx() const;

        /**
         * Acceleration on the y-axis
         */
        float get_agy() const;

        /**
         * Acceleration on the z-axis
         */
        float get_agz() const;

        int get_pitch() const;
        int get_roll() const;
        int get_bat() const;
        int get_yaw() const;

        /**
         * Low temperature in degree Celcius
         */
        int get_templ() const;

        /**
         * High temperature in degree Celcius
         */
        int get_temph() const;

        /**
         * Time Of Flight distance in cm
         */
        int get_tof() const;

        /**
         * Height in cm
         */
        int get_h() const;

        /**
         * Barometer measurement in cm
         */
        float get_baro() const;

        /**
         * Amount of time the motor has been used.
         */
        int get_time() const;

    private:
        unordered_map<string, string> values;
        float readFloat(string&& key, float defaultValue) const;
        int readInt(string&& key, int defaultValue) const;

    };
}