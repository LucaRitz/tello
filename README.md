# C++ Tello Lib
The intent of this library is its simplicity to use.
The following code section shows a basic setup for one Tello-drone.
```cpp
#include <tello/logger/logger_interface.hpp>
#include <tello/connection/tello_network.hpp>
#include <tello/tello.hpp>

using tello::LoggerSettings;
using tello::LoggerInterface;
using tello::TelloNetwork;
using tello::Tello;
using tello::Response;

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

int main() {
    // Initialize logging
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    LoggerInterface::initialize(settings);
    
    // Setup connection
    const bool isConnected = TelloNetwork::connect();
    assert(isConnected);
    
    // Create a tello drone object for interaction
    Tello tello(TELLO_IP_ADDRESS);
    
    // Enter command mode.
    future<Response> command_future = tello.command();
    // You don't have to wait for a response.
    // A response is timed-out after 15 seconds.
    command_future.wait();

    // There are some commands, which don't wait to a response
    // The status of the response will be 'UNKNOWN'
    future<Response> rc_future = tello.rc_control(10, -10, 0, 0);
    
    // More commands
    // ...
    
    // At least, the connections have to be closed.
    TelloNetwork::disconnect();
}
```

Another possibility is the setup of a swarm.
```cpp
// includes ...
#include <tello/swarm.hpp>
// usings ...

using tello::Swarm;

int main() {
    // setup ...

    // Create tello drones object for interaction
    Tello tello_1(TELLO_IP_ADDRESS_1);
    Tello tello_2(TELLO_IP_ADDRESS_2);
    // ...
    Tello tello_n(TELLO_IP_ADDRESS_n);
     
    // Create a swarm object and add tellos
    Swarm swarm;
    swarm << tello_1 << tello_2 << ... << tello_n;
     
    // Enter command mode
    unordered_map<ip_address, future<Response>> response_futures = swarm.command();
    for(auto& entry : response_futures) {
        entry.second.wait();
    }
    // May repeate command for a single tello if one failed.
     
     
    // More commands and tear down
    // ...   
}
```

## Build
Per default, a static library is built. One can set the option<br>
'TELLO_BUILD_SHARED_LIBS' to ON to build a shared library.<br>
Note: The target 'tello_test' cannot be built with this option on,<br>
because there are some usages, which are not exported (e.g. command_test.cpp).<br>
Use the target 'shared_linking_test' instead.

### OS & Compiler
C++ 17

OS
- Windows

Compiler
- nmake
- mingw/g++ 8.1.0 - Windows MSYS2

## Third-party libs
- Logging: spdlog (https://github.com/gabime/spdlog)
- Thread-pooling: ctpl (https://github.com/vit-vit/CTPL)
- Testing: googletest (https://github.com/google/googletest)

# Tello SDK 2.0

## Send Command & Receive Response
Tello IP: 192.168.10.1
UDP Port (Both): 8889

Listen to: 0.0.0.0

## Receive Tello State
Tello IP: 192.168.10.1

Listen to: 0.0.0.0
UDP Port: 8890

## Receive Tello Video Stream
Tello IP: 192.168.10.1

Listen to: 0.0.0.0
UDP Port: 11111

# Tello Commands
## Control Commands
|                        Command                       |                                                                                                                                                                             Description                                                                                                                                                                             | Possible Response |     Implemented    |
|:----------------------------------------------------:|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-----------------:|:------------------:|
|                        command                       |                                                                                                                                                                           Enter SDK mode.                                                                                                                                                                           |     ok / error    | :heavy_check_mark: |
|                        takeoff                       |                                                                                                                                                                            Auto takeoff.                                                                                                                                                                            |     ok / error    | :heavy_check_mark: |
|                         land                         |                                                                                                                                                                            Auto landing.                                                                                                                                                                            |     ok / error    | :heavy_check_mark: |
|                       streamon                       |                                                                                                                                                                         Enable video stream.                                                                                                                                                                        |     ok / error    | :heavy_check_mark: |
|                       streamoff                      |                                                                                                                                                                        Disable video stream.                                                                                                                                                                        |     ok / error    | :heavy_check_mark: |
|                       emergency                      |                                                                                                                                                                       Stop motors immediately.                                                                                                                                                                      |     ok / error    | :heavy_check_mark: |
|                        up "x"                        |                                                                                                                                                                   Ascend to "x" cm.<br>x = 20-500                                                                                                                                                                   |     ok / error    | :heavy_check_mark: |
|                       down "x"                       |                                                                                                                                                              down "x" Descend to "x" cm.<br>x = 20-500                                                                                                                                                              |     ok / error    | :heavy_check_mark: |
|                       left "x"                       |                                                                                                                                                                  Fly left for "x" cm.<br>x = 20-500                                                                                                                                                                 |     ok / error    | :heavy_check_mark: |
|                       right "x"                      |                                                                                                                                                                 Fly right for "x" cm.<br>x = 20-500                                                                                                                                                                 |     ok / error    | :heavy_check_mark: |
|                      forward "x"                     |                                                                                                                                                                Fly forward for "x" cm.<br>x = 20-500                                                                                                                                                                |     ok / error    | :heavy_check_mark: |
|                       back "x"                       |                                                                                                                                                                Fly backward for "x" cm.<br>x = 20-500                                                                                                                                                               |     ok / error    | :heavy_check_mark: |
|                        cw "x"                        |                                                                                                                                                              Rotate "x" degrees clockwise.<br>x = 1-360                                                                                                                                                             |     ok / error    | :heavy_check_mark: |
|                        ccw "x"                       |                                                                                                                                                        Rotate "x" degrees counterclockwise.<br><br>x = 1-360                                                                                                                                                        |     ok / error    | :heavy_check_mark: |               |
|                       flip "x"                       |                                                                                                                              Flip in "x" direction.<br>x = "l" or "r" or "f" or "b"<br>l = left<br>r = right<br>f = forward<br>b = back                                                                                                                             |     ok / error    | :heavy_check_mark: |
|                go "x" "y" "z" "speed"                |                                                                                        Fly to "x" "y" "z" at "speed" (cm/s).<br>x = -500-500<br>y = -500-500<br>z = -500-500<br>speed = 10-100<br>Note: "x", "y" and "z" values can't be set between<br>-20-20 siultaneously.                                                                                       |     ok / error    |         :x:        |
|                         stop                         |                                                                                                                                                            Hovers in the air.<br>Note: works at any time.                                                                                                                                                           |     ok / error    | :heavy_check_mark: |
|    curve "x1" "y1" "z1" "x2" "y2"<br>"z2" "speed"    |            Fly at a curve according to the two given coordinates<br>at "speed" (cm/s).<br>If the arc radius is not within a range of 0.5-10 meters,<br>it will respond with an error.<br>x1, x2 = -500-500<br>y1, y2 = -500-500<br>z1, z2 = -500-500<br>speed = 10-60<br><br>Note: "x", "y" and "z" values can't be set between<br>-20-20 siultaneously.            |     ok / error    |         :x:        |
|             go "x" "y" "z" "speed" "mid"             |                                                            Fly to the "x", "y" and "z" coordinates of the Mission<br>Pad.<br><br>mid = m1-m8<br>x = -500-500<br>y = -500-500<br>z = -500-500<br>speed = 10-100 (cm/s)<br><br>Note: "x", "y" and "z" values can't be set between<br>-20-20 siultaneously.                                                            |     ok / error    |         :x:        |
| curve "x1" "y1" "z1" "x2" "y2"<br>"z2" "speed" "mid" | Fly at a curve according to the two given coordinates<br>of the Mission Pad ID at "speed" (cm/s).<br>If the arc radius is not within a range of 0.5-10 meters,<br>it will respond with an error.<br>x1, x2 = -500-500<br>y1, y2 = -500-500<br>z1, z2 = -500-500<br>speed = 10-60<br><br>Note: "x", "y" and "z" values can't be set between<br>-20-20 siultaneously. |     ok / error    |         :x:        |
|    jump "x" "y" "z" "speed" "yaw"<br>"mid1" "mid2"   |                       Fly to coordinates "x", "y" and "z" of Mission Pad 1,<br>and recognize coordinates 0, 0, "7" of Mission Pad 2<br>and rotate to the yaw value.<br>mid = m1-m8<br>x = -500-500<br>y = -500-500<br>z = -500-500<br>speed = 10-100 (cm/s)<br><br>Note: "x", "y" and "z" values can't be set between<br>-20-20 siultaneously.                      |     ok / error    |         :x:        |

## Set Commands
|       Command      |                                                                                                                                                                                                       Description                                                                                                                                                                                                       | Possible Response | Implemented |
|:------------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-----------------:|:-----------:|
|      speed "x"     |                                                                                                                                                                                           Set speed to "x" cm/s.<br>x = 10-100                                                                                                                                                                                          |     ok / error    | :heavy_check_mark: |
| rc "a" "b" "c" "d" |                                                                                                                                 Set remote controller via four channels.<br>a = left/right (-100-100)<br>b = forward/backward (-100-100)<br>c = up/down (-100-100)<br>d = yaw (-100-100)                                                                                                                                |     ok / error    |     :x:     |
| wifi "ssid" "pass" |                                                                                                                                                                    Set Wi-Fi password.<br>ssid = updated Wi-Fi name<br>pass = updated Wi-Fi password                                                                                                                                                                    |     ok / error    |     :x:     |
|         mon        |                                                                                                                                                                          Enable mission pad detection (both forward and<br>downward detection).                                                                                                                                                                         |     ok / error    |     :x:     |
|        moff        |                                                                                                                                                                                              Disable mission pad detection.                                                                                                                                                                                             |     ok / error    |     :x:     |
|   mdirection "x"   | x = 0 or 1 or 2<br><br>0 = Enable downward detection only<br>1 = Enable forward detection only<br>2 = Enable both forward and downward detection<br><br>Notes:<br>Perform "mon" command before performing this<br>command.<br>The detection frequency is 20 Hz if only the forward<br>or downward detection is enabled. If both the forward<br>and downward detection are enabled, the detection<br>frequency is 10 Hz. |     ok / error    |     :x:     |
|  ap "ssid" "pass"  |                                                                                                                   Set the Tello to station mode, and connect to a<br>new access point with the access point's ssid and<br>password.<br><br>ssid = updated Wi-Fi name<br>pass = updated Wi-Fi password                                                                                                                   |     ok / error    |     :x:     |

## Read Commands
|  Command |             Description            | Possible Response | Implemented |
|:--------:|:----------------------------------:|:-----------------:|:-----------:|
|  speed?  |     Obtain current speed (cm/s)    |     x = 10-100    | :heavy_check_mark: |
| battery? | Obtain current battery percentage. |     x = 0-100     |     :x:     |
|   time?  |     Obtain current flight time.    |        time       |     :x:	  |
|   wifi?  |          Obtain Wi-Fi SNR.         |        snr        | :heavy_check_mark: |
|   sdk?   |    Obtain the Tello SDK version.   |    sdk version    |     :x:     |
|    sn?   |   Obtain the Tello serial number.  |   serial number   |     :x:     |

## Tello State
**Data string received when the mission pad detection feature is enabled:**
<br>
"ata string received when the mission pad detection feature is enabled:requency is 10 Hz.I<br>
theof:%d;h:%d;bat:%d;baro:%f;\r\nm"
<br>
<br>
**Data string received when the mission pad detection feature is disabled:**
<br>
"pitch:%d;roll%d;yaw:%d;vgw:%d;vgy:%d;vgz:%d;templ:%d;temph:%d;tof:%d;h:%d;bat:%d;baro:%.2f;<br>
time:%d;agx:%.2f;agy:%.2f;agz:%.2f;\r\n"
<br>
<br>
**Description**
<br>
**mid** = the ID of the Mission Pad detected. If no Mission Pad is detected,<br>a "-1" message will be reeived instead.
<br>
**x** = the "x" coordinate detected on the Mission Pad. If there is no Mission Pad,<br>a "0" message will be received instead.
<br>
**y** = the "y" coordinate detected on the Mission Pad. If there is no Mission Pad,<br>a "0" message will be received instead.
<br>
**z** = the "z" coordinate detected on the Mission Pad. If there is no Mission Pad,<br>a "0" message will be received instead.
<br>
**pitch** = the degree of the attitude pitch.
<br>
**roll** = the degreee of the attitude roll.
<br>
**yaw** = the degree of the attitude yaw.
<br>
**vgx** = the speed of "x" axis.
<br>
**vgy** = the speed of "y" axis.
<br>
**vgz** = the speed of "z" axis.
<br>
**templ** = the lowest temperature in degree Celsius.
<br>
**temph** = the highest temperature in degree Celsius.
<br>
**tof** = the time of flight distance in cm.
<br>
**h** = the height in cm.
<br>
**bat** = the percentage of the current battery level.
<br>
**baro** = the barometer measurement in cm.
<br>
**time** = the amount of time the motor has been used.
<br>
**agx** = the acceleration of the "x" axis.
<br>
**agy** = the acceleration of the "y" axis.
<br>
**agz** = the acceleration of the "z" axis.
<br>
