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
# 1 - Declare const or read project for the number of spaces / tabs for indentation
# 2 - Find bad syntax
# 3 - Support for prioritizing
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
staticEnumDefinitionRegEx = re.compile("STATIC_DISTRIBUTED_ENUM_DEFINITION\(\s*(([a-zA-Z0-9\_]+?)(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")
staticEnumValueRegEx = re.compile("STATIC_DISTRIBUTED_ENUM\(\s*([a-zA-Z0-9\_]+?)\s*\,\s*(([a-zA-Z0-9\_])+?\s*([=]\s*[a-zA-Z0-9\_]+?)?)\)")
#staticEnumValueGeneralSyntaxRegEx = re.compile("STATIC_DISTRIBUTED_ENUM\([a-zA-Z0-9\_\s=]*?)\)")
blockStartRegEx = re.compile("\s*{")
blockEndRegEx = re.compile("\s*}")
commentBlockRegEx = re.compile("^\s*//")
enumValueRegEx = re.compile("\s*([a-zA-Z0-9\_]+?)\s*(=\s*([a-zA-Z0-9\_]+?))?\s*,\s*//\s*(.*)")
outputFilename = "StaticDistributedEnumOutput.txt"

# Globals

StaticEnumDictionnary = {};
lineChangedCount = 0;

# Functions

def printWarning(enumName, message):
	print("Warning: Static Distributed Enum '" + enumName + "' " + message);

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
	if argumentValueArray[0] == 'COUNT' or argumentValueArray[0] == 'NULLABLE':
		if len(argumentValueArray) > 2:
				printWarning(enumName, "has bad syntax: Too much parameters for " + argumentValueArray[0] + ". The additional parameters will be ignored.");
				argumentValueArray = argumentValueArray[0:1];				
		# Create default Null value enum
		elif len(argumentValueArray) == 1:
			if argumentValueArray[0] == 'COUNT':
				argumentValueArray.append(enumName + "_COUNT");
			elif argumentValueArray[0] == 'NULLABLE':
				argumentValueArray.append(enumName + "_NONE");	
			
	elif argumentValueArray[0] == 'VALUE':
		if len(argumentValueArray) < 2:
			printWarning(enumName, "has not enough parameters for argument VALUE. This argument will be ignored.");
			argumentValueArray = None;
		else:
			# Validate parameter list
			index = 1;
			while index < len(argumentValueArray):
				if argumentValueArray[index] == "":
					printWarning(enumName, "has a missing parameters for argument VALUE. This argument will be ignored.");
					argumentValueArray.pop(index);
					index -= 1;
				index += 1;
	else:
		printWarning(enumName, "has bad syntax: Unknow argument '" + argumentValueArray[0] + "'. It has been ignored.");
		argumentValueArray = None;

	return argumentValueArray;

def buildStaticEnumFromCode(staticEnumCode):
	staticEnumValueList = []
	for codeLine in staticEnumCode:
		# ignore comment
		if re.search(commentBlockRegEx, codeLine) == None:
			matches = re.findall(enumValueRegEx, codeLine);
			matches = matches[0];
			staticEnumValue = {};
			staticEnumValue['value'] = matches[0];
			staticEnumValue['source'] = matches[3];
			if matches[2] != "":
				staticEnumValue['assignment'] = matches[2];
			
			staticEnumValueList.append(staticEnumValue)
			
	return staticEnumValueList;

def findBuiltStaticEnum(path):
	staticEnumCode = {};
	builtStaticEnum = {};
	# Parse the file
	file = fileinput.FileInput(path);
	for line in file:
		matches = re.findall(staticEnumDefinitionRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			staticEnumCode[matches[1]] = [];
			line = file.readline();
			if re.search(blockStartRegEx, line) != None:
				line = file.readline();
				while line != "" and re.search(blockEndRegEx, line) == None:					
					staticEnumCode[matches[1]].append(line);
					line = file.readline();
					
	
	for staticEnumName in staticEnumCode:
		builtStaticEnum[staticEnumName] = buildStaticEnumFromCode(staticEnumCode[staticEnumName]);
	
	return builtStaticEnum;
	
def findStaticEnumDefinition(path):
	global StaticEnumDictionnary;
	
	# Parse the file
	for line in fileinput.FileInput(path):
		matches = re.findall(staticEnumDefinitionRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			staticEnumDict = {};
			staticEnumDict['srcpath'] = path;
			staticEnumDict['code'] = {};
			staticEnumDict['code']['hasSemicolon'] = matches[7] == ";";
			indentation = 0;
			while line[indentation] == " ":
				indentation += 1;
			staticEnumDict['code']['indentation'] = indentation;
			
			if len(matches[0].split(',')) > 0: # It has arguments
				# remove enum name from string
				argumentString = matches[0][len(matches[1])+1:len(matches[0])];
				for argumentTupleString in argumentString.split(','):
					argumentTuple = BuildArgumentTupleFromString(matches[1], argumentTupleString);
					if argumentTuple[0] != None:
						if 'arguments' not in staticEnumDict:
							staticEnumDict['arguments'] = {};
						if argumentTuple[0] == "VALUE":
							if "VALUE" not in staticEnumDict['arguments']:
								staticEnumDict['arguments']['VALUE'] = [];
							staticEnumDict['arguments']["VALUE"].extend(argumentTuple[1:len(argumentTuple)]);
						else:
							if argumentTuple[0] not in staticEnumDict['arguments']:
								staticEnumDict['arguments'][argumentTuple[0]] = argumentTuple[1:len(argumentTuple)];
							else:
								printWarning(matches[1], "has a duplicate of a non-stacking arguments '" + argumentTuple[0] + "'. This additional argument will be ignored.");
							
			StaticEnumDictionnary[matches[1]] = staticEnumDict;
			
def buildValueTupleFromString(enumName, valueString):
	valueTuple = valueString.split("=");
	for index in range(len(valueTuple)):
		valueTuple[index] = valueTuple[index].strip();
	
	if len(valueTuple) > 2:
		printWarning(enumName, "has multiple assign for the same value '" + valueTuple[0] + "'. Additionnal assignment will be ignored.");
		for i in range(2, len(valueTuple)):
			valueTuple.pop(2);
	
	if len(valueTuple) > 1 and valueTuple[1] == "":
		printWarning(enumName, "has a syntax error with the '=' operator for the value '" + valueTuple[0] + "'. Assignment will be ignored.");
		valueTuple.pop(1);
	
	return valueTuple;
			
def validateStaticEnum():
	# Validate assignments
	for staticEnumName in StaticEnumDictionnary:
		staticEnum = StaticEnumDictionnary[staticEnumName];
		# for assignmentValues in staticEnum['values']:
		for assignmentValueIndex in range(len(staticEnum['values'])):
			assignmentValues = staticEnum['values'][assignmentValueIndex];
			# Only assignments
			if 'assignment' in assignmentValues:
				valid = False;
				
				if assignmentValues['assignment'] != assignmentValues['value']:
					for allValues in staticEnum['values']:
						if assignmentValues['assignment'] == allValues['value']:
							valid = True;
						
				if not valid:
					if assignmentValues['assignment'] != assignmentValues['value']:
						printWarning(staticEnumName, "has an assignment from '" + assignmentValues['value'] + "' to an undefined value '" + assignmentValues['assignment'] + "'. This assignment will be ignored");
					else:
						printWarning(staticEnumName, "has a self assignment on '" + assignmentValues['assignment'] + "'. This assignment will be ignored");
					# remove assignment
					staticEnum['values'][assignmentValueIndex].pop('assignment');
			
def doesValueExists(staticEnumName, value):
	for existingValue in StaticEnumDictionnary[staticEnumName]['values']:
		if value == existingValue['value']:
			return True;
	return False;
			
def addValueToStaticEnum(staticEnumName, value, source):
	valueTuple = buildValueTupleFromString(staticEnumName, value);
	valueDict = {'value': valueTuple[0], 'source': source};
	if len(valueTuple) > 1:
		valueDict['assignment'] = valueTuple[1];
		
	if doesValueExists(staticEnumName, valueTuple[0]):
		printWarning(staticEnumName, "has a duplicate '" + valueTuple[0] + "' value. This will generate a compilation error.");
		
	StaticEnumDictionnary[staticEnumName]['values'].append(valueDict);
			
def fillStaticEnum(path):
	global StaticEnumDictionnary;
	
	# Parse the file
	for line in fileinput.FileInput(path):
		matches = re.findall(staticEnumValueRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			if matches[0] in StaticEnumDictionnary:
				addValueToStaticEnum(matches[0], matches[1], "From file: '" + path[len(srcPath):] + "'");
			else:
				printWarning(matches[0], "has not been found ignoring enum value '" + valueTuple[0] + "'.");

def staticEnumValuesAreSame(staticEnumAValuesList, staticEnumBValuesList):
	indexValueA = 0;
	
	staticEnumAValuesList = staticEnumAValuesList[:]
	staticEnumBValuesList = staticEnumBValuesList[:]
	
	while indexValueA < len(staticEnumAValuesList):
		indexValueB = 0;
		while indexValueB < len(staticEnumBValuesList):
			if staticEnumAValuesList[indexValueA] == staticEnumBValuesList[indexValueB]:
				staticEnumAValuesList.pop(indexValueA);
				staticEnumBValuesList.pop(indexValueB);
				indexValueA -= 1;
				break;
			indexValueB += 1;
		indexValueA += 1;
	
	# They are the same if all values have been removed
	return max(len(staticEnumAValuesList), len(staticEnumBValuesList));

# Returns a list of values in a sorted order where they can be written for compilation
# The order is based on assigment dependency
def sortEnumValues(staticEnumName):
	sortedDict = {};
	sortedList = [];
	leftToSortList = [];
	lastLeftToSortLength = 0; # Uses to find unsortable enums
	
	staticEnum = StaticEnumDictionnary[staticEnumName];
	
	# build leftToSortList
	for valueIndex in range(len(staticEnum['values'])):
		if 'assignment' not in staticEnum['values'][valueIndex]:
			sortedList.append(valueIndex);
			sortedDict[staticEnum['values'][valueIndex]['value']] = valueIndex;
		else:
			leftToSortList.append(valueIndex);

	while len(leftToSortList) != 0 and lastLeftToSortLength != len(leftToSortList) != 0:
		lastLeftToSortLength = len(leftToSortList);
		index = 0;
		while index < len(leftToSortList):
			if staticEnum['values'][leftToSortList[index]]['assignment'] in sortedDict:
				sortedDict[staticEnum['values'][leftToSortList[index]]['value']] = leftToSortList[index];
				sortedList.append(leftToSortList[index]);
				leftToSortList.pop(index);
				index -= 1;
			index += 1;
		
	if len(leftToSortList) > 0:
		printWarning(staticEnumName, "has unsolvable dependency between value. Unsolvable dependency were ignored.");
		# Print unsolvable dependency
			
	# Re-order COUNT and NULL
	if 'COUNT' in staticEnum['arguments']:
		# Find COUNT index
		countIndex = 0;
		for valueIndex in range(len(staticEnum['values'])):
			if staticEnum['values'][valueIndex]['value'] == staticEnum['arguments']['COUNT'][0]:
				countIndex = valueIndex;
				break;
		# Remove count from list
		for index in range(len(sortedList)):
			if sortedList[index] == countIndex:
				sortedList.pop(index);
				break;
		# Add count at the end
		sortedList.append(countIndex);
	if 'NULLABLE' in staticEnum['arguments']:
		# Find NULLABLE index
		nullIndex = 0;
		for valueIndex in range(len(staticEnum['values'])):
			if staticEnum['values'][valueIndex]['value'] == staticEnum['arguments']['NULLABLE'][0]:
				nullIndex = valueIndex;
				break;
		# Remove count from list
		for index in range(len(sortedList)):
			if sortedList[index] == nullIndex:
				sortedList.pop(index);
				break;
		# Add count at the end
		sortedList.insert(0, nullIndex);
	
	return sortedList;
	
	
def staticEnumToCodeString(staticEnumName):
	baseIndent = " " * StaticEnumDictionnary[staticEnumName]['code']['indentation'];
	indentStep = " " * 3;
	
	staticEnum = StaticEnumDictionnary[staticEnumName];
	staticDistributedString = baseIndent + "{\n";
	for argument in staticEnum['arguments']:
		staticDistributedString += baseIndent + indentStep + "//" + argument;
		for parameterIndex in range(0, len(staticEnum['arguments'][argument])):
			if parameterIndex == 0:
				staticDistributedString += ": ";
			else:
				staticDistributedString += " | ";
			staticDistributedString += staticEnum['arguments'][argument][parameterIndex];
		staticDistributedString += "\n";
	for valueIndex in sortEnumValues(staticEnumName):
		staticDistributedString += baseIndent + indentStep + staticEnum['values'][valueIndex]['value'];
		if 'assignment' in staticEnum['values'][valueIndex]:
			staticDistributedString += " = " + staticEnum['values'][valueIndex]['assignment'];
		staticDistributedString += ", // " + staticEnum['values'][valueIndex]['source'];
		staticDistributedString += "\n";
	staticDistributedString += baseIndent + "};\n";	
	
	return staticDistributedString;

def injectStaticEnum(path):
	global StaticEnumDictionnary;
	global lineChangedCount;
	
	currentFileStaticEnumList = findBuiltStaticEnum(path);
	# Does file contains static enum definition (what it is?)
	for staticEnum in currentFileStaticEnumList:
		lineToChangeCount = staticEnumValuesAreSame(currentFileStaticEnumList[staticEnum], StaticEnumDictionnary[staticEnum]['values']);
		if lineToChangeCount > 0:
			lineChangedCount += lineToChangeCount;
			specificStaticEnumDefinitionRegEx = re.compile("STATIC_DISTRIBUTED_ENUM_DEFINITION\(\s*(" + staticEnum + "(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")
			file = fileinput.FileInput(path, inplace = True);
			for line in file:
				matches = re.findall(specificStaticEnumDefinitionRegEx, line);
				if len(matches) > 0: # A static distributed enum has been found
					matches = matches[0];
					
					# Remove trailing semi-colon from static enum definition
					if matches[6] == ";":
						line = re.sub(";","", line);
						lineChangedCount += 1;
						
					print(line, end="");
					print(staticEnumToCodeString(staticEnum), end="");
					
					#Inject text in file
					line = file.readline();
					if re.search(blockStartRegEx, line) != None:
						line = file.readline();
						while line != "" and re.search(blockEndRegEx, line) == None:					
							line = file.readline();		
						line = file.readline();
				print(line, end="");
		
			
def buildStaticEnumDefinition():
	# Parse files to find enum definition
	searchFolderAndApplyFunction(srcPath, findStaticEnumDefinition);
	
def buildStaticEnumValues():
	# Use arguments to start building static enum
	for staticEnumName in StaticEnumDictionnary:
		StaticEnumDictionnary[staticEnumName]['values'] = [];
		# fill Enum with default starting values
		if 'NULLABLE' in StaticEnumDictionnary[staticEnumName]['arguments']:
			addValueToStaticEnum(staticEnumName, StaticEnumDictionnary[staticEnumName]['arguments']['NULLABLE'][0], "Enum definition: NULLABLE");
		if 'VALUE' in StaticEnumDictionnary[staticEnumName]['arguments']:
			for value in StaticEnumDictionnary[staticEnumName]['arguments']['VALUE']:
				addValueToStaticEnum(staticEnumName, value, "Enum definition: VALUE");
	
	validateStaticEnum();
	
	# Parse files to build enums
	searchFolderAndApplyFunction(srcPath, fillStaticEnum);
	
	# Use arguments to end building static enum
	for staticEnumName in StaticEnumDictionnary:
		# fill Enum with default end values
		if('COUNT' in StaticEnumDictionnary[staticEnumName]['arguments']):
			addValueToStaticEnum(staticEnumName, StaticEnumDictionnary[staticEnumName]['arguments']['COUNT'][0], "Enum definition: COUNT");
		
	
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
	
# Generate Output Text
print("Static Distributed Enum Ouput: ", end = "");
if lineChangedCount > 0:
	print(str(lineChangedCount) + " line(s) changed");
else:
	print("No line changed");