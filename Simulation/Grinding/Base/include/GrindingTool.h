#ifndef GRINDING_TOOL_H
#define GRINDING_TOOL_H
#include <vector>

namespace Grinding
{
enum GrindingTool
{
	None,
	Sphere
};

const std::vector<const char*> GrindingToolStr = {
	"None", "Sphere"
};
}


#endif // GRINDING_TOOL_H