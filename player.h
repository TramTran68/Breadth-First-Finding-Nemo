#ifndef PLAYER_H_
#define PLAYER_H_

#include"entity.h"
#include"queue.h"
#include"stack.h"
#include"list.h"

class Room;
class Maze;

class Player :public Entity {
public:
	Player(Maze* maze, Room p, std::string name, char sprite, const bool tbacktrack);

	virtual void	update(); // The only function of player to implement


	virtual void	say();

	bool			foundExit() const;
	bool			stuck() const;
	Room			getTargetRoom() const;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif

	// Wrapper function to find if a room has been
	// placed inside the m_discoveredRooms List
	bool discovered(const Room &p) const;
	
	// Stack to decide where to look next
	Queue<Room>	m_lookingPaper;

	// List to save all the discovered areas, 
	// Helps us decide where to look next
	List<Room>		m_discoveredRooms;

	// ** Variables relating to backtracking. **

	// Read from settings.ini file, write at player creation
	// and cannot be modified, only read.
	// All back tracking behavior should be toggled using
	// BACKTRACKENABLED, if false the player teleports
	// the way seen in the algorithm from the lecture slides.
	const bool		BACKTRACKENABLED;

	// Used to facilitate backtracking, without backtracking, 
	// the player teleports to  rooms for looking.  
	//
	// To simulate real movement we need to interpolate between
	// The players current position to the next room to look.
	//
	// The premise is that if there is a room to look around, 
	// The player must have been next to that room earlier (by
	// definition of the algorithm) so if we keep track of where
	// the player has been we can back track our steps to reach
	// the desired room.
	//
	// You may need one or all.  It depends on how you 
	// plan your backtracking algorithm.  
	Stack<Room>	m_btStack;
	Queue<Room>	m_btQueue;
	List<Room>  m_btList;

};

#endif//PLAYER_H_