#pragma once
#include "DirectoryWatcher.h"
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
	DirectoryWatcher *pointerToDirectoryWatcher;
public:
	static bool AUTO_PEER_SELECTION;
	Peer();
	~Peer();
	void add(std::string fileName);
	void add(std::vector<std::string> fileName);
	bool retrieve(std::string fileName);
};

