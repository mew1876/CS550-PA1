#include "Peer.h"
#include "IndexServer.h"
#include <iostream>

int currentFileID = 1;

void createFile(int peerID, int fileSizeInKB);

int main(int argc, char* argv[]) {
	IndexServer server;
	Peer peer1;
	Peer peer2;

	std::ofstream file("Peers/Peer 1/test.txt");
	file << "Sample text";
	file.close();

	peer1.add("test.txt");
	
	peer2.retrieve("test.txt");

	createFile(1, 2);
}

void createFile(int peerID, int fileSizeInKB) {
	std::ofstream file("Peers/Peer " + std::to_string(peerID) + "/File " + std::to_string(currentFileID++) + ".txt");
	std::srand(std::time(nullptr));
	for (int KB = 0; KB < fileSizeInKB; KB++) {
		// write 1KB of random readable character to the file (ASCII: 32 to 126)
		for (int i = 0; i < 1024; i++) { 
			file << char((std::rand() % 95) + 32);
		}
	}
	file.close();
}