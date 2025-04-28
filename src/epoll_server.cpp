// ********************************************************************
// * 소 스 명: epoll_server.cpp 
// * 설    명: epoll을 핸들하기 위한 클래스 구현
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#include "protocol.h"
#include "constants.h"
#include "epoll_server.h"
#include "elevator.h"

#include <iostream>
#include <thread>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>


using std::cout;
using std::endl;

// ********************************************************************
// * 함 수 명: EpollServer
// * 설    명: EpollServer 생성자
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
EpollServer::EpollServer(int port) {
	server_fd = setup_server_socket(port);			//서버소켓생성
	epoll_fd = epoll_create1(0);				//epoll 등록준비
	epoll_event ev{};					//epoll event 구조체 정의 후 server_fd 등록
	ev.events = EPOLLIN;
	ev.data.fd = server_fd;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
}

// ********************************************************************
// * 함 수 명: run
// * 설    명: server를 실행시킨다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
void EpollServer::run() {

	//엘레베이터는 쓰레드로 계속 동작한다.
	std::thread elevator_thread([&]() {
		while (true) elevator.moveAndProcess(client_fds);		
	});

	while(true) {
		epoll_event events[Constants::MAX_EVENTS];
		int n = epoll_wait(epoll_fd, events, Constants::MAX_EVENTS, -1);

		for (int i=0; i<n; ++i) {
			int fd = events[i].data.fd;
			if (fd == server_fd) {
				int client_fd = accept(server_fd, nullptr, nullptr);
				fcntl(client_fd, F_SETFL, O_NONBLOCK);
				epoll_event cev{};
				cev.events = EPOLLIN;
				cev.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &cev);
				cout << "client connected " << client_fd << endl;
				send_start_signal(client_fd);
			} else {
				handle_client(fd);
			}
		}
	}
	elevator_thread.join();
}

// ********************************************************************
// * 함 수 명: setup_server_socket
// * 설    명: server socket을 설정한다..
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
int EpollServer::setup_server_socket(int port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
      
      	sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
      
      	bind(sock, (sockaddr*)&addr, sizeof(addr));
        listen(sock, SOMAXCONN);
        return sock;

}

// ********************************************************************
// * 함 수 명: send_start_signal
// * 설    명: 클라이언트들에게 start signal 을 보낸다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
void EpollServer::send_start_signal(int fd) {
	ElevatorMessage msg{ MSG_START, STAY, 0, 0};
	send(fd, &msg, sizeof(msg), 0);
	cout << fd << "에게 start signal 을 보냈습니다." << endl;
	client_fds.insert(fd);
}


// ********************************************************************
// * 함 수 명: handle_client
// * 설    명: client
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
void EpollServer::handle_client(int fd) {
	ElevatorMessage msg{};
	int len = recv(fd, &msg, sizeof(msg), 0);
	//읽을 것이 없다면 연결해제하고 리턴??
	if (len <= 0) {
		close(fd);
		client_fds.erase(fd);
		return;
	}
	
	if (msg.type == MSG_REQUEST) {
		elevator.addRequest(msg);
		cout << "요청 수신 : " << int(msg.src_floor) << ">>" << int(msg.dst_floor)
		     <<  endl;
	}

}
