#pragma once
#include "rpc/server.h"
#include <unordered_map>
#include <vector>

class IndexServer
{
	std::unordered_map<std::string, std::vector<int>> fileNameToPeerIDIndex;
	rpc::server indexServer;
	std::vector<int> search(std::string fileName);
	void remove(int peerID, std::string fileName);
	void add(int peerID, std::string fileName);
	void add(int peerID, std::vector<std::string> fileNames);
public:
	IndexServer();
	~IndexServer();
};

