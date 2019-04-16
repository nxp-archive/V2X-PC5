/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
  This software was developed at the National Institute of Standards and
  Technology by employees of the Federal Government in the course of
  their official duties. Pursuant to titleElement 17 Section 105 of the United
  States Code this software is not subject to copyright protection and
  is in the public domain.
  NIST assumes no responsibility whatsoever for its use by other parties,
  and makes no guarantees, expressed or implied, about its quality,
  reliability, or any other characteristic.

  We would appreciate acknowledgement if the software is used.

  NIST ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" CONDITION AND
  DISCLAIM ANY LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING
  FROM THE USE OF THIS SOFTWARE.
*/

#include <ns3/applications-module.h>
#include <ns3/config-store.h>
#include <ns3/core-module.h>
#include <ns3/internet-module.h>
#include <ns3/mobility-module.h>
#include <ns3/network-module.h>
#include <ns3/nist-lte-helper.h>
#include <ns3/nist-module.h>
#include "ns3/nist-sl-resource-pool-factory.h"
#include "ns3/nist-sl-preconfig-pool-factory.h"

/*
This example is used to validate the maximum achievable data rate on the Physical Sidelink Shared
Channel (PSSCH) between 2 UEs given a particular resource pool configuration.
Parameters include:
- period: duration of the sidelink period
- pscchLength: length of the pysical sidelink control channel (PSCCH)
- ktrp: repetition pattern defining how many subframe can be used for sidelink
- mcs: modulation and coding scheme for transmission on the sidelink shared channel
- rbSize: allocation size in resource block (RB)
*/
NS_LOG_COMPONENT_DEFINE ("wns3_2017_pssch");

using namespace ns3;


int
main (int argc, char *argv[])
{
  //DEBUG components

  //LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteUeMac", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteUePhy", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteSpectrumPhy", LOG_LEVEL_ALL);
  //LogComponentEnable("Node", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteUeRrc",LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteSpectrumValueHelper", LOG_LEVEL_ALL);
  //LogComponentEnable("PropagationLossModel", LOG_LEVEL_ALL);
  //LogComponentEnable("MultiModelSpectrumChannel", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteSlInterference", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteSlChunkProcessor", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLtePhyErrorModel", LOG_LEVEL_ALL);
  //LogComponentEnable("NistLteSpectrumValueHelper", LOG_LEVEL_ALL);



  // Initialize some values
  uint32_t mcs = 7; //modulation and coding scheme for transmission on the sidelink shared channel
  uint32_t rbSize = 16; //Number of RBs per subchannel
  uint32_t ktrp = 8; //used by the original implementation: repetition pattern defining how many subframe can be used for sidelink (set to 8 so the pool is totally used for Sidelink)
  uint32_t pscchLength = 8; //used by the original implementation: in the repetition pattern, PSCCH can be transmitted in all subframes
  std::string period="sf40";
  //double dist = 10; //old = 23000
  double simTime = 10.0; //Simulation time
  double ueTxPower = 23.0; //Transmission power: 23 dbm
  uint32_t ueCount = 200; //Number of vehicles
  bool verbose = false;
  double noisefigure = 9; //old=5.1


  // Command line arguments
  CommandLine cmd;
  cmd.AddValue ("period", "Sidelink period", period);
  cmd.AddValue ("pscchLength", "Length of PSCCH.", pscchLength);
  cmd.AddValue ("ktrp", "Repetition.", ktrp);
  cmd.AddValue ("mcs", "MCS.", mcs);
  cmd.AddValue ("rbSize", "Allocation size.", rbSize);
  cmd.AddValue ("verbose", "Print time progress.", verbose);

  cmd.Parse(argc, argv);

  NS_LOG_INFO ("Configuring UE settings...");
  Config::SetDefault ("ns3::NistLteUeMac::SlGrantMcs", UintegerValue (mcs));
  Config::SetDefault ("ns3::NistLteUeMac::SlGrantSize", UintegerValue (rbSize));
  Config::SetDefault ("ns3::NistLteUeMac::Ktrp", UintegerValue (ktrp));
  Config::SetDefault ("ns3::NistLteUePhy::TxPower", DoubleValue (ueTxPower));
  Config::SetDefault ("ns3::NistLteUePowerControl::Pcmax", DoubleValue (ueTxPower));
  Config::SetDefault ("ns3::NistLteUePowerControl::PscchTxPower", DoubleValue (ueTxPower));
  Config::SetDefault ("ns3::NistLteUePowerControl::PsschTxPower", DoubleValue (ueTxPower));
  //Config::SetDefault ("ns3::NistLteSpectrumPhy::DropRbOnCollisionEnabled",BooleanValue (true)); //in case of collision the packet is dropped
  Config::SetDefault ("ns3::NistLteUePhy::NoiseFigure", DoubleValue (noisefigure));
  //Config::SetDefault ("ns3::LOSNLOSPropagationLossModel::factor", DoubleValue(0.5)); //pathloss model for Manhatten Grid
  //Config::SetDefault ("ns3::LOSNLOSPropagationLossModel::DistanceBetween2ParallelRoad", DoubleValue(200)); //pathloss model for Manhatten Grid

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();

  NS_LOG_INFO ("Creating helpers...");
  Ptr<NistPointToPointEpcHelper>  epcHelper = CreateObject<NistPointToPointEpcHelper> ();
  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  Ptr<NistLteHelper> lteHelper = CreateObject<NistLteHelper> ();
  lteHelper->SetAttribute ("UseSidelink", BooleanValue (true));
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->DisableNewEnbPhy ();
  //lteHelper->SetAttribute("PathlossModel",StringValue("ns3::LOSNLOSPropagationLossModel")); //pathloss model for Manhatten Grid
  lteHelper->Initialize ();

  Ptr<NistLteProseHelper> proseHelper = CreateObject<NistLteProseHelper> ();
  proseHelper->SetLteHelper (lteHelper);

  NS_LOG_INFO ("Deploying UE's...");
  NodeContainer ueResponders;
  ueResponders.Create (ueCount);

  // Fix mobility model: vehicles are positionned in a grid
  /*MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0),
                                 "MinY", DoubleValue (-5.0),
                                 "DeltaX", DoubleValue (dist),
                                 "DeltaY", DoubleValue (dist),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueResponders);*/


  //set the position of vehicles: fix position
  /*Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < (int)ueCount; i++)
  	 {
	  positionAlloc->Add (Vector(5 * i, 0, 0));
	  	  if (i<101)
	  	  {
		  	  positionAlloc->Add (Vector(1.0 * i, 0, 0));
	  	  } else if (i<201){
		  	  positionAlloc->Add (Vector(1.0 * (i-100), 1, 0));
	  	  } else if (i<301) {
		  	  positionAlloc->Add (Vector(1.0 * (i-200), 2, 0));
	  	  } else {
		  	  positionAlloc->Add (Vector(1.0 * (i-300), 3, 0));

	  	  }
  	 }

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install (ueResponders);*/


  //Positions from a SUMO trace file
  //Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/500m3l200vh.tcl");
  Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/600m300vhs3l.tcl");
  //Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/ManhattenGrid300vhs.tcl");
  //Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/70vhs250kmh.tcl");
  //Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/123vhs140kmh.tcl");
  //Ns2MobilityHelper mobHelper = Ns2MobilityHelper("scratch/246vhs70kmh.tcl");

  mobHelper.Install (); //install the mobility model if we use the SUM trace file for mobility

  NS_LOG_INFO ("Installing UE network devices...");
  NetDeviceContainer ueDevs;
  NetDeviceContainer ueRespondersDevs = lteHelper->InstallUeDevice (ueResponders);
  ueDevs.Add (ueRespondersDevs);

  NS_LOG_INFO ("Installing IP stack...");
  InternetStackHelper internet;
  internet.Install (ueResponders);
  
  NS_LOG_INFO ("Allocating IP addresses and setting up network route...");
  Ipv4InterfaceContainer ueIpIface;
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  ueIpIface = epcHelper->AssignUeIpv4Address (ueDevs);
  for (uint32_t u = 0; u < ueResponders.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueResponders.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  
  NS_LOG_INFO ("Attaching UE's to LTE network...");
  lteHelper->Attach (ueDevs);
  
  NS_LOG_INFO ("Installing applications...");
  // UDP application
  Ipv4Address groupAddress ("225.0.0.0"); //use multicast address as destination  
  UdpClientHelper udpClient (groupAddress , 8000); 
  udpClient.SetAttribute ("MaxPackets", UintegerValue (50000)); //maximum number of packets transmitted per vehicle
  udpClient.SetAttribute ("Interval", TimeValue (Seconds (0.1))); //time between 2 message generation
  udpClient.SetAttribute ("PacketSize", UintegerValue (177)); //Data size

  ApplicationContainer clientApps = udpClient.Install (ueResponders);
  int r;
  for(int i=0; i< (int)ueCount; i++)
  {
	 r = rand() % 1000;
	 clientApps.Get(i)->SetStartTime ( MilliSeconds (2000.0 + r)); //random start time
  }

  
  NS_LOG_INFO ("Creating sidelink configuration...");
  uint32_t groupL2Address = 0xFF;  
  Ptr<NistSlTft> tft = Create<NistSlTft> (NistSlTft::BIDIRECTIONAL, groupAddress, groupL2Address);
  proseHelper->ActivateSidelinkBearer (Seconds(1.0), ueDevs, tft); 

  Ptr<LteUeRrcSl> ueSidelinkConfiguration = CreateObject<LteUeRrcSl> ();
  ueSidelinkConfiguration->SetSlEnabled (true);

  NistLteRrcSap::SlPreconfiguration preconfiguration;
  preconfiguration.preconfigGeneral.carrierFreq = 54990; //carrier frequency for 5.9 GHz V2X
  preconfiguration.preconfigGeneral.slBandwidth = 50; //number of RBs in the 10 MHz bandwidth
  preconfiguration.preconfigComm.nbPools = 1; //only one pool is considered
  NistSlPreconfigPoolFactory pfactory;
  //build PSCCH bitmap value
  uint64_t pscchBitmapValue = 0x0;
  for (uint32_t i = 0 ; i < pscchLength; i++) {
    pscchBitmapValue = pscchBitmapValue >> 1 | 0x8000000000;
  }
  std::cout << "bitmap=" << std::hex << pscchBitmapValue << '\n';
  pfactory.SetControlBitmap (pscchBitmapValue);
  pfactory.SetControlPeriod (period); //sidelink period for communication application
  pfactory.SetDataOffset (pscchLength); //data offset define when pscch subframes ends and the start of pssch subframes
  preconfiguration.preconfigComm.pools[0] = pfactory.CreatePool ();
  ueSidelinkConfiguration->SetSlPreconfiguration (preconfiguration);

  NS_LOG_INFO ("Installing sidelink configuration...");
  lteHelper->InstallSidelinkConfiguration (ueRespondersDevs, ueSidelinkConfiguration);

  NS_LOG_INFO ("Enabling LTE traces...");
  //lteHelper->EnableTraces (); //enable all traces
  lteHelper->EnableSlRxPhyTraces(); //enable PSSCH traces
  lteHelper->EnableSlPscchRxPhyTraces(); //enable PSCCH traces
  //lteHelper->CalcSidelinkRsrp(ueTxPower, 54990, 50,ueDevs.Get(1), ueDevs.Get(0));

  NS_LOG_INFO ("Starting simulation...");
  Simulator::Stop (Seconds (simTime+2));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("Done.");

  return 0;

}

