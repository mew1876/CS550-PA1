#pragma once
#include <fstream>
#include "rpc/client.h"
#include "rpc/server.h"

class Peer {
	static int currentPeerID;
	int peerID;
	rpc::client indexClient;
	rpc::server server;
	std::string getPath();
public:
	Peer();
	~Peer();
	void retrieve(std::string filename);
	std::ifstream getFile(std::string filename);
};

