## Version 
- Visual Studio 2019
- boost_1_82_0
- C++ 14
<br>

## To do List 
### 2024.6.25 ~
- [x] 클래스 분리, 라이브러리 분리
<br>

### 2024.7.2 ~
- [ ] 정상종료 구현 (스레드 등 사용하고 있는 자원 제대로 마무리하는거 확인되어야 함)
- [ ] 메모리 누수 체크 (현 서버 참고할 것)
- [ ] confing file을 사용하는 구조로 수정하기 (이 파일에는 port, thread size 등을 정의할 수 있다)
- [ ] cout 말고 format 사용해보기 (cout은 상대적으로 thread safe 하지 않다)
- [ ] log 파일로 기록되도록 심어보기
<br> ---------- 여기까지 우선순위 높음
- [ ] TCP / IP Builder 사용해서 통신 확인해보기
- [ ] Acceptor 클래스 따로 빼기, Accpetor 위치 변경 생각해보기
- [ ] Server() default 생성자 추가 해야할 듯
- [ ] asio에선 패킷을 어떤 크기로 어떤 방식으로 주고받는지 알기
- [ ] asio에선 패킷 전송 중 유실된 헤더가 있을 경우 어떻게 대처하는지 알기(header crush)
- [ ] asio와 iocp 패킷 처리 방식 차이점 알기
- [ ] server와 client가 공통적으로 쓰는 것들 하나의 class에 넣어 범용으로 쓸수있도록
- [ ] thread 생성하는 부분에서 sleep 무의미한듯 수정할 것
- [ ] boost의 thread 말고 std::thread도 사용해보기
- [ ] server client간 packet교환 자세히 설명할 수 있도록
- [ ] Session pool 관리 (생각없이 new 하지말고)
