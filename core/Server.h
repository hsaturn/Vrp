#ifndef SERVER_H
#define SERVER_H
#include <mutex>
#include <atomic>
#include <string>
#include <list>
#include <thread>

using namespace std;

class Server
{
	public:
		Server(int port);
		~Server();

		bool incoming(const string* buffer=0);

		string getIncoming();

		static void send(string);

		bool running() { return portno!=0; }
		
		void stop();
		
		typedef void (*sendListener)(const string&);
		static void addListener(sendListener);
		
  private:
	
	class ServerThread
	{
	  public:
		ServerThread(Server* p, int sock) : mparent(p), msock(sock), thr(0) {}
		~ServerThread();
		
		void stop();
		void run();
		void send(string);
		
	  private:
		static void loop(ServerThread*);
		bool isok() const;
		
		Server* mparent;
		int msock;
		std::thread*	thr;
		bool connect;
	};
	
  protected:
	void remove(ServerThread*);
		
	private:
		mutex	mtx;
		void run();
		string sIncoming;
		atomic<int>		portno;
		int sockfd;
		int newsockfd;
		std::thread*	thr;

		static void thread(Server*);
		static list<ServerThread*> threads;
		
		static list<sendListener>	listeners;
};

#endif
