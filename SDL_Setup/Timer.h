/*
This class was created by following the tutorials on Lazyfoo.net
*/

#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
	//The clock time when the timer started (resets each frame)
	int startTicks;
    
	//The ticks stored when the timer was paused
	int pausedTicks;
    
	//The timer status
	bool paused;
	bool started;
    
public:
	bool allowPhysics = true;

	//Initializes variables
	Timer();
    
	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();
	int currentFrameRate, frameCount;
    
	//The clock time when the FPS counter started (lasts until currentFrameRate is calculated)
	double firstStartTicks;

	//Gets the timer's time
	int get_ticks();

	//Gets the total time of the entire game
	double get_totalTicks();
    
	//Checks the status of the timer
	bool is_started();
	bool is_paused();    
};

#endif