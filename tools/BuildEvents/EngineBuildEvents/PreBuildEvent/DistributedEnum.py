#################################
# This file looks for patterns in .h & .cpp files
# to replace by a compile time optimization
#################################

#################################
# Here how the workflow goes:
# 1- Search for all DISTRIBUTED_ENUM_DEFINITION
#       Keep name and source file
# 2- Search for all DISTRIBUTED_ENUM
#       Add name to DISTRIBUTED_ENUM_DEFINITION
# 3- For all DISTRIBUTED_ENUM_DEFINITION, update source file
#    to what have been found
#################################

#################################
# TODO:
# 1 - Declare const or read project for the number of spaces / tabs for indentation
# 2 - Find bad syntax
# 3 - Support for prioritizing
# 4 - what happens when 2 def in the same file / different file
# 5 - What happens when a non exisiting argument is used
# 5 - What happens when unsortable enum values are made?
# 6 - What happens if the file is locked?
# 7 - Fix that if you remove the footer the enum falls through and clear stuff up
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
ignoreFile = ["DistributedEnum.h"]
sourceFileRegEx = re.compile(".*\.(cpp|h)$")
DistributedEnumHeaderMacro = "DISTRIBUTED_ENUM_DEFINITION_BEGIN";
DistributedEnumFooterMacro = "DISTRIBUTED_ENUM_DEFINITION_END";
DistributedEnumDefinitionRegEx = re.compile(DistributedEnumHeaderMacro + "\(\s*(([a-zA-Z0-9\_]+?)(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")
DistributedEnumValueRegEx = re.compile("DISTRIBUTED_ENUM\(\s*([a-zA-Z0-9\_]+?)\s*\,\s*(([a-zA-Z0-9\_])+?\s*([=]\s*[a-zA-Z0-9\_]+?)?)\)")
blockStartRegEx = re.compile("\s*{")
BlockEndRegEx = re.compile("^\s*}")
DistributedEnumDefinitionEndRegEx = re.compile("\s*DISTRIBUTED_ENUM_DEFINITION_END")
commentBlockRegEx = re.compile("^\s*//")
enumValueRegEx = re.compile("\s*([a-zA-Z0-9\_]+?)\s*(=\s*([a-zA-Z0-9\_]+?))?\s*,\s*//\s*(.*)")
outputFilename = "StaticDistributedEnumOutput.txt"

# Globals

EnumDictionary = {};
lineChangedCount = 0;

# Functions

##################################################
# PrintWarning
##################################################
# Common warning message
# ARGUMENTS:
#  enumName: Name of the enum for which the warning has been triggered
#  message: The warning message
def PrintWarning(enumName, message):
	print("Warning: Static Distributed Enum '" + enumName + "' " + message);

##################################################
# SearchFolderAndApplyFunction
##################################################
# For all files in the whole tree hierarchy of the given path, apply the given function on each 
#   file that is a source file
# ARGUMENTS:
#  path: Path to iterate through folder and files
#  functionToApply: The function to apply to each source files
def SearchFolderAndApplyFunction(path, functionToApply):
	dirList = os.listdir(path);

	for fileName in dirList:
		# Is a folder
		if os.path.isdir(os.path.join(path, fileName)):
			SearchFolderAndApplyFunction(os.path.join(path, fileName), functionToApply);
		else: # Is a file
			# Is a source file
			if re.match(sourceFileRegEx, fileName):
				if fileName not in ignoreFile:
					functionToApply(os.path.join(path, fileName));	

##################################################
# BuildArgumentListFromCodeString
##################################################
# Take an argument string direct from code, validates it and split it into an array.
# RETURNS:
#   If the value was valid: A list where the first item is the type of the argument and the rest are parameters.
#   If the value was invalid: None and a warning is printed
# ARGUMENTS:
#  enumName: The name of the enum described by this argument
#  argumentString: The argument string to validate and split
def BuildArgumentListFromCodeString(enumName, argumentString):
	# Split and clean the argument
	argumentValueArray = argumentString.split('|');
	for index in range(len(argumentValueArray)):
		argumentValueArray[index] = argumentValueArray[index].strip();

	# Validate argument and parameters 
	if argumentValueArray[0] == 'COUNT' or argumentValueArray[0] == 'NULLABLE':
		if len(argumentValueArray) > 2:
				PrintWarning(enumName, "has bad syntax: Too much parameters for " + argumentValueArray[0] + ". The additional parameters will be ignored.");
				argumentValueArray = argumentValueArray[0:1];				
		# Create default Null value enum
		elif len(argumentValueArray) == 1:
			if argumentValueArray[0] == 'COUNT':
				argumentValueArray.append(enumName + "_COUNT");
			elif argumentValueArray[0] == 'NULLABLE':
				argumentValueArray.append(enumName + "_NONE");	
			
	elif argumentValueArray[0] == 'VALUE':
		if len(argumentValueArray) < 2:
			PrintWarning(enumName, "has not enough parameters for argument VALUE. This argument will be ignored.");
			argumentValueArray = None;
		else:
			# Validate parameter list
			index = 1;
			while index < len(argumentValueArray):
				if argumentValueArray[index] == "":
					PrintWarning(enumName, "has a missing parameters for argument VALUE. This argument will be ignored.");
					argumentValueArray.pop(index);
					index -= 1;
				index += 1;
	else:
		PrintWarning(enumName, "has bad syntax: Unknow argument '" + argumentValueArray[0] + "'. It has been ignored.");
		argumentValueArray = None;

	return argumentValueArray;

##################################################
# FindDistributedEnumDefinition
##################################################
# Iterate through the given line of codes and rebuild a list enum values
# RETURNS:
#   List of values that are a dictionnary defined by a 'value', 'source' and optionally an 'assignment'
# ARGUMENTS:
#  enumValuesCodeList: List of code line to parse for values
def BuildDistributedEnumValuesFromCode(enumValuesCodeList):
	enumValueList = []
	for codeLine in enumValuesCodeList:
		# ignore comment
		if re.search(commentBlockRegEx, codeLine) == None:
			matches = re.findall(enumValueRegEx, codeLine);
			matches = matches[0];
			enumValue = {};
			enumValue['value'] = matches[0];
			enumValue['source'] = matches[3];
			if matches[2] != "":
				enumValue['assignment'] = matches[2];
			
			enumValueList.append(enumValue)
			
	return enumValueList;

##################################################
# FindBuiltDistributedEnum
##################################################
# Look into the specified file for definitons of distributed enums
# RETURNS:
#   A dictionnary of enum found, where the key is the enum name and the value is a list of values
# ARGUMENTS:
#  filePath: The path of the file to parse for distributed enums
def FindBuiltDistributedEnum(filePath):
	enumCode = {};
	builtEnum = {};
	
	# Parse the file
	file = fileinput.FileInput(filePath);
	for line in file:
		matches = re.findall(DistributedEnumDefinitionRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			enumCode[matches[1]] = [];
			line = file.readline();
			if re.search(blockStartRegEx, line) != None:
				line = file.readline();
				while line != "" and re.search(BlockEndRegEx, line) == None:					
					enumCode[matches[1]].append(line);
					line = file.readline();
	
	for enumName in enumCode:
		builtEnum[enumName] = BuildDistributedEnumValuesFromCode(enumCode[enumName]);
	
	return builtEnum;
	
##################################################
# FindDistributedEnumDefinition
##################################################
# Look into the specified code file for distrivuted enum definiton and build a complex
#   dictionnary in the global variable EnumDictionary with attributes:
#     srcpath:   Path of the source file
#     code:      Attributes that have to do the way the enum was written in code
#     arguments: The arguments given in the enum definition
# RETURNS:
#   Nothing, but it fills the global variable "EnumDictionary"
# ARGUMENTS:
#  filePath: The path of the file to parse for distributed enum definitions
def FindDistributedEnumDefinition(filePath):
	global EnumDictionary;
	
	# Parse the file
	for line in fileinput.FileInput(filePath):
		matches = re.findall(DistributedEnumDefinitionRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			enumDict = {};
			enumDict['srcpath'] = filePath;
			enumDict['code'] = {};
			indentation = 0;
			while line[indentation] == " ":
				indentation += 1;
			enumDict['code']['indentation'] = indentation;
			
			# It has arguments, build the arguments attributes
			if len(matches[0].split(',')) > 0:
				# remove enum name from string
				allArgumentsString = matches[0][len(matches[1])+1:len(matches[0])];
				# for each argument
				for argumentString in allArgumentsString.split(','):
					argumentList = BuildArgumentListFromCodeString(matches[1], argumentString);
					if argumentList != None:
						if 'arguments' not in enumDict:
							enumDict['arguments'] = {};
							
						# Special case, value arguments stacks
						if argumentList[0] == "VALUE":
							if "VALUE" not in enumDict['arguments']:
								enumDict['arguments'][argumentList[0]] = [];
							enumDict['arguments'][argumentList[0]].extend(argumentList[1:]);
						else:
							if argumentList[0] not in enumDict['arguments']:
								enumDict['arguments'][argumentList[0]] = argumentList[1:];
							else:
								PrintWarning(matches[1], "has a duplicate of a non-stacking arguments '" + argumentList[0] + "'. This additional argument will be ignored.");
							
			EnumDictionary[matches[1]] = enumDict;
			
##################################################
# BuildValueAssignmentTupleFromString
##################################################
# Takes a value string, validates and splits it into 2 part in a list.
# RETURNS:
#   An array where the first item is the value and if there is an assignment the second item is the assignment.
# ARGUMENTS:
#  enumName:    The name of the enum having this value
#  valueString: The string of the value
def BuildValueAssignmentTupleFromString(enumName, valueString):
	# Split and clean values
	valueTuple = valueString.split("=");
	for index in range(len(valueTuple)):
		valueTuple[index] = valueTuple[index].strip();
	
	# Assignment can only have 2 part
	if len(valueTuple) > 2:
		PrintWarning(enumName, "has multiple assign for the same value '" + valueTuple[0] + "'. Additionnal assignment will be ignored.");
		# Clear superfluous values
		for i in range(2, len(valueTuple)):
			valueTuple.pop(2);
	
	# Assignme has no value
	if len(valueTuple) > 1 and valueTuple[1] == "":
		PrintWarning(enumName, "has a syntax error with the '=' operator for the value '" + valueTuple[0] + "'. Assignment will be ignored.");
		# Clear empty assigment
		valueTuple.pop(1);
	
	return valueTuple;
			
##################################################
# ValidateDistributedEnum
##################################################
# Validate all assignment, by making sure they point to a value that exist inside the enum
def ValidateDistributedEnum():
	# Validate assignments
	for enumName in EnumDictionary:
		enum = EnumDictionary[enumName];
		# for assignmentValues in enum['values']:
		for assignmentValueIndex in range(len(enum['values'])):
			assignmentValues = enum['values'][assignmentValueIndex];
			# Only assignments
			if 'assignment' in assignmentValues:
				valid = False;
				
				# Skip search if self assignment
				if assignmentValues['assignment'] != assignmentValues['value']:
					# Look for a value that fits the assignment
					for allValues in enum['values']:
						if assignmentValues['assignment'] == allValues['value']:
							valid = True;
						
				# No match was found
				if not valid:
					# Not self assignment
					if assignmentValues['assignment'] != assignmentValues['value']:
						PrintWarning(enumName, "has an assignment from '" + assignmentValues['value'] + "' to an undefined value '" + assignmentValues['assignment'] + "'. This assignment will be ignored");
					else: # Self assignment
						PrintWarning(enumName, "has a self assignment on '" + assignmentValues['assignment'] + "'. This assignment will be ignored");
					# remove assignment
					enum['values'][assignmentValueIndex].pop('assignment');
			
##################################################
# DoesValueExistsInEnum
##################################################
# Look into the given distributed enum for the given value.
# RETURNS:
#   True if it exists in the enum
# ARGUMENTS:
#  enumName: The name of the enum to look for the value
#  valueName:    The name of the value to look for
def DoesValueExistsInEnum(enumName, valueName):
	# Look for value
	for existingValue in EnumDictionary[enumName]['values']:
		if valueName == existingValue['value']:
			return True;
	# Hasn't been found
	return False;
			

##################################################
# AddValueToDistributedEnum
##################################################
# Add the given value in the specified enum. If a value with the same name was already 
#   existing a warning is added, but the value is still added. This have been done, so 
#   the coder will have a compilation error and have access to the source of both enum 
#   and choose which one should be renamed.
# ARGUMENTS:
#  enumName: The name of the enum that contains the value
#  value:    The value to look for
#  source:   Source description string
def AddValueToDistributedEnum(enumName, value, source):
	valueTuple = BuildValueAssignmentTupleFromString(enumName, value);
	valueDict = {'value': valueTuple[0], 'source': source};
	if len(valueTuple) > 1:
		valueDict['assignment'] = valueTuple[1];
		
	if DoesValueExistsInEnum(enumName, valueTuple[0]):
		PrintWarning(enumName, "has a duplicate '" + valueTuple[0] + "' value. This will generate a compilation error.");
		
	EnumDictionary[enumName]['values'].append(valueDict);
			
##################################################
# FillDistributedEnum
##################################################
# Look for distributed enum values in the specified file and add them to the corresponding definition
# ARGUMENTS:
#  filePath: The path of the file to parse for values
def FillDistributedEnum(filePath):
	global EnumDictionary;
	
	# Parse the file
	for line in fileinput.FileInput(filePath):
		matches = re.findall(DistributedEnumValueRegEx, line);
		if len(matches) > 0: # A static distributed enum has been found
			matches = matches[0];
			# We have a definition?
			if matches[0] in EnumDictionary:
				AddValueToDistributedEnum(matches[0], matches[1], "From file: '" + filePath[len(srcPath):] + "'");
			else:
				PrintWarning(matches[0], "has not been found ignoring enum value '" + matches[1] + "'.");

##################################################
# DistributedEnumValuesDifferenceCount
##################################################
# Count the difference between the two given distributed enum value list
# RETURNS:
#   The difference count from these 2 values
# ARGUMENTS:
#  lhsDistributedEnumValuesList: Values for a distributed enum
#  rhsDistributedEnumValuesList: Values for another distributed enum
def DistributedEnumValuesDifferenceCount(lhsDistributedEnumValuesList, rhsDistributedEnumValuesList):
	lhsIndexValue = 0;
	
	lhsDistributedEnumValuesList = lhsDistributedEnumValuesList[:]
	rhsDistributedEnumValuesList = rhsDistributedEnumValuesList[:]
	
	while lhsIndexValue < len(lhsDistributedEnumValuesList):
		rhsIndexValue = 0;
		while rhsIndexValue < len(rhsDistributedEnumValuesList):
			if lhsDistributedEnumValuesList[lhsIndexValue] == rhsDistributedEnumValuesList[rhsIndexValue]:
				lhsDistributedEnumValuesList.pop(lhsIndexValue);
				rhsDistributedEnumValuesList.pop(rhsIndexValue);
				lhsIndexValue -= 1;
				break;
			rhsIndexValue += 1;
		lhsIndexValue += 1;
	
	# They are the same if all values have been removed
	return max(len(lhsDistributedEnumValuesList), len(rhsDistributedEnumValuesList));

##################################################
# SortEnumValues
##################################################
# Count the difference between the two given distributed enum value list
# RETURNS:
#   The list of index of values in a sorted order where they can be written for compilation. 
#   The order is based on assigment dependency. 
#   Special case: NULLABLE value is put at the beginning and COUNT at the end.
# ARGUMENTS:
#  enumName: The name of the enum to extract and sort its values
def SortEnumValues(enumName):
	sortedDict = {}; # All sorted values will be include in this dictionnary
	sortedList = []; # This is the list all sorted out
	leftToSortList = []; # This is the list of all unsorted values
	lastLeftToSortLength = 0; # Uses to find unsortable enums
	
	enum = EnumDictionary[enumName];
	
	# build leftToSortList
	for valueIndex in range(len(enum['values'])):
		# If we have no assignment, then already sorted!
		if 'assignment' not in enum['values'][valueIndex]:
			sortedList.append(valueIndex);
			# Include value in sorted dictionnary
			sortedDict[enum['values'][valueIndex]['value']] = valueIndex;
		else:
			leftToSortList.append(valueIndex);

	# While we still have some unsorted values and we're still making progress toward sorting them
	while len(leftToSortList) != 0 and lastLeftToSortLength != len(leftToSortList) != 0:
		# Update value to find unsortable enums
		lastLeftToSortLength = len(leftToSortList);
		# For all unsorted values
		index = 0;
		while index < len(leftToSortList):
			# If the assignment is now sorted
			if enum['values'][leftToSortList[index]]['assignment'] in sortedDict:
				# Add to sorted dictionnary and list
				sortedDict[enum['values'][leftToSortList[index]]['value']] = leftToSortList[index];
				sortedList.append(leftToSortList[index]);
				# Remove from unsorted list
				leftToSortList.pop(index);
				index -= 1;
			index += 1;
		
	# Not all values were sorted :(
	if len(leftToSortList) > 0:
		PrintWarning(enumName, "has unsolvable dependency between value. Unsolvable dependency were ignored.");
			
	# Re-order COUNT and NULL
	if 'COUNT' in enum['arguments']:
		# Find COUNT index
		countIndex = 0;
		for valueIndex in range(len(enum['values'])):
			if enum['values'][valueIndex]['value'] == enum['arguments']['COUNT'][0]:
				countIndex = valueIndex;
				break;
		# Remove count from list
		for index in range(len(sortedList)):
			if sortedList[index] == countIndex:
				sortedList.pop(index);
				break;
		# Add count at the end
		sortedList.append(countIndex);
		
	if 'NULLABLE' in enum['arguments']:
		# Find NULLABLE index
		nullIndex = 0;
		for valueIndex in range(len(enum['values'])):
			if enum['values'][valueIndex]['value'] == enum['arguments']['NULLABLE'][0]:
				nullIndex = valueIndex;
				break;
		# Remove count from list
		for index in range(len(sortedList)):
			if sortedList[index] == nullIndex:
				sortedList.pop(index);
				break;
		# Add count at the beggining
		sortedList.insert(0, nullIndex);
	
	return sortedList;
	
##################################################
# DistributedEnumToCodeString
##################################################
# Convert the given enum to code c++ that can be injected and compiled
# RETURNS:
#   The string of the enum, without header macros
# ARGUMENTS:
#  enumName: The name of the enum to convert to code
def DistributedEnumToCodeString(enumName):
	baseIndent = " " * EnumDictionary[enumName]['code']['indentation'];
	indentStep = " " * 3; # 3 Should be taken for VS project?
	
	staticEnum = EnumDictionary[enumName];
	staticDistributedString = baseIndent + "{\n";
	
	# For all sorted values index
	for valueIndex in SortEnumValues(enumName):
		staticDistributedString += baseIndent + indentStep + staticEnum['values'][valueIndex]['value'];
		if 'assignment' in staticEnum['values'][valueIndex]:
			staticDistributedString += " = " + staticEnum['values'][valueIndex]['assignment'];
		staticDistributedString += ", // " + staticEnum['values'][valueIndex]['source'];
		staticDistributedString += "\n";
	staticDistributedString += baseIndent + "}\n";	
	staticDistributedString += baseIndent + DistributedEnumFooterMacro;	
	
	return staticDistributedString;

##################################################
# InjectDistributedEnum
##################################################
# Look through files to find distributed enum definition and update them if necessary
# ARGUMENTS:
#  filePath: The path of the file to update distributed enum
def InjectDistributedEnum(filePath):
	global lineChangedCount;
	
	# Look for existing enum with values
	currentFileDistributedEnumList = FindBuiltDistributedEnum(filePath);
	
	for staticEnum in currentFileDistributedEnumList:
		lineToChangeCount = DistributedEnumValuesDifferenceCount(currentFileDistributedEnumList[staticEnum], EnumDictionary[staticEnum]['values']);
		# There are changes
		if lineToChangeCount > 0:
			lineChangedCount += lineToChangeCount;
			specificDistributedEnumDefinitionRegEx = re.compile(DistributedEnumHeaderMacro + "\(\s*(" + staticEnum + "(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")
			file = fileinput.FileInput(filePath, inplace = True);
			# Search for the specific enum
			for line in file:
				matches = re.findall(specificDistributedEnumDefinitionRegEx, line);
				# The specific enum has been found
				if len(matches) > 0:
					matches = matches[0];
					
					# Remove trailing semi-colon from static enum definition
					if matches[6] == ";":
						line = re.sub(";","", line);
						lineChangedCount += 1;
						
					# Print header without semi-colob
					print(line, end="");
					# Print values
					print(DistributedEnumToCodeString(staticEnum), end="");
					
					#Skip the last enum values
					line = file.readline();
					if re.search(blockStartRegEx, line) != None:
						line = file.readline();
						while line != "" and re.search(DistributedEnumDefinitionEndRegEx, line) == None:					
							line = file.readline();		
						line = file.readline();
						
				# Print unrelated line of code to file
				print(line, end="");
		
##################################################
# BuildDistributedEnumDefinition
##################################################
# Look through all source files for distributed enum definition and keep these definition in memory
def BuildDistributedEnumDefinition():
	# Parse files to find enum definition
	SearchFolderAndApplyFunction(srcPath, FindDistributedEnumDefinition);
	
##################################################
# BuildDistributedEnumDefinition
##################################################
# Fill all the distributed enum definitions with values from arguments and source files
def BuildDistributedEnumValues():
	# Use arguments to start building static enum
	for staticEnumName in EnumDictionary:
		EnumDictionary[staticEnumName]['values'] = [];
		# fill Enum with default arguments values (NULLABLE and COUNT will be sorted later on to be first and last respectively)
		if 'NULLABLE' in EnumDictionary[staticEnumName]['arguments']:
			AddValueToDistributedEnum(staticEnumName, EnumDictionary[staticEnumName]['arguments']['NULLABLE'][0], "Enum definition: NULLABLE");
		if 'VALUE' in EnumDictionary[staticEnumName]['arguments']:
			for value in EnumDictionary[staticEnumName]['arguments']['VALUE']:
				AddValueToDistributedEnum(staticEnumName, value, "Enum definition: VALUE");
		if 'COUNT' in EnumDictionary[staticEnumName]['arguments']:
			AddValueToDistributedEnum(staticEnumName, EnumDictionary[staticEnumName]['arguments']['COUNT'][0], "Enum definition: COUNT");				
	
	# Validate argument values
	ValidateDistributedEnum();
	
	# Parse files to build enums
	SearchFolderAndApplyFunction(srcPath, FillDistributedEnum);
	
	# Validate source values
	ValidateDistributedEnum();

##################################################
# InjectDistributedEnums
##################################################
# Inject into the source files the update values if necessary
def InjectDistributedEnums():
	# Parse files to build enums
	SearchFolderAndApplyFunction(srcPath, InjectDistributedEnum);	
	
#############################################
################## MAIN #####################
#############################################

# Inject static incrementator
BuildDistributedEnumDefinition();
BuildDistributedEnumValues();
InjectDistributedEnums();
	
# Generate Output Text
print("Distributed Enum Ouput: ", end = "");
if lineChangedCount > 0:
	print(str(lineChangedCount) + " line(s) changed");
else:
	print("No line changed");