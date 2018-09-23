#pragma once
#include "rpc/client.h"
#include <filesystem>

class DirectoryWatcher
{
	std::vector<std::filesystem::path> getFilesInDirectory(std::string path);
	void RefreshDirectory(LPTSTR);
	void WatchDirectory(LPTSTR);
	rpc::client indexClient;
public:
	DirectoryWatcher(std::string directoryString);
	~DirectoryWatcher();
};