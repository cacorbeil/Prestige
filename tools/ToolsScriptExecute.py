import os;
import sys;
import traceback, os.path

SolutionPath = sys.argv[1];
ExecutePath = sys.argv[2];

# Append All Constants Files 
sys.path.append(SolutionPath + "\\..\\Tools\\Constants\\")
sys.path.append(SolutionPath + "\\..\\Tools\\Utils\\")

# Shift arguments so that they only apply to the callee
sys.argv = sys.argv[2:];
try:
   exec(open(ExecutePath).read());
except Exception as errorDetail:
   exc_type, exc_value, exc_traceback = sys.exc_info()
   info = traceback.extract_tb(exc_traceback)[-1];
   lineNo = str(info[1]);
   functionName = info[2];
   print(ExecutePath + "(" + lineNo + "): Error: Exception of type '" + str(errorDetail) + "' has been raised in function '" + functionName + "' in tools script");

