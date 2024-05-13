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
    std::string fileNameWithNoExtension3 = "Throughput-vs-Loss";
    std::string graphicsFileName3 = fileNameWithNoExtension3 + ".png";
    std::string plotFileName3 = fileNameWithNoExtension3 + ".plt";
    std::string plotTitle3 = "Throughput-vs-Loss Graph";
    Gnuplot plot3(graphicsFileName3);
    plot3.SetTitle(plotTitle3);
    plot3.SetTerminal("png");
    plot3.SetLegend("X Values - Packet Loss Index", "Y Values - Throughput(kbit/s)");
    plot3.AppendExtra("set xrange [-7:-1]");
    Gnuplot2dDataset dataset3[6];

    for (int i = 1; i <= 2; i++)
    {
        std::string thFile = "";
        if (i == 1)
        {
            dataset3[i].SetTitle("TCP-New-Reno-Throughput");
            dataset3[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        }
        else
        {
            dataset3[i].SetTitle("TCP-Adaptive-Reno-Throughput");
            dataset3[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        }
        thFile = "Throughput-vs-Loss.txt";
        FILE* fp = freopen(thFile.c_str(), "r", stdin);
        for (int x = 1; x <= 5; x += 1)
        {
            double a, b, c, d;
            std::cin >> a >> b >> c >> d;
            if (i == 1)
                dataset3[i].Add(b, c);
            else
                dataset3[i].Add(b, d);
        }
        plot3.AddDataset(dataset3[i]);
        std::fclose(fp);
    }
    std::ofstream plotFile3(plotFileName3.c_str());
    plot3.GenerateOutput(plotFile3);
    plotFile3.close();
    return 0;
}