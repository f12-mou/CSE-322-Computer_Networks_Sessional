#!/bin/bash


./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=1 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=25 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=50 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=75 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=100 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=125 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=150 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=175 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=200 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=225 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=250 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=275 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"
./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=300 --plossRate=6 --fileNameTh=Throughput-vs-DataRate.txt"



for i in $(seq 2 6); do  #10^-6 to 10^-2
    ./ns3 run "scratch/1905012_offline3 --isPrint=false --bttlnkRate=50 --plossRate=$i --fileNameTh=Throughput-vs-Loss.txt"
done


touch "./flow1.cwnd"
touch "./flow2.cwnd"
./ns3 run "scratch/offline_tcp --isPrint=true --bttlnkRate=50 --plossRate=6 --fileNameTh=Throughput-vs-DataRate-congestion.txt"


#graphs_plt
./ns3 run "scratch/1905012_graph_1" 
./ns3 run "scratch/1905012_graph_2" 
./ns3 run "scratch/1905012_graph_3" 
./ns3 run "scratch/1905012_graph_4"
#convert to png 
 gnuplot TCP-Congestion-Window-vs-Time-Reno.plt
 gnuplot TCP-CongestionWindow-vs-Time-Adaptive-Reno.plt
 gnuplot Throughput-vs-DataRate.plt
 gnuplot Throughput-vs-Loss.plt