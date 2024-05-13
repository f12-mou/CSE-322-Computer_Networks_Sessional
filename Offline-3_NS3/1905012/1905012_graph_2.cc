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
    std::string fileNameWithNoExtension = "TCP-CongestionWindow-vs-Time-Adaptive-Reno";
    std::string graphicsFileName = fileNameWithNoExtension + ".png";
    std::string plotFileName = fileNameWithNoExtension + ".plt";
    std::string plotTitle = "TCP-CongestionWindow-vs-Time-Adaptive-Reno";
    Gnuplot plot(graphicsFileName);
    plot.SetTitle(plotTitle);
    plot.SetTerminal("png");
    plot.SetLegend("X Values - time(seconds)", "Y Values - Throughput(kbit/s)");
    plot.AppendExtra("set xrange [+0:+62]");
    Gnuplot2dDataset dataset[4];
    double x, y;
    int itr = 0;

    for (int i = 1; i <= 1; i++)
    {
        dataset[i].SetTitle("TCP-Reno - ");
        dataset[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile = "flow2.cwnd";
        FILE* fp = freopen(thFile.c_str(), "r", stdin);
        for (itr = 0; itr <= 2000; itr += 1)
        {
            std::cin >> x;
            std::cin >> y;
            std::cout << x << " " << y << std::endl;
            dataset[i].Add(x, y);
        }
        plot.AddDataset(dataset[i]);
        std::fclose(fp);
    }
    std::ofstream plotFile(plotFileName.c_str());
    plot.GenerateOutput(plotFile);
    plotFile.close();
    return 0;
}