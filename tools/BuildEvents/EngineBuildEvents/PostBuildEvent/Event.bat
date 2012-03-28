echo Custom Post-Build Event Start

set SolutionPath=%~1
set ProjectPath=%~2

set SharedBuildEventsPath=%SolutionPath%..\Tools\BuildEvents\SharedEvents\
set DocumentationOutputPath=%SolutionPath%..\Documentation\GameEngine\
SET DocumentationBatchPath=%SharedBuildEventsPath%Documentation\GenerateDocumentation.bat

CALL %DocumentationBatchPath% %ProjectPath% %DocumentationOutputPath% "Prestige Game Engine" development

echo Custom Post-Build Event End