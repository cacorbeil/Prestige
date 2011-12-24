#################################
# This file looks for patterns in .h & .cpp files
# to replace by a compile time optimization
#################################

#################################
# Here how the workflow goes:
# 1- Search for all STATIC_DISTRIBUTED_ENUM_DEFINITION
#       Keep name and source file
# 2- Search for all STATIC_DISTRIBUTED_ENUM
#       Add name to STATIC_DISTRIBUTED_ENUM_DEFINITION
# 3- For all STATIC_DISTRIBUTED_ENUM_DEFINITION, update source file
#    to what have been found
#################################

#################################
# TODO:
# 1- Look for a ; at end of definition and remove it
# 2- Remember the tabulation from definition
# 3- Actually Inject the code into the file
# 4- Declare const or read project for the number of spaces / tabs for indentation
# 5- System for enum parsing to find changes
# 6- Count like nullable with a default name if none specified
# 7- Multiple VALUE argument will stack
# 8- Sort enum so that assignment are defined in the right order
#################################

# Imports
import os;
import sys;
import re;
import fileinput
import datetime

if len(sys.argv) < 2:
	sys.exit("Not enough arguments");

# Constants

srcPath = sys.argv[1];
outputPath = sys.argv[2];
ignoreFile = ["StaticDistributedEnums.h"]
sourceFileRegEx = re.compile(".*\.(cpp|h)$")
staticEnumDefinitionRegEx = re.compile("STATIC_DISTRIBUTED_ENUM_DEFINITION\(\s*(([a-zA-Z0-9\_]+?)(\s*\,\s*([a-zA-Z0-9\_\|]+?))*)\)")
staticEnumValueRegEx = re.compile("STATIC_DISTRIBUTED_ENUM\(\s*([a-zA-Z0-9\_]+?)\s*\,\s*(([a-zA-Z0-9\_])+?\s*([=]\s*[a-zA-Z0-9\_]+?)?)\)")
outputFilename = "StaticDistributedEnumOutput.txt"

# Globals

StaticEnumDictionnary = {};
lineChangedCount = 0;

# Functions

def searchFolderAndApplyFunction(path, functionToApply):
	dirList = os.listdir(path);

	for fileName in dirList:
		# Is a folder
		if os.path.isdir(os.path.join(path, fileName)):
			searchFolderAndApplyFunction(os.path.join(path, fileName), functionToApply);
		else: # Is a file
			# Is a source file
			if re.match(sourceFileRegEx, fileName):
				if fileName not in ignoreFile:
					functionToApply(os.path.join(path, fileName));	
	
def BuildArgumentTupleFromString(enumName, argumentTupleString):
	argumentValueArray = argumentTupleString.split('|');
	for index in range(len(argumentValueArray)):
		argumentValueArray[index] = argumentValueArray[index].strip();

	# Argument 
	if argumentValueArray[0] == 'COUNT':
		if len(argumentValueArray) > 1:
			print("Warning: Static Distributed Enum '" + enumName + "' has too much parameters for argument COUNT.  The additional parameters will be ignored.");
			# Clear all parameters
			for index in range(1, len(argumentValueArray)):
				argumentValueArray.pop(index);
				
	elif argumentValueArray[0] == 'NULLABLE':
		if len(argumentValueArray) > 2:
				print("Warning: Static Distributed Enum '" + enumName + "' has bad syntax: Too much parameters for NULLABLE. The additional parameters will be ignored.");
				argumentValueArray = argumentValueArray[0:1];				
		# Create default Null value enum
		elif len(argumentValueArray) == 1:
			argumentValueArray.append(enumName + "_NONE");	
			
	elif argumentValueArray[0] == 'VALUE':
		if len(argumentValueArray) < 2:
			print("Warning: Static Distributed Enum '" + enumName + "' has not enough parameters for argument VALUE. This argument will be ignored.");
			argumentValueArray = None;
		else:
			# Validate parameter list
			index = 1;
			while index < len(argumentValueArray):
				if argumentValueArray[index] == "":
					print("Warning: Static Distributed Enum '" + enumName + "' has a missing parameters for argument VALUE. This argument will be ignored.");
					argumentValueArray.pop(index);
					index -= 1;
				index += 1;
	else:
		print("Warning: Static Distributed Enum '" + enumName + "' has bad syntax: Unknow argument '" + argumentValueArray[0] + "'. It has been ignored.");
		argumentValueArray = None;

	return argumentValueArray;

	
def findStaticEnumDefinition(path):
	global StaticEnumDictionnary;
	
	# Parse the file
	for line in fileinput.FileInput(path):
		matches = re.findall(staticEnumDefinitionRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			arguments = {'srcpath': path};
			if len(matches[0].split(',')) > 0: # It has arguments
				# remove enum name from string
				argumentString = matches[0][len(matches[1])+1:len(matches[0])];
				for argumentTupleString in argumentString.split(','):
					argumentTuple = BuildArgumentTupleFromString(matches[1], argumentTupleString);
					if argumentTuple[0] != None:
						if 'arguments' not in arguments:
							arguments['arguments'] = {};
						arguments['arguments'][argumentTuple[0]] = argumentTuple[1:len(argumentTuple)];
							
			StaticEnumDictionnary[matches[1]] = arguments;
			
def fillStaticEnum(path):
	global StaticEnumDictionnary;
	
	# Parse the file
	for line in fileinput.FileInput(path):
		matches = re.findall(staticEnumValueRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			valueTuple = matches[1].split("=");
			for index in range(len(valueTuple)):
				valueTuple[index] = valueTuple[index].strip();
			
			if len(valueTuple) > 2:
				print("Warning: Static Distributed Enum '" + matches[0] + "' has multiple assign for the same value '" + valueTuple[0] + "'. Additionnal assignment will be ignored.");
				for i in range(1, len(valueTuple)):
					valueTuple.pop(i);
			
			if len(valueTuple) > 1 and valueTuple[1] == "":
				print("Warning: Static Distributed Enum '" + matches[0] + "' has a syntax error with the '=' operator for the value '" + valueTuple[0] + "'. Assignment will be ignored.");
				valueTuple.pop(1);
			
			StaticEnumDictionnary[matches[0]]['values'].append(valueTuple);
			
def injectStaticEnum(path):
	global StaticEnumDictionnary;
	
	# Parse the file
	#for line in fileinput.FileInput(path, inplace = True):
	#	matches = re.findall(staticEnumDefinitionRegEx, line);
	#	if len(matches) > 0: # A static distributed enum has been found
	#		matches = matches[0];
			
			#Inject text in file
			
def buildStaticEnumDefinition():
	# Parse files to find enum definition
	searchFolderAndApplyFunction(srcPath, findStaticEnumDefinition);
	
def buildStaticEnumValues():
	# Use arguments to start building static enum
	for staticEnumName in StaticEnumDictionnary:
		StaticEnumDictionnary[staticEnumName]['values'] = [];
		# fill Enum with default starting values
		if 'NULLABLE' in StaticEnumDictionnary[staticEnumName]['arguments']:
			StaticEnumDictionnary[staticEnumName]['values'].append(StaticEnumDictionnary[staticEnumName]['arguments']['NULLABLE']);
			
			
		
	# Parse files to build enums
	searchFolderAndApplyFunction(srcPath, fillStaticEnum);
	
	# Use arguments to end building static enum
	for staticEnumName in StaticEnumDictionnary:
		# fill Enum with default end values
		if('COUNT' in StaticEnumDictionnary[staticEnumName]['arguments']):
			StaticEnumDictionnary[staticEnumName]['values'].append([staticEnumName + '_COUNT']);
		
	
def injectStaticEnums():
	# Parse files to build enums
	searchFolderAndApplyFunction(srcPath, injectStaticEnum);		
	
#############################################
################## MAIN #####################
#############################################

# Inject static incrementator
buildStaticEnumDefinition();
buildStaticEnumValues();
injectStaticEnums();

# Generate Output File
outputFile = open(os.path.join(outputPath, outputFilename),'w+');
outputFile.writelines("Last Updated On: " + str(datetime.datetime.now()) + "\n");
for staticEnumName in StaticEnumDictionnary:
	staticEnum = StaticEnumDictionnary[staticEnumName];
	outputFile.writelines("Enum " + staticEnumName + "\n");
	outputFile.writelines("{\n");
	for argument in staticEnum['arguments']:
		outputFile.writelines("   //" + argument);
		for parameterIndex in range(0, len(staticEnum['arguments'][argument])):
			if parameterIndex == 0:
				outputFile.writelines(": ");
			else:
				outputFile.writelines(" | ");
			outputFile.writelines(staticEnum['arguments'][argument][parameterIndex]);
		outputFile.writelines("\n");
	for valueIndex in range(len(staticEnum['values'])):
		outputFile.writelines("   " + staticEnum['values'][valueIndex][0]);
		if len(staticEnum['values'][valueIndex]) > 1:
			outputFile.writelines(" = " + staticEnum['values'][valueIndex][1]);
		outputFile.writelines(",\n");
	outputFile.writelines("};\n");
	
# Generate Output Text
print("Static Distributed Enum Ouput: ", end = "");
if lineChangedCount > 0:
	print(str(lineChangedCount) + " line(s) changed");
else:
	print("No line changed");