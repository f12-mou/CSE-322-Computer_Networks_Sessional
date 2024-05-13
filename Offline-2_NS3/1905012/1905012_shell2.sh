mkdir -p dynamic 
cd dynamic/
mkdir -p nodes
mkdir -p flows 
mkdir -p speeds
mkdir -p ppers
cd ..

pwd

# Throughput and ratio  graph varying node numbers in dynamic
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/nodes/thFile1.txt --ratioFile=dynamic/nodes/ratioFile1.txt"
./ns3 run "scratch/1905012_2 --nWifi=40 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/nodes/thFile2.txt --ratioFile=dynamic/nodes/ratioFile2.txt"
./ns3 run "scratch/1905012_2 --nWifi=60 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/nodes/thFile3.txt --ratioFile=dynamic/nodes/ratioFile3.txt"
./ns3 run "scratch/1905012_2 --nWifi=80 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/nodes/thFile4.txt --ratioFile=dynamic/nodes/ratioFile4.txt"
./ns3 run "scratch/1905012_2 --nWifi=100 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/nodes/thFile5.txt --ratioFile=dynamic/nodes/ratioFile5.txt"

# Throughput graph varying flows in dynamic
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=10 --speed=10 --thFile=dynamic/flows/thFile1.txt --ratioFile=dynamic/flows/ratioFile1.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=10 --thFile=dynamic/flows/thFile2.txt --ratioFile=dynamic/flows/ratioFile2.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=30 --speed=10 --thFile=dynamic/flows/thFile3.txt --ratioFile=dynamic/flows/ratioFile3.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=40 --speed=10 --thFile=dynamic/flows/thFile4.txt --ratioFile=dynamic/flows/ratioFile4.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=50 --speed=10 --thFile=dynamic/flows/thFile5.txt --ratioFile=dynamic/flows/ratioFile5.txt"

# Throughput graph varying ppers in dynamic
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=100 --num_flow=20 --speed=10 --thFile=dynamic/ppers/thFile1.txt --ratioFile=dynamic/ppers/ratioFile1.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=10 --thFile=dynamic/ppers/thFile2.txt --ratioFile=dynamic/ppers/ratioFile2.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=300 --num_flow=20 --speed=10 --thFile=dynamic/ppers/thFile3.txt --ratioFile=dynamic/ppers/ratioFile3.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=400 --num_flow=20 --speed=10 --thFile=dynamic/ppers/thFile4.txt --ratioFile=dynamic/ppers/ratioFile4.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=500 --num_flow=20 --speed=10 --thFile=dynamic/ppers/thFile5.txt --ratioFile=dynamic/ppers/ratioFile5.txt"



# Throughput graph varying speeds in dynamic
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=5 --thFile=dynamic/speeds/thFile1.txt --ratioFile=dynamic/speeds/ratioFile1.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=10 --thFile=dynamic/speeds/thFile2.txt --ratioFile=dynamic/speeds/ratioFile2.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=15 --thFile=dynamic/speeds/thFile3.txt --ratioFile=dynamic/speeds/ratioFile3.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=20 --thFile=dynamic/speeds/thFile4.txt --ratioFile=dynamic/speeds/ratioFile4.txt"
./ns3 run "scratch/1905012_2 --nWifi=20 --appPacketRate=200 --num_flow=20 --speed=25 --thFile=dynamic/speeds/thFile5.txt --ratioFile=dynamic/speeds/ratioFile5.txt"



./ns3 run "scratch/1905012_graphPlot2"
gnuplot dynamic-nodes-throughput.plt
gnuplot dynamic-nodes-ratio.plt
gnuplot dynamic-flows-throughput.plt
gnuplot dynamic-flows-ratio.plt
gnuplot dynamic-ppers-throughput.plt
gnuplot dynamic-ppers-ratio.plt
gnuplot dynamic-speeds-throughput.plt
gnuplot dynamic-speeds-ratio.plt


