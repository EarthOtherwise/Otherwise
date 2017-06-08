#include "PathFinding.h"
#include <iostream>

namespace Otherwise
{

	bool operator==(const Node& lhs, const Node& rhs)
	{
		if (lhs.mPosition == rhs.mPosition)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator==(const NodeRecord& lhs, const NodeRecord& rhs)
	{
		if (lhs.node == rhs.node)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!=(const Node& lhs, const Node& rhs)
	{
		if (lhs.mPosition != rhs.mPosition)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ASharp::ASharp()
	{
	}

	ASharp::~ASharp()
	{
	}

	void ASharp::findPath(std::vector<Node> nodeGraph, Node startNode, Node endNode, float heuristic)
	{
		/*



		*/
	}

	std::vector<glm::vec2> Dijkstra::findPath(std::vector<Node> nodeGraph, Node startNode, Node endNode)
	{
		//the record of the first node
		NodeRecord startRecord;

		//set the record of the first node to point at the first node
		startRecord.node = &startNode;

		//total cost at the beginning is zero
		startRecord.costSoFar = 0.0f;

		//the starting node doesn't have a parent node
		startRecord.parentNodeRecordNum = 0;

		//the list of nodes we haven't looked at
		std::vector<NodeRecord> openList;

		//put the record of the first node on the list
		openList.push_back(startRecord);

		//the record of the nodes we have looked at
		std::vector<NodeRecord> closedList;

		//so long as there is a node to process, process it.
		while (!openList.empty())
		{
			//a pointer to the node record with the smallest cost so far
			NodeRecord* smallestElement;

			//set the smallest element to a guaranteed spot on the open list so that it isn't null;
			smallestElement = &openList[0];

			unsigned int smallestElementNumber = 0;

			//find the smallest element in the open list
			for (unsigned int i = 0; i < openList.size(); i++)
			{
				if (openList[i].costSoFar < smallestElement->costSoFar)
				{
					smallestElement = &openList[i];
					smallestElementNumber = i;
				}
			}

			//put the current node in the closed list so that the nodes we create can point to it
			closedList.push_back(openList[smallestElementNumber]);

			//for each connection in this node
			for (unsigned int i = 0; i < smallestElement->node->mConnections.size(); i++)
			{
				//only check the connected node if it's not been checked or put on the open list
				bool isUnvisited = true;

				//check if the connection is on the closed list
				for (unsigned int k = 0; k < closedList.size(); k++)
				{
					if (*smallestElement->node->mConnections[i].toNode == *closedList[k].node)
					{
						isUnvisited = false;
						break;
					}
				}
				//check if the connection is on the open list
				for (unsigned int j = 0; j < openList.size(); j++)
				{
					if (*smallestElement->node->mConnections[i].toNode == *openList[j].node)
					{
						isUnvisited = false;
						break;
					}
				}
				//go to the next connection if this one is on either list
				if (isUnvisited = false)
				{
					continue;
				}

				//record for the new node pointed to by the connection in the current smallest node
				NodeRecord newNodeRecord;
				newNodeRecord.node = smallestElement->node->mConnections[i].toNode;
				newNodeRecord.parentNodeRecordNum = (closedList.size() - 1);
				newNodeRecord.costSoFar = smallestElement->costSoFar + smallestElement->node->mConnections[i].weight;
				openList.push_back(newNodeRecord);
			};

			//double check to see if the end node has been processed
			if (*closedList.back().node == endNode)
			{
				break;
			}

			openList.erase(openList.begin() + smallestElementNumber);
		}

		if (closedList.back().node->mPosition == endNode.mPosition)//if we are here, either there is no way to the end, or we found the end
		{
			NodeRecord* path;
			path = &closedList.back();
			//return the path
			while (path->node->mPosition[0] != startNode.mPosition[0])
			{
				std::cout << path->node->mPosition[0] << std::endl;
				path = &closedList[path->parentNodeRecordNum];
			}
		}
		else
		{
			std::cout << "There is no path" << std::endl;
		}

		std::vector<glm::vec2> standinVector;
		return standinVector;
	}

	Dijkstra::Dijkstra()
	{

	}

	Dijkstra::~Dijkstra()
	{

	}

}