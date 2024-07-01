#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Library.h"
#include "EnumDefine.h"

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"

using namespace boost;
using std::cout;
using std::endl;
using std::string;

struct Session
{
	shared_ptr<asio::ip::tcp::socket> socket;
	asio::ip::tcp::endpoint endpoint;
	string id;
	int num = -1;

	string sendBuffer;
	string receiveBuffer;
	char buf[80];
};

class Server
{
public:
	Server(string, unsigned short); // ipAddress, portNum
	~Server();

	void Start();
	void WorkerThread();
	void OpenGate();
	void StartAccept();
	void OnAccept(const system::error_code& err, Session* session);
	void Receive(Session* session);
	void PacketManager(Session* session);
	EIDState TranslatePacket(string message);
	void OnSend(const system::error_code& err);
	void SetID(Session* session);
	void CloseSession(Session* session);


private:
	asio::io_context context;					// object 
	shared_ptr<asio::io_context::work> work;	// N���� �����忡�� 1���� io_context�� ����ϴ� ���� ��
	asio::ip::tcp::endpoint endpoint;
	asio::ip::tcp::acceptor gate;
	std::vector<Session*> sessions;
	boost::thread_group threadGroup;
	boost::mutex lock;							// lock�� �ϳ� �ɸ��� ���� lock�� ���� ù��° �� unlock�� ������ ���
	std::vector<int> waitingRooms;				// ������ Ŭ���̾�Ʈ ������ �׾Ƴ��� ��
	const int THREAD_SIZE = 4;					// ��Ƽ������ ����
};