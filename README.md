# Thieves

2023 한국공학대학교 게임공학과 팀 thieves의 졸업작품인 thieves입니다.

- 최대 8인이 매칭해 플레이하는 배틀로얄 게임 개발
- 작업인원: 클라이언트 2명, 서버 1명

## 팀원

정극훈

김혁동

이도영

## 개발환경
- Visual Studio 2022
- C/C++, Lua
- DirectX 12
- IOCP
- My SQL
- Unity


<img src="https://github.com/rmrgns/Thieves/assets/70666642/64769fad-4819-430e-ac99-999be8afb81f" width="650" height="350"/>
<img src="https://github.com/rmrgns/Thieves/assets/70666642/5739b276-d037-41c4-9a42-a40a96a0bb96" width="650" height="350"/>

## 쓰레드구조
### worker Thread
GQCS를 호출해 얻은 확장 Overlapped 구조체의 결과에 따라 처리 수행

- 서버 시작시 worker thread 생성
- OnRecv(): 패킷 재조립 및 처리 함수 실행
- OnAccept() 세션 생성 후 AcceptEX 호출

### Timer Thread
Worker Thread에서 요청한 timer_event 만큼 기다린후 worker thread에 다시 처리를 요청

- Worker Thread에서 ID, 시간, 이벤트타입을 담아 Timer에 전달 
- Timer 저장하여 빠른 순으로 시간체크 후 딜레이 없이 Threadpool에 반납
- 시간에 딜레이가 생기는것의 방지를 위해 로직을 수행하지 않음
