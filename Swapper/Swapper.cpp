#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>//C++
#include <conio.h>//getch
#include <Windows.h>//Sleep()
#include <fstream>//입출력
#include <iomanip>
#include <string>

using namespace std;

#define SIZE 3
#define FULLSIZE 9
#define SIZE_H 4
#define FULLSIZE_H 16
#define SHUFFLECOUNT 1000
#define USERCOUNT 32767


//변수 선언부
int menualselect; // 타이틀 화면 메뉴 선택값
int Difficultyselect; // 난이도 선택값

char Nickname[7]; // 해당 유저의 닉네임
string Nick; // 닉네임을 입력하지 않았을경우 임시로 사용할 닉네임 버퍼 
int user_number[32767] = { 0 }; // 유저 번호 중복여부를 저장할 배열변수
//함수 선언부

//커서 관련 함수
void gotoxy(int x, int y); // 커서 위치 이동 함수
void CursorView(bool show); // 입출력 커서 깜빡임

//타이틀 관련 함수
void Title(); // 타이틀 함수
void Print_LogoBox(); // 타이틀 화면 출력함수 - 1
void Print_Soild_LogoBox(); // 타이틀 화면 출력함수 - 2
void Print_GameName(); // 타이틀 화면 출력함수 - 3
void Print_VerSionAndCreator(); // 타이틀 화면 출력함수 - 4
int Title_Menual(int x , int y); // 메뉴 선택 함수

//게임 관련 함수
void clearMessage(); // 승리 메시지
void GameGuide(); // 게임 방법
void Input_nickname(); // 닉네임 입력 함수
int Difficulty(); // 난이도 선택 함수

int Game_Easy(int *score);//메인 게임 함수 - 쉬움 난이도
void printSlide_easy(string* slide); // 퍼즐 판 출력 함수 - 쉬움 난이도
void shuffle_easy(string* slide); // 퍼즐 판 섞기 함수 - 쉬움 난이도
void inputEvent_easy(string* slide, int* x, int* y); // 입력 이벤트 - 쉬움 난이도
bool checkClear_easy(string* slide); // 승리조건 검사 함수 - 쉬움 난이도

void Game_Hard();//메인 게임 함수 - 어려움 난이도
void printSlide_hard(string* slide); // 퍼즐 판 출력 함수 - 어려움 난이도
void shuffle_hard(string* slide); // 퍼즐 판 섞기 함수 - 어려움 난이도
void inputEvent_hard(string* slide, int* x, int* y); // 입력 이벤트 - 어려움 난이도
bool checkClear_hard(string* slide); // 승리조건 검사 함수 - 어려움 난이도

void printInfo(); // 승리 info - 인게임

//랭킹 관련 함수
void go_toRanking();


//타이틀 화면 출력 함수
void Title() {
	CursorView(0);
	Print_LogoBox();
	Print_Soild_LogoBox();
	Print_GameName();
	Print_VerSionAndCreator();
}

//입출력 커서 위치 이동 함수
void gotoxy(int x, int y) //커서 위치 이동함수. 가로  , 세로 최대 25
{
	COORD Print_Pos; //입출력 커서의 좌표값을 지정할 좌표클래스
	Print_Pos.X = x; // 가로 좌표값을 받은 매개값으로 지정
	Print_Pos.Y = y; // 세로 좌표값을 받은 매개값으로 지정
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Print_Pos);
}

//커서 깜빡임
void CursorView(bool show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

//글자와 바탕색 설정
void scolor(short text = 15, short back = 16)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

//메뉴 화면의 기능 함수
int Title_Menual(int x, int y)
{
	int mum = 1; // 선택 메뉴번호가 몇번인지 암시 1 = 게임시작 , 2 = 랭킹 보기
	int put; // 키보드 입력
	int max = 0; // 최대 메뉴 개수 값 해당 프로그램에서는 2가 최대
	while (true)
	{
		scolor(7, 0); max = 0; // 메뉴리스트 글씨와 배경을 각각 하얀색 글씨, 검은색 배경으로 바꿈
		gotoxy(x, y+3+max); max++; if (mum == max) { scolor(12, 0); cout << " ■"; scolor(7, 0); }
		else cout << " □"; cout << " 게임 시작";
		gotoxy(x, y+3+max); max++; if (mum == max) { scolor(12, 0); cout << " ■"; scolor(7, 0); }
		else cout << " □"; cout << " 게임 방법";
		gotoxy(x, y + 3 + max); max++; if (mum == max) { scolor(12, 0); cout << " ■"; scolor(7, 0); }
		else cout << " □"; cout << " 게임 종료";
		put = _getch();
		if (put == 224)
		{
			put = _getch();
			if (put == 72) { mum--; if (mum < 1)mum = max; }//mum
			if (put == 80) { mum++; if (mum > max)mum = 1; }//put
		}
		if (put == 13)
		{
			for (int end = 0; end < max; end++)
			{
				if (mum == end) return end;// 메인 메뉴에서 선택된 값을 반환.
			}
		}
		if (put == 13 && mum == max || put == 27)return -1;//종료
	
	}
}

//타이틀 화면 출력함수 1 - 네모난 제목 로고박스를 출력하는 함수
void Print_LogoBox() //네모난 제목 로고박스를 출력하는 함수
{
	int Title_bottom_pos = 83; // 아래에서 오른쪽 -> 왼쪽으로 가기위해 설정해놓은 좌표값 건드리지마세요


	gotoxy(15, 5);
	cout << "┏";
	for (int i = 0; i < 34; i++)
	{
		Sleep(15);
		cout << "━";
	}
	cout << "┓" << endl;
	Sleep(15);
	for (int i = 6; i < 20; i++)
	{
		gotoxy(85, i);
		cout << "┃" << endl;
		Sleep(15);
	}
	gotoxy(85, 20);
	cout << "┛" << endl;
	Sleep(15);

	for (int i = 0; i < 34; i++)
	{
		gotoxy(Title_bottom_pos, 20);
		Sleep(15);
		cout << "━";
		Title_bottom_pos -= 2;
	}
	gotoxy(15, 20);
	cout << "┗" << endl;
	Sleep(15);
	for (int i = 19; i > 5; i--)
	{
		gotoxy(15, i);
		cout << "┃" << endl;
		Sleep(15);
	}

}  

//타이틀 화면 출력함수 2 - 로고박스 주위 알파벳 돌게하기
void Print_Soild_LogoBox()
{
	int Title_bottom_pos = 83; // 아래에서 오른쪽 -> 왼쪽으로 가기위해 설정해놓은 좌표값 건드리지마세요


	gotoxy(15, 4);
	cout << "◈";
	for (int i = 0; i < 34; i++)
	{
		Sleep(15);
		cout << "◈";
	}
	cout << "◈" << endl;
	Sleep(15);
	for (int i = 6; i < 20; i++)
	{
		gotoxy(87, i);
		cout << "◈" << endl;
		Sleep(15);
	}
	gotoxy(87, 20);
	cout << "◈" << endl;
	Sleep(15);

	for (int i = 0; i < 34; i++)
	{
		gotoxy(Title_bottom_pos, 21);
		Sleep(15);
		cout << "◈";
		Title_bottom_pos -= 2;
	}
	gotoxy(15, 21);
	cout << "◈" << endl;
	Sleep(15);
	for (int i = 19; i > 5; i--)
	{
		gotoxy(13, i);
		cout << "◈" << endl;
		Sleep(15);
	}
}

//타이틀 화면 출력함수 3 - 게임 제목을 표현하는 함수
void Print_GameName()
{
	gotoxy(19, 10);
	cout << " SSS   W             W      A       PPPP   PPPP   EEEE   RRRR" << endl;
	Sleep(75);
	gotoxy(19, 11);
	cout << "S       W           W      A A      P   P  P   P  E      R   R" << endl;
	Sleep(75);
	gotoxy(19, 12);
	cout << " SS      W    W    W      A   A     PPPP   PPPP   EEEE   RRRR" << endl;
	Sleep(75);
	gotoxy(19, 13);
	cout << "   S      W W   W W      AAAAAAA    P      P      E      R  R" << endl;
	Sleep(75);
	gotoxy(19, 14);
	cout << "SSS  .     W     W    . A       A . P    . P    . EEEE . R   R" << endl;
	Sleep(75);
}

//타이틀 화면 출력함수 4 - 게임 버전, 제작자 출력함수
void Print_VerSionAndCreator()
{

	gotoxy(83, 29);
	cout << "제작팀 : S.W.A.P ";
	gotoxy(60, 30);
	cout << "제작자 : 2팀 문지훈,박천혁,임성현,장기태";
	gotoxy(1, 1);
	cout << "ver 1.0 Beta";
}

//게임 설명 함수
void GameGuide() {
		int put;
		gotoxy(27, 1);
		cout << "━━━━━━━  게임 방법 설명 ━━━━━━━━" << endl;
		Sleep(75);
		gotoxy(3, 3);
		cout << "방향키로 특별하게 생긴 커서를 조작하여 배열의 자리를 바꿔가며 흐트러진 숫자 퍼즐을 정렬하세요." << endl;
		Sleep(75);
		gotoxy(18, 5);
		cout << "난이도(쉬움, 어려움)에 따라, 3x3 또는 4x4의 퍼즐판이 주어집니다." << endl;
		Sleep(75);
		gotoxy(15, 7);
		cout << "커서 이동횟수는 200회 제한이며, 게임이 시작되면 9999점이 주어집니다!" << endl;
		Sleep(75);
		gotoxy(28, 9);
		cout << "커서를 이동 할 때 마다 감점이 이루어지며" << endl;
		Sleep(75);
		gotoxy(10, 11);
		cout << "이동횟수를 모두 사용하면 자리에 맞지않는 수에 대해 추가 감정이 이루어 집니다." << endl;
		gotoxy(3, 13);
		cout << "숫자퍼즐을 정방향 또는 역방향으로 완벽하게 정렬하거나, 퍼즐 판의 1,1 위치를 기준으로 잡아" << endl;
		Sleep(75);
		gotoxy(25, 15);
		cout << "가로 방향으로 1부터 시작하여 2씩 더해지는 수열," << endl;
		Sleep(75);
		gotoxy(15, 17);
		cout << "세로 방향으로 1부터 시작하여 2씩 곱해지는 수열이 완성되면 당신의 승리!" << endl;
		Sleep(75);
		gotoxy(13, 19);
		cout << "자리에 맞지 않는 수가 지나치게 많거나, 점수가 0점이 되버리면 당신의 패배입니다." << endl;
		Sleep(75);
		gotoxy(23, 21);
		cout << "중간에 R키를 누르면, 게임을 초기화 시킬 수 있습니다!" << endl;
		Sleep(75);
		gotoxy(28, 23);
		cout << "이동횟수, 점수 모두 맨 처음으로 돌아옵니다." << endl;
		Sleep(75);
		gotoxy(28, 25);
		cout << "단 한번만 사용할 수 있으니 신중히 고민하세요!" << endl;
		Sleep(75);
		gotoxy(20, 27);
		cout << "설명을 모두 읽으셨다면 게임시작을 위해 엔터키를 눌러주세요." << endl;
		Sleep(75);
		put = _getch();
}

//닉네임 입력 함수 
void Input_nickname() {
	system("cls");
	gotoxy(35, 10);
	cout << "본격적인 게임 시작에 앞서," << endl;
	Sleep(75);
	gotoxy(34, 11);
	cout << "한가지 궁금한 것이 있습니다." << endl;
	Sleep(75);
	gotoxy(31, 12);
	cout << "당신의 이름(닉네임)은 무엇입니까? ";
	gotoxy(31, 13);
	cout << "이름 : " << endl;
	Sleep(75);
	CursorView(1);
	gotoxy(37, 13);
	cin.getline(Nickname, 7);

	if (strlen(Nickname) == 0) // 닉네임이 공란일시
	{
		srand(unsigned int(time(0)));
		int rand_num;
		while (true)
		{
			rand_num = 1 + rand() % 32767;
			if (user_number[rand_num] == 0) // 해당 유저 번호가 중복되지 않은 번호일 경우
			{ 
				user_number[rand_num]++; // 중복 여부가 중복으로 바뀐 후 , 해당 유저 번호로 닉네임을 결정
				break;
			}
			else
				continue; // 중복된 번호이므로 번호를 다시 부여함.
		}
		Nick = "유저" + to_string(rand_num);
		system("cls");
		gotoxy(35, 10);
		cout << "OK! " << Nick <<"씨, 반갑습니다."<< endl;
		Sleep(1000);
		gotoxy(26, 11);
		cout << "부디 이 게임에서 승리해서 높은 점수를 기록하고" << endl;
		Sleep(1000);
		gotoxy(34, 12);
		cout << "높은 순위를 가지시길 바랍니다.";
		Sleep(1000);
		gotoxy(30, 13);
		cout << "행운을 빕니다. 그럼 이제 시작합시다!";
		gotoxy(65, 29);
		cout << "3초 후에 다음 화면으로 넘어갑니다...";
		Sleep(3000);
		system("cls");
	}
	else
	{
		system("cls");
		gotoxy(34, 10);
		cout << "OK! " << Nickname << "씨, 반갑습니다." << endl;
		Sleep(1000);
		gotoxy(24, 11);
		cout << "부디 이 게임에서 승리해서 높은 점수를 기록하고" << endl;
		Sleep(1000);
		gotoxy(34, 12);
		cout << "높은 순위를 가지시길 바랍니다.";
		Sleep(1000);
		gotoxy(30, 13);
		cout << "행운을 빕니다. 그럼 이제 시작합시다!";
		gotoxy(65, 29);
		cout << "3초 후에 다음 화면으로 넘어갑니다...";
		Sleep(3000);
		system("cls");
	}
}

//난이도 선택 함수
int Difficulty()
{
	CursorView(0);
	int x = 42, y = 20;
	Print_GameName();

	gotoxy(40, 18);
	cout << "난이도를 선택해주세요.";

	int mum = 1;
	int put; // 키보드 입력
	int max = 0;
	while (true)
	{
		scolor(7, 0); max = 0; // 메뉴리스트 글씨와 배경을 각각 하얀색 글씨, 검은색 배경으로 바꿈
		gotoxy(x, y + 3 + max); max++; if (mum == max) { scolor(12, 0); cout << " ■"; scolor(7, 0); }
		else cout << " □"; cout << " 쉬움(3x3)";
		gotoxy(x, y + 3 + max); max++; if (mum == max) { scolor(12, 0); cout << " ■"; scolor(7, 0); }
		else cout << " □"; cout << " 어려움(4x4)";

		put = _getch();
		if (put == 224)
		{
			put = _getch();
			if (put == 72) { mum--; if (mum < 1)mum = max; }//mum
			if (put == 80) { mum++; if (mum > max)mum = 1; }//put
		}
		if (put == 13)
		{
			for (int end = 0; end < max; end++)
			{
				if (mum == end) return end;
			}
		}
		if (put == 13 && mum == max) return 2;
	}
}

int Show_End_Game()
{
	char input;

	system("cls");

	gotoxy(45, 12);
	cout << "게임종료" << endl;
	gotoxy(35, 15);
	cout << "아무키나 입력하면 종료됩니다." << endl;

	input = _getch();
	return 1;
}

//게임 함수 - 쉬움 난이도
int Game_Easy(int *score) {
	string* slide = new string[FULLSIZE];
	int x, y;
	int move = 10;
	int &rScore = *score;
	x = 2, y = 2;

	for (int i = 0; i < FULLSIZE; i++) {
		if (i == FULLSIZE - 1) slide[i] = "★";
		else slide[i] = to_string(i);
	}

	shuffle_easy(slide);

	while (true) {
		
		if (move == 0)
			return Show_End_Game();
		char input;

		system("cls");

		printInfo();
		printSlide_easy(slide);

		if (checkClear_easy(slide)) {
			clearMessage();
			break;
		}

		gotoxy(80, 5);
		cout << move << endl;
		gotoxy(80, 8);
		cout << rScore << endl;

		input = _getch();

		switch (input) {

		case 'w':

			if (y > 0) y--;
			break;

		case 'a':

			if (x > 0) x--;
			break;

		case 's':

			if (y < 2) y++;
			break;

		case 'd':
			if (x < 2) x++;
			break;
		}
		inputEvent_easy(slide, &x, &y);
		move--;
		rScore -= 33;
	}
}

//퍼즐판 출력 함수 - 쉬움 난이도
void printSlide_easy(string* slide) {

	for (int i = 0; i < SIZE; i++) {

		for (int j = 0; j < SIZE; j++) {

			cout << slide[i * SIZE + j] << "\t";

		}
		cout << endl << endl;
	}

}

//퍼즐판 섞기 함수 - 쉬움 난이도
void shuffle_easy(string* slide) {

	srand((unsigned)time(NULL));

	for (int i = 0; i < SHUFFLECOUNT; i++) {

		int sour = rand() % 8;
		int dest = rand() % 8;
		string temp = slide[sour];
		slide[sour] = slide[dest];
		slide[dest] = temp;

	}

}

//퍼즐 내 입력 함수 - 쉬움 난이도
void inputEvent_easy(string* slide, int* x, int* y) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (slide[i*SIZE + j] == "★") {

				string temp = slide[*y * SIZE + *x];
				slide[*y * SIZE + *x] = "★";
				slide[i * SIZE + j] = temp;

			}
		}
	}
}

//승리조건 검사 함수 - 쉬움 난이도
bool checkClear_easy(string* slide)
{
	bool isFinish = true;
	for (int i = 0; i < FULLSIZE - 1; i++) {
		if (slide[i] != to_string(i)) isFinish = false;
	}

	return isFinish;
}

//게임 함수 - 어려움 난이도
int Game_Hard(int* score) {
	string* slide = new string[FULLSIZE_H];
	int move = 30;
	int& rScore = *score;
	int x, y;

	x = 2, y = 2;

	for (int i = 0; i < FULLSIZE_H; i++) {
		if (i == FULLSIZE_H - 1) slide[i] = "★";
		else slide[i] = to_string(i);
	}

	shuffle_hard(slide);

	while (true) {

		if (move == 0)
			return Show_End_Game();

		char input;

		system("cls");

		printInfo();
		printSlide_hard(slide);

		if (checkClear_hard(slide)) {
			clearMessage();
			break;
		}

		gotoxy(80, 5);
		cout << move << endl;
		gotoxy(80, 8);
		cout << rScore << endl;

		input = _getch();

		switch (input) {

		case 'w':

			if (y > 0) y--;
			break;

		case 'a':

			if (x > 0) x--;
			break;

		case 's':

			if (y < 3) y++;
			break;

		case 'd':
			if (x < 3) x++;
			break;
		}
		inputEvent_hard(slide, &x, &y);
		move--;
		rScore -= 33;
	}
}

//퍼즐판 출력 함수 - 어려움 난이도
void printSlide_hard(string* slide) {

	for (int i = 0; i < SIZE_H; i++) {

		for (int j = 0; j < SIZE_H; j++) {

			cout << slide[i * SIZE_H + j] << "\t";

		}
		cout << endl << endl;
	}

}

//퍼즐판 섞기 함수 - 어려움 난이도
void shuffle_hard(string* slide) {

	srand((unsigned)time(NULL));

	for (int i = 0; i < SHUFFLECOUNT; i++) {

		int sour = rand() % 15;
		int dest = rand() % 15;
		string temp = slide[sour];
		slide[sour] = slide[dest];
		slide[dest] = temp;

	}

}

//퍼즐 내 입력 함수 - 어려움 난이도
void inputEvent_hard(string* slide, int* x, int* y) {

	for (int i = 0; i < SIZE_H; i++) {
		for (int j = 0; j < SIZE_H; j++) {
			if (slide[i*SIZE_H + j] == "★") {

				string temp = slide[*y * SIZE_H + *x];
				slide[*y * SIZE_H + *x] = "★";
				slide[i * SIZE_H + j] = temp;

			}
		}
	}
}

//승리조건 검사 함수 - 어려움 난이도
bool checkClear_hard(string* slide)
{
	bool isFinish = true;
	for (int i = 0; i < FULLSIZE_H - 1; i++) {
		if (slide[i] != to_string(i)) isFinish = false;
	}

	return isFinish;
}

//랭킹창 가기 함수
void go_toRanking()
{
	int input;

	system("cls");
	cout << "랭킹보기 1페이지" << endl;
	cout << "다음페이지: 엔터키 " << endl;
	cout << "타이틀로 돌아가기 : ESC키 " << endl;
	while (true)
	{
		input = _getch();

		if (input == 13)
		{
			system("cls");
			cout << "랭킹보기 2페이지" << endl;
		}
		else if (input == 27)
		{
			system("cls");
			Title();
			menualselect = Title_Menual(43, 20);
			break;
		}
	}
}


//승리정보
void printInfo() {

	cout << endl;
	cout << "==================================" << endl << endl;
	cout << "모든 숫자를 정렬하면 클리어됩니다." << endl << endl;
	cout << "==================================" << endl << endl;

}

//승리 메시지
void clearMessage(){
	cout << "===========================" << endl << endl;
	cout << "모든 숫자를 정렬하였습니다." << endl << endl;
	cout << "===========================" << endl << endl;
}

int main(void)
{
	int score = 1000;
	int &rScore = score;

	CursorView(0);
	system("title S.W.A.P.P.E.R - 스와핑 퍼즐게임!");
	system("mode con: cols=101 lines=31");
	
	while (true)
	{
		system("cls");
		Title();

		menualselect = Title_Menual(43, 20);
		system("cls");

		if (menualselect == -1)
		{
			cout << "게임 종료됨" << endl;
			return 0;
		}
		if (menualselect == 1)
		{
			system("cls");
			Difficultyselect = Difficulty();
			if (Difficultyselect == 1)
			{
				system("cls");
				if (1 == Game_Easy(&score))
				{
					rScore = 1000;
						continue;
				}
			}
			else if (Difficultyselect == 2)
			{
				system("cls");
				if (1 == Game_Hard(&score))
				{
					rScore = 1000;
					continue;
				}
			}
		}
		else if (menualselect == 2)
		{
			int input;

			system("cls");
			GameGuide();
		}
	}
}