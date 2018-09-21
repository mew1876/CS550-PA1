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
	std::vector<uint8_t> getFile(std::string fileName);
	std::string getPath();
public:
	Peer();
	~Peer();
	void add(std::string fileName);
	void add(std::vector<std::string> fileName);
	void retrieve(std::string fileName);
};

