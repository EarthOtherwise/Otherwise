#include "PathMapTranslator.h"

#include "ErrorHandle.h"

#include <fstream>

namespace Otherwise
{

	PathMapTranslator::PathMapTranslator()
	{
	}


	PathMapTranslator::~PathMapTranslator()
	{
	}

	std::vector<Node> Otherwise::PathMapTranslator::zGameMapToNode(const std::string & fileName)
	{
		std::vector<Node> pathMap;
		std::vector<std::string> _levelData;

		std::ifstream file;
		file.open(fileName);

		// Error checking
		if (file.fail()) {
			Otherwise::fatalError("Failed to open " + fileName);
		}

		// Throw away the first string in tmp
		std::string tmp;

		std::getline(file, tmp); // Throw away the rest of the first line

								 // Read the level data
		while (std::getline(file, tmp)) {
			_levelData.emplace_back(tmp);
		}


		// Render all the tiles
		for (unsigned int y = 0; y < _levelData.size(); y++) {
			for (unsigned int x = 0; x < _levelData[y].size(); x++) {
				// Grab the tile
				char tile = _levelData[y][x];

				Node newNode;

				// Process the tile
				switch (tile) {
				case '@':
					newNode.mPosition.push_back(x);
					newNode.mPosition.push_back(y);

					pathMap.push_back(newNode);
					break;
				case 'Z':
					newNode.mPosition.push_back(x);
					newNode.mPosition.push_back(y);

					pathMap.push_back(newNode);
					break;
				case '.':
					newNode.mPosition.push_back(x);
					newNode.mPosition.push_back(y);

					pathMap.push_back(newNode);
					break;
				default:
					break;
				}
			}
		}

		for (unsigned int i = 0; i < pathMap.size(); i++)
		{
			for (unsigned int j = 0; j < pathMap.size(); j++)
			{
				//std::cout << "Checking node " << levelGraph[i].mPosition[0] << ", " << levelGraph[i].mPosition[1] << std::endl;
				//std::cout << "against node " << levelGraph[j].mPosition[0] << ", " << levelGraph[j].mPosition[1] << std::endl;
				if (fabs(pathMap[i].mPosition[0] - pathMap[j].mPosition[0]) == 1 && fabs(pathMap[i].mPosition[1] - pathMap[j].mPosition[1]) == 0)
				{
					//std::cout << "Connection horizontally" << std::endl;
					Connection newConnection;
					newConnection.toNode = &pathMap[j];
					newConnection.weight = 1.0f;
					pathMap[i].mConnections.push_back(newConnection);
				}
				else if (fabs(pathMap[i].mPosition[1] - pathMap[j].mPosition[1]) == 1 && fabs(pathMap[i].mPosition[0] - pathMap[j].mPosition[0]) == 0)
				{
					//std::cout << "Connection vertically" << std::endl;
					Connection newConnection;
					newConnection.toNode = &pathMap[j];
					newConnection.weight = 1.0f;
					pathMap[i].mConnections.push_back(newConnection);
				}
				/*else if (fabs(levelGraph[i].mPosition[0] - levelGraph[j].mPosition[0]) == 1 && fabs(levelGraph[i].mPosition[1] - levelGraph[j].mPosition[1]) == 1)
				{
				Connection newConnection;
				newConnection.toNode = &levelGraph[j];
				newConnection.weight = 1.414f;
				levelGraph[i].mConnections.push_back(newConnection);
				}*/
			}
		}


		return pathMap;
	}

}