#include "IndexServer.h"
#include <vector>

IndexServer::IndexServer()
{
	//std::unordered_map<std::string, std::vector<int>> fileNameToPeerIDIndex
	fileNameToPeerIDIndex = {};
}

std::vector<int> IndexServer::search(std::string fileName) {
	auto index = fileNameToPeerIDIndex.find(fileName);
	if (index != fileNameToPeerIDIndex.end()) {
		return index->second; // vector of PeerID variables
	}
	else
	{
		return {};
	}
}
void IndexServer::remove(int peerID, std::string fileName) {
	auto mapIndex = fileNameToPeerIDIndex.find(fileName);
	if (mapIndex != fileNameToPeerIDIndex.end()) {
		std::vector<int> peerIDVector = mapIndex->second;
		for (std::vector<int>::iterator peerIDIndex = peerIDVector.begin(); peerIDIndex != peerIDVector.end();) {
			if (*peerIDIndex == peerID) {
				fileNameToPeerIDIndex.erase(mapIndex);
			}
		}
		if (peerIDVector.empty()) {
			fileNameToPeerIDIndex.erase(mapIndex);
		}
	}
}

void IndexServer::add(int peerID, std::string fileName) {
	auto mapIndex = fileNameToPeerIDIndex.find(fileName);
	if (mapIndex != fileNameToPeerIDIndex.end()) {
		fileNameToPeerIDIndex.insert({ fileName, {peerID} });
	}
	else
	{
		auto peerIDVector = mapIndex->second;
		for (auto peerIDIndex = peerIDVector.begin(); peerIDIndex != peerIDVector.end();) {
			if (*peerIDIndex == peerID) {
				fileNameToPeerIDIndex.erase(mapIndex);
			}
		}
		if (peerIDVector.empty()) {
			fileNameToPeerIDIndex.erase(mapIndex);
		}
	}
}
void IndexServer::add(int peerID, std::vector<std::string> fileNames) {
	for (std::string filename : fileNames) {
		add(peerID, filename);
	}
}


IndexServer::~IndexServer()
{
}
