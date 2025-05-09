// ********************************************************************
// * 소 스 명: elevator.cpp 
// * 설    명: Elevator class의 멤버함수 구현
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#include "constants.h"
#include "protocol.h"
#include "elevator.h"

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <cstring>
#include <climits>
#include <ctime>

using std::cout;
using std::endl;
using std::set;
using std::vector;
using std::priority_queue;

// ********************************************************************
// * 클래스명: Elevator
// * 설    명: elevator 동작을 정의
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

// ********************************************************************
// * 함 수 명: Elevator
// * 설    명: Elevator class 생성자
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
Elevator::Elevator() : current_floor(1), direction(STAY), is_boarding(false)
		       , total_waiting_time(0), total_passengers(0), avg_waiting_time(0.0) {}


// ********************************************************************
// * 함 수 명: addRequest
// * 설    명: 엘베 작업queue에다가 요청을 넣는다.  
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
void Elevator::addRequest(const ElevatorMessage& msg) {
	std::lock_guard<std::mutex> lock(queue_mutex);
	// 엘레베이터 버튼을 누른다.
	cout << msg.timestamp << " : " << int(msg.dst_floor) << endl;
	floor_waiting[msg.src_floor].push(msg);

	this->total_passengers++;
}

// ********************************************************************
// * 함 수 명: moveAndProcess 
// * 설    명: 
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
void Elevator::moveAndProcess(set<int>& clients) {
	std::this_thread::sleep_for(std::chrono::milliseconds(Constants::MOVE_INTERVAL_MS));

	
	//하차 처리
	int before_cnt = elevator_passengers.size();
	elevator_passengers.erase(
			std::remove(elevator_passengers.begin(), elevator_passengers.end(),current_floor),
		       	elevator_passengers.end()
	);
	//remove 함수로 안내릴 승객들을 모으고 그 다음부터 끝까지 내린다.
	int after_cnt = elevator_passengers.size();

	if (after_cnt == 0) direction = STAY;
	//하차 하는 시간 처리
	if(before_cnt != after_cnt) {
		cout << "하차 : " << before_cnt - after_cnt << "명이 내렸습니댜." << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(Constants::DEPARTURE_TIME_MS));
	}
	
	//승차 처리
	bool is_boarding = false;		//lock 가지고 잠들수 없기에 따로 선언
	{
		std::lock_guard<std::mutex> lock(queue_mutex);

		if (!floor_waiting[current_floor].empty()) {
			auto& waiting = floor_waiting[current_floor];
			priority_queue<ElevatorMessage> remaining;
			while (!waiting.empty()) {
				const auto msg = waiting.top();
				waiting.pop();

				calc_waiting_time(msg.timestamp);	//total waiting time 계산

				if (msg.dir == direction || direction == STAY) {
                       		         cout << msg.timestamp << "승차 : " << int(msg.src_floor) << " >> " << int(msg.dst_floor) << endl;
                       		         elevator_passengers.push_back(msg.dst_floor);
                            		 direction = msg.dir;      //처음 승차한 손님의 방향으로 엘베를 바꾼다.
					 is_boarding = true;
                    	        } else {
                               	         remaining.push(msg);
                       	        }
			}
			waiting = remaining;
		}
	}
	if (is_boarding) std::this_thread::sleep_for(std::chrono::milliseconds(Constants::BOARDING_TIME_MS));


	//방향결정 및 이동
	bool is_not_waiting_anybody = is_empty_waiting();	//엘베를 기다리는 사람이 있는지 여부
	bool is_not_in_anybody = elevator_passengers.empty();	//엘베안에 사람이 있는지 여부

	if (is_not_in_anybody) {
		if (!is_not_waiting_anybody) {
			int next_floor = next_waiting_floor();
			direction = (current_floor > next_floor)? DOWN : UP;
		} else {
			direction = STAY;
		}
	}



	if (direction == UP) {
		current_floor++;
	} else if (direction ==DOWN) {
		current_floor--;
	}
	//프로토콜 파싱후 예외처리는 따로 해주자.
	if (current_floor > Constants::MAX_FLOORS || current_floor <1)
		std::cerr << "갈 수 없는 층입니다." << endl;
	
	//클라이언트들에게 상태전송
	StatusMessage msg{ MSG_STATUS, static_cast<uint8_t>(current_floor)};
	for (int fd : clients) {
		send(fd, &msg, sizeof(msg), 0);
	}
}

// ********************************************************************
// * 함 수 명: is_empty_waiting
// * 설    명: 현재 엘레베이터를 기다리는 클라이언트가 있다면 참
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************
bool Elevator::is_empty_waiting() {
	std::lock_guard<std::mutex> lock(queue_mutex);

	for (const auto& floor : floor_waiting) {
		if (!floor.second.empty()) return false;
	}
	return true;
}

// ********************************************************************
// * 함 수 명: next_waiting_floor
// * 설    명: 현재 엘레베이터를 기다리는 사람들 중 가장 빠르게 버튼을
// 	       누른 사람의 층을 반환한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 28
// ********************************************************************
int Elevator::next_waiting_floor() {
	std::lock_guard<std::mutex> lock(queue_mutex);

	int ret = -1;
	int32_t min_timestamp = INT_MAX;
	for (const auto& floor : floor_waiting) {
		if (!floor.second.empty() && (floor.second.top().timestamp < min_timestamp)) {
			ret = floor.first;
			min_timestamp = floor.second.top().timestamp;
		}
	}

	return ret;
}
// ********************************************************************
// * 함 수 명: calc_waiting_time
// * 설    명: 엘레베이터를 기다린 시간을 계산한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 05. 07
// ********************************************************************
void Elevator::calc_waiting_time(const uint32_t start) {
	this->total_waiting_time += static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1)) - start;

}
// ********************************************************************
// * 함 수 명: print_avg_waiting_time
// * 설    명: 엘레베이터를 기다린 평균 시간을 출력한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 05. 07
// ********************************************************************
void Elevator::print_avg_waiting_time() {
	avg_waiting_time = (double)total_waiting_time / total_passengers;
	
	cout << "==================== 사후 검토 ===========================" << endl;
	cout << "총 엘레베이터를 이용한 승객   : " << total_passengers << endl;
	cout << "평균 엘레베이터를 기다린 시간 : " << avg_waiting_time  << endl;
	cout << "==========================================================" << endl;
}
