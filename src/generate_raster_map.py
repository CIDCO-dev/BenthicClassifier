import pygmt
import sys, csv
import numpy as np
import pandas as pd

colormap = {0:"red", 1:"grey", 2:"purple", 3:"pink", 4:"brown", 5:"black", 6:"green", 7:"blue", 8:"yellow", 9:"orange"}
gbxClassMap = {0:"blocks", 1:"cobble", 2:"gravels", 3:"rocky", 4:"sand", 5:"sandy mud"}

if len(sys.argv) != 3:
	sys.stderr.write("Usage: python3 generate_raster_map.py inputData Map_title\n")
	sys.exit(1)
	
inputFilePath = sys.argv[1]
mapTitle = sys.argv[2]


data = pd.read_csv(inputFilePath, delimiter=" ", names=["lat", "long", "depth", "gbxClass", "gmmClass"])
print("[+] Read {} lines".format(len(data.index)))


minLat = data.min(axis=0)[0]
minLon = data.min(axis=0)[1]
maxLat = data.max(axis=0)[0]
maxLon = data.max(axis=0)[1]

#gbxClass = data[3].unique()
#gmmClass = data[4].unique()

gbxGroups = data.groupby("gbxClass")
gmmGroups = data.groupby("gmmClass")


fig = pygmt.Figure()
fig.coast(
    resolution="h",
    region=[minLon - 0.005, maxLon + 0.005, minLat - 0.005, maxLat + 0.005],
    shorelines="thinnest",
    land="lightgreen",
    water="lightblue",
    frame=["a", "+t{}".format(mapTitle)],
    projection="U19R/12c"
)

for classID in gbxGroups.groups:
	df = gbxGroups.get_group(classID)
	fig.plot(x=df["long"], y=df["lat"], style="c0.01c", fill=colormap[classID])



fig.show()



