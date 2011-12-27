##################################################
# PrintWarning
##################################################
# Common function to output warning message in a Visual Studio kind of way
# ARGUMENTS:
#  filePath: The path of the file that generated the warning
#  lineNo: The number of the line in the specified file that generated the warning
#  message: The actual content of the warning message that will be shown
def PrintWarning(filePath, lineNo, message):
	_PrintOutput(filePath, lineNo, "warning", message);

##################################################
# PrintError
##################################################
# Common function to output error message in a Visual Studio kind of way
# ARGUMENTS:
#  filePath: The path of the file that generated the error
#  lineNo: The number of the line in the specified file that generated the error
#  message: The actual content of the error message that will be shown
def PrintError(filePath, lineNo, message):
	_PrintOutput(filePath, lineNo, "error", message);
	
##################################################
# _PrintOutput
##################################################
# Common function to format message in a Visual Studio kind of way
# ARGUMENTS:
#  filePath: The path of the file that generated the message
#  lineNo: The number of the line in the specified file that generated the message
#  type: The type of message. 'error' and 'warning' are caught, by VS and shown as such.
#  message: The actual content of the message that will be shown
def _PrintOutput(filePath, lineNo, type, message):
	print(filePath + "(" + str(lineNo) + "): " + type + ": " + message);