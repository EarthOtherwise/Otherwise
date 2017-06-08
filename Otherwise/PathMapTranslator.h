#pragma once

#include "PathFinding.h"

namespace Otherwise
{
	class PathMapTranslator
	{
	public:
		PathMapTranslator();
		~PathMapTranslator();

		std::vector<Node> zGameMapToNode(const std::string& fileName);
	};

}