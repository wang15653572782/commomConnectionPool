#include <iostream>
#include "Connection.h"
#include "CommonConnectionPool.h"
#define test_size 1250
using namespace std;
int main()
{
	/*Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");*/
	//for (int i = 0; i < 10; ++i) {
	//	Connection conn;
	//	char sql[1024] = { 0 };
	//	sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", i, "ZHANG SAN", i+20, "male");
	//	conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
	//	conn.update(sql);
	//}
	//
	//static int test_size = 250;
	clock_t begin = clock();
	
	thread t1([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < test_size; ++i) {
			shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')",  i, "ZHANG SAN", rand() % 100, "male");
			//conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sp->update(sql);
		}
	/*	for (int i = 0; i < test_size; ++i) {
		char sql[1024] = { 0 };
		Connection conn;
		conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
		sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", i, "ZHANG SAN", rand()%1000, "male");
		conn.update(sql);
	}*/
		});
	thread t2([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < test_size; ++i) {
			shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size + i, "ZHANG SAN", rand() % 100, "male");
			//conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sp->update(sql);
		}
		/*for (int i = 0; i < test_size; ++i) {
			char sql[1024] = { 0 };
			Connection conn;
			conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size + i, "ZHANG SAN", rand() % 1000, "male");
			conn.update(sql);
		}*/
		});
	thread t3([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < test_size; ++i) {
			shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size*2 + i, "ZHANG SAN", rand() % 100, "male");
			//conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sp->update(sql);
		}
		/*for (int i = 0; i < test_size; ++i) {
			char sql[1024] = { 0 };
			Connection conn;
			conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size * 2+ i, "ZHANG SAN", rand() % 1000, "male");
			conn.update(sql);
		}*/
		});
	thread t4([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < test_size; ++i) {
			shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size*3 + i, "ZHANG SAN", rand() % 100, "male");
			//conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sp->update(sql);
		}
	/*	for (int i = 0; i < test_size; ++i) {
			char sql[1024] = { 0 };
			Connection conn;
			conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
			sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", test_size * 3 + i, "ZHANG SAN", rand() % 1000, "male");
			conn.update(sql);
		}*/
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	
	/*for (int i = 0; i < 10000; ++i) {
	//	Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')",1000+i, "ZHANG SAN", i+20, "male");
		conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
		conn.update(sql);
	}*/
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();
	//for (int i = 0; i < 1000; ++i) {
	//	shared_ptr<Connection> sp = cp->getConnection();
	//	char sql[1024] = { 0 };
	//	sprintf(sql, "insert into user (id,name,age,sex) values(%d,'%s',%d,'%s')", 1000+i, "ZHANG SAN", rand()%100, "male");
	//	//conn.connect("127.0.0.1", 3306, "root", "jack2002", "chat");
	//	sp->update(sql);
	//}
	clock_t end = clock();
	cout << (end - begin) << endl;
	return 0;
}
