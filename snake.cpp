/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <string>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS = 60;
string sFPS = "";
int ORIGINAL_SPEED = 1;
int SPEED = 1;
string sSPEED = "";
int SCORE = 0;
const int width = 800;
const int height = 600;
bool PAUSE = false;
bool RESTART = false;
bool GAMEOVER = false;
bool SPLASH = true;
// int HP = 1;
vector< vector <int> > locations;
int slowStamp = 0;

/*
 * Enum types for direction
 */
enum Direction{
	NORTH,
	SOUTH,
	WEST,
	EAST,
};


/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};       


class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
        }

        Fruit() {
			regen();
        }

		bool eatble(int i, int j){
			if (i == x && j == y){
				++SCORE;
				return true;
			}
			return false;
		}

		bool regen(){
			while(true){
				x = rand() % 80;
				y = rand() % 60;
				x *= 10;
				y *= 10;
				int size = locations.size();
				int count = 0;
				for(int i = 0; i <  size; ++i){
					if(locations[i][0] != x && locations[i][1] != y){
						++count;
					}
				}
				if(count == size){
					break;
				}
			}
			// cout << "regenerated fruit at (" << x << ", " << y << ")" << endl;
		}
        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

    private:
        int x;
        int y;
};


Fruit fruit;

class Life : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+2, y+2, 6, 6);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+1, y+1, 3, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+6, y+1, 3, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+1, y+2, 1, 5);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+8, y+2, 1, 5);

			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y+2, 1, 3);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+9, y+2, 1, 3);

			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+3, y+8, 4, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+4, y+9, 2, 1);
        }

        Life() {
			regen();
        }

		bool edible(int i, int j){
			if (i == x && j == y){
				++SCORE;
				return true;
			}
			return false;
		}

		bool regen(){
			while(true){
				x = rand() % 80;
				y = rand() % 60;
				x *= 10;
				y *= 10;
				int size = locations.size();
				int count = 0;
				for(int i = 0; i <  size; ++i){
					if(locations[i][0] != x && locations[i][1] != y){
						++count;
					}
				}
				if(x == fruit.getX() && y == fruit.getY()){
					continue;
				}
				if(count == size){
					break;
				}
			}
			// cout << "regenerated fruit at (" << x << ", " << y << ")" << endl;
		}
        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

    private:
        int x;
        int y;
};

Life life;

class TimeSlow : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			if(!exists) return;
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 2);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+1, y+2, 8, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+2, y+3, 6, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+3, y+4, 4, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+3, y+5, 4, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+2, y+6, 6, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x+1, y+7, 8, 1);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y+8, 10, 2);
        }

        TimeSlow() {
			int i = rand()%10;
			regen();
        }

		bool edible(int i, int j){
			if (exists && i == x && j == y){
				SPEED = (int)floor(SPEED / 1.5);
				if (SPEED <= 0) {
					SPEED = 1;
				}
				slowStamp = now();
				exists = false;
				return true;
			}
			return false;
		}

		bool regen(){
			while(true){
				x = rand() % 80;
				y = rand() % 60;
				x *= 10;
				y *= 10;
				int size = locations.size();
				int count = 0;
				for(int i = 0; i <  size; ++i){
					if(locations[i][0] != x && locations[i][1] != y){
						++count;
					}
				}
				if(x == fruit.getX() && y == fruit.getY()){
					continue;
				}
				if(count == size){
					break;
				}
			}
			exists = true;
			// cout << "regenerated fruit at (" << x << ", " << y << ")" << endl;
		}

		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

		int getExists() {
			return exists;
		}

    private:
        int x;
        int y;
		bool exists;
};


TimeSlow timeS;

class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			for(int i = 0; i < locations.size(); ++i){
				// cout << locations[i][0] <<" " ;

				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], locations[i][0], locations[i][1], blockSize, blockSize);
			}		
		}
		
		void move(XInfo &xinfo) {

			// for(int i = 0; i < locations.size(); ++i){
			// 	cout << "("<< locations[i][0] <<", " << locations[i][1] <<") ";
			// }
			// cout << direction;
			// cout << endl;	

			didHitObstacle();

			if(didEatFruit()){
				vector<int> tempV;
				tempV.push_back(x);
				tempV.push_back(y);
				locations.push_back(tempV);
				if (direction == NORTH){
					y-= blockSize;
				} else if (direction == SOUTH){
					y += blockSize;
				} else if (direction == WEST){
					x -= blockSize;
				} else if (direction == EAST){
					x += blockSize;
				}
				tempV.clear();
				locations.erase(locations.begin()+0);
				tempV.push_back(x);
				tempV.push_back(y);
				locations.push_back(tempV);
				if (direction == NORTH){
					y-= blockSize;
				} else if (direction == SOUTH){
					y += blockSize;
				} else if (direction == WEST){
					x -= blockSize;
				} else if (direction == EAST){
					x += blockSize;
				}
			} else {
				locations.erase(locations.begin()+0);
				vector<int> tempV;
				tempV.push_back(x);
				tempV.push_back(y);
				locations.push_back(tempV);
				if (direction == NORTH){
					y-= blockSize;
				} else if (direction == SOUTH){
					y += blockSize;
				} else if (direction == WEST){
					x -= blockSize;
				} else if (direction == EAST){
					x += blockSize;
				}
			}

			// print locations
			// for(int i = 0; i < locations.size(); ++i){
			// 	cout << "(" << locations[i][0] << ", " << locations[i][1] << ")" ;
			// }
			// cout << endl;
		

			// fruit eating logic


            // ** ADD YOUR LOGIC **
            // Here, you will be performing collision detection between the snake, 
            // the fruit, and the obstacles depending on what the snake lands on.
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */ 
        bool didEatFruit() {
			// if(life.edible()){
			// 	int i = rand() % 10;
			// 	if (i == 5) life.regen();
			// }
			timeS.edible(x,y);
			if(fruit.eatble(x,y)){
				fruit.regen();
				return true;
			}
			return false;
        }

        bool didHitObstacle() {
			if (x < 0 || x > width) {
				// --HP;
				// if (!HP) GAMEOVER = true;
				GAMEOVER = true;
			}
			if (y < 0 || y > height) {
				// --HP;
				// if (!HP) GAMEOVER = true;
				GAMEOVER = true;
			}
			for(int i = 0; i < locations.size(); ++i){
				if(x == locations[i][0] && y == locations[i][1]){
					// --HP;
					// if (!HP) GAMEOVER = true;
					GAMEOVER = true;
				}
			}

        }

        void turnLeft() {
			if(direction != EAST) {
				direction = WEST;
			}
        }

        void turnRight() {
			if(direction != WEST) {
				direction = EAST;
			}
        }
		
        void turnNorth() {
			if(direction != SOUTH) {
				direction = NORTH;
			}
        }

        void turnSouth() {
			if(direction != NORTH) {
				direction = SOUTH;
			}
        }

		void init(int newX, int newY){
			locations.clear();
			x = newX;
			y = newY;
            blockSize = 10;
			for(int i = 5; i >= 1; --i){
				vector<int> tempV;
				tempV.push_back(x - 10 * i);
				tempV.push_back(y);
				locations.push_back(tempV);
			}
			direction = EAST;
			length = 5;
		}

		Snake(int x, int y): x(x), y(y) {
            blockSize = 10;
			locations.clear();
			for(int i = 5; i >= 1; --i){
				vector<int> tempV;
				tempV.push_back(x - 10 * i);
				tempV.push_back(y);
				locations.push_back(tempV);
			}
			// cout << "initial locations are: ";
			
			// for(int i = 0; i < locations.size(); ++i){
			// 	cout << locations[i][0] <<" " ;
			// }
			// cout << endl;	
			direction = EAST;
			length = 5;
		}
	
	private:
		int x;
		int y;
		int blockSize;
		Direction direction;
		int length;
};


class GameInfo : public Displayable{
	public:
		virtual void paint(XInfo &xinfo){
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							800-45, this->y,  "FPS: ", 5);
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							800-20, this->y,  this->FPS.c_str(), this->FPS.length());
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							800-48, this->y + 12,  "SPEED: ", 7);
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							800-12, this->y + 12,  this->SPEED.c_str(), this->SPEED.length());
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							this->x, this->y,  "SCORE: ", 7);
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							this->x + 37, this->y,  to_string(SCORE).c_str(),to_string(SCORE).length());
		}

	GameInfo(int x, int y, string FPS, string SPEED): x(x), y(y), FPS(FPS), SPEED(SPEED)  {}
	
	private:
		int x;
		int y;
		string FPS;
		string SPEED;
};


class GameOver : public Displayable{
	public:
		virtual void paint(XInfo &xinfo){
			XDrawImageString( xinfo.display, xinfo.window, *xinfo.gc,
							250, 300,  "Game over, press \"r\" to restart, \"q\" to quit",44 );
		}

	GameOver(){}
	
};

string splashFirst = "Name: Xianliang He, Student id: 20525705";
string splashSecond = "This game of snake is controlled by w - up, s - down, a - left, d - right";
string splashThird = "Press b to begin playing";

class SplashScreen : public Displayable{
	public:
		virtual void paint(XInfo &xinfo){
			XDrawImageString(xinfo.display, xinfo.window, *xinfo.gc,
							280, 250,  "Name: Xianliang He, Student id: 20525705",splashFirst.length());
			XDrawImageString(xinfo.display, xinfo.window, *xinfo.gc,
							150, 280,  "This game of snake is controlled by w - up, s - down, a - left, d - right",splashSecond.length());
			XDrawImageString(xinfo.display, xinfo.window, *xinfo.gc,
							310, 310,  "Press b to begin playing",splashThird.length());
		}

	SplashScreen(){}
	
};

list<Displayable *> dList;           // list of Displayables
Snake snake(100, 450);
GameOver gOver;
SplashScreen sScreen;

/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"Snake",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	XSelectInput(xInfo.display, xInfo.window, KeyPressMask | 
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}
/*
 * Function to repaint a display list
 */


void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background

	// draw display list
	while( begin != end ) {
		if(GAMEOVER){
			gOver.paint(xinfo);
			break;
		}
		if(SPLASH){
			sScreen.paint(xinfo);
			break;
		}
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}

	
	XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		// printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}else if (text[0] == 'w') {
			snake.turnNorth();
		}else if (text[0] == 's') {
			snake.turnSouth();
		}else if (text[0] == 'a') {
			snake.turnLeft();
		}else if (text[0] == 'd') {
			snake.turnRight();
		}else if (text[0] == 'p') {
			PAUSE = !PAUSE;
		}else if (text[0] == 'r') {
			RESTART = true;
		}else if (text[0] == 'b') {
			SPLASH = false;
		}
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
}
void restart(){
	RESTART = false;
	GAMEOVER = false;
	// HP = 1;
	snake.init(100,450);
	SCORE = 0;
	SPEED = ORIGINAL_SPEED;
	fruit.regen();
	if(PAUSE){
		PAUSE = false;
	}
}


void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&snake);
    dList.push_front(&fruit);
    // dList.push_front(&life);
    dList.push_front(&timeS);
    dList.push_front(new GameInfo(2,10,sFPS,sSPEED));
	
	XEvent event;
	unsigned long lastRepaint = 0;
	unsigned long lastMove = 0;
	int inside = 0;


	while( true ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			// cout << "event.type=" << event.type << "\n";

			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
			}
		} 
		if(SPLASH){			
			usleep(1000000/FPS);	
			repaint(xinfo);
			lastRepaint = now();
			continue;
		}

		if(PAUSE || GAMEOVER){
			usleep(1000000/5);
			if(RESTART) restart();
			continue;
		}

		if(RESTART) restart();

		unsigned long end = now();
		
		if(end - lastMove > 750000/SPEED){
			snake.move(xinfo);
			lastMove = now();
		}
		end = now();
		if (end - lastRepaint > 1000000 / FPS) {			
			repaint(xinfo);
			lastRepaint = now();
		}


		if(!timeS.getExists()){
			timeS.regen();
		}

		
		if (XPending(xinfo.display) == 0) {
			usleep(1000000 / FPS - (end - lastRepaint));
		}
		
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	if (argc == 3){
		sFPS = argv[1];
		FPS = atoi(argv[1]);
		// cout << "fps is " << to_string(FPS);
		sSPEED = argv[2];
		SPEED = atoi(argv[2]);
		ORIGINAL_SPEED = SPEED;
	} else {
		sFPS = "30";
		FPS = 30;
		// cout << "fps is " << to_string(FPS);
		sSPEED = "5";
		SPEED = 5;
		ORIGINAL_SPEED = 5;
	}
	XInfo xInfo;
	srand( time( NULL ) );
	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
