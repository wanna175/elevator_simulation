// ********************************************************************
// * 소 스 명: protocol.h
// * 설    명: 엘레베이터 요청을 위한 프로토콜을 정의한다.
// * 작 성 자: KJH
// * 작성일자: 2025. 04. 25
// ********************************************************************

#pragma once

#include <cstdint>
#include <chrono>

enum MessageType : uint8_t {
    MSG_START = 0x01,
    MSG_REQUEST = 0x02,
    MSG_STATUS = 0x03
};

enum Direction : uint8_t {
    STAY = 0x00,
    UP = 0x01,
    DOWN = 0x02
};

#pragma pack(push, 1)
struct ElevatorMessage {
    MessageType type;
    Direction dir;
    uint8_t src_floor;
    uint8_t dst_floor;
    uint32_t timestamp;
    
    ElevatorMessage() {
    	timestamp = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1));
    }    

    ElevatorMessage(MessageType t, Direction d, uint8_t src, uint8_t dst)
        : type(t), dir(d), src_floor(src), dst_floor(dst) {
        timestamp = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1));
    }

    bool operator<(const ElevatorMessage& other) const {
        return timestamp > other.timestamp; 
    }	//우선순위 큐에 들어갈 것이므로 작은것이 먼저 나오도록..
    
    void set_timestamp() {
    	timestamp = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1));
    }
};

struct StatusMessage {
    MessageType type;
    uint8_t current_floor;
    uint32_t timestamp;
};
#pragma pack(pop)

