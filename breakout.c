//
// breakout.c
//
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400
#define P_WIDTH 80
#define P_HEIGHT 10

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
GRect initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
	// number of lives initially
    int bricks=36;
    int lives = LIVES;
    int points = 0;
	 // instantiate window
    	GWindow window = newGWindow(WIDTH, HEIGHT);
	// instantiate bricks
	GRect rect=initBricks(window);
	//A:
	
	    // seed pseudorandom number generator
	    srand48(time(NULL));

   

    
    

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    //int bricks = COLS * ROWS;

    
	
    // number of points initially
    waitForClick();
	updateScoreboard(window,label,points);
    double velocity_x = drand48()+1.5;
    double velocity_y = drand48()+1.5;	
    // keep playing until game over
   while (lives > 0 && bricks > 0)
    {
	printf("bricks:%d\n",bricks);
     	updateScoreboard(window,label,points);
	 // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
	//lsprintf("x=%0.0f, y=%0.0f :\n", getX(circle),getY(circle));
	

        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure circle follows top cursor
		//printf("Ex=%0.0f, Ey=%0.0f :\n", getX(event),getY(event));
                
		 double y = HEIGHT-10;
		
		if((getX(paddle)+getX(event)-getWidth(paddle))<=0)
			{
				//printf("true\n");
				double x = 0;
				setLocation(paddle, x, y);
			}
		else
			{
				double x = getX(event) - getWidth(paddle);
				setLocation(paddle, x, y);
			}
               
               // setLocation(paddle, x, y);
		//pause(100);
		
            }
        } 


	move(ball, velocity_x, velocity_y);

        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
           //printf("true:\n");
	 velocity_x = -velocity_x;
        }
	if(getY(ball) + getHeight(ball) >= getHeight(window))
	{
		lives--;
		//while(!())
		//if(waitForClick())
			setLocation(ball, WIDTH/2, HEIGHT/2);
			waitForClick();
			
		
	}
        // bounce off left edge of window
        if (getX(ball) <= 0)
        {
            velocity_x = -velocity_x;
        }
	if (getY(ball) <= 0)
        {
            velocity_y = -velocity_y;
        }

        // linger before moving again
        pause(7);


	GObject object = detectCollision(window, ball);
	//if(object!=NULL)	
	{
		
	
		if ((object!=NULL)&&(strcmp(getType(object), "GRect") == 0))
		{
			//printf("true\n");
			velocity_y = -velocity_y;
			if(!(object == paddle))			
				{
					removeGWindow(window,object);
					points++;bricks--;
				}
		}
	}
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
GRect initBricks(GWindow window)
{
	GRect rect;
    for(int i=10;i<HEIGHT/5;i=i+20)
	{
		for(int j=15;j<getWidth(window);j=j+65)
			{
				rect = newGRect(j, i, 40, 10);
				setFilled(rect, true);
				setColor(rect, "red");
				add(window, rect);
			}
			
	}
	return rect;
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval circle = newGOval(WIDTH/2, HEIGHT/2, 20, 20);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    				GRect rect = newGRect(WIDTH/2-P_WIDTH/2,HEIGHT-30, P_WIDTH, P_HEIGHT);
				setFilled(rect, true);
				setColor(rect, "black");
				//addToRegion(window, rect, "SOUTH");
				add(window, rect);
    				return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
     // center label
	

        // convert i from int to string
       

       
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
