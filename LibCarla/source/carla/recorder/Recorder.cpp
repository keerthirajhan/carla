// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Recorder.h"

#include <ctime>
#include <sstream>

namespace carla {
namespace recorder {


Recorder::Recorder(){
}

Recorder::~Recorder(){
}

std::string Recorder::start(std::string path, std::string name, std::string mapName) {

    // reset
    stop();

    std::stringstream filename;
    filename << path << name;

    // files
    // file.open(filename.str(), std::ios::binary | std::ios::trunc | std::ios::out);
    file.open(filename.str(), std::ios::binary);
    if (!file.is_open()) {
        log_error("Recorder file couldn't be created");
        return "";
    }

    // log
    log.open(filename.str() + ".log");

    std::string magic("CARLA_RECORDER");
    // save info
    info.version = 1;
    info.magic.copy_from(magic);
    info.date = std::time(0);
    info.mapfile.copy_from(mapName);

    // write general info
    info.write(file);

    frames.reset();
    enabled = true;

    return filename.str();
}

void Recorder::stop() {
    enabled = false;
    if (file.is_open()) file.close();
    if (log.is_open()) log.close();
    // log << "Stop\n";
    clear();
}

void Recorder::clear(void) {
    events.clear();
    positions.clear();
    states.clear();

    // log << "Clear\n";
}

void Recorder::write(void) {
    // update this frame data
    // log << "Write\n";
    frames.setFrame();
    frames.write(file, log);
    events.write(file, log);
    positions.write(file, log);
    states.write(file, log);

    clear();
}

void Recorder::addPosition(const RecorderPosition position) {
    if (enabled)
        positions.addPosition(std::move(position));
}
void Recorder::addEvent(RecorderEventAdd event) {
    if (enabled)
        events.addEvent(std::move(event));
}
void Recorder::addEvent(const RecorderEventDel event) {
    if (enabled)
        events.addEvent(std::move(event));
}
void Recorder::addEvent(const RecorderEventParent event) {
    if (enabled)
        events.addEvent(std::move(event));
}
void Recorder::addState(const RecorderStateTrafficLight state) {
    if (enabled)
        states.addState(std::move(state));
}

}
}