# 엘레베이터 모의 모델 서버 프로그램 

이 프로젝트에서는 리눅스(Ubuntu) 환경에서 epoll 을 활용하여 각 클라이언트를 층으로 가정하여 엘레베이터 시뮬레이션을 구현한 프로젝트입니다.
각 클라이언트들은 정의된 프로토콜을 모의 모델 서버에 전송하여 엘레베이터 버튼을 누를 수 있으며, time stamp 또한 프로토콜에 포함되어 요청된 순서대로 서버가 처리할 수 있도록 구현하였습니다.
서버는 epoll을 사용하여 클라이언트 소켓파일 이벤트를 효율적으로 처리할 수 있도록 하였습니다.

<img src="https://raw.githubusercontent.com/wanna175/elevator_simulation/main/gif/elevator_model_ex.gif" width="800" />


## 주요 기능
- **클라이언트 - 파일로 데이터 입력:** 파일을 통해 클라이언트 프로그램은 데이터를 읽고 엘레베이터 프로토콜에 맞게 보냅니다.
- **프로토콜 파싱:** 엘레베이터 동작에 맞게 프로토콜을 정의하였습니다.
- **time stamp:** 프로토콜에 timestamp를 넣어 여러 클라이언트의 요청을 순서대로 처리할 수 있도록 하였습니다.

## 사용된 주요 기술
- **Epoll:** 클라이언트 소켓파일을 효율적으로 다루기 위하여 사용하였습니다.

## elevator event protocol
- **MessageType (8bit):** message가 어떤 타입인지 (MSG_START, MSG_REQUEST, MSG_STATUS) 알려주는 비트
- **Direction (8bit):** 엘레베이터가 버튼 조작 (UP, DOWN, STAY)
- **src_floor (8bit):** 요청을 보낸 층
- **dst_floor (8bit):** 가고자 하는 층
- **timestamp (32bit):** 버튼을 누른 시점(요청을 보낸 시점)을 초단위로 계산.

## 컴파일 & 실행 방법 
- **클라이언트 컴파일:** g++ -std=c++17 -Iinclude src/client.cpp -o clientTest
- **서버 컴파일:** g++ -std=c++17 -Iinclude src/main.cpp src/epoll_server.cpp src/elevator.cpp -o serverc
- **실행:** ./serverc 후에 ./clientTest [파일 명]

