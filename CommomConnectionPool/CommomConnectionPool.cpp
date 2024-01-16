// CommomConnectionPool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "CommonConnectionPool.h"
#include "public.h"
#include<iostream>
using namespace std;
ConnectionPool* ConnectionPool::getConnectionPool() {
	static ConnectionPool pool;
	return &pool;
}
bool ConnectionPool::loadConfigFile() {
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr) {
		LOG("mysql.ini file is not exist!");
		return false;
	}
	while (!feof(pf)) {
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1)continue;
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);
		if (key == "ip") {
			_ip = value;
		}
		else if (key == "port") {
			_port = atoi(value.c_str());
		}
		else if (key == "username") {
			_username = value;
		}
		else if (key == "password") {
			_password = value;
		}
		else if (key == "dbname") {
			_dbname = value;
		}
		else if (key == "initSize") {
			_initSize = atoi(value.c_str());
		}
		else if (key == "maxSize") {
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeout") {
			_connectionTimeout =atoi(value.c_str());
		}

	}
	return true;
}
ConnectionPool::ConnectionPool() {
	if (!loadConfigFile())return;
	for (int i = 0; i < _initSize; ++i) {
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();//刷新时间
		_connectionQue.push(p);
		_connectionCnt++;
	}
	//连接的生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
	produce.detach();
	//启动一个新的定时线程，扫描超过 maxIdleTime时间的空闲连接，进行连接的回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}
void ConnectionPool::produceConnectionTask() {
	for (;;) {
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty()) {
			cv.wait(lock);
		}
		//连接数量没有到达上限，继续创建新的连接
		if (_connectionCnt < _maxSize) {
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();//刷新时间
			_connectionQue.push(p);
			_connectionCnt++;
		}
		//通知消费者线程，可以消费连接了
		cv.notify_all();

	}
}
shared_ptr<Connection> ConnectionPool::getConnection() {
	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty()) {
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout))) {
			if (_connectionQue.empty()) {
				LOG("获取空闲连接超时了...获取连接失败了！");
				return nullptr;
			}
		}
		

	}
	/*shared_ptr智能指针析构时，会把connection资源直接delete掉，相当于调用connection的析构函数，connection就被
	close掉了，这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue当中去*/
	shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection* pcon) {
			unique_lock<mutex> lock(_queueMutex);
			pcon->refreshAliveTime();//刷新时间
			_connectionQue.push(pcon);
		}
		);
	_connectionQue.pop();

	if(_connectionQue.empty())cv.notify_all();//谁消费了最后一个连接，就去通知一下生产者生产连接
	return sp;
}
void ConnectionPool::scannerConnectionTask() {
	for (;;) {
		//通过sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		//扫描整个队列，释放多余的连接
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize) {
			Connection* p = _connectionQue.front();
			if (p->getAliceTime() >= (_maxIdleTime * 1000)) {
				_connectionCnt--;
				_connectionQue.pop();
				delete p;
			}
			else {
				break;//队头连接没有超过_maxIdleTime,其他连接肯定就没超时
			}
		}
	}
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
