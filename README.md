#  Linux Shell 구현
2017 Dongguk Univ. "Operating System" Assignment <Linux Shell Programming>


## 구현된 기능
-기본 기능
-history 기능
-pipe 기능
-redirection 기능
-현재 위치(cur_dir) 알림


## 컴파일
gcc main.c –o main33
./main33


## 기능별 자세한 설명
|함수이름|내용|실행화면캡처|
|----|---|---|
|main 함수|현재 디렉토리의 위치를 출력하고 명령을 입력 받는다. Sigaction으로 ctrl+z(SIGINT) 인터럽트에도 방해받지 않게 handling 하고, 만약 exit 을 입력 받을 시에 입력을 그만둔다.|테스트3|
|command 함수|명령어를 수행하게 되는 처음 함수이다. 명령어 history 스택에 저장하고서 what_command 함수로 넘어가 명령을 수행하게 된다.|테스트3|
|basic_command 함수| cd(change directory) 기능, execvp 함수들로 이루어졌으며, 명령어에 다른 특별한 기호가 포함되어 있지 않고 간단한 것들의 명령을 수행하는 함수이다. cd 기능은 execvp 기능이 아니라 chdir함수(parameter를 path로 하여 이동)를 사용해서 진행하였다. 만약 chdir 함수가 에러가 뜨게 되면 움직일 수 있는 directory가 없으므로 error handling을 해주었다. 마지막으로 history기능을 인식하여 history 함수로 넘어가 수행하게 해주었다.|테스트3|
|history_command 함수| type 1은 “history” 시에 전부 출력하게 하였다. 프로젝트 예제에서 10개 까지만 출력하라고 하였으므로 check 를 통해서 최근 10개까지 출력하였다. type 2는 “!_” 명령어를 구현하였다. 만약 “!!” 명령어 이면, 가장 최근에 한 명령을 다시 command 하며 수행한다. “!숫자” 라면 string을 자르고 숫자로 바꿔준 뒤 해당 명령을 수행한다. “!_” 명령어는 명령 스택에 포함되지 않기 때문에 index를 줄임으로써 저장되었던 “!!” 명령을 스택에서 빼낸다.|테스트3|
|pipe 함수| '\|' 를 기준으로 tokenize를 한다. Pipe()를 열어주고 앞의 명령어를 먼저 실행, 뒤에 명령어를 실행해준다. Dup()를 통해 pd[1]:출력 복사하고 파이프를 연결한다. 명령어를 실행하고, 명령어 부분에서 child_process 를 생성할 수 있으므로, 기다려 준 후, 끝낸다. 두 번째 뒤에 명령어는 pd[0]:입력 복사하고 표준입력 파이프를 연결한다. 명령어를 수행하고 파이프를 닫아줌으로써 종료한다.|


	
