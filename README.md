
 Linux Shell 구현 
 ==============
2017 Dongguk Univ. "Operating System" Assignment <Linux Shell Programming>


## 구현된 기능 
* 기본 기능 
* history 기능 
* pipe 기능
* redirection 기능 
* 현재 위치(cur_dir) 알림


## 컴파일
         gcc main.c –o main
         ./main 


## 기능별 자세한 설명
|함수이름|<center>내용</center>|실행화면캡처|
|:---:|:---|:-------------:|
|main 함수|현재 디렉토리의 위치를 출력하고 명령을 입력 받는다.</br> Sigaction으로 ctrl+z(SIGINT) 인터럽트에도 방해받지 않게 handling 하고,</br> 만약 exit 을 입력 받을 시에 입력을 그만둔다.| |
|command 함수|명령어를 수행하게 되는 처음 함수이다. 명령어 history 스택에 저장하고서 what_command 함수로 넘어가 명령을 수행하게 된다.| |
|basic_command 함수| cd(change directory), execvp 로 이루어졌으며, 간단한 명령어를 수행하는 함수이다.</br> cd 기능은 execvp 기능이 아니라 chdir함수(parameter를 path로 하여 이동)를 사용해서 진행하였다.</br> 만약 chdir 함수가 에러가 뜨게 되면 움직일 수 있는 directory가 없으므로 error handling을 해주었다.</br> 마지막으로 history기능을 인식하여 history 함수로 넘어가 수행하게 해주었다.|<img src="https://user-images.githubusercontent.com/37530599/78252511-ebe73e00-752d-11ea-9b39-c41cd4db1185.png" width="500px" heigth="400px"></img>|
|history_command </br>함수| type 1은 “history” 시에 전부 출력하게 하였다.</br> 프로젝트 예제에서 10개 까지만 출력하라고 하였으므로 check 를 통해서 최근 10개까지 출력하였다.</br> type 2는 “!_” 명령어를 구현하였다.</br> 만약 “!!” 명령어 이면, 가장 최근에 한 명령을 다시 command 하며 수행한다.</br> “!숫자” 라면 string을 자르고 숫자로 바꿔준 뒤 해당 명령을 수행한다.</br> “!_” 명령어는 명령 스택에 포함되지 않기 때문에 index를 줄임으로써 저장되었던 “!!” 명령을 스택에서 빼낸다.|<img src="https://user-images.githubusercontent.com/37530599/78252520-f0abf200-752d-11ea-9fb6-8fb48672fb9c.png"  width="500px" heigth="400px"></img>|
|pipe 함수| '\|' 를 기준으로 tokenize를 한다. Pipe()를 열어주고 앞의 명령어를 먼저 실행, 뒤에 명령어를 실행해준다.</br>Dup()를 통해 pd[1]:출력 복사하고 파이프를 연결한다. 명령어를 실행하고, 명령어 부분에서 child_process 를 생성할 수 있으므로, 기다려 준 후, 끝낸다.</br>두 번째 뒤에 명령어는 pd[0]:입력 복사하고 표준입력 파이프를 연결한다. 명령어를 수행하고 파이프를 닫아줌으로써 종료한다.|<img src="https://user-images.githubusercontent.com/37530599/78252779-55ffe300-752e-11ea-9418-891a2c9feeca.png" width="500px" heigth="400px"></img>|
|add_command 함수 (re_out_command, re_in_command)|추가하는 기능을 구현하는 함수이다.</br> Re_out_command(“>,>>”), re_in_command (“<”)는 redirection 기능을 수행하는 함수이다.</br>기호 별로 다르게 하였지만, 알고리즘은 똑같다. 두 번째 명령어에 해당하는 파일을 열어 첫 번째 명령어를 실행하는 것이다. </br>파일 디스크립터에 대한 내용은 다음과 같다.</br> 1. O_RDONLY, O_WRONLY : 읽기전용, 쓰기 전용으로 열기 </br>2. O_CREAT :파일이 없으면 생성</br> 3. O_TRUNC : 기존파일 내용 모두 삭제 </br>4. O_APPEND : 기존파일에 덮어쓰지 않고 추가 ( “>>” : 추가 출력 리다이렉션 기능)</br> 5. STDIN , STDOUT : linux 파일 디스크립터 번호>|<img src="https://user-images.githubusercontent.com/37530599/78252806-62843b80-752e-11ea-919d-c655b28a2f15.png" width="500px" heigth="400px"></img>|

	
