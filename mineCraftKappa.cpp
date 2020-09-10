//Marina Stergiou 2546
//Vezos Konstantinos-Ioannis 2026
#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <sstream>

float rAngle  = 0.0;

using namespace std;




int ***backup; // the world of game.
int **terrainCubes; //only the first floor of the game.
int n;//size of world. Gives from the player.
int humanPosition[3];//the position of player.
float x1,y1,z1;//need it for smooth move camera.. gluLookAt..
float z2 = 0.5;//for comparison in seeUp or seeDown.
int rotate = 0;		//disable by default 1 == enable, 'S' enable it, rotate i mean: you will turn 90degrees.
int changes = 0;	//disalbe by default, 1 == not yet 0 == done, for 'R' to know if all cubes fell down.
int wait = 0;		//disable by default 1 == enable, for slideDown.
int firstTimeInSlideDown = 1; //enable by default, 0 disable.

int humanView = 0; // 0 = forward, 1 = right, 2 = back, 3 = left
int holdCubes = 0; // ta koutia pou krataei o paikths mas
int points = 50; //player start with 50points by default.
int life = 3;	//player start with 3lifes by default.

void goForward(); //check if you can go forward. if yes. call the upOrDownByX or upOrDownByY. humanView control this.
void upOrDownByX(int);
void upOrDownByY(int);
void goRightOrLeft(int);
void slowRightOrLeft(int);
void drawScene();//drawDisplay.
void grabCube();//grab one cube. ' ' spacebar.
void buildCubeX(int);//left click to build looking right == 1 or left == -1
void buildCubeY(int);//left click to build looking forward == -1 or back == 1.
void destroyCube();//only one Cube action q.
void destroyCubeY(int);
void destroyCubeX(int);
void destroyCubes();// its Cubes action E.
void destroyCubesByX(int);
void destroyCubesByY(int);
void slideDown(int);//collapse.
void kickCubeByX(int);//right click
void kickCubeByY(int);
void seeDownOrUp(int); //1 == see up, 0 == seeDown
void slowSeeDown(int);
void slowSeeUp(int);
void createGame(); //constactur for the game.
void restart();	//if u want to play again. player give 'y' or 'Y' for yes 'n' or 'N' for no
void colorCube(int);

//-------------------------------------------------------------------------------------------
//	 normal keys from Keyboard.
//--------------------------------------------------------------------------------------------
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			glutLeaveMainLoop(); //exit(0)
		case ' ':
			//grabCube 1 per space.
			grabCube();
			colorCube(0);
			break;
		case 'W':
		case 'w': 
			//forward
			goForward();
			break;
		case 's':
		case 'S':
			//back
			if (humanView == 0 || humanView == 1 || humanView == 2 || humanView == 3 ) {
				if ((rand()%50) % 2 == 0) goRightOrLeft(rotate = 1);
				else goRightOrLeft(rotate = -1);
			
			}
			break;
		case 'd':
		case 'D':
			//right
			goRightOrLeft(1);
			break;
		case 'a':
		case 'A':
			//left
			goRightOrLeft(-1);
			break;
		case 'q':
		case 'Q':
			//destroy Cube front of me.
			destroyCube();
			break;
		case 'e':
		case 'E':
			//destroy all colon front of me.
			destroyCubes();
			break;
		case 'r':
		case 'R':
			//colapse.
			// copy the terrain of backup. to know if stack the cubes on floor or will lost in blackhall. 
			// if u put R while colapse will updated.
			for (int x = 0; x < n; x++) {
				for (int y = 0; y < n; y++) {
					terrainCubes[x][y] = backup[x][y][0];
				}
			}
			if (changes == 0) {
				wait = 0;	
				changes = 1;
				firstTimeInSlideDown =1;
				slideDown(123);
			}
			break;
	}
	glutPostRedisplay();
}
//--------------------------------------------------------------------------------
// special keys from keyboard.
//---------------------------------------------------------------------------------
void specialKeypress (int key, int x, int y) {
	if (humanView != 0 && humanView != 1 && humanView != 2 && humanView != 3) return;
	switch (key) {
		case GLUT_KEY_UP:
			seeDownOrUp(1); //1 go up
			break;
		case GLUT_KEY_DOWN:
			seeDownOrUp(0); // 0 go down
			break;
	}
}
//-----------------------------------------------------------------------------------
// 	its ' ' action spaceBar.
//---------------------------------------------------------------------------------
void grabCube() {
	if (points < 5) return;
	else if (humanPosition[2]-1 < 0) return;
	else if (backup[humanPosition[0]][humanPosition[1]][humanPosition[2]-1] == -1) return;
	else if (backup[humanPosition[0]][humanPosition[1]][humanPosition[2]-1] == 0) return;
	else if (backup[humanPosition[0]][humanPosition[1]][humanPosition[2]-1] == 4 ) return;
	backup[humanPosition[0]][humanPosition[1]][humanPosition[2]-1]--;
	holdCubes++;
	points -= 5;
}
//---------------------------------------------------------------------------------
// print the color of cube.
//----------------------------------------------------------------------------------
void colorCube(int value) {
	ostringstream convert;   // stream used for the conversion
	string str;
	string strLifes = " Lifes ";
	string strPoints = " Points ";
	string strCube = "Cube color ";
	string strLifesInt;
	string strPointsInt;
	
//print :: the color i stayed.
	if (humanPosition[2]-1 > -1) {
		switch (backup[humanPosition[0]][humanPosition[1]][humanPosition[2]-1]) {
			case 0:
				str = "blue";
				break;
			case 1:
				str = "yellow";
				break;
			case 2:
				str = "red";
				break;
			case 3:
				str = "green";
				break;
			case 4:
				str = "magenta";
				break;
			case -1:
				str = "\b\b\b\b\b\b\b\b\b\b\bYou are flying!!!";//delete the "Cube color".
				break;
		}
	} else {
		str = "\b\b\b\b\b\b\b\b\b\b\bBLACKHALL";
	}
	if (value == 0) cout << "Lifes == " << life << "\nPoints == "<< points << "\n" << "Cube color " << str << "\n";
	
	glColor3d(1.0, 1.0, 1.0);
	if(x1 == 0 && y1 == 0) glRasterPos3d(humanPosition[0], humanPosition[2]+0.40, humanPosition[1]-1);
	else glRasterPos3d(humanPosition[0]+(1*x1), humanPosition[2]+0.40, humanPosition[1]+(1*y1));
	//color Cube	
	for (unsigned i = 0; i < strCube.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, strCube.at(i));
	}
	for (unsigned i = 0; i < str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str.at(i));
	}
	//Lifes
	convert << life;      // insert the textual representation of 'Number' in the characters in the stream
	strLifesInt = convert.str(); // set 'Result' to the contents of the stream	
	convert.str("");
	convert.clear();

	for (unsigned i = 0; i < strLifes.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, strLifes.at(i));
	}
	
	for (unsigned i = 0; i < strLifesInt.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, strLifesInt.at(i));
	}
	// Points
	convert << points;      // insert the textual representation of 'Number' in the characters in the stream
	strPointsInt = convert.str(); // set 'Result' to the contents of the stream	
	convert.str("");
	convert.clear();

	for (unsigned i = 0; i < strPoints.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, strPoints.at(i));
	}
	for (unsigned i = 0; i < strPointsInt.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, strPointsInt.at(i));
	}

}
//-----------------------------------------------------------------------------------
//its 'w' forward. move.
//-----------------------------------------------------------------------------------
void goForward() {
	switch (humanView) {
		case 0:
			if (humanPosition[1]-1 >= 0) upOrDownByY(-1);
			break;
		case 1:
			if (humanPosition[0]+1 < n) upOrDownByX(1);
			break;
		case 2:
			if (humanPosition[1]+1 < n) upOrDownByY(1);
			break;
		case 3:
			if (humanPosition[0]-1 >= 0) upOrDownByX(-1);
			break;
	}
	if (humanPosition[2] == 0) { //turn back to the start Position (Magenta)
		backup[n/2][n/2][1] = -1;
		humanPosition[0] = n/2;
		humanPosition[1] = n/2;
		humanPosition[2] = 1;
		x1 = 0;
		y1 = 0;
		humanView = 0;
	}
	colorCube(0);
	
}
//you will move forward in the same floor or you will climb one cube or you will fell down.
void upOrDownByX(int value) { //looking right value == 1 left == -1.
	int z;
	int changeFloor = 0;
	if (backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] == -1) {
		for (z = humanPosition[2]-1; z > -1; z--) {
			if (backup[humanPosition[0]+value][humanPosition[1]][z] != -1) break;
			changeFloor--;
		}
	} else if (humanPosition[2]+1 < n && backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]+1] == -1) changeFloor++;
	else return;

	humanPosition[0] += value;
	humanPosition[2] += changeFloor;
	if (humanPosition[2] == n-1) life++, points += 100;
	else if (changeFloor < 0) {
		if (points + (5 * (changeFloor+1)) < 0 || humanPosition[2] == 0) life--;
		else points += 5 * (changeFloor+1);
	} else points += 5 * changeFloor;
	if (life == 0) restart(); 
	
}

//you will move forward in the same floor or you will climb one cube or you will fell down.	
void upOrDownByY(int value) { //if you looking forward value == -1 else == 1.
	int z;
	int changeFloor = 0;
	if (backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] == -1) {
		for (z = humanPosition[2]-1; z > -1; z--) {
			if (backup[humanPosition[0]][humanPosition[1]+value][z] != -1) break; // when you find cube stop. for dont leave you to go out of terrain.
			changeFloor--;
		}
	} else if (humanPosition[2]+1 < n && backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]+1] == -1) changeFloor++; //you will climb one cube.
	else return;

	humanPosition[1] = humanPosition[1] + value;
	humanPosition[2] = humanPosition[2] + changeFloor;
	if (humanPosition[2] == n-1) life++, points += 100;
	else if (changeFloor < 0) {
		if (points + (5 * (changeFloor+1)) < 0 || humanPosition[2] == 0) life--;
		else points += 5 * (changeFloor+1);
	} else points += 5 * changeFloor;
	if (life == 0) restart();
}	
// ------------------------------------------------------------------------------------------------
//		turn Right or Left slowly.
//-------------------------------------------------------------------------------------------------
void slowRightOrLeft(int move) {//if you looking right value == 1 ,left value == -1.
	if (humanView == 4 && move == -1) {
		x1-= 0.05;
		y1-= 0.05;
		if (x1 > 0) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 0;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0; // you put 'S' if random gives you -1 you will turn again left 1 you will turn again right.
		}
	} else if (humanView == 4 && move == 1) {
		x1+= 0.05;
		y1+= 0.05;
		if (y1 < 0) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 1;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	} else if (humanView == 5 && move == -1) {
		x1+= 0.05;
		y1-= 0.05;
		if (x1 < 1) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 1;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	} else if (humanView == 5 && move == 1) {
		x1-= 0.05;
		y1+= 0.05;
		if (y1 < 1) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 2;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	}  else if (humanView == 6 && move == -1) {
		x1+= 0.05;
		y1+= 0.05;
		if (x1 < 0) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 2;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	} else if (humanView == 6 && move == 1) {
		x1-= 0.05;
		y1-= 0.05;
		if (y1 > 0) {
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
		} else {
			humanView = 3;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	} else if (humanView == 7 && move == -1) {
		x1-= 0.05;
		y1+= 0.05;
		if (x1 > -1) {
			
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
			
		} else {			
			humanView = 3;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}
	} else if (humanView == 7 && move == 1) {
		x1+= 0.05;
		y1-= 0.05;
		if (y1 > -1) {
			
			glutPostRedisplay();
			glutTimerFunc(60, slowRightOrLeft, move);
			
		} else {			
			humanView = 0;
			glutPostRedisplay();
			if (rotate != 0) goRightOrLeft(rotate); rotate = 0;
		}	
	}
}
//turnRightOrleft.
void goRightOrLeft(int move) {
	if (humanView == 1 && move == -1) {
		x1 = 1;
		y1 = 0;
		humanView = 4;
		slowRightOrLeft(move);
	} else if (humanView == 0 && move == 1) {
		x1 = 0;
		y1 = -1;
		humanView = 4;
		slowRightOrLeft(move);
	} else if (humanView == 2 && move == -1) {
		x1 = 0;
		y1 = 1;
		humanView = 5;
		slowRightOrLeft(move);
	} else if (humanView == 1 && move == 1) {
		x1 = 1;
		y1 = 0;
		humanView = 5;
		slowRightOrLeft(move);
	} else if (humanView == 3 && move == -1) {
		x1 = -1;
		y1 = 0;
		humanView = 6;
		slowRightOrLeft(move);
	} else if (humanView == 2 && move == 1) {
		x1 = 0;
		y1 = 1;
		humanView = 6;
		slowRightOrLeft(move);
	} else if (humanView == 0 && move == -1) {
		x1 = 0;
		y1 = -1;
		humanView = 7;	
		slowRightOrLeft(move);
	} else if (humanView == 3 && move == 1) {
		x1 = -1;
		y1 = 0;
		humanView = 7;	
		slowRightOrLeft(move);
	}
}
// ------------------------------------------------------------------------
// destroy one cube per action. q action.
//------------------------------------------------------------------------
void destroyCube() {
	switch (humanView) {
		case 0:
			if (humanPosition[1]-1 >= 0) destroyCubeY(-1);
			break;
		case 1:
			if (humanPosition[0]+1 < n) destroyCubeX(1);
			break;
		case 2:
			if (humanPosition[1]+1 < n) destroyCubeY(1);
			break;
		case 3:
			if (humanPosition[0]-1 >= 0) destroyCubeX(-1);
			break;
	}
}
//q action.
void destroyCubeY(int value) {
	if (backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] == 4) return;
	if (backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] != -1) backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] = -1;
}
//q action.
void destroyCubeX(int value) {
	if (backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] == 4) return;
	if (backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] != -1) backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] = -1;
}

//------------------------------------------------------------------------
// 		destroy Cubes in colums.	its E action.
//----------------------------------------------------------------------------
void destroyCubes() {
	switch (humanView) {
		case 0:
			if (humanPosition[1]-1 >= 0) destroyCubesByY(-1);
			break;
		case 1:
			if (humanPosition[0]+1 < n) destroyCubesByX(1);
			break;
		case 2:
			if (humanPosition[1]+1 < n) destroyCubesByY(1);
			break;
		case 3:
			if (humanPosition[0]-1 >= 0) destroyCubesByX(-1);
			break;
	}
}
// E action.
void destroyCubesByY(int value) {
	int z = 0, checkCubes;
	if (points < 20) return;
	if (backup[humanPosition[0]][humanPosition[1]+value][0] == 4) z = 1;
	for (z; z < n; z++) {
		if (backup[humanPosition[0]][humanPosition[1]+value][z] != -1) checkCubes = 1; break;
	}
	if (checkCubes == 0) return;
	for(z; z < n; z++) backup[humanPosition[0]][humanPosition[1]+value][z] = -1;
	points -= 20;
	life++;
}

// E action.
void destroyCubesByX(int value) {
	int z = 0, checkCubes;
	if (points < 20) return;
	if (backup[humanPosition[0]+value][humanPosition[1]][z] == 4) z = 1;
	for (z; z < n; z++) {
		if (backup[humanPosition[0]+value][humanPosition[1]][z] != -1) checkCubes = 1; break;
	}
	if (checkCubes == 0) return;
	for(z; z < n; z++) backup[humanPosition[0]+value][humanPosition[1]][z] = -1;
	points -= 20;
	life++;
}
//------------------------------------------------------------------------------------------------
//				Colapse. R action.
//--------------------------------------------------------------------------------------------------
void slideDown(int value) {
	if (changes > 0 && wait == 0) {
		changes = 0;
		for (int z = 0; z < n; z++) {
			for (int y = 0; y < n; y++) {
				for (int x = 0; x < n; x++) {
					if (x != humanPosition[0] || y != humanPosition[1]) {
						if (backup[x][y][z] != -1 && terrainCubes[x][y] == -1) {
							if (z-1 < 0 || backup[x][y][z-1] == -1) {
								if (z-1 < 0) z++;
								backup[x][y][z-1] = backup[x][y][z];
								backup[x][y][z] = -1;
								changes++;
							}
						} else if (z-1 >= 0 && backup[x][y][z] != -1 && backup[x][y][z-1] == -1) {
							backup[x][y][z-1] = backup[x][y][z];
							backup[x][y][z] = -1;
							changes++;
						}
					}
				}
		
			} 
		}
		if (changes > 0) {
			wait = 1; //enable, you will wait.
			if (firstTimeInSlideDown == 1) {
				points+= 10;
				firstTimeInSlideDown = 0;
			}
		}
	
		glutTimerFunc(60, slideDown, 35000);
	} else if (changes > 0 && wait == 1) { //wait to see the collapse.
		wait = 0;
		glutPostRedisplay();
		glutTimerFunc(180, slideDown, 3500); //slideDown excecute in 180ms.
	} else {
		return;	
	}
}
//--------------------------------------------------------------------------------------------------------------
//		UP OR DOWN ARROWS looking up or down. for view secs and return back in the star position.
//---------------------------------------------------------------------------------------------------------------
//lookking slowly up.
void slowSeeUp(int value) {
	if (humanView == 20) { //i am looking forward.
		if (value > 0) {
			z1+= 0.01;
			y1+= 0.012;
			glutPostRedisplay();
			if (z2-z1 < 0.00000001) value = 0; // when u look 0.5 up you will turn back again.
			glutTimerFunc(60, slowSeeUp, value);
		} else {
			z1-= 0.01;
			y1-= 0.012;
			if (z1 < 0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 0;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60, slowSeeUp, value);
		}
	} else if (humanView == 30) { //i am looking right.
		if (value > 0) {
			z1+= 0.01;
			x1-= 0.012;
			glutPostRedisplay();
			if (z2-z1 < 0.00000001) value = 0;
			glutTimerFunc(60, slowSeeUp, value);
		} else {
			z1-= 0.01;
			x1+= 0.012;
			if (z1 < 0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 1;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60, slowSeeUp, value);
		}
	} if (humanView == 40) {
		if (value > 0) {
			z1+= 0.01;
			y1-= 0.012;
			glutPostRedisplay();
			if (z2-z1 < 0.00000001) value = 0;
			glutTimerFunc(60, slowSeeUp, value);
		} else {
			z1-= 0.01;
			y1+= 0.012;
			if (z1 < 0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 2;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60, slowSeeUp, value);
		}
	} else if (humanView == 50) {
		if (value > 0) {
			z1+= 0.01;
			x1+= 0.012;
			glutPostRedisplay();
			if (z2-z1 < 0.00000001) value = 0;
			glutTimerFunc(60, slowSeeUp, value);
		} else {
			z1-= 0.01;
			x1-= 0.012;
			if (z1 < 0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 3;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60, slowSeeUp, value);
		}
	}
}

//loking slowly down.
void slowSeeDown(int value) {
	if (humanView == 20) {
		if (value < 1) {
			z1-= 0.01;
			y1+= 0.012;
			glutPostRedisplay();
			if (z2+z1 < 0.0000001) value = 1;
			glutTimerFunc(60, slowSeeDown, value);
		} else {
			z1+= 0.01;
			y1-= 0.012;
			if (z1 > -0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 0;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60,slowSeeDown, value);
		}
	} else if (humanView == 30) {
		if (value < 1) {
			z1-= 0.01;
			x1-= 0.012;
			glutPostRedisplay();
			if (z2+z1 < 0.0000001) value = 1;
			glutTimerFunc(60, slowSeeDown, value);
		} else {
			z1+= 0.01;
			x1+= 0.012;
			if (z1 > -0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 1;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60,slowSeeDown, value);
		} 

	} else if (humanView == 40) {
		if (value < 1) {
			z1-= 0.01;
			y1-= 0.012;
			glutPostRedisplay();
			if (z2+z1 < 0.0000001) value = 1;
			glutTimerFunc(60, slowSeeDown, value);
		} else {
			z1+= 0.01;
			y1+= 0.012;
			if (z1 > -0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 2;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60,slowSeeDown, value);
		}
	} else if (humanView == 50) {
		if (value < 1) {
			z1-= 0.01;
			x1+= 0.012;
			glutPostRedisplay();
			if (z2+z1 < 0.0000001) value = 1;
			glutTimerFunc(60, slowSeeDown, value);
		} else {
			z1+= 0.01;
			x1-= 0.012;
			if (z1 > -0.01) {
				z1 = 0;
				y1 = -1;
				humanView = 3;
				glutPostRedisplay();
				return;
			}
			glutPostRedisplay();
			glutTimerFunc(60,slowSeeDown, value);
		}

	}
}

//arrow Down Or Up you will look down or up. 
void seeDownOrUp(int value) {
	if (humanView == 0) {
		y1= -1;
		x1= 0;
		z1= 0;
		humanView = 20;
	} else if (humanView == 1) {
		y1= 0;
		x1= 1;
		z1= 0;
		humanView = 30;
	} else if (humanView == 2) {
		y1= 1;
		x1= 0;
		z1= 0;
		humanView = 40;
	} else if (humanView == 3) {
		y1= 0;
		x1= -1;
		z1= 0;
		humanView = 50;
	}
	if (value == 0) slowSeeDown(0);
	if (value == 1) slowSeeUp(1);
}


//-------------------------------------------------
//		enable the 3Dimension.
//-------------------------------------------------
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_COLOR_MATERIAL );
	glShadeModel( GL_SMOOTH );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_LIGHTING);	
}

//---------------------------------------------------------
//		resize thw window. remake after refocus in this window.
//--------------------------------------------------------
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
				   (double)w / (double)h,
				   1.0,
				   200.0);
}

void setLight(){
	glMatrixMode(GL_MODELVIEW);													
	//glLoadIdentity();															
	glPushMatrix();
	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1.0 };
	
	GLfloat position1[] = { 0, n, 0, 1.0 };
	GLfloat position2[] = { 0, n, n, 1.0 };
	GLfloat position3[] = { n, n, 0, 1.0 };
	GLfloat position4[] = { n, n, n, 1.0 };
	//GLfloat position0[] = {0.0f, 0.0, -1.0f};
	GLfloat spot_direction[] = {0.0, -1.0, 0.0};
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT1, GL_POSITION, position1 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_SPECULAR, specular );
	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 35 );
	glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction );
	glEnable( GL_LIGHT1 );
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT2, GL_POSITION, position2 );
	glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT2, GL_SPECULAR, specular );
	glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 35 );
	glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction );
	glEnable( GL_LIGHT2 );
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT3, GL_POSITION, position3 );
	glLightfv( GL_LIGHT3, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT3, GL_SPECULAR, specular );
	glLightf( GL_LIGHT3, GL_SPOT_CUTOFF, 35 );
	glLightfv( GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction );
	glEnable( GL_LIGHT3 );
	
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT4, GL_POSITION, position4 );
	glLightfv( GL_LIGHT4, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT4, GL_SPECULAR, specular );
	glLightf( GL_LIGHT4, GL_SPOT_CUTOFF, 35 );
	glLightfv( GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction );
	glEnable( GL_LIGHT4 );
	glPopMatrix();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//   			Draw the Display
//---------------------------------------------------------------------------------------------------------------------------------------------
void drawScene() {
	
	int x,y,z;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
		
	glLoadIdentity();
	
	switch (humanView) {
		case 0:
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0],humanPosition[2],humanPosition[1]-1, 0,1,0);//koitaw mesa
			break;
		case 1:
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0]+1,humanPosition[2],humanPosition[1], 0,1,0);//koitaw de3ia
			break;
		case 2:
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0],humanPosition[2],humanPosition[1]+1, 0,1,0);//koitaw pisw
			break;
		case 3:
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0]-1,humanPosition[2],humanPosition[1], 0,1,0);//koitaw aristera
			break;
		case 20://looking forward and want to see my up or down cube> y--> - (0)
		case 30://looking right and want to see my up or down cube> x--> +
		case 40://looking back and want to see my up or down cube> y--> +
		case 50://looking left and want to see my up or down cube> x--> - (0)
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0]+x1,humanPosition[2]+z1,humanPosition[1]+y1, 0,1,0); 
			break;
		default :
			gluLookAt(humanPosition[0],humanPosition[2],humanPosition[1], humanPosition[0]+x1,humanPosition[2],humanPosition[1]+y1, 0,1,0);//allazw siga siga kateu8unsh.
			break;
	}

	setLight();
	
	
	for(z = 0; z < n; z++) {
		for(y = 0; y < n; y++) {
			for(x = 0; x < n; x++) {
				if(backup[x][y][z] != -1){
					glPushMatrix();
					glTranslatef(x,z,y);
					if(backup[x][y][z] == 4){
						glColor3ub(255,0,255);
					}
					else if(backup[x][y][z] == 3){
						glColor3ub(0,255,0);
					}
					else if(backup[x][y][z] == 2){
						glColor3ub(255,0,0);
					}
					else if(backup[x][y][z] == 1){
						glColor3ub(255,255,0);
					}
					else {
						glColor3ub(0,0,255);
					}
					glutSolidCube(1);
					glColor3ub(0,0,0);
					glutWireCube(1);
					glPopMatrix();
				}
			}
			
		}
	}
	colorCube(1); //DrawText;
	glutSwapBuffers(); //Send the 3D scene to the screen
}
//------------------------------------------------------------------------------------------------------------
//		action from mouse
//----------------------------------------------------------------------------------------------------------
void mouse_button(int button, int state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		if (holdCubes == 0) return; //you cant build cubes.
		srand (time(NULL));
		switch (humanView) {
			case 0:
				if (humanPosition[1]-1 >= 0) buildCubeY(-1); //looking the blackHall.
				break;			
			case 1:
				if ( humanPosition[0]+1 < n) buildCubeX(1);
				break;
			case 2:
				if (humanPosition[1]+1 < n) buildCubeY(1);
				break;
			case 3:
				if ( humanPosition[0]-1 >= 0) buildCubeX(-1);
				break;		

		}
	} else if (button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
		switch (humanView) {
			case 0:
				if (humanPosition[1]-1 >= 0) kickCubeByY(-1); // looking the blackHall.
				break;
			case 1:
				if (humanPosition[0]+1 < n) kickCubeByX(1);
				break;
			case 2:
				if (humanPosition[1]+1 < n) kickCubeByY(1);
				break;
			case 3:
				if (humanPosition[0]-1 >= 0) kickCubeByX(-1);
				break;
		}
	}
	glutPostRedisplay();
}
//-----------------------------------------------------------------------------------
//			build one random cube fornt of you.
//-------------------------------------------------------------------------------------
void buildCubeY(int value) {
	int z;
	srand (time(NULL));
	for (z = humanPosition[2]; z < n; z++) {
		if (backup[humanPosition[0]][humanPosition[1]+value][z] == -1) {
			backup[humanPosition[0]][humanPosition[1]+value][z] = rand() % 4;
			points += 5;
			holdCubes--;
			return;	
		}
	}
}

void buildCubeX(int value) {
	int z;
	srand (time(NULL));
	for (z = humanPosition[2]; z < n; z++) {
		if (backup[humanPosition[0]+value][humanPosition[1]][z] == -1) {
			backup[humanPosition[0]+value][humanPosition[1]][z] = rand() % 4;
			points += 5;
			holdCubes--;
			return;	
		}
	}
}
//------------------------------------------------------------------------------------------
//		kick a cube front of you.
//------------------------------------------------------------------------------------------
//if you are looking forward the value = -1 else if you are looking back value = 1.
void kickCubeByY(int value) {
	int y = 0;
	int depth = 0;
	//you try kick the hall.	
	if (backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] == -1) return;
	
	//find the depth of of cubes in stack from one step front of me.
	for (y = humanPosition[1]+value+value; y < n && y > -1; y+=value) {
		if (backup[humanPosition[0]][y][humanPosition[2]] == -1) break; //when find the hall i will stop.
		depth++;
	}
	if (humanPosition[1]+value+(value*depth) == n-1 || humanPosition[1]+value+(value*depth) == 0) depth--; 
	if (depth < 0) depth = 0;
	if (humanPosition[1]+value+(value*depth) == 0 || humanPosition[1]+value+(value*depth) == n-1) { // if after depth still you get in here that means u have only one cube front of you deleted.
		backup[humanPosition[0]][humanPosition[1]+value][humanPosition[2]] = -1;
		return;
	}
	//start from the hall to me and grab 1step all cubes.
	for (y = humanPosition[1]+value+(value*depth)+value; y != humanPosition[1]; y-=value) backup[humanPosition[0]][y][humanPosition[2]] = backup[humanPosition[0]][y-value][humanPosition[2]];
}
void kickCubeByX(int value) {
	int x = 0;
	int depth = 0;
	if (backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] == -1) return;
	
	for (x = humanPosition[0]+value+value; x < n && x > -1; x+=value) {
		if (backup[x][humanPosition[1]][humanPosition[2]] == -1) break;
		depth++;
	}
	if (humanPosition[0]+value+(value*depth) == n-1 || humanPosition[0]+value+(value*depth) == 0) depth--;
	if (depth < 0) depth = 0;
	if (humanPosition[0]+value+(value*depth) == 0 || humanPosition[0]+value+(value*depth) == n-1) {
		backup[humanPosition[0]+value][humanPosition[1]][humanPosition[2]] = -1;
		return;
	}
	for (x = humanPosition[0]+value+(value*depth)+value; x != humanPosition[0]; x-=value) backup[x][humanPosition[1]][humanPosition[2]] = backup[x-value][humanPosition[1]][humanPosition[2]];
}
//-------------------------------------------------------------------------------------------------
//		restart or exit.
//-------------------------------------------------------------------------------------------------
//your life == 0. you can play again if you want.
void restart() {
	char listener;
	cout << "Do you want to play again? y/n\n";
	do {
		cin >> listener;
	} while (listener != 'y' && listener != 'n' && listener != 'Y' && listener != 'N');
	if (listener == 'n' || listener == 'N') {
		cout << "See you next time!\n";
		glutLeaveMainLoop(); //exit(1);
	}
	else createGame();
	
}

//-------------------------------------------------------------------------------------------------
//	new Game create the world of the game.
//-------------------------------------------------------------------------------------------------
void createGame() {
	int  x,y,z;
	rotate = 0;		//disable by default 1 == enable, 'S' enable it, rotate i mean: you will turn 90degrees.
	changes = 0;	//disalbe by default, 1 == not yet 0 == done, for 'R' to know if all cubes fell down.
	wait = 0;		//disable by default 1 == enable, for slideDown.
	firstTimeInSlideDown = 1; //enable by default, 0 disable.
	life = 3;
	points = 50;
	humanView = 0;		//lookForward by default.
	holdCubes = 0;		//sum of the Cubes.
	x1 = 0;
	y1 = 0;
	z1 = 0;
	rotate = 0;			//disable by default. if 1 he will rotate 90degrees.
	
	do {	
		cout << "Give the size of the room\n";
		cin >> n;
	} while (n < 2);
	
	//allocate the memory
	try {
		backup =  new int**[n];
	} catch (std::bad_alloc& ba) {
		std::cerr << "bad_alloc_caught" << ba.what() << "\n";
	}
	for (x = 0; x < n; x++) {
		try {
			backup[x] = new int*[n];
		} catch (std::bad_alloc& ba) {
			std::cerr << "bad_alloc_caught" << ba.what() << "\n";
		}
			for (y = 0; y < n; y++) {
				try {
					backup[x][y] = new int [n];
				} catch (std::bad_alloc& ba) {
					std::cerr << "bad_alloc_caught" << ba.what() << "\n";
				}	
			}
	}
	
	//constractor for the 1floor of cubes.
	srand (time(NULL));
	for(z = 0; z < n; z++) {
		for(y = 0; y < n; y++) {
			for(x = 0; x < n; x++) {
				if (z != 0)	backup[x][y][z] = -1;
				else backup[x][y][z] = rand() % 4;

			}
		}
	}
	
	//mangenta.
	backup[n/2][n/2][0] = 4;	

	//myPosition
	humanPosition[0] = n/2;
	humanPosition[1] = n/2;
	humanPosition[2] = 1;

	//allocate memory for terrainCubes. i need this for Collapse.
	try {
		terrainCubes =  new int*[n];
	} catch (std::bad_alloc& ba) {
		std::cerr << "bad_alloc_caught" << ba.what() << "\n";
	}
	for (int y = 0; y < n; y++) {
		try {
			terrainCubes[y] =  new int[n];
		} catch (std::bad_alloc& ba) {
			std::cerr << "bad_alloc_caught" << ba.what() << "\n";
		}
	}
}

main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("MineCraftKappa");
	initRendering();
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(specialKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouse_button); // Set the mouse button function
	//glutTimerFunc(25,update,0);
	
	//create new Game.	
	createGame();
	
	
	glutMainLoop();
	
	//clear the memory
	for(int i = 0; i < n; ++i) {
    	for(int j = 0; j < n; ++j) {
       		delete[] backup[i][j];
   		}
    	delete[] backup[i];
	}
	delete[] backup;

	for (int i = 0; i < n; ++i) {
		delete[] terrainCubes[i];
	}
	delete[] backup;
	
}




