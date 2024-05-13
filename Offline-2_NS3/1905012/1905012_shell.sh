mkdir -p static 
mkdir -p mobile
cd static/
mkdir -p nodes
mkdir -p flows 
mkdir -p ranges
mkdir -p ppers
cd ..
cd mobile/
mkdir -p nodes
mkdir -p flows 
mkdir -p ranges
cd ..

# Throughput and ratio graph varying node numbers in Static
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/nodes/thFile1.txt --ratioFile=static/nodes/ratioFile1.txt"
./ns3 run "scratch/1905012_1 --nWifi=40 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/nodes/thFile2.txt --ratioFile=static/nodes/ratioFile2.txt"
./ns3 run "scratch/1905012_1 --nWifi=60 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/nodes/thFile3.txt --ratioFile=static/nodes/ratioFile3.txt"
./ns3 run "scratch/1905012_1 --nWifi=80 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/nodes/thFile4.txt --ratioFile=static/nodes/ratioFile4.txt"
./ns3 run "scratch/1905012_1 --nWifi=100 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/nodes/thFile5.txt --ratioFile=static/nodes/ratioFile5.txt"


# Throughput graph varying flows in Static
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=10 --Ty_range=4 --thFile=static/flows/thFile1.txt --ratioFile=static/flows/ratioFile1.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=4 --thFile=static/flows/thFile2.txt --ratioFile=static/flows/ratioFile2.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=30 --Ty_range=4 --thFile=static/flows/thFile3.txt --ratioFile=static/flows/ratioFile3.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=40 --Ty_range=4 --thFile=static/flows/thFile4.txt --ratioFile=static/flows/ratioFile4.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=50 --Ty_range=4 --thFile=static/flows/thFile5.txt --ratioFile=static/flows/ratioFile5.txt"


# Throughput graph varying ppers in Static
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=100 --num_flow=20 --Ty_range=4 --thFile=static/ppers/thFile1.txt --ratioFile=static/ppers/ratioFile1.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=4 --thFile=static/ppers/thFile2.txt --ratioFile=static/ppers/ratioFile2.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=300 --num_flow=20 --Ty_range=4 --thFile=static/ppers/thFile3.txt --ratioFile=static/ppers/ratioFile3.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=400 --num_flow=20 --Ty_range=4 --thFile=static/ppers/thFile4.txt --ratioFile=static/ppers/ratioFile4.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=500 --num_flow=20 --Ty_range=4 --thFile=static/ppers/thFile5.txt --ratioFile=static/ppers/ratioFile5.txt"



# Throughput graph varying ranges in Static
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=1 --thFile=static/ranges/thFile1.txt --ratioFile=static/ranges/ratioFile1.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=2 --thFile=static/ranges/thFile2.txt --ratioFile=static/ranges/ratioFile2.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=3 --thFile=static/ranges/thFile3.txt --ratioFile=static/ranges/ratioFile3.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=4 --thFile=static/ranges/thFile4.txt --ratioFile=static/ranges/ratioFile4.txt"
./ns3 run "scratch/1905012_1 --nWifi=20 --appPacketRate=200 --num_flow=20 --Ty_range=5 --thFile=static/ranges/thFile5.txt --ratioFile=static/ranges/ratioFile5.txt"


./ns3 run "scratch/1905012_graphPlot"
gnuplot static-nodes-throughput.plt
gnuplot static-nodes-ratio.plt
gnuplot static-flows-throughput.plt
gnuplot static-flows-ratio.plt
gnuplot static-ppers-throughput.plt
gnuplot static-ppers-ratio.plt
gnuplot static-ranges-throughput.plt
gnuplot static-ranges-ratio.plt


