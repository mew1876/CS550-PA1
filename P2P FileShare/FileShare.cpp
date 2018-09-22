#include "Peer.h"
#include "IndexServer.h"
#include <iostream>

int main(int argc, char* argv[]) {
	IndexServer server;
	Peer peer1;
	Peer peer2;
	Peer peer3;

	std::ofstream file("Peers/Peer 1/test.txt");
	file << "Sample text";
	file.close();

	peer1.add("test.txt");
	
	std::cout << "Retrieve status: " << peer2.retrieve("test.txt") << std::endl;
}