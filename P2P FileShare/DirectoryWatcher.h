#pragma once
#include "rpc/client.h"
#include <filesystem>

class DirectoryWatcher
{
	rpc::client indexClient;
	LPTSTR directory;
	int peerID;
	std::vector<std::filesystem::path> originalFileListing; 
	std::vector<std::filesystem::path> getFilesInDirectory(std::string path);
	void RefreshDirectory(LPTSTR);
	void WatchDirectory(LPTSTR);
public:
	DirectoryWatcher(std::string directoryString, int creatorPeerID);
	~DirectoryWatcher();
};