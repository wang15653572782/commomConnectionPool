#pragma once
#include<string>
#include<queue>
#include<mutex>
#include<atomic>
#include<memory>
#include<thread>
#include<functional>
#include<condition_variable>
#include "Connection.h"
using namespace std;
/*实现连接池模块*/
class ConnectionPool {
public:
	static ConnectionPool* getConnectionPool();
	//给外部提供接口，从连接池中获取一个可用的空闲的连接
	shared_ptr<Connection> getConnection();
private:
	ConnectionPool();
	bool loadConfigFile();
	//运行在独立的线程中,专门负责生产新连接
	void produceConnectionTask();
	//扫描超过 maxIdleTime时间的空闲连接，进行连接的回收
	void scannerConnectionTask();
	string _ip;//mysql:ip
	unsigned short _port;//mysql:port
	string _username;//mysql:username
	string _password;//mysql:password
	string _dbname;//mysql:dbname
	int _initSize;//initsize of connectionpool
	int _maxSize;//maxsize of connectionpool
	int _maxIdleTime;//max idle time of connectionpool
	int _connectionTimeout;//connectionTimeout of connectionpool

	queue<Connection*> _connectionQue;//存储mysql连接的队列
	mutex _queueMutex;//维护连接队列的线程安全互斥锁
	atomic_int _connectionCnt;//记录连接所创建的connection连接的总数量
	condition_variable cv;//设置条件变量，用于连接生产线程和消费线程的通信
};