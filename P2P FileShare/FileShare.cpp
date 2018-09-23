#include "Peer.h"
#include "IndexServer.h"
#include "DirectoryWatcher.h"
#include <iostream>
#include <mutex>
#include <condition_variable>

static int currentFileID = 1;

void testPeer();
void createFile(int peerID, int fileSizeInKB);

const int THREAD_COUNT = 10;
int threadCompletionCount = 0;
std::mutex print;
std::mutex count, condition;
std::condition_variable cv;

int main(int argc, char* argv[]) {
	IndexServer server;
	std::vector<std::thread*> threads;
	for (int i = 0; i < THREAD_COUNT; i++) {
		threads.push_back(new std::thread(testPeer));
	}
	for (int i = 0; i < THREAD_COUNT; i++) {
		threads[i]->join();
		delete threads[i];
	}
}

void testPeer() {
	Peer peer;
	int id = peer.peerID;
	createFile(id, id);
	peer.add("File " + std::to_string(id) + ".txt");
	if (id > 1) {
		bool success = false;
		while (!success) {
			success = peer.retrieve("File " + std::to_string(id / 2) + ".txt");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}		
	}

	count.lock();
	threadCompletionCount++;
	count.unlock();

	std::unique_lock<std::mutex> lk(condition);
	cv.notify_all();
	cv.wait(lk, [] { return threadCompletionCount >= THREAD_COUNT; });
}

void createFile(int peerID, int fileSizeInKB) {
	std::ofstream file("Peers/Peer " + std::to_string(peerID) + "/File " + std::to_string(peerID) + ".txt");
	std::srand(unsigned int(std::time(nullptr)));
	for (int KB = 0; KB < fileSizeInKB; KB++) {
		// write 1KB of random readable character to the file (ASCII: 32 to 126)
		for (int i = 0; i < 1024; i++) { 
			file << char((std::rand() % 95) + 32);
		}
	}
	file.close();
}