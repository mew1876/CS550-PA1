#include "IndexServer.h"
#include <vector>

IndexServer::IndexServer()
: indexServer("localhost", 8000) {
	indexServer.bind("search", [this](std::string fileName) { return this->search(fileName); });
	indexServer.bind("remove", [this](int peerID, std::string fileName) { return this->remove(peerID, fileName); });
	indexServer.bind("add", [this](int peerID, std::string fileName) { return this->add(peerID, fileName); });
	indexServer.bind("addVector", [this](int peerID, std::vector<std::string> fileNames) { return this->add(peerID, fileNames); });
	indexServer.async_run(4);
	fileNameToPeerIDIndex = {};
}

std::vector<int> IndexServer::search(std::string fileName) {
	auto index = fileNameToPeerIDIndex.find(fileName);
	if (index != fileNameToPeerIDIndex.end()) {
		return index->second; // vector of PeerID variables
	}
	else {
		return {};
	}
}

void IndexServer::remove(int peerID, std::string fileName) {
	auto mapIndex = fileNameToPeerIDIndex.find(fileName);
	// check that fileName is in the index
	if (mapIndex != fileNameToPeerIDIndex.end()) {
		std::vector<int> peerIDVector = mapIndex->second;
		// loop through all peerIDs that have fileName
		for (std::vector<int>::iterator peerIDIndex = peerIDVector.begin(); peerIDIndex != peerIDVector.end();) {
			if (*peerIDIndex == peerID) {
				// erase the vector entry for the peerID
				peerIDVector.erase(peerIDIndex);
			}
		}
		// if the file doesn't exist in any peers anymore, delete the file's map entry
		if (peerIDVector.empty()) {
			fileNameToPeerIDIndex.erase(mapIndex);
		}
	}
}

void IndexServer::add(int peerID, std::string fileName) {
	auto mapIndex = fileNameToPeerIDIndex.find(fileName);
	// if the fileName is not in the index, make a new entry
	if (mapIndex != fileNameToPeerIDIndex.end()) {
		fileNameToPeerIDIndex.insert({ fileName, {peerID} });
	}
	// otherwise add the new peerID to the fileName entry
	else {
		auto peerIDVector = mapIndex->second;
		peerIDVector.push_back(peerID);
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
