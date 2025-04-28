// ********************************************************************
// * 소 스 명: epoll_server.h 
// * 설    명: epoll을 핸들하기 위한 클래스를 정의
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#pragma once

#include "elevator.h"

// ********************************************************************
// * 클래스명: EpollServer
// * 설    명: epoll을 핸들하기 위한 클래스를 정의
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 24
// ********************************************************************

class EpollServer {
private:
	int 		server_fd;
	int 		epoll_fd;
	set<int> 	client_fds;
	Elevator 	elevator;	
public:
	EpollServer(int port);
	void run();
private:
	int	setup_server_socket(int port);
	void 	send_start_signal(int fd);
	void 	handle_client(int fd);
};
