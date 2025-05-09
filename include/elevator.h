// ********************************************************************
// * 소 스 명: elevator.h 
// * 설    명: elevator의 동작을 정의하기 위한 클래스
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#pragma once

#include "protocol.h"

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <mutex>

using std::vector;
using std::map;
using std::mutex;
using std::set;
using std::priority_queue;

// ********************************************************************
// * 클래스명: Elevator
// * 설    명: elevator동작을 정의하는 클래스
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

class Elevator {
private:
	int 						current_floor; 		//현재 층
	Direction 					direction;		//현재 엘베 방향
	bool 						is_boarding;		
	vector<int> 					elevator_passengers;	//현재 엘베안에 타고있는 사람(목적층)
	map<int, priority_queue<ElevatorMessage>> 	floor_waiting;		//현재 엘베를 기다리고 있는 사람
	mutex 						queue_mutex;
	
	//사후처리를 위한 변수들
	int 						total_waiting_time;	//총 기다린 시간 (time stamp)
	int 						total_passengers;	//총 엘레베이터를 이용한 승객
	double						avg_waiting_time;	//기다린 평균 시간
public:
	Elevator();
	void addRequest(const ElevatorMessage& msg);
	void moveAndProcess(set<int>& clients);
	void print_avg_waiting_time();
private:
	bool is_empty_waiting();
	int  next_waiting_floor();
	void calc_waiting_time(const uint32_t start);

};
