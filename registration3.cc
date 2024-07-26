#include "ns3/core-module.h"

#include "ns3/network-module.h"

#include "ns3/internet-module.h"

#include "ns3/point-to-point-module.h"

#include "ns3/mobility-module.h"

#include "ns3/lte-module.h"

#include "ns3/applications-module.h"

#include "ns3/config-store-module.h"
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("5GPduSessionExample");
 
int main (int argc, char *argv[])

{

  Time::SetResolution (Time::NS);

  LogComponentEnable ("5GPduSessionExample", LOG_LEVEL_INFO);
 
  // Command line arguments

  CommandLine cmd;

  cmd.Parse (argc, argv);
 
  // Create nodes: one eNodeB and one UE

  NodeContainer ueNode;

  ueNode.Create (1);

  NodeContainer enbNode;

  enbNode.Create (1);
 
  // Install Mobility Model

  MobilityHelper mobility;

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (ueNode);

  mobility.Install (enbNode);
 
  // Create LTE modules

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
 
  // Install LTE Devices to the nodes

  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNode);

  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNode);
 
  // Install the IP stack on the UEs

  InternetStackHelper internet;

  internet.Install (ueNode);
 
  // Assign IP address to UEs

  Ipv4AddressHelper ipv4h;

  ipv4h.SetBase ("7.0.0.0", "255.0.0.0");

  Ipv4InterfaceContainer ueIpIface;

  ueIpIface = ipv4h.Assign (ueLteDevs);
 
  // Attach a UE to a eNodeB

  lteHelper->Attach (ueLteDevs.Get (0), enbLteDevs.Get (0));
 
  // Activate a PDU session

  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;

  EpsBearer bearer (q);

  lteHelper->ActivateDataRadioBearer (ueLteDevs, bearer);
 
  // Install and start applications on UEs and remote host

  uint16_t dlPort = 1234;

  uint16_t ulPort = 2000;

  ApplicationContainer clientApps, serverApps;
 
  UdpServerHelper dlPacketSinkHelper (dlPort);

  serverApps.Add (dlPacketSinkHelper.Install (ueNode.Get (0)));
 
  UdpClientHelper dlClient (ueIpIface.GetAddress (0), dlPort);

  dlClient.SetAttribute ("MaxPackets", UintegerValue (1000000));

  dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds (10)));

  dlClient.SetAttribute ("PacketSize", UintegerValue (1024));

  clientApps.Add (dlClient.Install (ueNode.Get (0)));
 
  UdpServerHelper ulPacketSinkHelper (ulPort);

  serverApps.Add (ulPacketSinkHelper.Install (ueNode.Get (0)));
 
  UdpClientHelper ulClient (ueIpIface.GetAddress (0), ulPort);

  ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));

  ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds (10)));

  ulClient.SetAttribute ("PacketSize", UintegerValue (1024));

  clientApps.Add (ulClient.Install (ueNode.Get (0)));
 
  serverApps.Start (Seconds (1.0));

  clientApps.Start (Seconds (1.0));
 
  // Enable tracing

  lteHelper->EnableTraces ();
 
  // Run simulation

  Simulator::Stop (Seconds (10.0));

  Simulator::Run ();

  Simulator::Destroy ();

  return 0;

}
