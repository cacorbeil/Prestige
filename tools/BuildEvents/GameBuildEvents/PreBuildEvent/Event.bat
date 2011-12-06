echo Custom Pre-Build Event Start

set SolutionPath=%~1
set PreBuildToolsPath=%SolutionPath%..\Tools\EngineBuildEvent\PreBuildEvent\
set PythonExecutablePath=%SolutionPath%..\ThirdParty\Python\python.exe

set IncrementatorPath="%PreBuildToolsPath%\Incrementator.py"

%PythonExecutablePath% %IncrementatorPath% "%SolutionPath%GameEngine\\" "%PreBuildToolsPath%..\Output\\"

echo Custom Pre-Build Event End