#include "Peer.h"
#include <vector>
#include <iostream>

int Peer::currentPeerID = 1;

Peer::Peer()
	: indexClient("localhost", 8000), server(8000 + currentPeerID) {
	peerID = currentPeerID++;	
	server.bind("getFile", &getFile);
}

void Peer::retrieve(std::string filename) {
	std::vector<int> sources = indexClient.call("search", filename).as<std::vector<int>>();
	if (sources.size() == 0) {
		std::cout << "No peers found with " << filename << std::endl;
	}
	else {
		std::cout << "Select a peer ID:" << std::endl;
		for (auto sourceID : sources) {
			std::cout << sourceID << " ";
		}
		std::cout << std::endl;
		int sourceID;
		std::cin >> sourceID;
	
		rpc::client peerClient("localhost", 8000 + sourceID);
		std::ifstream source = peerClient.call("getFile", filename).as<std::ifstream>();
		std::ofstream destination(getPath() + filename, std::ios::binary);
		destination << source.rdbuf();
	}
}

std::ifstream Peer::getFile(std::string filename) {
	std::ifstream stream(getPath() + filename, std::ios::binary);
	return stream;
}

std::string Peer::getPath() {
	return "Peers/Peer " + std::to_string(peerID) + "/";
}

Peer::~Peer() {
}
