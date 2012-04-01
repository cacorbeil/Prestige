echo Custom Post-Build Event Start

set SolutionPath=%~1
set ProjectPath=%~2

set SharedBuildEventsPath=%SolutionPath%..\Tools\BuildEvents\SharedEvents\
set DocumentationOutputPath=%SolutionPath%..\Documentation\Generated\Game\
SET DocumentationBatchPath=%SharedBuildEventsPath%Documentation\GenerateDocumentation.bat

REM Make Profiler dir
MKDIR %SolutionPath%ProfilerData\
DEL %SolutionPath%ProfilerData\*.xml

REM CALL %DocumentationBatchPath% %ProjectPath% %DocumentationOutputPath% "Game Template" development

echo Custom Post-Build Event End