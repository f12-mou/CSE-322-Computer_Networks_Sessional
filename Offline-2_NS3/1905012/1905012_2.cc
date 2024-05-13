#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/traffic-control-module.h"
#include "ns3/yans-wifi-helper.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace ns3;

//  10.1.2.0  --------           10.1.1.0   ---------           10.1.3.0
//
//
//  senderNode- -                                            -  receiverNode-1
//  senderNode-2 - -                                       - -  receiverNode-2
//  senderNode-3 - - -                                   - - -  receiverNode-3
//  senderNode-4 - - - - node-0 --------------- node-1 - - - -  receiverNode-4
//  senderNode-5 - - -                                   - - -  receiverNode-5
//  senderNode-6 - -                                       - -  receiverNode-6
//  senderNode-7 -                                           -  receiverNode-7
//  ............                                                ..............
//
uint total = 0;
uint total2 = 0;
uint totalReceived = 0;
std::vector<double> vec1;
std::vector<double> vec2;

void
FirstTraceFunction(std::string context, Ptr<const Packet> packet)
{
    total++;
}

void
SecondTraceFunction(std::string context, Ptr<const Packet> packet, const Address& address)
{
    total2++;
    totalReceived = (total2 * 1024 * 8);
}

void
CalculateMetrics()
{
    Time now = Simulator::Now(); /* Return the simulator's virtual time. */
                                 // double cur = (sink->GetTotalRx() - lastTotalRx) * 8.0 / 1e6 /
    //            0.5; /* Convert Application RX Packets to MBits. */
    // std::cout << now.GetSeconds() << "s: \t" << cur << " Mbit/s" << std::endl;
    double currRatio = ((double)total2 / (double)total) * 100.0;
    double totalNow = total2 * 1024 * 8;
    totalNow = totalNow / 1e6;              // Mbit
    totalNow = totalNow / now.GetSeconds(); // MbitPerSec
    // NS_LOG_UNCOND(now.GetSeconds() << "s: \t" << cur << " Mbit/s");
    std::cout << "now is " << now.GetSeconds() << " sec and ratio is " << currRatio
              << " percent and MbitPerSec is " << totalNow << std::endl;
    vec1.push_back(totalNow);
    vec2.push_back(currRatio);
    Simulator::Schedule(MilliSeconds(100), &CalculateMetrics);
}

int
main(int argc, char* argv[])
{
    // Constant Variables to use
    uint16_t port = 5001;
    std::string bottleNeckLinkBw = "5Mbps";
    std::string bottleNeckLinkDelay = "50ms";
    uint16_t packetSize = 1024; // in Bytes

    // Variable Parameters
    uint32_t nWifi = 20;          // 20, 40, 60, 80, 100.
    uint32_t appPacketRate = 100; // 100, 200, 300, 400, 500
    int32_t num_flow = 100;       // 10 , 20 , 40 , 70 , 100  -----> x 2
    int32_t speed = 10; //  5 m/s, 10 m/s, 15 m/s, 20 m/s, 25 m/s (Only in wireless mobile topology)
    std::string thFile = "";
    std::string ratioFile = "";

    // LogComponentEnable("PacketSink", LOG_LEVEL_ALL);
    // LogComponentEnable("OnOffApplication", LOG_LEVEL_ALL);

    CommandLine cmd(__FILE__);
    cmd.AddValue("nWifi", "Number of nodes", nWifi);
    cmd.AddValue("appPacketRate", "Number of packets per second", appPacketRate);
    cmd.AddValue("num_flow", "Number of flows", num_flow);
    cmd.AddValue("speed", "Speed of nodes", speed);
    cmd.AddValue("thFile", "Throughput File Name", thFile);
    cmd.AddValue("ratioFile", "Ratio File name", ratioFile);
    cmd.Parse(argc, argv);

    nWifi -= 2;
    nWifi /= 2;

    // Data rate for On-Off application in Bytes
    std::string appDataRate = std::to_string(appPacketRate * packetSize) + "Bps";

    Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(packetSize));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue(appDataRate));

    // Topology Building

    // step -1
    // Creating the BottleNeck Nodes
    NodeContainer p2pBottleNeckNodes;
    p2pBottleNeckNodes.Create(2);

    // Create the point-to-point link helpers
    PointToPointHelper bottleNeckLink;
    bottleNeckLink.SetDeviceAttribute("DataRate", StringValue(bottleNeckLinkBw));
    bottleNeckLink.SetChannelAttribute("Delay", StringValue(bottleNeckLinkDelay));
    // Installing Net devices
    NetDeviceContainer p2pBottleNeckDevices;
    p2pBottleNeckDevices = bottleNeckLink.Install(p2pBottleNeckNodes);

    /// step-2
    // creating the wifi nodes
    NodeContainer wifiStaNodesLeft, wifiStaNodesRight;
    wifiStaNodesLeft.Create(nWifi);
    wifiStaNodesRight.Create(nWifi);
    NodeContainer wifiApNodeLeft = p2pBottleNeckNodes.Get(0);
    NodeContainer wifiApNodeRight = p2pBottleNeckNodes.Get(1);

    // PHY helper
    // constructs the wifi devices and the interconnection channel between these wifi nodes.
    YansWifiChannelHelper channelLeft = YansWifiChannelHelper::Default();
    YansWifiChannelHelper channelRight = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phyLeft, phyRight;
    phyLeft.SetChannel(
        channelLeft.Create()); //  all the PHY layer objects created by the YansWifiPhyHelper share
                               //  the same underlying channel
    phyRight.SetChannel(channelRight.Create());

    // MAC layer
    WifiHelper wifi;
    WifiMacHelper macLeft, macRight;
    Ssid ssidLeft =
        Ssid("ns-3-ssid-left"); //  creates an 802.11 service set identifier (SSID) object
    Ssid ssidRight = Ssid("ns-3-ssid-right");

    // configure Wi-Fi for all of our STA nodes
    macLeft.SetType("ns3::StaWifiMac",
                    "Ssid",
                    SsidValue(ssidLeft),
                    "ActiveProbing",
                    BooleanValue(false));

    macRight.SetType("ns3::StaWifiMac",
                     "Ssid",
                     SsidValue(ssidRight),
                     "ActiveProbing",
                     BooleanValue(false));

    NetDeviceContainer staDevicesLeft, staDevicesRight;
    staDevicesLeft = wifi.Install(phyLeft, macLeft, wifiStaNodesLeft);
    staDevicesRight = wifi.Install(phyRight, macRight, wifiStaNodesRight);

    //  configure the AP (access point) node
    macLeft.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssidLeft));
    macRight.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssidRight));

    NetDeviceContainer apDevicesLeft, apDevicesRight;
    apDevicesLeft =
        wifi.Install(phyLeft,
                     macLeft,
                     wifiApNodeLeft); // single AP which shares the same set of PHY-level Attributes
                                      // (and channel) as the station
    apDevicesRight = wifi.Install(phyRight, macRight, wifiApNodeRight);

    /**
     *
     * Mobility Model
     *
     * We want the STA nodes to be mobile, wandering around inside a bounding box,
     * and we want to make the AP node stationary
     *
     **/
    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(0.5),
                                  "DeltaY",
                                  DoubleValue(1.0),
                                  "GridWidth",
                                  UintegerValue(3),
                                  "LayoutType",
                                  StringValue("RowFirst"));
    mobility.SetMobilityModel(
        "ns3::RandomWalk2dMobilityModel",
        "Bounds",
        RectangleValue(Rectangle(-500, 500, -500, 500)),
        "Speed",
        StringValue("ns3::ConstantRandomVariable[Constant=" + std::to_string(speed) + "]"));

    // install on STA nodes
    mobility.Install(wifiStaNodesLeft);
    mobility.Install(wifiStaNodesRight);

    // tell AP node to stay still
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    // install on AP node
    mobility.Install(wifiApNodeLeft);
    mobility.Install(wifiApNodeRight);

    // Install Stack
    InternetStackHelper stack;
    stack.Install(wifiApNodeLeft);
    stack.Install(wifiApNodeRight);
    stack.Install(wifiStaNodesLeft);
    stack.Install(wifiStaNodesRight);

    // Assign IP Addresses
    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pBottleNeckDevices);

    Ipv4InterfaceContainer staNodeInterfacesLeft, staNodeInterfacesRight;
    Ipv4InterfaceContainer apNodeInterfaceLeft, apNodeInterfaceRight;

    address.SetBase("10.1.2.0", "255.255.255.0");
    staNodeInterfacesLeft = address.Assign(staDevicesLeft);
    apNodeInterfaceLeft = address.Assign(apDevicesLeft);

    address.SetBase("10.1.3.0", "255.255.255.0");
    staNodeInterfacesRight = address.Assign(staDevicesRight);
    apNodeInterfaceRight = address.Assign(apDevicesRight);

    // Install on/off app on all right side nodes
    OnOffHelper clientHelper("ns3::TcpSocketFactory", Address());
    clientHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    Address sinkLocalAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", sinkLocalAddress);

    ApplicationContainer sinkApps;
    for (uint32_t i = 0; i < nWifi; ++i)
    {
        // create sink app on right side node
        sinkApps.Add(packetSinkHelper.Install(wifiStaNodesRight.Get(i)));
    }
    sinkApps.Start(Seconds(1.0));
    sinkApps.Stop(Seconds(6.5));

    ApplicationContainer clientApps;
    int cur_flow_count = 0;
    while (cur_flow_count < num_flow)
    {
        for (uint32_t i = 0; i < nWifi; ++i)
        {
            // Create an on/off app on lefts
            AddressValue remoteAddress(
                InetSocketAddress(staNodeInterfacesRight.GetAddress(i), port));
            clientHelper.SetAttribute("Remote", remoteAddress);
            clientApps.Add(clientHelper.Install(wifiStaNodesLeft.Get(i)));

            cur_flow_count++;
            if (cur_flow_count >= num_flow)
                break;
        }
        if (cur_flow_count >= num_flow)
            break;
    }
    clientApps.Start(Seconds(2.0)); // Start 1 second after sink
    clientApps.Stop(Seconds(6.0));  // Stop before the sink

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    /*for (int i = 0; i < nWifi; i++)
    {
        std::ostringstream oss;
        oss << "/NodeList/" << wifiStaNodesLeft.Get(i)->GetId() << "/ApplicationList/"
            << clientApps.Get(i) << "/$ns3::OnOffApplication/Tx";
        Config::Connect(oss.str(), MakeCallback(&CourseHehe));
    }

    for (int i = 0; i < nWifi; i++)
    {
        std::ostringstream oss;
        oss << "/NodeList/" << wifiStaNodesRight.Get(i)->GetId() << "/ApplicationList/"
            << sinkApps.Get(i) << "/$ns3::PacketSink/Rx";
        Config::Connect(oss.str(), MakeCallback(&CourseHehe2));
    }*/
    for (int i = 0; i < num_flow; i++)
    {
        std::ostringstream oss;
        oss << "/NodeList/" << clientApps.Get(i)->GetNode()->GetId() << "/ApplicationList/"
            << clientApps.Get(i) << "/$ns3::OnOffApplication/Tx";
        Config::Connect(oss.str(), MakeCallback(&FirstTraceFunction));
    }

    for (int i = 0; i < nWifi; i++)
    {
        std::ostringstream oss;
        oss << "/NodeList/" << sinkApps.Get(i)->GetNode()->GetId() << "/ApplicationList/"
            << sinkApps.Get(i) << "/$ns3::PacketSink/Rx";
        Config::Connect(oss.str(), MakeCallback(&SecondTraceFunction));
    }

    Simulator::Schedule(Seconds(2.5), &CalculateMetrics);
    Simulator::Stop(Seconds(6.5)); // force stop,
    Simulator::Run();
    Simulator::Destroy();

    std::cout << nWifi << std::endl;
    std::cout << thFile << std::endl;
    std::cout << ratioFile << std::endl;
    std::cout << totalReceived << std::endl;
    freopen(thFile.c_str(), "w", stdout);
    for (int i = 0; i < vec1.size(); i++)
    {
        std::cout << vec1[i] << std::endl;
    }
    freopen(ratioFile.c_str(), "w", stdout);
    for (int i = 0; i < vec2.size(); i++)
    {
        std::cout << vec2[i] << std::endl;
    }
    return 0;
}