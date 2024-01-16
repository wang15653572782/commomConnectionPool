#pragma once
#include<string>
#include<mysql.h>
#include<ctime>
using namespace std;
/*ʵ��mysql���ݿ�Ĳ���*/
class Connection {
public:
	// ��ʼ�����ݿ�����
	Connection();
	// �ͷ����ݿ�������Դ
	~Connection();
	// �������ݿ�
	bool connect(string ip, unsigned short port, string user, string password,
		string dbname);

		// ���²��� insert��delete��update
		bool update(string sql);
	
	// ��ѯ���� select
		MYSQL_RES* query(string sql);
	//ˢ��һ�����ӵ���ʼ�Ŀ���ʱ���
		void refreshAliveTime() {
			_alivetime = clock();
		}
		//���ش���ʱ��
		clock_t getAliceTime() const{
			return clock() - _alivetime;
		}
private:
	MYSQL* _conn; // ��ʾ��MySQL Server��һ������
	clock_t _alivetime;//��¼�������״̬�����ʼ���ʱ��
 };