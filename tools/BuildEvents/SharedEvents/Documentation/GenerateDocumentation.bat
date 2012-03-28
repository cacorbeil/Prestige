@ECHO OFF

SET ProjectDirectory=%~1
SET ProjectDocumentationOutputDirectory=%~2
SET ProjectName=%~3
SET ProjectVersion=%~4

SET DocumentGeneratorDirectory=%ProjectDirectory%..\..\..\thirdparty\doxygen\
SET DocumentGeneratorExecutable=doxygen.exe
SET DocumentationGenerationSettingsFilePath=%DocumentGeneratorDirectory%CommonSettings

MKDIR %ProjectDocumentationOutputDirectory%
SET Command=( type %DocumentationGenerationSettingsFilePath% ^
				^& echo INPUT=%ProjectDirectory% ^
				^& echo PROJECT_NUMBER=%ProjectVersion% ^
				^& echo PROJECT_NAME=%ProjectName% ^
				^& echo OUTPUT_DIRECTORY=%ProjectDocumentationOutputDirectory% ^
				) ^
			^| "%DocumentGeneratorDirectory%%DocumentGeneratorExecutable%" -

ECHO Generating Documentation
%Command%
