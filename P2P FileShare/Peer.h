#pragma once
#include "rpc/client.h"
#include "rpc/server.h"
#include <fstream>
#include <vector>

class Peer {
	static int currentPeerID;
	int peerID;
	rpc::client indexClient;
	rpc::server server;
	std::vector<uint8_t> getFile(std::string filename);
	std::string getPath();
public:
	Peer();
	~Peer();
	void retrieve(std::string filename);
};

