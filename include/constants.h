// ********************************************************************
// * 소 스 명: constants.h
// * 설    명: 상수를 정의한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 24
// ********************************************************************


#pragma once

namespace Constants {
    constexpr int PORT = 12345;				//서버포트번호
    constexpr int MAX_EVENTS = 64;			//epoll최대 등록 값
    constexpr int BUFFER_SIZE = 1024;			//버퍼 사이즈
    constexpr const char* SERVER_IP = "127.0.0.1";	//서버 ip addres
    constexpr int START_DELAY_MS = 3000;		//
    constexpr int MOVE_INTERVAL_MS = 1000;		//엘레베이터가 이동하는 시간
    constexpr int BOARDING_TIME_MS = 3000;		//승객이 타는  시간
    constexpr int DEPARTURE_TIME_MS = 1000;		//승객이 내리는 시간
    constexpr int MAX_FLOORS = 10;			//최대 층(최대 클라이언트 수)
    constexpr int TEST_REQUEST_COUNT = 5;
}

