#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
 
using namespace ns3;
 
// Define the UPF Application
class UpfApplication : public Application {
private:
    void HandleRequest(std::string pduSessionId) {
        std::cout << "UPF: Handling N4 Session Establishment Request for PDU Session ID: " << pduSessionId << std::endl;
        // Simulating a successful response
        std::cout << "UPF: Session Established Successfully for Session ID: " << pduSessionId << std::endl;
    }
 
protected:
    virtual void StartApplication() override {
        std::cout << "UPF Application Started." << std::endl;
    }
 
    virtual void StopApplication() override {
        std::cout << "UPF Application Stopped." << std::endl;
    }
 
public:
    // Public method to initiate request handling from SMF
    void ReceiveN4Request(std::string pduSessionId) {
        HandleRequest(pduSessionId);
    }
};
 
// Define the SMF Application
class SmfApplication : public Application {
private:
    Ptr<UpfApplication> upfApp;
 
protected:
    virtual void StartApplication() override {
        std::cout << "SMF Application Started." << std::endl;
        Simulator::Schedule(Seconds(1.0), &SmfApplication::TriggerSessionEstablishment, this, "PDU1234");
    }
 
    virtual void StopApplication() override {
        std::cout << "SMF Application Stopped." << std::endl;
    }
 
public:
    void SetUpfApp(Ptr<UpfApplication> app) {
        upfApp = app;
    }
 
    void TriggerSessionEstablishment(std::string pduSessionId) {
        std::cout << "SMF: Triggering Session Establishment for PDU Session ID: " << pduSessionId << std::endl;
        upfApp->ReceiveN4Request(pduSessionId);
    }
};
 
int main() {
    NodeContainer nodes;
    nodes.Create(2);
 
    InternetStackHelper stack;
    stack.Install(nodes);
 
    Ptr<UpfApplication> upfApp = CreateObject<UpfApplication>();
    nodes.Get(1)->AddApplication(upfApp);
    upfApp->SetStartTime(Seconds(0.1));
    upfApp->SetStopTime(Seconds(10.0));
 
    Ptr<SmfApplication> smfApp = CreateObject<SmfApplication>();
    smfApp->SetUpfApp(upfApp);
    nodes.Get(0)->AddApplication(smfApp);
    smfApp->SetStartTime(Seconds(0.1));
    smfApp->SetStopTime(Seconds(10.0));
 
    Simulator::Run();
    Simulator::Destroy();
 
    return 0;
}
