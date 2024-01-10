import pygmt
import sys, csv
import numpy as np
import pandas as pd

colormap = {0:"red", 1:"grey", 2:"blue", 3:"pink", 4:"orange", 5:"black", 6:"green", 7:"purple", 8:"yellow", 9:"brown"}
gbxClassMap = {0:"blocks", 1:"cobble", 2:"gravels", 3:"rocky", 4:"sand", 5:"sandy mud"}

if len(sys.argv) != 4:
	sys.stderr.write("Usage: python3 generate_raster_map.py inputData Map_title\n")
	sys.exit(1)
	
inputFilePath = sys.argv[1]
outputFilename = sys.argv[2]
mapTitle = sys.argv[3]


data = pd.read_csv(inputFilePath, delimiter=",", dtype=float, header=0)
print("[+] Read {} lines".format(len(data.index)))


minLat = data.min(axis=0)[0]
minLon = data.min(axis=0)[1]
maxLat = data.max(axis=0)[0]
maxLon = data.max(axis=0)[1]

gbxGroups = data.groupby("boosting class")


fig = pygmt.Figure()
fig.basemap(region=[minLon - 0.005, maxLon + 0.005, minLat - 0.005, maxLat + 0.005], 
			frame=["a", "+t{}".format(mapTitle)],
			projection="M6i"
)

fig.coast(land="lightblue", water="lightblue")

for classID in gbxGroups.groups:
	df = gbxGroups.get_group(classID)
	fig.plot(x=df["longitude"], y=df["latitude"], style="c0.03c", fill=colormap[classID], label=gbxClassMap[classID]+"+S0.25c")


fig.legend(box=True)
fig.image(
    imagefile="https://cidco.ca/themes/cidco/css/img/logo_cidco.png",
    position="jBR+w2+o0.2c",
    box=True,
)

with fig.inset(
	position="jBL",
	region=[round(minLon-0.3,3), round(maxLon+0.3,3), round(minLat-0.3,3), round(maxLat+0.3,3)],
	box="+gwhite+p1p",
	projection="M2c"
):
	fig.coast(shorelines="thinnest", land="green", water="lightblue")
	fig.plot(data=[[minLon, minLat, maxLon, maxLat]], style="r+s", pen="1p,red")

#fig.show()
fig.savefig("{}".format(outputFilename), crop=True, dpi=1200)


