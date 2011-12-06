echo Custom Pre-Build Event Start

set SolutionPath=%~1
set PreBuildToolsPath=%SolutionPath%..\Tools\BuildEvents\EngineBuildEvents\PreBuildEvent\
set PythonExecutablePath=%SolutionPath%..\ThirdParty\Python\python.exe

set IncrementatorPath="%PreBuildToolsPath%\Incrementator.py"

%PythonExecutablePath% %IncrementatorPath% "%SolutionPath%Projects\GameEngine\\" "%PreBuildToolsPath%..\Output\\"

echo Custom Pre-Build Event End