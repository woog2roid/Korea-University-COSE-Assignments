#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>


// COSE101, Spring Semester, 2019
// Computer Science and Engineering, College of Informatics, Korea University, Seoul.

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define SPACE 32 
#define p 112 
#define P 80 
#define ESC 27

#define MAP_ADJ_X 10
#define MAP_ADJ_Y 5
#define X 8
#define Y 5

//구조체 확인
typedef struct _block {
	int pos_x; //position of block
	int pos_y;
	int num;
	int nextnum;
	int isactive;
}Block;

Block block, block2;

/*'이 함수들을 고치지 마세요.'	 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd에서 커서를 x, y 위치로 세팅

/* display fuctions */
void goMainMenuScreen(); // 메인메뉴로 이동 
void printGameScreen();  // 게임화면 출력	
void printPauseScreen(); // 일시 정지 화면	
void printEndScreen(int endType); // 스테이지 종료 화면 출력. 스테이지클리어, 모든스테이지클리어, 패배.

void clearScreen();
/*'이 함수들을 고치지 마세요' 끝.*/
//===============================================================================================================//


/*이 아래에있는 함수들을 고치세요.  */
//이 함수들을 구현하셔서 프로젝트를 완성하세요.

/* 블록 제작 함수 */
int getRandomNumber();
void newBlock();

/* 블록 컨트롤 및 합성 함수  */
int takeBlockControl();				//블록 조작 방향 입력 받음	
void flipBlock();					// Fliping function.
void moveBlock(int direction);		// 입력 받은 방향으로 블록 이동	
int checkAdjacentBlock(int, int, int* cntp);	//Checking adjacent blocks for merging conditions
void checkNumber(int, int, int* cntp);			// 블록 인접 블록숫자 확인	
		/* game flow control functions */
int isStageEnd(int* cntp);  // 스테이지가 끝났는지 체크. (스테이지 클리어, 모든 스테이지 클리어, 패배)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // 각 스테이지에 맞는 게임 환경 세팅.   Setting up status values for each stages.

//takeBlockControl 클리어
//flip Block 클리어
//move Block 클리어
//ADjacentBlock 클리어
//isStageEnd클리어
//SetGameFLow 클리어

//빼기 X 차이 ㅇ
int dif(int a, int b)
{
	if (a - b > 0)
	{
		return (a - b);
	}
	else
	{
		return (b - a);
	}
}



int time_interval_moveBlockDown = 100;
int goal = 64;
int currentStage = 0;//현재스테이지 레벨
int score = 0;
int best_score = 0;
int gameScreen[X][Y] = { 0 };

//귀찮으니까 한번에
void setting(int x, int y, int* cntp)
{
	if (gameScreen[x][y] == 0)
	{
		gameScreen[x][y] = 999;
	}

	if (gameScreen[x][y] == 64)
	{
		gameScreen[x][y] = 0;
		(*cntp)++;
		if (*cntp == 2)
		{
			score += 10;
		}
		if (*cntp > 2)
		{
			score += 10;
		}
	}
}
//함수 하나 더 만들자!!!!!! 밑으로 쿵 하는 함수
void KUNG(void)
{
	for (int j = 6; j >= 0; j--)//얘는 위아래
	{
		//얘는 좌우
		for (int i = 0; i < Y; i++) {
			if (gameScreen[j+1][i] == 0 && gameScreen[j][i]!=0) { 
				gameScreen[j+1][i]=gameScreen[j][i];
				gameScreen[j][i]=0;
				j++;
			}
		}
	}
}


void main() {
	int cnt = 0;
	int* cntp = &cnt;

	system("mode con cols=120 lines=38");
	srand(time(NULL));
	removeCursorFromCMD();
	goMainMenuScreen();//초기화면+넘어가기

	while (1) {


		for (int j = 0; j < 5; j++) {
			if (takeBlockControl() == SPACE) break;


			printGameScreen();
			Sleep(time_interval_moveBlockDown);
		}
		moveBlock(DOWN);
		if (!block.isactive && !block2.isactive) { // 이 부분은 떨어지는 블록이 바닥이나 다른블록에 닿았는지 체크합니다. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
			checkNumber(block.pos_x, block.pos_y, cntp);
			KUNG();
			if (isStageEnd(cntp) != 1) //cntp는 64가 몇개만들어졌는지 카운터 하는놈임, 별일없으면 새블록 만드는 놈, 별일 있으면 게임 끝내는 놈
				newBlock();
		}
	}
}

//clear!!
int getRandomNumber() {
	/*

	일정 확률로 2, 4, 8 중 하나를 생성하여 그 값을 리턴함.
	Generate 2, 4, or 8 with certain percentage and return the number.

	2는 50퍼 4는 30퍼 8은 20퍼
	*/
	int i = rand() % 10;
	if (i < 5)
	{
		return 2;
	}
	else if (i >= 5 && i <= 7)
	{
		return 4;
	}
	else
	{
		return 8;
	}
}

//clear!?
char getRandomOper() {
	/*

	일정 확률로 +, - 중 하나를 생성하여 그 값을 리턴함.
	Generate +, or - with certain percentage and return the number.

	*/
	if (currentStage < 4)
	{
		return '+';
	}
	else
	{
		int i = rand() % 2;
		if (i == 0)
		{
			return '+';
		}
		else
		{
			return '-';
		}
	}
	// You should modify example value for complete this function. 예시값을 고치셔서 함수를 완성하세요. 	
}

//clear!!
void newBlock() {
	/*

	이 함수는 생성된 값과 떨어지기 시작하는 위치를 입력해서 새 블록을 만듭니다.
	그리고 나서, 다음 블록에 사용될 새 값을 생성합니다.
	This fuction makes new blocks with setting generated value and the initial position where starts falling down.
	After that, generate new number for value of the next block.

	*/

	// You should modify these example values for complete this function.
	if (block.nextnum != 0)
		block.num = block.nextnum;
	else
		block.num = getRandomNumber();

	if (block.nextnum != 0)
		block2.num = block2.nextnum;
	else
		block2.num = getRandomOper();

	block.nextnum = getRandomNumber();
	block2.nextnum = getRandomOper();


	int i = rand() % 4;
	block.pos_x = 0; // top
	block.pos_y = i; // i는 0부터 3까지
	block.isactive = 1;
	gameScreen[block.pos_x][block.pos_y] = block.num;

	block2.pos_x = 0; // top
	block2.pos_y = i + 1; // 한칸 오른쪽
	block2.isactive = 1;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;

}

//clear!!
int takeBlockControl() {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //방향키인경우  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//방향키지시값을 버림  dropping the value of direction
			switch (input_blockControl) {
			case UP:
				flipBlock();
				break;
			case DOWN:
				moveBlock(DOWN);
				break;
			case LEFT:
				moveBlock(LEFT);
				break;
			case RIGHT:
				moveBlock(RIGHT);
				break;
			}
		}
		else { //방향키가 아닌경우  Cases for other keys
			switch (input_blockControl) {

				//Implement hard drop  하드드랍을 구현하세요
			case SPACE:
				while (block.isactive != 0 || block2.isactive != 0)
					moveBlock(DOWN);
				score += 5;
				break;

			case P:
			case p:
				printPauseScreen();
				break;
			case ESC:
				printEndScreen(0);
				break;
			}
		}
	}
	while (_kbhit()) _getch(); //키버퍼를 비움. Emptying key buffer.
	return 0;
}


//clear!!
void flipBlock(void) {//좌우 반전함수		Block flipping function
		/*
		Fliping blocks when the 'up' butten is pressed.
		위로 버튼이 눌렸을경우 블록을 바꿉니다.
		*/
	gameScreen[block.pos_x][block.pos_y] = block2.num;
	gameScreen[block2.pos_x][block2.pos_y] = block.num;
	int tmp_x, tmp_y;
	tmp_x = block2.pos_x;	tmp_y = block2.pos_y;

	block2.pos_x = block.pos_x;
	block2.pos_y = block.pos_y;

	block.pos_x = tmp_x;
	block.pos_y = tmp_y;
}


//clear!!
void moveBlock(int direction) { // 좌,우,아래 입력시 움직임 함수	Moving blocks for left, reight, and down input
	switch (direction) {
	case LEFT:

		if (block.pos_y < block2.pos_y) {
			if (block.pos_y > 0 && gameScreen[block.pos_x][block.pos_y - 1] == 0) {
				gameScreen[block.pos_x][block.pos_y - 1] = block.num;
				gameScreen[block.pos_x][block.pos_y] = 0;
				block.pos_y--;
			}
			else {
				block.isactive = 0;
			}

			if (block2.pos_y > 0 && gameScreen[block2.pos_x][block2.pos_y - 1] == 0) {
				gameScreen[block2.pos_x][block2.pos_y - 1] = block2.num;
				gameScreen[block2.pos_x][block2.pos_y] = 0;
				block2.pos_y--;
			}
			else {
				block2.isactive = 0;
			}

			break;
		}
		else
		{
			if (block2.pos_y > 0 && gameScreen[block2.pos_x][block2.pos_y - 1] == 0) {
				gameScreen[block2.pos_x][block2.pos_y - 1] = block2.num;
				gameScreen[block2.pos_x][block2.pos_y] = 0;
				block2.pos_y--;
			}
			else {
				block2.isactive = 0;
			}
			if (block.pos_y > 0 && gameScreen[block.pos_x][block.pos_y - 1] == 0) {
				gameScreen[block.pos_x][block.pos_y - 1] = block.num;
				gameScreen[block.pos_x][block.pos_y] = 0;
				block.pos_y--;
			}
			else {
				block.isactive = 0;
			}
			break;
		}
	case RIGHT:
		if (block.pos_y < block2.pos_y) {
			if (block2.pos_y < Y - 1 && gameScreen[block2.pos_x][block2.pos_y + 1] == 0) {
				gameScreen[block2.pos_x][block2.pos_y + 1] = block2.num;
				gameScreen[block2.pos_x][block2.pos_y] = 0;
				block2.pos_y++;
			}
			else {
				block2.isactive = 0;
			}

			if (block.pos_y < Y - 1 && gameScreen[block.pos_x][block.pos_y + 1] == 0) {
				gameScreen[block.pos_x][block.pos_y + 1] = block.num;
				gameScreen[block.pos_x][block.pos_y] = 0;
				block.pos_y++;
			}
			else {
				block.isactive = 0;
			}

			break;
		}
		else
		{
			if (block.pos_y < Y - 1 && gameScreen[block.pos_x][block.pos_y + 1] == 0) {
				gameScreen[block.pos_x][block.pos_y + 1] = block.num;

				gameScreen[block.pos_x][block.pos_y] = 0;
				block.pos_y++;
			}
			else {
				block.isactive = 0;
			}

			if (block2.pos_y < Y - 1 && gameScreen[block2.pos_x][block2.pos_y + 1] == 0) {
				gameScreen[block2.pos_x][block2.pos_y + 1] = block2.num;
				gameScreen[block2.pos_x][block2.pos_y] = 0;
				block2.pos_y++;
			}
			else {
				block2.isactive = 0;
			}


			break;
		}
	case DOWN:
		if (block.pos_x < X - 1 && gameScreen[block.pos_x + 1][block.pos_y] == 0) {
			gameScreen[block.pos_x + 1][block.pos_y] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_x++;
		}
		else {
			block.isactive = 0;
		}

		if (block2.pos_x < X - 1 && gameScreen[block2.pos_x + 1][block2.pos_y] == 0) {
			gameScreen[block2.pos_x + 1][block2.pos_y] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_x++;
		}
		else {
			block2.isactive = 0;
		}
	}
}


//x,y는 방금 떨어진 숫자블록의 position 
int checkAdjacentBlock(int x, int y, int* cntp) { //Merging 조건 확인 함수		Checking merging condition	---xy엔 숫자블록 위치가 들어감
	 /*

	 조건확인하여 연산하고 Merging//clear!!

	  64 생성시 점수 계산
	 */
	 // 0 for none, 1 for mergable.  예시값. 0이면 합칠 블록이 없고, 1이면 있음.	(후에 IF 문에서 이를 확인 할거임)	

	if ((y == 2) && (gameScreen[x][y + 1] == '+' || gameScreen[x][y + 1] == '-') && (gameScreen[x][y - 1] == '+' || gameScreen[x][y - 1] == '-')
		&& ((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x][y + 2] != '+') && (gameScreen[x][y + 2] != '-') && (gameScreen[x][y + 2] != 999) && (gameScreen[x][y + 2] > 0))
		&& ((gameScreen[x][y - 2] != '+') && (gameScreen[x][y - 2] != '-') && (gameScreen[x][y - 2] != 999) && (gameScreen[x][y - 2] > 0)))
	{
		if ((gameScreen[x][y + 1] == '+') && (gameScreen[x][y - 1] == '+'))
		{
			gameScreen[x][y - 2] = gameScreen[x][y - 2] + gameScreen[x][y] + gameScreen[x][y + 2];
			gameScreen[x][y] = 0;
			gameScreen[x][y + 2] = 0;
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y - 1] = 0;
			setting(x, y-2, *cntp);
		}
		else if ((gameScreen[x][y + 1] == '-') && (gameScreen[x][y - 1] == '+'))
		{
			gameScreen[x][y - 2] = dif((gameScreen[x][y - 2] + gameScreen[x][y]) , gameScreen[x][y + 2]);
			gameScreen[x][y] = 0;
			gameScreen[x][y + 2] = 0;
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y - 1] = 0;
			setting(x, y-2, *cntp);
		}
		else if ((gameScreen[x][y + 1] == '+') && (gameScreen[x][y - 1] == '-'))
		{
			gameScreen[x][y - 2] = dif(gameScreen[x][y - 2] , gameScreen[x][y] + gameScreen[x][y + 2]);
			gameScreen[x][y] = 0;
			gameScreen[x][y + 2] = 0;
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y - 1] = 0;
			setting(x, y-2, *cntp);
		}
		else if ((gameScreen[x][y + 1] == '-') && (gameScreen[x][y - 1] == '-'))
		{
			gameScreen[x][y - 2] = dif(dif(gameScreen[x][y - 2], gameScreen[x][y]), gameScreen[x][y + 2]);
			gameScreen[x][y] = 0;
			gameScreen[x][y + 2] = 0;
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y - 1] = 0;
			setting(x, y-2, *cntp);
		}
	}
	
	else if ((y < 3) && (gameScreen[x][y + 1] == '+')
		&& ((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x][y + 2] != '+') && (gameScreen[x][y + 2] != '-') && (gameScreen[x][y + 2] != 999) && (gameScreen[x][y + 2] > 0)))
	{
		gameScreen[x][y] += gameScreen[x][y + 2];
		gameScreen[x][y + 2] = 0;
		gameScreen[x][y + 1] = 0;

		if (gameScreen[x][y] == 64)
		{
			gameScreen[x][y] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}

		return 1;
	}//xy기준으로 오른쪽으로 +존재

	else if ((y > 1) && (gameScreen[x][y - 1] == '+')
		&& ((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x][y - 2] != '+') && (gameScreen[x][y - 2] != '-') && (gameScreen[x][y - 2] != 999) && (gameScreen[x][y - 2] > 0)))
	{
		gameScreen[x][y - 2] += gameScreen[x][y];

		gameScreen[x][y] = 0;
		gameScreen[x][y - 1] = 0;

		if (gameScreen[x][y - 2] == 64)
		{
			gameScreen[x][y - 2] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}
		return 1;
	}//xy기준으로 왼쪽으로 +존재

	else if ((y < 3) && (gameScreen[x][y + 1] == '-') &&
		((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x][y + 2] != '+') && (gameScreen[x][y + 2] != '-') && (gameScreen[x][y + 2] != 999) && (gameScreen[x][y + 2] > 0)))
	{
		if (gameScreen[x][y] > gameScreen[x][y + 2])
		{
			gameScreen[x][y] = gameScreen[x][y] - gameScreen[x][y + 2];
		}
		else
		{
			gameScreen[x][y] = gameScreen[x][y + 2] - gameScreen[x][y];
		}
		//0이면 쓰레기값으로 convert
		if (gameScreen[x][y] == 0)
		{
			gameScreen[x][y] = 999;
		}

		gameScreen[x][y] = 0;
		gameScreen[x][y - 1] = 0;

		if (gameScreen[x][y - 1] == 64)
		{
			gameScreen[x][y - 1] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}

		return 1;
	}//xy기준으로 오른쪽으로 -존재

	else if ((y > 1) && (gameScreen[x][y - 1] == '-') &&
		((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x][y - 2] != '+') && (gameScreen[x][y - 2] != '-') && (gameScreen[x][y - 2] != 999) && (gameScreen[x][y - 2] > 0)))
	{
		if (gameScreen[x][y] > gameScreen[x][y - 2])
		{
			gameScreen[x][y - 2] = gameScreen[x][y] - gameScreen[x][y - 2];
		}
		else
		{
			gameScreen[x][y - 2] = gameScreen[x][y - 2] - gameScreen[x][y];
		}
		//0이면 쓰레기값으로 convert
		if (gameScreen[x][y - 2] == 0)
		{
			gameScreen[x][y - 2] = 999;
		}

		gameScreen[x][y] = 0;
		gameScreen[x][y - 1] = 0;

		if (gameScreen[x][y - 2] == 64)
		{
			gameScreen[x][y - 2] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}

		return 1;
	}//xy기준으로 왼쪽으로 -존재

	else if ((x < 6) && (gameScreen[x + 1][y] == '+')
		&& ((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x + 2][y] != '+') && (gameScreen[x + 2][y] != '-') && (gameScreen[x + 2][y] != 999) && (gameScreen[x + 2][y] > 0)))
	{
		gameScreen[x + 2][y] += gameScreen[x][y];
		gameScreen[x][y] = 0;
		gameScreen[x + 1][y] = 0;

		return 1;

		if (gameScreen[x + 2][y] == 64)
		{
			gameScreen[x + 2][y] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}
	}//xy기준으로 아래쪽으로 +존재


	else if ((x < 6) && (gameScreen[x + 1][y] == '-')
		&& ((gameScreen[x][y] != '+') && (gameScreen[x][y] != '-') && (gameScreen[x][y] != 999) && (gameScreen[x][y] != 0))
		&& ((gameScreen[x + 2][y] != '+') && (gameScreen[x + 2][y] != '-') && (gameScreen[x + 2][y] != 999) && (gameScreen[x + 2][y] > 0)))
	{
		if (gameScreen[x][y] > gameScreen[x + 2][y])
		{
			gameScreen[x + 2][y] = gameScreen[x][y] - gameScreen[x + 2][y];
		}
		else
		{
			gameScreen[x + 2][y] = gameScreen[x + 2][y] - gameScreen[x][y];
		}
		//0이면 쓰레기값으로 convert
		if (gameScreen[x + 2][y] == 0)
		{
			gameScreen[x + 2][y] = 999;
		}

		if (gameScreen[x + 2][y] == 64)
		{
			gameScreen[x + 2][y] = 0;
			(*cntp)++;
			if (*cntp == 2)
			{
				score += 10;
			}
			if (*cntp > 2)
			{
				score += 10;
			}
		}

		gameScreen[x][y] = 0;
		gameScreen[x + 1][y] = 0;

		return 1;
	}//xy기준으로 아래쪽으로 -존재
	
	return 0;
	//아무것도 아닐때
}

//x,y는 방금 떨어진 숫자블록의 position 
void checkNumber(int x, int y, int* cntp) {

	//떨어진 애가 Merging 이 가능한가?를 확인하는 코드임
	checkAdjacentBlock(x, y, cntp);
	printGameScreen();
	Sleep(200);

	//그니까 얘는 아마도 계속 연쇄적으로 일어날 수 있는 Merging을 대비하는 코드인듯
	for (int i = X - 1; i > 0; i--) {
		for (int j = 0; j < Y; j++) {
			if (checkAdjacentBlock(i, j, cntp)) {
				printGameScreen();
				Sleep(200);
				i = X;
				break;
			}
		}
	}
}

//clear!!
void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  스테이지의 조건들을 설정하기위해 이 함수를 완성하세요.
	switch (setGameFlowType) {
	case 0:  /* initialize game */
		if (best_score < score)
			best_score = score;

		currentStage = 0;
		score = 0;

	case 1: /* To next stage */
		currentStage++;

		switch (currentStage) {
		case 1:
			time_interval_moveBlockDown = 100;
			goal = 100;
			break;
		case 2:
			time_interval_moveBlockDown = 76.923076;
			goal = 300;
			break;
		case 3:
			time_interval_moveBlockDown = 66.666666;
			goal =
				500;
			break;
		case 4:
			time_interval_moveBlockDown = 58.823529;
			goal = 600;
			break;
		case 5:
			time_interval_moveBlockDown = 50;
			goal = 750;
			break;
		case 6:
			time_interval_moveBlockDown = 40;
			goal = 850;
			break;
		}
		clearScreen();
		newBlock();
		break;

	default:
		clearScreen();
		setCoordinate(MAP_ADJ_X + 2, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}

int isStageEnd(int* cntp) { // 스테이지가 끝났는지 체크      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {
				if (currentStage < 6)
				{
					printEndScreen(1);
				}
				else
				{
					(*cntp) = 0;
					printEndScreen(2);
				}
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //패배			Game over
			(*cntp) = 0;
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}


/* @ ↓↓↓'DO NOT MODIFY FROM HERE.'↓↓↓ starts		 이 아래로는 수정하지 마세요.	*/
void removeCursorFromCMD() { // cmd에서 깜빡이는 커서없앰 Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void setCoordinate(int x, int y) { //cmd에서 커서 위치를 x, y로 세팅   Setting curser to (x,y).
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printGameScreen() {
	int i, j, k;

	for (i = 0; i < X; i++) {
		for (j = 0; j < Y; j++) {
			setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
			if (gameScreen[i][j] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("     ┿      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     ┿      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("             ");
				}
			}

			else {
				for (k = 0; k < 12;) if (1 << (++k) == gameScreen[i][j]) break;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
				if (gameScreen[i][j] == 45 || gameScreen[i][j] == 43)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("┌─────────┐ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("└─────────┘ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("┌─────────┐  ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);

					//999!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					//확인할것

					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);

					//!!!!!!!!!!!!!!!!
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("└─────────┘  ");
				}

			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y); printf(" Stage : %4d", currentStage);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 1); printf(" GOAL  : %4d", goal);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 2); printf(" SPEED  : %.1f", 100.0 / (float)time_interval_moveBlockDown);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 3); printf("+-  N E X T  -+ ");
	for (k = 0; k < 12;) if (1 << (++k) == block.nextnum) break;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  ┌─────────┐    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  └─────────┘    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  ┌─────────┐    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  └─────────┘    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  △   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("◁  ▷ : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ▽   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //시작화면 출력 함수. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("──────────────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("☆ ────────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o ☆ ─────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo ☆ ────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo ☆ ─────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo ☆ ────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─ 64 PuyoPuyo ☆─────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─── 64 PuyoPuyo ☆ ───────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──── 64 PuyoPuyo ☆ ──────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────  64 PuyoPuyo ☆ ────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────  64 PuyoPuyo ☆ ──────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────  64 PuyoPuyo ☆ ─────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────────  64 PuyoPuyo ☆ ────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──────────  64 PuyoPuyo ☆ ───────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────────  64 PuyoPuyo ☆ ──────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────────  64 PuyoPuyo ☆ ─────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────────────  64 PuyoPuyo ☆ ────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──────────────  64 PuyoPuyo ☆ ───");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────────────  64 PuyoPuyo ☆ ──");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────────────  64 PuyoPuyo ☆ ─");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" ▤▤▤▤ HOW TO CONTROL ▤▤▤▤");
	setCoordinate(x, y + 7);  printf(" ▤                            ▤");
	setCoordinate(x, y + 8);  printf(" ▤      △   : Flip           ▤");
	setCoordinate(x, y + 9); printf(" ▤    ◁  ▷ : Left / Right   ▤");
	setCoordinate(x, y + 10); printf(" ▤      ▽   : Soft Drop      ▤");
	setCoordinate(x, y + 11); printf(" ▤    SPACE  : Hard Drop      ▤");
	setCoordinate(x, y + 12); printf(" ▤      P    : Pause          ▤");
	setCoordinate(x, y + 13); printf(" ▤     ESC   : Quit           ▤");
	setCoordinate(x, y + 14); printf(" ▤                            ▤");
	setCoordinate(x, y + 15); printf(" ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	setCoordinate(x, y + 16); printf("          ESC: Exit game.     ");



	while (_kbhit())
		_getch();
	input_command = _getch();


	if (input_command == ESC) {
		clearScreen();
		exit(0);
	}
	else
		setGameFlow(0); /* Reset the game */

}

void printPauseScreen() { // 게임 일시정지		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	setCoordinate(x, y + 1); printf("▤                              ▤");
	setCoordinate(x, y + 2); printf("▤  +-----------------------+   ▤");
	setCoordinate(x, y + 3); printf("▤  |       P A U S E       |   ▤");
	setCoordinate(x, y + 4); printf("▤  +-----------------------+   ▤");
	setCoordinate(x, y + 5); printf("▤   Press any key to resume.   ▤");
	setCoordinate(x, y + 6); printf("▤                              ▤");
	setCoordinate(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	_getch();

	system("cls");
	printGameScreen();
}



void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  |  G A M E  O V E R..   |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤   YOUR SCORE: %6d         ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤   Press any key to restart.  ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 16); printf("      ESC: Back to main menu.     ");

		while (_kbhit()) _getch();
		input_command = _getch();

		if (input_command == ESC) {
			while (_kbhit()) _getch();
			clearScreen();
			goMainMenuScreen();
			return;
		}

		setGameFlow(0); /* Reset the game */
		break;

	case 1:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  |      STAGE CLEAR      |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤   YOUR SCORE: %6d         ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤ Press any key to next stage. ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-------------------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  | WINNER WINNER CHICKEN DINNER! |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-------------------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤         YOUR SCORE: %6d           ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤         Cleared final stage.         ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

		while (_kbhit()) _getch();
		input_command = _getch();
		setGameFlow(0); /* Reset the game */
		break;

	default:
		clearScreen();
		setCoordinate(x, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}
void clearScreen() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			gameScreen[i][j] = 0;
		}
	}
	block.pos_x = 0;
	block.pos_x = 0;
	block.num = 0;
	block.nextnum = 0;
	block.isactive = 0;

	block2.pos_x = 0;
	block2.pos_x = 0;
	block2.num = 0;
	block2.nextnum = 0;
	block2.isactive = 0;

	system("cls");
}
/* @ '↑↑↑DO NOT MODIFY.↑↑↑'ends. */
