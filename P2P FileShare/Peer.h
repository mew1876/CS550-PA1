#pragma once
#include "rpc/client.h"
#include "rpc/server.h"
#include <fstream>
#include <vector>

class Peer {
	static int currentPeerID;
	rpc::client indexClient;
	rpc::server *server;
	std::vector<uint8_t> getFile(std::string fileName);
	std::string getPath();
public:
	static bool AUTO_PEER_SELECTION;
	int peerID;
	Peer();
	~Peer();
	void add(std::string fileName);
	void add(std::vector<std::string> fileName);
	bool retrieve(std::string fileName);
	void ping(int port);
};

