// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Recorder.h"
#include "RecorderPosition.h"
#include "RecorderHelpers.h"

namespace carla {
namespace recorder {

void RecorderPosition::write(std::ofstream &file) {
    // database id
    writeValue<uint32_t>(file, this->databaseId);
    // transform
    writeTransform(file, this->transform);
    // velocity
    // writeVector3D(file, this->velocity);
    // velocity
    // writeVector3D(file, this->angularVelocity);
}
void RecorderPosition::read(std::ifstream &file) {
    // database id
    readValue<uint32_t>(file, this->databaseId);
    // transform
    readTransform(file, this->transform);
    // velocity
    // readVector3D(file, this->velocity);
    // velocity
    // readVector3D(file, this->angularVelocity);
}

//---------------------------------------------

void RecorderPositions::clear(void) {
    positions.clear();
}

void RecorderPositions::addPosition(const RecorderPosition &position) {
    positions.push_back(std::move(position));
    //log << "add position (" << sizeof(position) << ") " << position.transform.location.x << "," << position.transform.location.y << "," << position.transform.location.z << std::endl;
    //log << "add position for id." << position.id << std::endl;
}

void RecorderPositions::write(std::ofstream &file, std::ofstream &log) {

    // write the packet id
    writeValue<char>(file, static_cast<char>(RecorderPacketId::Position));

    // write the packet size
    uint32_t total = 2 + positions.size() * sizeof(RecorderPosition);
    writeValue<uint32_t>(file, total);

    // write total records
    total = positions.size();
    writeValue<short>(file, total);

    // write records
    file.write(reinterpret_cast<const char*>(positions.data()), positions.size() * sizeof(RecorderPosition));
/*
    // write records
    for (auto rec : positions) {
        writeValue<uint32_t>(file, rec.id);                  // actor id
        writeTransform(file, rec.transform);            // transform
        writeVector3D(file, rec.velocity);              // velocity
        writeVector3D(file, rec.angularVelocity);       // angular velocity
    }
*/

    log << "write positions (" << positions.size() << " * " << sizeof(RecorderPosition) << ")\n";
}

}
}