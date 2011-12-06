#################################
# This file looks for pattern in .h & .cpp files
# to replace by a compile time optimization
#################################

# Imports
import os;
import sys;
import re;
import fileinput
import datetime

if len(sys.argv) < 3:
	for i in range(len(sys.argv)):
		print(sys.argv[i]);
	sys.exit("Not enough arguments");

# Constants

srcPath = sys.argv[1];
outputPath = sys.argv[2];
ignoreFile = ["Incrementator.h"]
sourceFileRegEx = re.compile(".*\.(cpp|h)$")
incrementatorRegEx = re.compile("STATIC_INCREMENTATOR\(([a-zA-Z_]*?)\,?(\d*?)\)")
outputFilename = "IncrementatorOutput.txt"

# Globals

IncrementatorDictionnary = {};
MockUpIncrementatorDictionnary = {};
lineChangedCount = 0;

# Functions

def searchFolder(path, depth = 0):
	dirList = os.listdir(path);

	for fileName in dirList:
		# Is a folder
		if os.path.isdir(os.path.join(path, fileName)):
			searchFolder(os.path.join(path, fileName), depth + 1);
		else: # Is a file
			# Is a source file
			if re.match(sourceFileRegEx, fileName):
				if fileName not in ignoreFile and injectIncrementator(os.path.join(path, fileName), True):
					injectIncrementator(os.path.join(path, fileName), False);
	
def injectIncrementator(path, mockup):
	global IncrementatorDictionnary;
	global MockUpIncrementatorDictionnary;
	global lineChangedCount;
	
	lineChanged = False;
	
	# Get the good dictionnary
	Dictionnary = {};
	if mockup:
		Dictionnary = MockUpIncrementatorDictionnary;
	else:
		Dictionnary = IncrementatorDictionnary;
	
	# Parse the file
	for line in fileinput.FileInput(path,inplace= not mockup):
		matches = re.findall(incrementatorRegEx, line);
		if len(matches) > 0:
			matches = matches[0];
			# Make sure key is in dictionnary
			if matches[0] not in Dictionnary:
				Dictionnary[matches[0]] = 0;
			else:
				Dictionnary[matches[0]] += 1;
				
			if len(matches) < 2 or (len(matches) >= 2 and str(Dictionnary[matches[0]]) != str(matches[1])):
				lineChanged = True;
				if not mockup:
					lineChangedCount += 1;
					line = re.sub(incrementatorRegEx, "STATIC_INCREMENTATOR(" + matches[0] + "," + str(Dictionnary[matches[0]]) + ")", line);
			
		#Print inside the file at the same line
		if not mockup: 
			print(line, end="");
			
		if lineChanged: #Update Real Dictionnaries
			if mockup:
				MockUpIncrementatorDictionnary = Dictionnary;
			else:
				IncrementatorDictionnary = Dictionnary;
		
	return lineChanged;

	
#############################################
################## MAIN #####################
#############################################

# Inject static incrementator
searchFolder(srcPath)

# Generate Output File
outputFile = open(os.path.join(outputPath, outputFilename),'w+');
outputFile.writelines("Last Updated On: " + str(datetime.datetime.now()) + "\n");
for incrementTuple in IncrementatorDictionnary:
	outputFile.writelines(incrementTuple + ": " + str(IncrementatorDictionnary[incrementTuple] + 1) + " item(s)\n");
	
# Generate Output Text
print("Incrementator Ouput: ", end = "");
if lineChangedCount > 0:
	print(str(lineChangedCount) + " line(s) changed");
else:
	print("No line changed");