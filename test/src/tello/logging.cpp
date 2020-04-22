#include "logging.hpp"

void tello::test::initLogging()  {
    static bool alreadyBuilt = false;

    if (!alreadyBuilt) {
        string logDir("./log/");
        const LoggerSettings settings{logDir + "default.txt", logDir + "video.txt", logDir + "status.txt"};
        Logger::instance().initialize(settings);
        alreadyBuilt = true;
    }
}