// ********************************************************************
// * 소 스 명: main.cpp
// * 설    명: epoll server를 실행시키고 엘레베이터 시뮬레이션을 동작
// 	       시킨다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
#include "epoll_server.h"
#include "constants.h"

#include <iostream>

using std::cout;
using std::endl;

// ********************************************************************
// * 함 수 명: main
// * 설    명: main 함수를 정의한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

int32_t main(int32_t argc, char** argv) {
	EpollServer server(Constants::PORT);

	cout << "elevator simulation server 를 실행합니다." << endl;
	server.run();

	return 0;
}
