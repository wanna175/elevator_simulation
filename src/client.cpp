// ********************************************************************
// * 소 스 명: client.cpp
// * 설    명: 엘베시뮬을 테스트 할 client program 
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#include "constants.h"
#include "protocol.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>

void receive_status(int sock) {
    StatusMessage status;
    while (recv(sock, &status, sizeof(status), 0) > 0) {
        if (status.type == MSG_STATUS) {
            std::cout << " 엘리베이터 현재 층: " << int(status.current_floor) << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file>\n";
        return 1;
    }

    std::string line;
    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "파일 열기 실패: " << argv[1] << "\n";
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(Constants::PORT);
    inet_pton(AF_INET, Constants::SERVER_IP, &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // START 대기
    ElevatorMessage start_msg;
    recv(sock, &start_msg, sizeof(start_msg), 0);
    if (start_msg.type != MSG_START) {
        std::cerr << "START 메시지 수신 실패\n";
        return 1;
    }

    std::cout << "START 수신. 요청을 시작합니다...\n";
    srand(static_cast<unsigned>(time(nullptr)));

    // 상태 수신 스레드 시작
    std::thread status_thread(receive_status, sock);

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string type_str, dir_str;
	std::string  src_str, dst_str;

        if (!(iss >> type_str >> dir_str >> src_str >> dst_str)) {
            continue; // 잘못된 라인은 무시
        }

        MessageType type;
        Direction dir;
	uint8_t src, dst;

        // 문자열을 enum으로 변환
        if (type_str == "MSG_REQUEST") type = MSG_REQUEST;
        else if (type_str == "MSG_START") type = MSG_START;
        else if (type_str == "MSG_STATUS") type = MSG_STATUS;
        else continue; // 알 수 없는 타입 무시

        if (dir_str == "UP") dir = UP;
        else if (dir_str == "DOWN") dir = DOWN;
        else if (dir_str == "STAY") dir = STAY;
        else continue; // 알 수 없는 방향 무시

	src = stoi(src_str);
	dst = stoi(dst_str);


        // 요청 추가
        ElevatorMessage req{type, dir, src, dst};
	if (req.type == MSG_REQUEST) {
		std::cout << "요청 전송 : 층 " << int(req.src_floor) << " → " << int(req.dst_floor) << std::endl;
		req.set_timestamp();
		send(sock, &req, sizeof(req), 0);
		sleep(rand() % 3 + 1); //1 ~ 3 초 대기
	}
    }
    status_thread.join();
    close(sock);
    return 0;
}
