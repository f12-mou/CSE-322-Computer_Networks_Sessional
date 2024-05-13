
#include "ns3/applications-module.h"
#include "ns3/callback.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/stats-module.h"

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("1905012_TaskB1");
std::string isPrint;

// ===========================================================================
//            t0----      -----h0
//  senders - t1---r0 --- r1---h1 - receivers
//            t2----      -----h2
// ===========================================================================

class TutorialApp : public Application
{
  public:
    TutorialApp();
    virtual ~TutorialApp();

    /**
     * Register this type.
     * \return The TypeId.
     */
    static TypeId GetTypeId(void);
    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               DataRate dataRate,
               uint32_t simultime);

  private:
    virtual void StartApplication(void);
    virtual void StopApplication(void);

    void ScheduleTx(void);
    void SendPacket(void);

    Ptr<Socket> m_socket;
    Address m_peer;
    uint32_t m_packetSize;
    DataRate m_dataRate;
    EventId m_sendEvent;
    bool m_running;
    uint32_t m_packetsSent;
    uint32_t m_simultime;
};

TutorialApp::TutorialApp()
    : m_socket(nullptr),
      m_peer(),
      m_packetSize(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0)

{
}

TutorialApp::~TutorialApp()
{
    m_socket = nullptr;
}

/* static */
TypeId
TutorialApp::GetTypeId()
{
    static TypeId tid = TypeId("TutorialApp")
                            .SetParent<Application>()
                            .SetGroupName("Tutorial")
                            .AddConstructor<TutorialApp>();
    return tid;
}

void
TutorialApp::Setup(Ptr<Socket> socket,
                   Address address,
                   uint32_t packetSize,
                   DataRate dataRate,
                   uint32_t simultime)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_dataRate = dataRate;
    m_simultime = simultime;
}

void
TutorialApp::StartApplication()
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
TutorialApp::StopApplication()
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
TutorialApp::SendPacket()
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    if (Simulator::Now().GetSeconds() < m_simultime)
        ScheduleTx();
}

void
TutorialApp::ScheduleTx()
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &TutorialApp::SendPacket, this);
    }
}

static void
CwndChange(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    if (isPrint == "true")
    {
        *stream->GetStream() << Simulator::Now().GetSeconds() << " " << newCwnd << std::endl;
    }
}

int
main(int argc, char* argv[])
{
    uint32_t payloadSize = 1024;
    std::string tcpVariant1 = "ns3::TcpNewReno";   // TcpNewReno
    std::string tcpVariant2 = "ns3::TcpHighSpeed"; // TcpAdaptiveReno
    int nLeaf = 2;
    int nFlows = 2;
    std::string senderDataRate = "1Gbps";
    std::string senderDelay = "1ms";
    std::string fileNameTh = "";

    int simulationTimeInSeconds = 60; // 1 minute
    int cleanupTime = 2;
    int bttlnkRate = 50;
    int bttlnkDelay = 100; // Delay of bottleneck link will be 100ms.
    int packet_loss_exp = 6;
    int exp = 1;

    // input from CMD
    CommandLine cmd(__FILE__);
    cmd.AddValue("nLeaf", "Number of left and right side leaf nodes", nLeaf);
    cmd.AddValue("bttlnkRate", "Max Packets allowed in the device queue", bttlnkRate);
    cmd.AddValue("plossRate", "Packet loss rate", packet_loss_exp);
    cmd.AddValue("exp", "1 for bttlnck, 2 for packet loss rate", exp);
    cmd.AddValue("fileNameTh", "Throughput File Name", fileNameTh);
    cmd.AddValue("isPrint", "Whether cwnd will be captured", isPrint);
    cmd.Parse(argc, argv);

    nFlows = nLeaf;
    double packet_loss_rate = (1.0 / std::pow(10, packet_loss_exp)); // 10^-(x)
    std::string bottleNeckDataRate = std::to_string(bttlnkRate) + "Mbps";
    std::string bottleNeckDelay = std::to_string(bttlnkDelay) + "ms";

    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(payloadSize));

    // SETUP NODE AND DEVICE
    // Create the point-to-point link helpers
    PointToPointHelper bottleNeckLink;
    bottleNeckLink.SetDeviceAttribute("DataRate", StringValue(bottleNeckDataRate));
    bottleNeckLink.SetChannelAttribute("Delay", StringValue(bottleNeckDelay));

    PointToPointHelper leftToRightLeaf;
    leftToRightLeaf.SetDeviceAttribute("DataRate", StringValue(senderDataRate));
    leftToRightLeaf.SetChannelAttribute("Delay", StringValue(senderDelay));
    // 	To set the router buffer capacity to the bandwidth delay product and to get the buffer
    // employ drop-tail discarding as mentioned in the reference paper, add this line to your
    // senders and receivers’ P2P Helper:
    leftToRightLeaf.SetQueue(
        "ns3::DropTailQueue",
        "MaxSize",
        StringValue(std::to_string((bttlnkDelay * bttlnkRate) / payloadSize) + "p"));

    PointToPointDumbbellHelper d(nLeaf, leftToRightLeaf, nLeaf, leftToRightLeaf, bottleNeckLink);
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(packet_loss_rate));
    d.m_routerDevices.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));

    // INSTALL STACK
    // tcp variant 1
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue(tcpVariant1));
    InternetStackHelper stack1;
    for (uint32_t i = 0; i < d.LeftCount(); i += 2)
    {
        stack1.Install(d.GetLeft(i)); // left leaves
    }
    for (uint32_t i = 0; i < d.RightCount(); i += 2)
    {
        stack1.Install(d.GetRight(i)); // right leaves
    }
    stack1.Install(d.GetLeft());
    stack1.Install(d.GetRight());

    // tcp variant 2
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue(tcpVariant2));
    InternetStackHelper stack2;
    for (uint32_t i = 1; i < d.LeftCount(); i += 2)
    {
        stack2.Install(d.GetLeft(i)); // left leaves
    }
    for (uint32_t i = 1; i < d.RightCount(); i += 2)
    {
        stack2.Install(d.GetRight(i)); // right leaves
    }

    // ASSIGN IP Addresses
    d.AssignIpv4Addresses(Ipv4AddressHelper("10.1.1.0", "255.255.255.0"),  // left nodes
                          Ipv4AddressHelper("10.2.1.0", "255.255.255.0"),  // right nodes
                          Ipv4AddressHelper("10.3.1.0", "255.255.255.0")); // routers
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();                      // populate routing table

    // install flow monitor
    FlowMonitorHelper flowmon;
    flowmon.SetMonitorAttribute("MaxPerHopDelay", TimeValue(Seconds(cleanupTime)));
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    uint16_t sp = 8080;
    for (int i = 0; i < nFlows; i++)
    {
        Address sinkAddress(InetSocketAddress(d.GetRightIpv4Address(i), sp));
        PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",
                                          InetSocketAddress(Ipv4Address::GetAny(), sp));
        ApplicationContainer sinkApps = packetSinkHelper.Install(d.GetRight(i));
        sinkApps.Start(Seconds(0));
        sinkApps.Stop(Seconds(simulationTimeInSeconds + cleanupTime));

        Ptr<Socket> ns3TcpSocket =
            Socket::CreateSocket(d.GetLeft(i), TcpSocketFactory::GetTypeId());
        Ptr<TutorialApp> app = CreateObject<TutorialApp>();
        app->Setup(ns3TcpSocket,
                   sinkAddress,
                   payloadSize,
                   DataRate(senderDataRate),
                   simulationTimeInSeconds);
        d.GetLeft(i)->AddApplication(app);
        app->SetStartTime(Seconds(1));
        app->SetStopTime(Seconds(simulationTimeInSeconds));

        std::ostringstream oss;
        oss << "flow" << std::to_string(i + 1) << ".cwnd";
        AsciiTraceHelper asciiTraceHelper;
        Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream(oss.str());
        ns3TcpSocket->TraceConnectWithoutContext("CongestionWindow",
                                                 MakeBoundCallback(&CwndChange, stream));
    }

    Simulator::Stop(Seconds(simulationTimeInSeconds + cleanupTime));
    Simulator::Run();

    // flow monitor statistics
    int j = 0;
    float CurThroughput = 0;
    float CurThroughputArr[] = {0, 0}; // variant 1 and variant 2

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

    for (auto iter = stats.begin(); iter != stats.end(); ++iter)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
        // classifier returns FiveTuple in correspondance to a flowID
        // iter->second.rxBytes ===== bytes received

        CurThroughput =
            iter->second.rxBytes * 8.0 / ((simulationTimeInSeconds + cleanupTime) * 1000);
        if (j % 2 == 0)
        {
            CurThroughputArr[0] += iter->second.rxBytes;
        }
        if (j % 2 == 1)
        {
            CurThroughputArr[1] += iter->second.rxBytes;
        }
        j = j + 1;
    }
    CurThroughputArr[0] /= ((simulationTimeInSeconds + cleanupTime) * 1000);
    CurThroughputArr[1] /= ((simulationTimeInSeconds + cleanupTime) * 1000);

    NS_LOG_UNCOND("Bottlenack Datarate(in Mbps): " << bttlnkRate);
    FILE* fp = freopen(fileNameTh.c_str(), "a", stdout);
    std::cout << bttlnkRate << " " << -1 * packet_loss_exp << " " << CurThroughputArr[0] << " "
              << CurThroughputArr[1] << std::endl;
    std::fclose(fp);
    Simulator::Destroy();

    return 0;
}