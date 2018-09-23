// modified code from Microsoft documentation about 
// Obtaining Directory Change Notifications

#include "DirectoryWatcher.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <vector>

DirectoryWatcher::DirectoryWatcher(std::string directoryString)
	: indexClient("localhost", 8000) {
	LPTSTR directory = LPTSTR(directoryString.c_str());
	WatchDirectory(directory);
}

std::vector<std::filesystem::path> DirectoryWatcher::getFilesInDirectory(std::string path)
{
	std::cout << path << std::endl;
	std::vector<std::filesystem::path> returnVector = {};

	if (!(std::filesystem::is_directory(path))) {
		std::cout << "printFilesInDirectory: The given path was not a valid directory";
		return {};
	}
	else {
		for (auto &p : std::filesystem::directory_iterator(path))
			// only print if not a directory / is a file
			if (!(std::filesystem::is_directory(p.path()))) {
				returnVector.push_back(p.path());
			}
	}
	return returnVector;
}

void DirectoryWatcher::WatchDirectory(LPTSTR lpDir)
{
	DWORD dwWaitStatus;
	HANDLE dwChangeHandle;
	TCHAR lpDrive[4];
	TCHAR lpFile[_MAX_FNAME];
	TCHAR lpExt[_MAX_EXT];

	_tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

	lpDrive[2] = (TCHAR)'\\';
	lpDrive[3] = (TCHAR)'\0';

	// Watch the directory for file creation and deletion. 

	dwChangeHandle = FindFirstChangeNotification(
		lpDir,                         // directory to watch 
		FALSE,                         // do not watch subtree 
		FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

	if (dwChangeHandle == INVALID_HANDLE_VALUE)
	{
		printf("\n ERROR: FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError());
	}

	
	// Make a final validation check on our handle.

	if ((dwChangeHandle == NULL))
	{
		printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
		ExitProcess(GetLastError());
	}

	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 

	while (TRUE)
	{
		// Wait for notification.

		printf("\nWaiting for notification...\n");

		dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// A file was created, renamed, or deleted in the directory.
			// Refresh this directory and restart the notification.

			RefreshDirectory(lpDir);
			if (FindNextChangeNotification(dwChangeHandle) == FALSE)
			{
				printf("\n ERROR: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			printf("\nNo changes in the timeout period.\n");
			break;

		default:
			printf("\n ERROR: Unhandled dwWaitStatus.\n");
			ExitProcess(GetLastError());
			break;
		}
	}
}

void DirectoryWatcher::RefreshDirectory(LPTSTR lpDir)
{
	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	_tprintf(TEXT("Directory (%s) changed.\n"), lpDir);
}


DirectoryWatcher::~DirectoryWatcher()
{
}
