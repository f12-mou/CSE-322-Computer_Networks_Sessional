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
double startVal = 2.5;
double endVal = 5.5;
double inc = 0.1;
int nodesArr[6] = {0, 20, 40, 60, 80, 100};
int flowsArr[6] = {0, 20, 40, 60, 80, 100};
int ppersArr[6] = {0, 100, 200, 300, 400, 500};
int rangesArr[6] = {0, 1, 2, 3, 4, 5};

int
main(int argc, char* argv[])
{
    // STATIC MODEL
    // NODES COUNT -> THROUGHPUT
    std::string fileNameWithNoExtension = "static-nodes-throughput";
    std::string graphicsFileName = fileNameWithNoExtension + ".png";
    std::string plotFileName = fileNameWithNoExtension + ".plt";
    std::string plotTitle = "Static nodes varying Throughput";
    Gnuplot plot(graphicsFileName);
    plot.SetTitle(plotTitle);
    plot.SetTerminal("png");
    plot.SetLegend("X Values - time(seconds)", "Y Values - Throughput(kbit/s)");
    plot.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset[6];
    double x, y;

    for (int i = 1; i <= 5; i++)
    {
        dataset[i].SetTitle("Node Count - " + std::to_string(nodesArr[i]));
        dataset[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile = "./static/nodes/thFile" + std::to_string(i) + ".txt";
        freopen(thFile.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            y *= 100.0;
            dataset[i].Add(x, y);
        }
        plot.AddDataset(dataset[i]);
    }
    std::ofstream plotFile(plotFileName.c_str());
    plot.GenerateOutput(plotFile);
    plotFile.close();

    // STATIC MODEL
    // NODES COUNT -> RATIO
    std::string fileNameWithNoExtension2 = "static-nodes-ratio";
    std::string graphicsFileName2 = fileNameWithNoExtension2 + ".png";
    std::string plotFileName2 = fileNameWithNoExtension2 + ".plt";
    std::string plotTitle2 = "Static nodes varying Packet Ratio";
    Gnuplot plot2(graphicsFileName2);
    plot2.SetTitle(plotTitle2);
    plot2.SetTerminal("png");
    plot2.SetLegend("X Values - time(seconds)", "Y Values - Ratio");
    plot2.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset2[6];
    for (int i = 1; i <= 5; i++)
    {
        dataset2[i].SetTitle("Node Count - " + std::to_string(nodesArr[i]));
        dataset2[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile2 = "./static/nodes/ratioFile" + std::to_string(i) + ".txt";
        freopen(thFile2.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            dataset2[i].Add(x, y);
        }
        plot2.AddDataset(dataset2[i]);
    }
    std::ofstream plotFile2(plotFileName2.c_str());
    plot2.GenerateOutput(plotFile2);
    plotFile2.close();

    // STATIC MODEL
    // FLOWS -> THROUGHPUT
    std::string fileNameWithNoExtension3 = "static-flows-throughput";
    std::string graphicsFileName3 = fileNameWithNoExtension3 + ".png";
    std::string plotFileName3 = fileNameWithNoExtension3 + ".plt";
    std::string plotTitle3 = "Static - Throughput - varying flows";
    Gnuplot plot3(graphicsFileName3);
    plot3.SetTitle(plotTitle3);
    plot3.SetTerminal("png");
    plot3.SetLegend("X Values - time(seconds)", "Y Values - Throughput(kbit/s)");
    plot3.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset3[6];

    for (int i = 1; i <= 5; i++)
    {
        dataset3[i].SetTitle("Flow Count - " + std::to_string(flowsArr[i]));
        dataset3[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile = "./static/flows/thFile" + std::to_string(i) + ".txt";
        freopen(thFile.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            y *= 100.0;
            dataset3[i].Add(x, y);
        }
        plot3.AddDataset(dataset3[i]);
    }
    std::ofstream plotFile3(plotFileName3.c_str());
    plot3.GenerateOutput(plotFile3);
    plotFile3.close();

    // STATIC MODEL
    // FLOWS -> RATIO
    std::string fileNameWithNoExtension4 = "static-flows-ratio";
    std::string graphicsFileName4 = fileNameWithNoExtension4 + ".png";
    std::string plotFileName4 = fileNameWithNoExtension4 + ".plt";
    std::string plotTitle4 = "Static - Packet Ratio - varying flows";
    Gnuplot plot4(graphicsFileName4);
    plot4.SetTitle(plotTitle4);
    plot4.SetTerminal("png");
    plot4.SetLegend("X Values - time(seconds)", "Y Values - Ratio");
    plot4.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset4[6];
    for (int i = 1; i <= 5; i++)
    {
        dataset4[i].SetTitle("Flow Count - " + std::to_string(flowsArr[i]));
        dataset4[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile4 = "./static/flows/ratioFile" + std::to_string(i) + ".txt";
        freopen(thFile4.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            dataset4[i].Add(x, y);
        }
        plot4.AddDataset(dataset4[i]);
    }
    std::ofstream plotFile4(plotFileName4.c_str());
    plot4.GenerateOutput(plotFile4);
    plotFile4.close();

    // STATIC MODEL
    // PPERS -> THROUGHPUT
    std::string fileNameWithNoExtension5 = "static-ppers-throughput";
    std::string graphicsFileName5 = fileNameWithNoExtension5 + ".png";
    std::string plotFileName5 = fileNameWithNoExtension5 + ".plt";
    std::string plotTitle5 = "Static - Throughput - varying - ppers";
    Gnuplot plot5(graphicsFileName5);
    plot5.SetTitle(plotTitle5);
    plot5.SetTerminal("png");
    plot5.SetLegend("X Values - time(seconds)", "Y Values - Throughput(kbit/s)");
    plot5.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset5[6];

    for (int i = 1; i <= 5; i++)
    {
        dataset5[i].SetTitle("Ppers Count - " + std::to_string(ppersArr[i]));
        dataset5[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile = "./static/ppers/thFile" + std::to_string(i) + ".txt";
        freopen(thFile.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            y *= 100.0;
            dataset5[i].Add(x, y);
        }
        plot5.AddDataset(dataset5[i]);
    }
    std::ofstream plotFile5(plotFileName5.c_str());
    plot5.GenerateOutput(plotFile5);
    plotFile5.close();

    // STATIC MODEL
    // PPERS -> RATIO
    std::string fileNameWithNoExtension6 = "static-ppers-ratio";
    std::string graphicsFileName6 = fileNameWithNoExtension6 + ".png";
    std::string plotFileName6 = fileNameWithNoExtension6 + ".plt";
    std::string plotTitle6 = "Static - Packet Ratio - varying flows";
    Gnuplot plot6(graphicsFileName6);
    plot6.SetTitle(plotTitle6);
    plot6.SetTerminal("png");
    plot6.SetLegend("X Values - time(seconds)", "Y Values - Ratio");
    plot6.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset6[6];
    for (int i = 1; i <= 5; i++)
    {
        dataset6[i].SetTitle("Flow Count - " + std::to_string(flowsArr[i]));
        dataset6[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile6 = "./static/ppers/ratioFile" + std::to_string(i) + ".txt";
        freopen(thFile6.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            dataset6[i].Add(x, y);
        }
        plot6.AddDataset(dataset6[i]);
    }
    std::ofstream plotFile6(plotFileName6.c_str());
    plot6.GenerateOutput(plotFile6);
    plotFile6.close();

    // STATIC MODEL
    // RANGES -> THROUGHPUT
    std::string fileNameWithNoExtension7 = "static-ranges-throughput";
    std::string graphicsFileName7 = fileNameWithNoExtension7 + ".png";
    std::string plotFileName7 = fileNameWithNoExtension7 + ".plt";
    std::string plotTitle7 = "Static - Throughput - varying -ranges";
    Gnuplot plot7(graphicsFileName7);
    plot7.SetTitle(plotTitle7);
    plot7.SetTerminal("png");
    plot7.SetLegend("X Values - time(seconds)", "Y Values - Throughput(kbit/s)");
    plot7.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset7[6];

    for (int i = 1; i <= 5; i++)
    {
        dataset7[i].SetTitle("Ranges Count - " + std::to_string(rangesArr[i]));
        dataset7[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile = "./static/ranges/thFile" + std::to_string(i) + ".txt";
        freopen(thFile.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            y *= 100.0;
            dataset7[i].Add(x, y);
        }
        plot7.AddDataset(dataset7[i]);
    }
    std::ofstream plotFile7(plotFileName7.c_str());
    plot7.GenerateOutput(plotFile7);
    plotFile7.close();

    // STATIC MODEL
    // RANGES -> RATIO
    std::string fileNameWithNoExtension8 = "static-ranges-ratio";
    std::string graphicsFileName8 = fileNameWithNoExtension8 + ".png";
    std::string plotFileName8 = fileNameWithNoExtension8 + ".plt";
    std::string plotTitle8 = "Static - Packet Ratio - varying ranges";
    Gnuplot plot8(graphicsFileName8);
    plot8.SetTitle(plotTitle8);
    plot8.SetTerminal("png");
    plot8.SetLegend("X Values - time(seconds)", "Y Values - Ratio");
    plot8.AppendExtra("set xrange [+1:+8]");
    Gnuplot2dDataset dataset8[6];
    for (int i = 1; i <= 5; i++)
    {
        dataset8[i].SetTitle("Range Count - " + std::to_string(rangesArr[i]));
        dataset8[i].SetStyle(Gnuplot2dDataset::LINES_POINTS);
        std::string thFile8 = "./static/ranges/ratioFile" + std::to_string(i) + ".txt";
        freopen(thFile8.c_str(), "r", stdin);
        for (x = startVal; x <= endVal; x += inc)
        {
            std::cin >> y;
            dataset8[i].Add(x, y);
        }
        plot8.AddDataset(dataset8[i]);
    }
    std::ofstream plotFile8(plotFileName8.c_str());
    plot8.GenerateOutput(plotFile8);
    plotFile8.close();

    return 0;
}