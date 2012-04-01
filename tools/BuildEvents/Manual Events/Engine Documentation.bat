@ECHO OFF

SET ProjectDirectory=..\..\..\src\Projects\GameEngine
SET ProjectDocumentationOutputDirectory=..\..\..\documentation\Generated\Engine
SET ProjectName=Prestige Game Engine
SET ProjectVersion=0.0

SET DocumentGeneratorDirectory=..\..\..\thirdparty\doxygen\
SET DocumentGeneratorExecutable=doxygen.exe
SET DocumentationGenerationSettingsFilePath=%DocumentGeneratorDirectory%CommonSettings

SET Command=( type %DocumentationGenerationSettingsFilePath% ^
				^& echo INPUT=%ProjectDirectory% ^
				^& echo PROJECT_NUMBER=%ProjectVersion% ^
				^& echo PROJECT_NAME=%ProjectName% ^
				^& echo OUTPUT_DIRECTORY=%ProjectDocumentationOutputDirectory% ^
				) ^
			^| "%DocumentGeneratorDirectory%%DocumentGeneratorExecutable%" -

ECHO Generating Documentation
%Command%
