#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"
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

int
main(int argc, char* argv[])
{
    std::string fileNameWithNoExtension2 = "Throughput-vs-DataRate";
    std::string graphicsFileName2 = fileNameWithNoExtension2 + ".png";
    std::string plotFileName2 = fileNameWithNoExtension2 + ".plt";
    std::string plotTitle2 = "Throughput-vs-DataRate graph";
    Gnuplot plot2(graphicsFileName2);
    plot2.SetTitle(plotTitle2);
    plot2.SetTerminal("png");
    plot2.SetLegend("X Values -Datarate(Mbps)", "Y Values -Throughput(kbps)");
    plot2.AppendExtra("set xrange [+0:+320]");
    Gnuplot2dDataset dataset2[6];
    dataset2[1].SetTitle("TCP-New-Reno");
    dataset2[2].SetTitle("TCP-Adaptive-Reno");
    dataset2[1].SetStyle(Gnuplot2dDataset::LINES_POINTS);
    dataset2[2].SetStyle(Gnuplot2dDataset::LINES_POINTS);
    std::string thFile2 = "Throughput-vs-DataRate.txt";
    FILE* fp = std::freopen(thFile2.c_str(), "r", stdin);
    std::cout << thFile2 << std::endl;
    for (int x = 1; x <= 13; x += 1)
    {
        double z = 0.0, q = 0.0;
        int a = 0, y = 0;
        std::cin >> a;
        std::cout << a << std::endl;
        std::cin >> y;
        std::cin >> z;
        std::cin >> q;

        dataset2[1].Add(a, z);
        dataset2[2].Add(a, q);
    }
    fclose(fp);
    plot2.AddDataset(dataset2[1]);
    plot2.AddDataset(dataset2[2]);
    std::ofstream plotFile2(plotFileName2.c_str());
    plot2.GenerateOutput(plotFile2);
    plotFile2.close();
    return 0;
}