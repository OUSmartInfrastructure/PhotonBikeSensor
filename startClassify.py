''' 
This is a program meant to be run at midnight each night to bulk classify images an update
the resulting counts into a separate data sheet. This way, data can be processed in batches
so that the process is run reliably
'''

import subprocess, os
from os import walk

imageDirectory = "testImages"
resultFile = "results.tsv"
countFile = "counts.txt"

files = []
results = []
finalList = []
d = {}

#Get list of all files in directory
for (dirpath, dirnames, filenames) in walk(imageDirectory):
	files.extend(filenames)

#organize names alphabetically
files.sort()

#Classify each image individually (I dont know how to run it over a directory :[ )
for file in files:
	print("Evaluating: {}".format(file))
	image = "--image=testImages/"+file
	command = ['python', 'scripts/label_image.py', image]
	process = subprocess.Popen(['python','scripts/label_image.py',image], stdout=subprocess.PIPE)
	out,err = process.communicate()
#	print (err)
	results.append(out.replace('\n',' '))


#For each item in results (each evaluation of a picture), split into separate elements in a list
for result in results:
	if type(result) is not list:
		finalList.append(result.split(' '))

#Remove first 6 elements of each result, relate to timing and are useless
try:
	for i in range(len(finalList)):
		for j in range(6):
			finalList[i].pop(0)
except:
	pass
#Now each item in the list is just the classification for each picture. Element 0 and 1 are the
#predicted category and associated percentage

#Send results to a file to track each evaulation and the highest scoring category
with open(resultFile, 'w+') as f:
	for result in finalList:
		for file in files:
			if(len(result) >=2):
				f.write(file+ "\t result: " + (str(result[0]+": "+result[1])))
				f.write("\n")

#Update counts, get current counts
with open(countFile) as f:
	lines = [line.replace('\n',"").split('\t') for line in f.readlines()]


#get initial values
d = {key:value for (key,value) in lines}

for result in finalList:
	if (len(result) >=2):
		matchName = result[0]
		if matchName in d:
			#Update count in dictionary
			d[str(matchName)] = str(int(d[str(matchName)]) + 1)
print d


with open(resultFile,'w+') as f:
	for i in range(len(files)):
		result = str(files[i]+"\t{}\t{}".format(finalList[i][0], finalList[i][1]))
		print(result)
		f.write(result+"\n")

