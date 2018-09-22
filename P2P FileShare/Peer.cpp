#include "Peer.h"
#include "rpc/this_handler.h"
#include <vector>
#include <iostream>
#include <algorithm>

int Peer::currentPeerID = 1;
bool Peer::AUTO_PEER_SELECTION = true;

Peer::Peer()
	: indexClient("localhost", 8000), server(8000 + currentPeerID) {
	peerID = currentPeerID++;
	CreateDirectory("Peers", NULL);
	CreateDirectory(getPath().c_str(), NULL);
	server.bind("getFile", [this](std::string fileName) { return this->getFile(fileName); });
	server.async_run(1);
}

void Peer::add(std::string fileName) {
	indexClient.call("add", peerID, fileName);
}

void Peer::add(std::vector<std::string> fileNames) {
	indexClient.call("addVector", peerID, fileNames);
}

bool Peer::retrieve(std::string fileName) {
	//Get a vector of peer IDs from central server, let user pick an ID to copy from
	//If AUTO_PEER_SELECTION == true then always choose last option
	std::vector<int> sources = indexClient.call("search", fileName).as<std::vector<int>>();
	while (sources.size() > 0) {
		int sourceID;
		if (AUTO_PEER_SELECTION) {
			sourceID = sources.back();
		}
		else {
			do {
				std::cout << "Select a valid peer ID: ";
				for (auto sourceID : sources) {
					std::cout << sourceID << " ";
				}
				std::cout << std::endl << ">";
				std::cin >> sourceID;
			} while (std::find(sources.begin(), sources.end(), sourceID) == sources.end());
		}
		rpc::client peerClient("localhost", 8000 + sourceID);
		try {
			std::vector<uint8_t> bytes = peerClient.call("getFile", fileName).as<std::vector<uint8_t>>();
			std::ofstream destination(getPath() + fileName, std::ios::binary);
			destination.write((char *)bytes.data(), bytes.size());
			return true;
		}
		catch (...) {
			std::cout << "Peer failed to provide file" << std::endl;
			if (AUTO_PEER_SELECTION) {
				sources.pop_back();
			}
			else {
				sources.erase(std::find(sources.begin(), sources.end(), sourceID));
			}
		}		
	}
	return false;
}

std::vector<uint8_t> Peer::getFile(std::string fileName) {
	//Read file into byte vector, return vector
	try {
		std::ifstream file(getPath() + fileName, std::ios::binary);
		file.unsetf(std::ios::skipws);

		std::streampos fileSize;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> bytes;
		bytes.reserve((unsigned int)fileSize);
		bytes.insert(bytes.begin(),
			std::istream_iterator<uint8_t>(file),
			std::istream_iterator<uint8_t>());
		return bytes;
	}
	catch (...) {
		rpc::this_handler().respond_error("Error reading file");
		return {};
	}	
}

std::string Peer::getPath() {
	return "Peers/Peer " + std::to_string(peerID) + "/";
}

Peer::~Peer() {
}
