/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <Server.h>
#include <iostream>
#include <list>

using namespace std;

list<Server::ServerThread*> Server::threads;
list<Server::sendListener> Server::listeners;

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

Server::~Server()
{
	cout << "SERVER STOP" << endl;
	stop();
}

void Server::addListener(sendListener f)
{
	listeners.push_back(f);
}

void Server::thread(Server* svr)
{
	svr->run();
}

bool Server::incoming(const string* buffer)
{
	bool result = false;
	
	mtx.lock();
	if (buffer)
	{
		sIncoming += (*buffer);
		cout << "Incomming message: [" << (*buffer) << "]" << endl;
	}
	else
	{
		while(sIncoming[0]=='\r' || sIncoming[0]=='\n') sIncoming.erase(0,1);
		if (sIncoming.length())
			result=true;
	}
	mtx.unlock();
	return result;
}

Server::Server(int port)
	:
	portno(port),
	newsockfd(-1)
{
	thr=new std::thread(Server::thread, this);
	cout << "Running server on port " << port << endl;
}

string Server::getIncoming()
{
	string s;
	mtx.lock();

	if (sIncoming.length())
	{
		while(sIncoming[0]=='\r' || sIncoming[0]=='\n') sIncoming.erase(0,1);

		string::size_type p=sIncoming.find('\r');
		string::size_type q=sIncoming.find('\n');
		if (p==string::npos) p=q;
		if (p!=string::npos)
		{
			if (q!=string::npos)
				if (p>q) p=q;
		}
		if (p!=string::npos)
		{
			s=sIncoming.substr(0,p);
			sIncoming.erase(0,p);
		}
		else
		{
			s=sIncoming;
			sIncoming="";
		}
	}
	mtx.unlock();

	return s;
}

void Server::run()
{
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	int br = ::bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr));
	if (br < 0) error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while(portno)
	{
		try
		{
			int newsockfd = accept(sockfd, 
					(struct sockaddr *) &cli_addr, 
					&clilen);
			if (newsockfd < 0)
				error("ERROR on accept");
			else
			{
				cout << "NEW CLIENT ON SOCKET #" << newsockfd << " (" << threads.size()+1 << " client(s)" << endl;
				ServerThread* thr=new ServerThread(this, newsockfd);
				mtx.lock();
				threads.push_back(thr);
				mtx.unlock();
				thr->run();
			}
		}
		catch(...)
		{
			cout << "EXCEPTION" << endl;
		}
	}
	close(sockfd);
	cout << "CONNEXION CLOSED "<< endl;
}

void Server::stop()
{
	cout << "STOPPING ALL THREADS" << endl;
	for(auto it=threads.begin(); it!=threads.end(); it++)
	{
		Server::ServerThread* thr=*it;
		thr->stop();
	}
	cout << "OK" << endl;
	portno=0;
	cout << "JOIN" << endl;
	thr->detach();
	cout << "OK2" << endl;
}

void Server::send(string s)
{
	static mutex mtx;
	mtx.lock();
	for(auto it=threads.begin(); it!=threads.end(); it++)
	{
		(*it)->send(s);
	}
	for(auto it=listeners.begin(); it!=listeners.end(); it++)
	{
		(*(*it))(s);
	}
	mtx.unlock();
}

void Server::ServerThread::send(string s)
{
	if (msock>=0)
	{
		s+="\n";
		int n = write(msock,s.c_str(),s.length()+1);
		if (n < 0) error("ERROR writing to socket");
	}
}

void Server::ServerThread::run()
{
	thr=new std::thread(Server::ServerThread::loop, this);
}

void Server::ServerThread::stop()
{
	if (isok())
	{
		connect=false;
		thr->join();
		cout << "  JOINED" << endl;
	}
}

bool Server::ServerThread::isok() const
{
	if (thr)
		return true;
	cerr << "ATTEMPTING OPERATION ON NULL THREAD" << endl;
	return false;
}

void Server::remove(Server::ServerThread* thr)
{
	mtx.lock();
	threads.remove(thr);
	mtx.unlock();
}

void Server::ServerThread::loop(Server::ServerThread* thr)
{
	char buffer[256];
	int n;
	thr->send("Welcome to cube server v0.0");
	thr->connect=true;
	while(thr->connect)
	{
		bzero(buffer,256);
		n = read(thr->msock,buffer,255);
		if (n < 0)
		{
			error("ERROR reading from socket");
			thr->connect=false;
		}
		else if (n>0)
		{
			string income(buffer,n);
			if (income.substr(0,4)=="exit") break;
			thr->mparent->incoming(&income);
		}
	}
	close(thr->msock);
	thr->msock=-1;
	thr->mparent->remove(thr);
	cout << "CLIENT GONE " << threads.size() << " client(s) left." << endl;
}
