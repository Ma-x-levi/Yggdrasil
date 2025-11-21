#include <NanoYggdrasil.h>
#include "common/OsalTask.h"
#include "common/OsalLogger.h"


#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>


static OsalTaskManager osalTaskManager;
static OsalLogger osalLogger("console", true);


int main(int argc, char *argv[]) 
{
    NanoYggdrasil::instance().RegisterLoggerHandle(std::ref(std::reference_wrapper<OsalLogger>(osalLogger)));
    NanoYggdrasil::instance().RegisterOsalTaskManager(std::ref(std::reference_wrapper<OsalTaskManager>(osalTaskManager)));

    NanoYggdrasil::instance().Init();

    NanoYggdrasil::instance().Execute();
    return EXIT_SUCCESS;
}
