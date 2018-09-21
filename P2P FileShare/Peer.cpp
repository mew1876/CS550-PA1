#include "Peer.h"
#include <vector>
#include <iostream>

int Peer::currentPeerID = 1;

Peer::Peer()
	: indexClient("localhost", 8000), server(8000 + currentPeerID) {
	peerID = currentPeerID++;
	server.bind("getFile", [this](std::string filename) { return this->getFile(filename); });
	server.async_run(8);
}

void Peer::retrieve(std::string filename) {
	//Get a vector of peer IDs from central server, let user pick an ID to copy from
	//std::vector<int> sources = indexClient.call("search", filename).as<std::vector<int>>();
	std::vector<int> sources{ 1 };
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
		std::vector<uint8_t> source = peerClient.call("getFile", filename).as<std::vector<uint8_t>>();
		std::ofstream destination(getPath() + filename, std::ios::binary);
		destination.write((char *)source.data(), source.size());
	}
}

std::vector<uint8_t> Peer::getFile(std::string filename) {
	//Read file into byte vector, return vector
	std::ifstream file(getPath() + filename, std::ios::binary);
	file.unsetf(std::ios::skipws);

	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> bytes;
	bytes.reserve((unsigned int) fileSize);
	bytes.insert(bytes.begin(),
		std::istream_iterator<uint8_t>(file),
		std::istream_iterator<uint8_t>());
	return bytes;
}

std::string Peer::getPath() {
	return "Peers/Peer " + std::to_string(peerID) + "/";
}

Peer::~Peer() {
}
