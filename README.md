# 엘레베이터 모의 모델 서버 프로그램 

이 프로젝트에서는 리눅스(Ubuntu) 환경에서 epoll 을 활용하여 각 클라이언트를 층으로 가정하여 엘레베이터 시뮬레이션을 구현한 프로젝트입니다.
각 클라이언트들은 정의된 프로토콜을 모의 모델 서버에 전송하여 엘레베이터 버튼을 누를 수 있으며, time stamp 또한 프로토콜에 포함되어 요청된 순서대로 서버가 처리할 수 있도록 구현하였습니다.
서버는 epoll을 사용하여 클라이언트 소켓파일 이벤트를 효율적으로 처리할 수 있도록 하였습니다.

<img src="https://raw.githubusercontent.com/wanna175/elevator_simulation/main/gif/elevator_model_ex.gif" width="800" />


## 주요 기능
- **패킷 캡쳐:** Npcap을 사용하여 실시간 네트워크 트래픽을 캡쳐합니다.
- **프로토콜 파싱:** 이더넷, IPv4, IPv6, UDP, TCP, ARP 프로토콜을 파싱합니다.
- **패킷 필터링:** 향후 특정 조건에 맞는 패킷을 필터링하는 기능을 추가할 예정입니다.
- **패킷 그룹화:** `sourceIP`와 `destinationIP`를 기준으로 패킷을 그룹화하여 더 쉽게 분석할 수 있습니다.
- **프론트엔드:** C++와 wxWidgets를 사용하여 UI를 구성하며, 메인 로직과 UI는 `pcaplib`라는 정적 라이브러리를 통해 분리되었습니다.

## 사용된 주요 기술
- **Epoll:** 패킷 캡쳐를 위한 라이브러리입니다.
- **C++/wxWidgets:** 프론트엔드 UI를 구축하는 데 사용되었습니다.
- **정적 링크:** `pcaplib`는 정적 링크 라이브러리로, 패킷 캡쳐 로직과 UI를 분리하여 모듈화했습니다.

## 미완성 기능 (향후 작업)
- **패킷 덤프:** 캡쳐된 패킷을 파일(PCAP 형식)로 저장하는 기능.
- **패킷 필터링:** 특정 프로토콜이나 조건에 맞는 패킷을 필터링하는 기능을 추가할 예정입니다.
- **패킷 통계 시각화:** 캡쳐된 패킷에 대한 통계를 시각적으로 보여주는 페이지를 추가할 예정입니다.
- **다양한 프로토콜 파싱:** 여러 다양한 프로토콜 역시 파싱될 수 있습니다.

## 화면 미리보기
<img src="https://raw.githubusercontent.com/wanna175/PacketCapture/main/gif/img/mainScreen.PNG" width="600" />

*패킷 캡쳐 화면 예시.*

<img src="https://raw.githubusercontent.com/wanna175/PacketCapture/main/gif/img/tcp.PNG" width="600" />
<img src="https://raw.githubusercontent.com/wanna175/PacketCapture/main/gif/img/udp.PNG" width="600" />
<img src="https://raw.githubusercontent.com/wanna175/PacketCapture/main/gif/img/arp.PNG" width="600" />
