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
		typedef void (*sendListener)(const string&);

		bool running() { return portno!=0; }
		
		void stop();
      
		static void addListener(sendListener);
		
  private:
     
	
	class ServerThread
	{
	  public:
		ServerThread(Server* p, int sock) : mparent(p), msock(sock) {}
      ServerThread(const ServerThread&) = delete;
      
		~ServerThread();
		
		void stop();
		void run();
		void send(string);
		
	  private:
		void loop();
      friend class Server;
      
		bool isok() const;
		
		Server* mparent;
		int msock;
		std::thread*	thr = nullptr;
		bool connect;
	};
   static void ServerThreadLoop(ServerThread*);
	
  protected:
	void remove(ServerThread*);
		
	private:
		mutex	mtx;
		void run();
		string sIncoming;
		atomic<int>    portno;
		int sockfd;
		int newsockfd;
		std::thread*   thr;

		static void thread(Server*);
		static list<ServerThread*> threads;
		
		static list<sendListener>	listeners;
};

#endif
