echo Custom Post-Build Event Start

set SolutionPath=%~1
set ProjectPath=%~2

set SharedBuildEventsPath=%SolutionPath%..\Tools\BuildEvents\SharedEvents\
set DocumentationOutputPath=%SolutionPath%..\Documentation\Game\
SET DocumentationBatchPath=%SharedBuildEventsPath%Documentation\GenerateDocumentation.bat

CALL %DocumentationBatchPath% %ProjectPath% %DocumentationOutputPath% "Game Template" development

echo Custom Post-Build Event End