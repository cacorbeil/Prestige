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

# Imports
import os;
import stat;
import sys;
import re;
import fileinput;
import datetime;
import SolutionConstants;
import ErrorHandling;

if len(sys.argv) - 1 < 1:
	sys.exit("Not enough arguments, need 1, but got " + str(len(sys.argv) - 1));

# Constants

CodeSourcePath = sys.argv[1];
IgnoredFileList = ["DistributedEnum.h"];
SourceFileRegEx = re.compile(".*\.(cpp|h)$")
DistributedEnumHeaderMacro = "DISTRIBUTED_ENUM_DEFINITION_BEGIN";
DistributedEnumFooterMacro = "DISTRIBUTED_ENUM_DEFINITION_END";
VARIABLE_STRING = "[a-zA-Z\_]+?[a-zA-Z0-9\_]*";
ARGUMENT_STRING = "[a-zA-Z\_]+?[a-zA-Z0-9\_\|]*";
PARAMETER_STRING = "[a-zA-Z0-9\_]*";
DistributedEnumDefinitionFullSyntaxRegEx = re.compile("^\s*" + DistributedEnumHeaderMacro + "\(\s*(("+VARIABLE_STRING+")(\s*\,\s*("+ARGUMENT_STRING+")|(VALUE\s*(|\s*("+ARGUMENT_STRING+"\s*[=]\s*"+PARAMETER_STRING+")+)?)\s*)*)\s*\)([;])?");
DistributedEnumDefinitionHeaderRegEx = re.compile("^\s*" + DistributedEnumHeaderMacro + "\(("+VARIABLE_STRING+")");
DistributedEnumValueRegEx = re.compile("^\s*DISTRIBUTED_ENUM\(\s*([a-zA-Z0-9\_]+?)\s*\,\s*(([a-zA-Z0-9\_])+?\s*([=]\s*[a-zA-Z0-9\_]+?)?)\)");
BlockStartRegEx = re.compile("^\s*{");
BlockEndRegEx = re.compile("^\s*}");
DistributedEnumDefinitionEndRegEx = re.compile("^\s*DISTRIBUTED_ENUM_DEFINITION_END");
CommentBlockRegEx = re.compile("^\s*//");
EmptyLineRegEx = re.compile("^\s*\n");
EnumValueRegEx = re.compile("^\s*([a-zA-Z0-9\_]+?)\s*(=\s*([a-zA-Z0-9\_]+?))?\s*,\s*//\s*(.*)");
NumberValueRegEx = re.compile("^\d+$");

# Globals

EnumDictionary = {};
LineChangedCount = 0;

CurrentPath = "Undefined";
CurrentLine = -1;


# Functions

##################################################
# PrintWarning
##################################################
# Common warning message
# NOTES:
#   Expect global variable CurrentPath and CurrentLine to be set before this function is called without these parameters
# ARGUMENTS:
#  enumName: Name of the enum for which the warning has been triggered
#  message: The warning message
#  path: The path of the of the origin of the warning, will use global variable 'CurrentPath' if the argument is ommited
#  lineNo: The line number of the of the origin of the warning, will use global variable 'CurrentLine' if the argument is ommited
def PrintWarning(enumName, message, path = None, lineNo = None):
	if path == None:
		path = CurrentPath;
	if lineNo == None:
		lineNo = CurrentLine;		
	ErrorHandling.PrintWarning(path, lineNo, "Distributed Enum '" + enumName + "' " + message);

##################################################
# PrintError
##################################################
# Common error message
# NOTES:
#   Expect global variable CurrentPath and CurrentLine to be set before this function is called without these parameters
# ARGUMENTS:
#  enumName: Name of the enum for which the error has been triggered
#  message: The warning message
#  path: The path of the of the origin of the error, will use global variable 'CurrentPath' if the argument is ommited
#  lineNo: The line number of the of the origin of the error, will use global variable 'CurrentLine' if the argument is ommited
def PrintError(enumName, message, path = None, lineNo = None):
	if path == None:
		path = CurrentPath;
	if lineNo == None:
		lineNo = CurrentLine;		
	ErrorHandling.PrintError(path, lineNo, "Distributed Enum '" + enumName + "' " + message);

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
			if re.match(SourceFileRegEx, fileName):
				if fileName not in IgnoredFileList:
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
				PrintWarning(enumName, "has too much parameters for " + argumentValueArray[0] + ". The additional parameters will be ignored.");
				argumentValueArray = argumentValueArray[:1];
		# Create default Null value enum
		elif len(argumentValueArray) == 1:
			if argumentValueArray[0] == 'COUNT':
				argumentValueArray.append("COUNT");
			elif argumentValueArray[0] == 'NULLABLE':
				argumentValueArray.append("NONE");	
			
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
	elif argumentValueArray[0] == 'UNIQUE' or argumentValueArray[0] == 'NO_ASSIGNMENT':
		if len(argumentValueArray) > 1:
			PrintWarning(enumName, "has too much parameters for " + argumentValueArray[0] + ". The additional parameters will be ignored.");
			argumentValueArray = argumentValueArray[:0];
	else:
		PrintWarning(enumName, "has an unknow argument '" + argumentValueArray[0] + "'. It has been ignored.");
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
		if re.search(CommentBlockRegEx, codeLine) == None:
			matches = re.findall(EnumValueRegEx, codeLine);
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
		matches = re.findall(DistributedEnumDefinitionFullSyntaxRegEx, line);
		if len(matches) > 0: # A distributed enum has been found
			matches = matches[0];
			enumCode[matches[1]] = [];
			line = file.readline();
			if re.search(BlockStartRegEx, line) != None:
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
#     srcPath:   Path of the source file
#     code:      Attributes that have to do the way the enum was written in code
#     arguments: The arguments given in the enum definition
# RETURNS:
#   Nothing, but it fills the global variable "EnumDictionary"
# ARGUMENTS:
#  filePath: The path of the file to parse for distributed enum definitions
def FindDistributedEnumDefinition(filePath):
	global EnumDictionary;
	global CurrentPath, CurrentLine;
	
	# Parse the file
	sourceCodeFile = fileinput.FileInput(filePath);
	for line in sourceCodeFile:
		headerMatches = re.findall(DistributedEnumDefinitionHeaderRegEx, line);
		if len(headerMatches) > 0: # A distributed enum has been found
			CurrentPath = filePath;
			CurrentLine = sourceCodeFile.lineno();			
			matches = re.findall(DistributedEnumDefinitionFullSyntaxRegEx, line);
			if len(matches) > 0: # A distributed enum has been found			
				matches = matches[0];
				# Verify if the same enum has been found
				if matches[1] not in EnumDictionary:
					enumDict = {};
					enumDict['code'] = {};
					enumDict['code']['srcPath'] = CurrentPath;
					enumDict['code']['srcLine'] = CurrentLine;					
					indentation = 0;
					while line[indentation] == " ":
						indentation += 1;
					enumDict['code']['indentation'] = indentation;
					enumDict['code']['enumContentLineCount'] = EnumContentLineCount(filePath, matches[1]);
					
					# Update dictionnary, so PrintWarning will have the enum data
					EnumDictionary[matches[1]] = enumDict;				
					
					enumDict['arguments'] = {};
					# It has arguments, build the arguments attributes
					if len(matches[0].split(',')) > 1:
						# remove enum name from string
						allArgumentsString = matches[0][len(matches[1])+1:len(matches[0])];
						# for each argument
						for argumentString in allArgumentsString.split(','):
							argumentList = BuildArgumentListFromCodeString(matches[1], argumentString);
							if argumentList != None:
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
									
					# Update the dictionnary so the definition will be complete
					EnumDictionary[matches[1]] = enumDict;
				else:
					PrintWarning(matches[1], "has a redefinition. A compilation error will be generated.");					
			else: # Syntax malformed
				PrintError(headerMatches[0], "has a bad syntax. Could not parse definition, all distributed enum value will not be linked to this definition");
			
				
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
	
	# Assignment validation
	if len(valueTuple) > 1:
		if valueTuple[1] == "":
			PrintWarning(enumName, "has a syntax error with the '=' operator for the value '" + valueTuple[0] + "'. Assignment will be ignored.");
			# Clear empty assigment
			valueTuple.pop(1);
		elif 'NO_ASSIGNMENT' in EnumDictionary[enumName]['arguments']:
			PrintError(enumName, "has an assignment on value '" + valueTuple[0] + "'. Assignment are not tolerated for this enum since it has the argument 'NO_ASSIGNMENT'.");
		elif 'UNIQUE' in EnumDictionary[enumName]['arguments'] and re.search(NumberValueRegEx, valueTuple[1]) == None:
			PrintError(enumName, "has an assignment on value '" + valueTuple[0] + "' to another value '" + valueTuple[1] + "'. Assignments are to be unique for this enum since it has the argument 'UNIQUE'.");						
		
	return valueTuple;
			
##################################################
# DoesEnumHasArgument
##################################################
# Returns if the specified argument is in the specified enum
# RETURNS:
#   True if the argument is in the enum
# PARAMETERS:
#   enumName: The name of the enum from which to look in for the argument
#   argumentName: The name of the argument to look for
def DoesEnumHasArgument(enumName, argumentName):
	return argumentName in EnumDictionary[enumName]['arguments'];
			
##################################################
# ValidateDistributedEnum
##################################################
# Validate all assignment, by making sure they point to a value that exist inside the enum
def ValidateDistributedEnum():
	global CurrentPath, CurrentLine;
	
	# Validate assignments
	for enumName in EnumDictionary:
		enum = EnumDictionary[enumName];
		assignmentDict = {};
		numericAssignment = False;
		
		# for assignmentValues in enum['values']:
		for assignmentValueIndex in range(len(enum['values'])):
			assignmentValues = enum['values'][assignmentValueIndex];
			CurrentPath = assignmentValues['srcPath']
			CurrentLine = assignmentValues['srcLine']			
			# Only assignments
			if 'assignment' in assignmentValues:
				valid = re.search(NumberValueRegEx, assignmentValues['assignment']) != None;
				
				# Skip search if self assignment
				if not valid and assignmentValues['assignment'] != assignmentValues['value']:
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
				else:
					if assignmentValues['assignment'] not in assignmentDict:
						assignmentDict[assignmentValues['assignment']] = {'generatedError': False, 'valueData': enum['values'][assignmentValueIndex]};
						# if assignement is to zero and we have a NULLABLE value
						if re.search(NumberValueRegEx, assignmentValues['assignment']) != None:
							if int(assignmentValues['assignment']) == 0 and DoesEnumHasArgument(enumName, 'NULLABLE'):
								PrintError(enumName, "is NULLABLE, but the value '" + assignmentValues['value'] + "' overrides the NULLABLE value. Remove the NULLABLE argument from the distributed enum definition or change the value of '" + assignmentValues['value'] + "' to something non-zero.");
							if DoesEnumHasArgument(enumName, 'COUNT'):
								PrintError(enumName, "has the COUNT argument, but the value '" + assignmentValues['value'] + "' sets a numerical value, which make COUNT useless. Remove the numerical assignment or remove the COUNT argument in the distributed enum definition");
							
					elif DoesEnumHasArgument(enumName, 'UNIQUE'):
						if not assignmentDict[assignmentValues['assignment']]['generatedError']:
							originalValue = assignmentDict[assignmentValues['assignment']]['valueData'];
							PrintError(enumName, "has an assignment on value '" + assignmentDict[assignmentValues['assignment']]['valueData']['value'] + "' that is a duplicate of another one. Assignments are to be unique for this enum since it has the argument 'UNIQUE'.", originalValue['srcPath'], originalValue['srcLine']);
							assignmentDict[assignmentValues['assignment']]['generatedError'] = True;						
						PrintError(enumName, "has an assignment on value '" + assignmentValues['value'] + "' that is a duplicate of another one. Assignments are to be unique for this enum since it has the argument 'UNIQUE'.");						
				
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
def AddValueToDistributedEnum(enumName, value, source, srcPath = None, srcLine = None):
	valueTuple = BuildValueAssignmentTupleFromString(enumName, value);
	valueDict = {'value': valueTuple[0], 'source': source};
	if srcPath != None and srcLine != None:
		valueDict['srcPath'] = srcPath;
		valueDict['srcLine'] = srcLine;
	else:
		valueDict['srcPath'] = EnumDictionary[enumName]['code']['srcPath'];
		valueDict['srcLine'] = EnumDictionary[enumName]['code']['srcLine'];
		
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
	global CurrentPath, CurrentLine;
	
	# Parse the file
	file = fileinput.FileInput(filePath);
	for line in file:
		matches = re.findall(DistributedEnumValueRegEx, line);
		if len(matches) > 0: # A distributed enum has been found
			CurrentPath = filePath;
			CurrentLine = file.lineno();				
			matches = matches[0];
			# We have a definition?
			if matches[0] in EnumDictionary:
				AddValueToDistributedEnum(matches[0], matches[1], "From file: '" + filePath[len(CodeSourcePath):] + "'", CurrentPath, CurrentLine);
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
	
	attributesEqualityList = ['value', 'assignment', 'source'];
	
	lhsDistributedEnumValuesList = lhsDistributedEnumValuesList[:]
	rhsDistributedEnumValuesList = rhsDistributedEnumValuesList[:]
	
	while lhsIndexValue < len(lhsDistributedEnumValuesList):
		rhsIndexValue = 0;
		while rhsIndexValue < len(rhsDistributedEnumValuesList):
			equals = True;
			for attribute in attributesEqualityList:
				# Does both have the attributes or have it missing?
				lhsHasAttribute = attribute in lhsDistributedEnumValuesList[lhsIndexValue];
				rhsHasAttribute = attribute in rhsDistributedEnumValuesList[rhsIndexValue];
				equals = equals and lhsHasAttribute == rhsHasAttribute
				# If both have the attribute, check if it has the same value
				if lhsHasAttribute and rhsHasAttribute:
					equals = equals and lhsDistributedEnumValuesList[lhsIndexValue][attribute] == rhsDistributedEnumValuesList[rhsIndexValue][attribute]
			if equals:
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
	global CurrentPath, CurrentLine;
	
	sortedDict = {}; # All sorted values will be include in this dictionnary
	sortedList = []; # This is the list all sorted out
	leftToSortList = []; # This is the list of all unsorted values
	lastLeftToSortLength = 0; # Uses to find unsortable enums
	
	enum = EnumDictionary[enumName];
	
	# build leftToSortList
	for valueIndex in range(len(enum['values'])):
		# If we have no assignment, then already sorted!
		if 'assignment' not in enum['values'][valueIndex] or re.search(NumberValueRegEx, enum['values'][valueIndex]['assignment']) != None:
			sortedList.append(valueIndex);
			# Include value in sorted dictionnary
			sortedDict[enum['values'][valueIndex]['value']] = valueIndex;
		else:
			leftToSortList.append(valueIndex);
	
	assignmentSortedList	= [];
	index = 0;
	while index < len(sortedList):
		valueToSort = enum['values'][sortedList[index]];
		if 'assignment' in valueToSort and re.search(NumberValueRegEx, valueToSort['assignment']) != None:
			sortedIndex = 0;
			while sortedIndex < len(assignmentSortedList):
				sortedValue = enum['values'][assignmentSortedList[sortedIndex]];
				if int(valueToSort['assignment']) <= int(sortedValue['assignment']):
					break;
				sortedIndex += 1;
			assignmentSortedList.insert(sortedIndex, sortedList[index]);
			sortedList.pop(index);
			index -= 1;
		index += 1;
		
	assignmentSortedList.extend(sortedList);
	sortedList = assignmentSortedList;

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
		for index in leftToSortList:
			CurrentPath = enum['values'][index]['srcPath'];
			CurrentLine = enum['values'][index]['srcLine'];
			PrintWarning(enumName, "has unsolvable dependency between values on '" + enum['values'][index]['value'] + "'. Unsolvable dependencies were ignored.");
			
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
	indentStep = " " * SolutionConstants.NUMBER_OF_SPACES_FOR_TABS;
	
	enum = EnumDictionary[enumName];
	distributedEnumString = baseIndent + "{\n";
	
	# For all sorted values index
	for valueIndex in SortEnumValues(enumName):
		distributedEnumString += baseIndent + indentStep + enum['values'][valueIndex]['value'];
		if 'assignment' in enum['values'][valueIndex]:
			distributedEnumString += " = " + enum['values'][valueIndex]['assignment'];
		distributedEnumString += ", // " + enum['values'][valueIndex]['source'];
		distributedEnumString += "\n";
	distributedEnumString += baseIndent + "}\n";	
	distributedEnumString += baseIndent + DistributedEnumFooterMacro + "\n";	
	
	return distributedEnumString;

##################################################
# EnumContentLineCount
##################################################
# Counts the number of line into the enum definition
# RETURNS:
#  The number of line, that should be part of the enum. If the syntax if messed up, 
#    it returns the safer bet and prints a warning
# ARGUMENTS:
#  filePath: The path of the source code file
#  enumName: The name of the distributed enum to count the number of line
def EnumContentLineCount(filePath, enumName):
	lineCount = 0;
	startBlockValid = False;
	endBlockValid = False;
	endMacroValid = False;
	trailingPossibleUnrelatedLineCount = 0;
	
	specificDistributedEnumDefinitionFullSyntaxRegEx = re.compile("^\s*" + DistributedEnumHeaderMacro + "\(\s*(" + enumName + "(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")	
	
	file = fileinput.FileInput(filePath);
	# Search for the specific enum
	for line in file:
		matches = re.findall(specificDistributedEnumDefinitionFullSyntaxRegEx, line);
		# The specific enum has been found
		if len(matches) > 0:				
			matches = matches[0];
			
			line = file.readline();			
			
			validEnumContent = True;
			while validEnumContent:
				validEnumContent = False;
				# We need a "{" to start the content (and only one)
				if not startBlockValid and re.search(BlockStartRegEx, line) != None:
					validEnumContent = startBlockValid = True;
				
				# Enum value need to be in a block starting with "{", that hasn't closed 
				if startBlockValid and not endBlockValid and re.search(EnumValueRegEx, line) != None:
					validEnumContent = True;
				
				# End block value needs to be after a "{" and before the end macro
				if startBlockValid and not endBlockValid and re.search(BlockEndRegEx, line) != None:
					validEnumContent = endBlockValid = True;
				
				# End macro needs an end block and can only be these once
				if endBlockValid and not endMacroValid and re.search(DistributedEnumDefinitionEndRegEx, line) != None:
					endMacroValid = validEnumContent = True;
				
				# if nothing was valid, but we have an unrelated line
				mightBeUnrelatedLine = re.search(EmptyLineRegEx, line) != None or re.search(CommentBlockRegEx, line) != None;
				if not validEnumContent and mightBeUnrelatedLine:
					# Take note of the empty line we might have to discard these lines
					trailingPossibleUnrelatedLineCount += 1
					validEnumContent = True;
				elif validEnumContent:
					# Reset newline counter, these newlines were legit
					trailingPossibleUnrelatedLineCount = 0;
				if validEnumContent:
					lineCount += 1;
					line = file.readline();
					if endMacroValid:
						# End of content
						break;
			break;
			
	file.close();
	
	# Warning message if necessary
	if not startBlockValid or not endBlockValid or not endMacroValid:
		ErrorHandling.PrintWarning(CurrentPath, CurrentLine, "Distributed enum '" + enumName + "' has syntax error. Please verify the syntax. Compilation errors might occurs.");
	
	return lineCount - trailingPossibleUnrelatedLineCount;

##################################################
# InjectDistributedEnum
##################################################
# Look through files to find distributed enum definition and update them if necessary
# ARGUMENTS:
#  filePath: The path of the file to update distributed enum
def InjectDistributedEnum(filePath):
	global LineChangedCount;
	global CurrentPath, CurrentLine;
	
	# Look for existing enum with values
	currentFileDistributedEnumList = FindBuiltDistributedEnum(filePath);
	
	for enum in currentFileDistributedEnumList:
		lineToChangeCount = DistributedEnumValuesDifferenceCount(currentFileDistributedEnumList[enum], EnumDictionary[enum]['values']);
		# There are changes
		if lineToChangeCount > 0:
			LineChangedCount += lineToChangeCount;
			specificDistributedEnumDefinitionFullSyntaxRegEx = re.compile("^\s*" + DistributedEnumHeaderMacro + "\(\s*(" + enum + "(\s*\,\s*([a-zA-Z0-9\_\|]+?)|(VALUE\s*(|\s*([a-zA-Z0-9\_\|]+?\s*[=]\s*[a-zA-Z0-9\_\|]+?)+)?))*)\s*\)([;])?")
			enumCode = DistributedEnumToCodeString(enum);
			# is file writable?
			if (os.stat(filePath)[0] & stat.S_IWRITE): 
				file = fileinput.FileInput(filePath, inplace = True);
				# Search for the specific enum
				for line in file:
					matches = re.findall(specificDistributedEnumDefinitionFullSyntaxRegEx, line);
					# The specific enum has been found
					if len(matches) > 0:
						CurrentPath = filePath;
						CurrentLine = file.lineno();					
						matches = matches[0];
						
						# Remove trailing semi-colon from enum definition
						if matches[6] == ";":
							line = re.sub(";","", line);
							LineChangedCount += 1;
							
						# Print header without semi-colon
						print(line, end="");
						# Print values
						print(enumCode, end="");
						
						# Skip all content + header
						for i in range(EnumDictionary[enum]['code']['enumContentLineCount'] + 1):
							line = file.readline();
							
					# Print unrelated line of code to file
					print(line, end="");
			else: # File is read-only
				ErrorHandling.PrintError(filePath, 0, "Distributed enum definition '" + enum + "' was found in a file that is read-only. Enum values is outdated.");
		
##################################################
# BuildDistributedEnumDefinition
##################################################
# Look through all source files for distributed enum definition and keep these definition in memory
def BuildDistributedEnumDefinition():
	# Parse files to find enum definition
	SearchFolderAndApplyFunction(CodeSourcePath, FindDistributedEnumDefinition);
	
##################################################
# BuildDistributedEnumDefinition
##################################################
# Fill all the distributed enum definitions with values from arguments and source files
def BuildDistributedEnumValues():
	# Use arguments to start building distributed enum
	for enumName in EnumDictionary:
		EnumDictionary[enumName]['values'] = [];
		# fill Enum with default arguments values (NULLABLE and COUNT will be sorted later on to be first and last respectively)
		if DoesEnumHasArgument(enumName, 'NULLABLE'):
			AddValueToDistributedEnum(enumName, EnumDictionary[enumName]['arguments']['NULLABLE'][0], "Enum definition: NULLABLE");
		if DoesEnumHasArgument(enumName, 'VALUE'):
			for value in EnumDictionary[enumName]['arguments']['VALUE']:
				AddValueToDistributedEnum(enumName, value, "Enum definition: VALUE");
		if DoesEnumHasArgument(enumName, 'COUNT'):
			AddValueToDistributedEnum(enumName, EnumDictionary[enumName]['arguments']['COUNT'][0], "Enum definition: COUNT");				
	
	# Validate argument values
	ValidateDistributedEnum();
	
	# Parse files to build enums
	SearchFolderAndApplyFunction(CodeSourcePath, FillDistributedEnum);
	
	# Validate source values
	ValidateDistributedEnum();
	
	for enumName in EnumDictionary:
		if DoesEnumHasArgument(enumName, 'COUNT'):
			for valueIndex  in range(len(EnumDictionary[enumName]['values'])):
				value = EnumDictionary[enumName]['values'][valueIndex];
				if value['value'] == EnumDictionary[enumName]['arguments']['COUNT'][0]:
					EnumDictionary[enumName]['values'][valueIndex]['assignment'] = str(len(EnumDictionary[enumName]['values']) - 1);
					break;

##################################################
# InjectDistributedEnums
##################################################
# Inject into the source files the update values if necessary
def InjectDistributedEnums():
	# Parse files to build enums
	SearchFolderAndApplyFunction(CodeSourcePath, InjectDistributedEnum);	
	
#############################################
################## MAIN #####################
#############################################
BuildDistributedEnumDefinition();
BuildDistributedEnumValues();
InjectDistributedEnums();
	
# Generate Output Text
print("Distributed Enum Ouput: ", end = "");
if LineChangedCount > 0:
	print(str(LineChangedCount) + " line(s) changed");
else:
	print("No line changed");
