#include "tello/response/status_response.hpp"

#include <gtest/gtest.h>

using tello::StatusResponse;
using std::string;

const string STRING_TO_PARSE = "mid:-1;x:-100;y:-100;z:-100;mpry:-1,-1,-1;pitch:-1;roll:0;yaw:0;vgx:10;vgy:10;vgz:10;templ:55;temph:57;tof:10;h:0;bat:55;baro:681.32;time:0;agx:-10.00;agy:-3.00;agz:-1000.00;";

TEST(StatusResponse, ParseString_mpry_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ("-1,-1,-1", response.get_mpry());
}

TEST(StatusResponse, ParseString_x_Test) {
	// Arrange && Act
    StatusResponse response = StatusResponse(STRING_TO_PARSE);

    // Assert
	ASSERT_EQ(-100, response.get_x());
}

TEST(StatusResponse, ParseString_y_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-100, response.get_y());
}

TEST(StatusResponse, ParseString_z_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-100, response.get_z());
}


TEST(StatusResponse, ParseString_vgx_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(10, response.get_vgx());
}

TEST(StatusResponse, ParseString_vgy_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(10, response.get_vgy());
}

TEST(StatusResponse, ParseString_vgz_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(10, response.get_vgz());
}


TEST(StatusResponse, ParseString_agx_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-10, response.get_agx());
}

TEST(StatusResponse, ParseString_agy_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-3, response.get_agy());
}

TEST(StatusResponse, ParseString_agz_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-1000, response.get_agz());
}


TEST(StatusResponse, ParseString_pitch_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-1, response.get_pitch());
}

TEST(StatusResponse, ParseString_roll_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-1, response.get_roll());
}

TEST(StatusResponse, ParseString_bat_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(55, response.get_bat());
}

TEST(StatusResponse, ParseString_yaw_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-1, response.get_yaw());
}


TEST(StatusResponse, ParseString_templ_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(55, response.get_templ());
}

TEST(StatusResponse, ParseString_temph_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(57, response.get_temph());
}


TEST(StatusResponse, ParseString_tof_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(10, response.get_tof());
}

TEST(StatusResponse, ParseString_h_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(0, response.get_h());
}

TEST(StatusResponse, ParseString_baro_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(681.32, response.get_baro());
}

TEST(StatusResponse, ParseString_time_Test) {
	// Arrange && Act
	StatusResponse response = StatusResponse(STRING_TO_PARSE);

	// Assert
	ASSERT_EQ(-1, response.get_time());
}