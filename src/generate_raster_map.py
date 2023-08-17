import pygmt
import sys, csv
import numpy as np
import pandas as pd

colormap = {0:"red", 1:"grey", 2:"blue", 3:"pink", 4:"orange", 5:"black", 6:"green", 7:"purple", 8:"yellow", 9:"brown"}
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
    frame=["a", "+t{}".format(mapTitle)]
)

for classID in gbxGroups.groups:
	df = gbxGroups.get_group(classID)
	fig.plot(x=df["long"], y=df["lat"], style="c0.01c", fill=colormap[classID], label=gbxClassMap[classID]+"+S0.25c")


fig.legend(box=True)
fig.image(
    imagefile="https://cidco.ca/themes/cidco/css/img/logo_cidco.png",
    position="jBR+w2",
    box=True,
)


"""
with fig.inset(
    position="jBL+o0.1c",
    region=[minLon-1, maxLon + 1,minLat-1, maxLat+1],
):
    # Highlight the Japan area in "lightbrown"
    # and draw its outline with a pen of "0.2p".
    fig.coast(
        dcw="JP+glightbrown+p0.2p",
        area_thresh=10000,
    )
    # Plot a rectangle ("r") in the inset map to show the area of the main
    # figure. "+s" means that the first two columns are the longitude and
    # latitude of the bottom left corner of the rectangle, and the last two
    # columns the longitude and latitude of the uppper right corner.
    fig.plot(data=[minLon, maxLon, minLat, maxLat], style="r+s", pen="1p,blue")
"""

fig.show()



