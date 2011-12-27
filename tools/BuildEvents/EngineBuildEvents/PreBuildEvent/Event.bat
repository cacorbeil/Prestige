echo Custom Pre-Build Event Start

set SolutionPath=%~1
set PreBuildToolsPath=%SolutionPath%..\Tools\BuildEvents\EngineBuildEvents\PreBuildEvent\
set PythonExecutablePath="%SolutionPath%..\ThirdParty\Python\python.exe" "%SolutionPath%..\Tools\ToolsScriptExecute.py" %SolutionPath%

set IncrementatorPath="%PreBuildToolsPath%\Incrementator.py"
set DistributedEnumPath="%PreBuildToolsPath%\DistributedEnum.py"

%PythonExecutablePath% %IncrementatorPath% "%SolutionPath%Projects\GameEngine\\" "%PreBuildToolsPath%..\Output\\"
%PythonExecutablePath% %DistributedEnumPath% "%SolutionPath%Projects\GameEngine\\" "%PreBuildToolsPath%..\Output\\"

echo Custom Pre-Build Event End