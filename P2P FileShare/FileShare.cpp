#include "Peer.h"

int main(int argc, char* argv[]) {
	Peer peer1;
	Peer peer2;

	CreateDirectory("Peers", NULL);
	CreateDirectory("Peers/Peer 1", NULL);
	CreateDirectory("Peers/Peer 2", NULL);
	std::ofstream file("Peers/Peer 1/test.txt");
	file << "Sample text";
	file.close();
	
	peer2.retrieve("test.txt");
}