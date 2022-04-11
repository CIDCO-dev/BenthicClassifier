import sys
import utm
import re
import os

def readLabelFile(filename):
	labels = []

	substrate_map = {
		''				:	'',
		'blocks'			:	'blocks',
		'coarse sediment mixed'		:	'coarse sediment mixed',
		'coarse sediments undetermined' :	'coarse sediments undetermined',
		'cobble'			:	'cobble',
		'cobbles'			:	'cobble',
		'gravels'			:	'gravels',
		'medium-coarse sands'		:	'sands',
		'nd'				:	'',
		'rocky'				:	'rocky',
		'Rocky'				:	'rocky',
		'sands'				:	'sands',
		'sandy mud'			:	'sandy mud'
	}

	vegetation_map = {
		'Alariacées'			:	'vegetated',
		'Algae (s) undifferentiated (s)':	'vegetated',
		'Bangiacées'			:	'vegetated',
		'Chordacées'			:	'vegetated',
		'Cladophoracées'		:	'vegetated',
		'Corallinales encrusting'	:	'vegetated',
		'Costariacées'			:	'vegetated',
		'Delesseriacées'		:	'vegetated',
		'Desmarestiacées'		:	'vegetated',
		'Ectocarpales'			:	'vegetated',
		'Laminariacées'			:	'vegetated',
		'Palmariacées'			:	'vegetated',
		'Ulva'				:	'vegetated',
		'Ulvacées'			:	'vegetated',
		'eelgrass'			:	'vegetated',
		'fucoids'			:	'vegetated',
		'mixed algae'			:	'vegetated',
		'nd'				:	'',
		'not planted'			:	'unvegetated',
		'not vegetated'			:	'unvegetated',
		'undifferentiated algae'	:	'vegetated',
		'undifferentiated vegetation'	:	'vegetated'
	}

	lineMatch = re.compile("^([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([^,]+),([^,]*),([^,]*),([^,]*),([^,]+),(.*)$")

	with open(filename) as file:
		for line in file.readlines():
			lineMatchResult = lineMatch.match(line)

			if lineMatchResult:
				latitude  = float(lineMatchResult.group(1))
				longitude = float(lineMatchResult.group(2))
				substrate1= substrate_map.get(lineMatchResult.group(3),False)
				substrate2= substrate_map.get(lineMatchResult.group(4),False)
				substrate3= substrate_map.get(lineMatchResult.group(5),False)
				vegetation= vegetation_map.get(lineMatchResult.group(7),False)

				if substrate1 and len(substrate1) > 0 and vegetation and len(vegetation)>0 :
					[utm_east, utm_north, utm_zone, utm_letter] = utm.from_latlon(latitude,longitude)
					#print("{},{},{},{}".format(utm_east,utm_north,substrate1,vegetation))
					labels.append((utm_east,utm_north,substrate1,vegetation))
		return labels


# Give identical labels to soundings within specified radius
def bootstrapLabels(surfaceFile,labeledData,radius):
	sys.stderr.write("Reading surface file {}\n".format(surfaceFile))
	with open(surfaceFile) as file:

		lineMatch = re.compile("^([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+),([-+]?[0-9]*\.?[0-9]+)")

		#TODO: Holy O(m*n) batman ! Optimize this!
		for line in file:
			lineMatchResult = lineMatch.match(line)

			if lineMatchResult:
				line_east  = float(lineMatchResult.group(1))
				line_north = float(lineMatchResult.group(2))
				line_depth = float(lineMatchResult.group(3))

				#print("{} {} {}".format(line_east,line_north,line_depth))

				for labeledPoint in labeledData:
					#print("-{} {}".format(labeledPoint[0],labeledPoint[1]))
					if (line_east >= labeledPoint[0] - radius)  and (line_east <= labeledPoint[0] + radius)  and (line_north >= labeledPoint[1] - radius) and (line_north <= labeledPoint[1] + radius):
						print("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(line_east,line_north,line_depth,lineMatchResult.group(4),lineMatchResult.group(5),lineMatchResult.group(6),lineMatchResult.group(7),lineMatchResult.group(8),lineMatchResult.group(9),lineMatchResult.group(10),lineMatchResult.group(11),lineMatchResult.group(12),lineMatchResult.group(13),lineMatchResult.group(14),lineMatchResult.group(15),lineMatchResult.group(16),lineMatchResult.group(17),lineMatchResult.group(18),lineMatchResult.group(19),labeledPoint[2],labeledPoint[3]))

			else:
				sys.stderr.write("Line doesn't match:{}\n".format(line))

		file.close()

################################################################################
# Main                                                                         #
################################################################################

if len(sys.argv) != 4:
	sys.stderr.write("Usage: preprocess-labels.py radius labelfile.csv surfacedirectory\n")
	sys.exit(1)

# Load params from CLI parameters
radius = int(sys.argv[1])
labelfile= sys.argv[2]
surfacedirectory = os.fsencode(sys.argv[3])

sys.stderr.write("Loading ground truthing from {} using a radius of {}m \n".format(labelfile,radius))

# Load ground truthing labels
labeledData = readLabelFile(labelfile)

if labeledData:
	sys.stderr.write("{} ground truthing stations read\n".format(len(labeledData)))

	# Bootstrap labels using neighborhoods in surface files found in surfacedirectory

	sys.stderr.write("Reading point cloud data from directory: {}\n".format(surfacedirectory))

	for filename in os.listdir(surfacedirectory):
		if filename.endswith(b'.Hackel'):
			bootstrapLabels(os.path.join(surfacedirectory,filename),labeledData,radius)

else:
	sys.stderr.write("Cannot load label data from file\n")
