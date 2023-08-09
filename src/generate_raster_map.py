import pygmt
import sys, csv
import numpy as np


if len(sys.argv) != 3:
	sys.stderr.write("Usage: python3 generate_raster_map.py inputData Map_title\n")
	sys.exit(1)
	
inputFilePath = sys.argv[1]
mapTitle = sys.argv[2]

lat = []
lon = []
gmmClass = []
gbxClass = []

# Load file
with open(inputFilePath) as f:
	reader = csv.reader(f, delimiter=" ")
	labeled_data = list(reader)
	sys.stderr.write("[+] Loaded {} lines\n".format(len(labeled_data)))
	
	for line in labeled_data:
		data = [ float(h) for h in line]
		lat.append(data[0])
		lon.append(data[1])
		gbxClass.append(data[2])
		gmmClass.append(data[3])
		
minLat = min(lat)
minLon = min(lon)
maxLat = max(lat)
maxLon = max(lon)


fig = pygmt.Figure()
fig.coast(
    region=[minLon - 0.003, maxLon + 0.003, minLat - 0.003, maxLat + 0.003],
    shorelines=True,
    land="lightgreen",
    water="lightblue",
    frame=["a", "+t{}".format(mapTitle)],
)

fig.show()



