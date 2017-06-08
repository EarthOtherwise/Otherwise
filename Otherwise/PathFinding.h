#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Otherwise
{

	enum class NodeState
	{
		OPEN,
		CLOSED,
		BLOCKED,
		UNVISITED
	};

	struct Node;

	struct Connection
	{
		Node* toNode;
		float weight;
	};

	struct Node
	{
		std::vector<float> mPosition;
		std::vector<Connection> mConnections;
	};

	struct NodeRecord
	{
		Node* node;
		unsigned int parentNodeRecordNum;
		float costSoFar;
	};

	class Dijkstra
	{
	public:
		Dijkstra();
		~Dijkstra();

		std::vector<glm::vec2> findPath(std::vector<Node> nodeGraph, Node startNode, Node endNode);

	};

	class ASharp
	{
	public:
		ASharp();
		~ASharp();

		void findPath(std::vector<Node> nodeGraph, Node startNode, Node endNode, float heuristic);
	};

}