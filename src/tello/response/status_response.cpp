#include <iostream>
#include <tello/response/status_response.hpp>
#include <sstream>

using std::stringstream;

tello::StatusResponse::StatusResponse(const string& response) :
	Response(Status::OK), values(unordered_map<string, string>()) {

    stringstream ss(response);
    while (ss)
    {
        string keyValue{};
        std::string key{}, value{};
        {
            getline(ss, keyValue, ';');

            stringstream kvs(keyValue);
            string key, value;
            getline(kvs, key, ':');
            getline(kvs, value);

            values[key] = value;
        }
    }

	//Debug output
    for (const auto& elem : values)
    {
        std::cout << elem.first << " " << elem.second << "\n";
    }
}


float tello::StatusResponse::readFloat(string& key, float defaultValue)
{
    if (values.empty())
    {
        return defaultValue;
    }

    auto item = values.find(key);
    float value = stof(item->second);
    return value;
}

int tello::StatusResponse::readInt(string& key, int defaultValue)
{
    if (values.empty())
    {
        return defaultValue;
    }

    auto item = values.find(key);
    int value = stoi(item->second);
    return value;
}


string tello::StatusResponse::get_mpry()
{
	//TODO: Hanlde mpry response
    return string("TODO");
}


int tello::StatusResponse::get_x() {
    return readInt(string("x"), 0);
}

int tello::StatusResponse::get_y() {
    return readInt(string("y"), 0);
}

int tello::StatusResponse::get_z() {
    return readInt(string("z"), 0);
}


int tello::StatusResponse::get_vgx() {
    return readInt(string("vgx"), 0);
}

int tello::StatusResponse::get_vgy() {
    return readInt(string("vgy"), 0);
}

int tello::StatusResponse::get_vgz() {
    return readInt(string("vgz"), 0);
}


float tello::StatusResponse::get_agx() {
    return readFloat(string("agx"), 0.0f);
}

float tello::StatusResponse::get_agy() {
    return readFloat(string("agy"), 0.0f);
}

float tello::StatusResponse::get_agz() {
    return readFloat(string("agz"), 0.0f);
}


int tello::StatusResponse::get_pitch() {
    return readInt(string("pitch"), 0);
}

int tello::StatusResponse::get_roll() {
    return readInt(string("roll"), 0);
}

int tello::StatusResponse::get_bat() {
    return readInt(string("bat"), 0);
}

int tello::StatusResponse::get_yaw() {
    return readInt(string("yaw"), 0);
}

int tello::StatusResponse::get_templ() {
    return readInt(string("templ"), 0);
}

int tello::StatusResponse::get_temph() {
    return readInt(string("temph"), 0);
}


int tello::StatusResponse::get_tof() {
    return readInt(string("tof"), 0);
}

int tello::StatusResponse::get_h() {
    return readInt(string("h"), 0);
}

float tello::StatusResponse::get_baro() {
    return readFloat(string("baro"), 0);
}

int tello::StatusResponse::get_time() {
    return readInt(string("time"), 0);
}


int get_pitch();
int get_roll();
int get_bat();
int get_yaw();