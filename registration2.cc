#include "ns3/core-module.h"

#include "ns3/network-module.h"

#include "ns3/internet-module.h"

#include "ns3/applications-module.h"
 
#include <iostream>

#include <vector>

#include <string>
 
using namespace ns3;
 
// Define a class for Network Slice

class NetworkSlice {

public:

    NetworkSlice(int id, std::string characteristics)

        : sliceId(id), serviceCharacteristics(characteristics) {}
 
    // Handle message reception from another slice

    void receiveMessage(const std::string& message, int fromSliceId) {

        std::cout << "Slice " << sliceId << " received message from Slice "

                  << fromSliceId << ": " << message << std::endl;

    }
 
    // Get slice ID

    int getId() const { return sliceId; }
 
private:

    int sliceId;

    std::string serviceCharacteristics;

};
 
// Function to simulate sending a message from one slice to another

void sendMessage(NetworkSlice& sender, NetworkSlice& receiver, const std::string& message) {

    std::cout << "Slice " << sender.getId() << " sending message to Slice " << receiver.getId() << std::endl;

    receiver.receiveMessage(message, sender.getId());

}
