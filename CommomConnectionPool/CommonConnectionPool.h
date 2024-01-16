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
/*ʵ�����ӳ�ģ��*/
class ConnectionPool {
public:
	static ConnectionPool* getConnectionPool();
	//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ�����õĿ��е�����
	shared_ptr<Connection> getConnection();
private:
	ConnectionPool();
	bool loadConfigFile();
	//�����ڶ������߳���,ר�Ÿ�������������
	void produceConnectionTask();
	//ɨ�賬�� maxIdleTimeʱ��Ŀ������ӣ��������ӵĻ���
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

	queue<Connection*> _connectionQue;//�洢mysql���ӵĶ���
	mutex _queueMutex;//ά�����Ӷ��е��̰߳�ȫ������
	atomic_int _connectionCnt;//��¼������������connection���ӵ�������
	condition_variable cv;//���������������������������̺߳������̵߳�ͨ��
};