#include"player.h"
#include"entity.h"
#include"stack.h"
#include"list.h"
#include"room.h"
#include"maze.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Entity and its member variables
//		Remembers and discovers the starting room.
Player::Player(Maze* maze, Room p, std::string name, char sprite, const bool tbacktrack)
	:Entity(maze, p, name, sprite),
	m_lookingPaper(),
	m_discoveredRooms(),
	m_btQueue(),
	m_btStack(),
	BACKTRACKENABLED(tbacktrack) {

	// Discover the starting room
	m_discoveredRooms.push_front(p);
	m_lookingPaper.enqueue(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Entity::state() == State::NOEXIT;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Entity::state() == State::EXIT;
}

//  getTargetRoom()
//		Get the room the player wants to look around next.
//      If m_lookingPaper is empty then return an invalid room.
Room Player::getTargetRoom() const {
	if (m_lookingPaper.empty()) return Room(-1, -1);
	return m_lookingPaper.front();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Room &p) const {
	return (m_discoveredRooms.find(p) != -3);
}



// say()
//		What does the player say?
void Player::say() {

	// Freedom supercedes being eaten
	if (state() == State::EXIT) {
		std::cout << name() << ": WEEEEEEEEE!";
		return;
	} 
 
	// Being eaten supercedes being lost
	switch (interact()) {
	case Interact::ATTACK:
		std::cout << name() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (state()) {
		case State::LOOK:
			std::cout << name() << ": Where is the exit?";
			break;
		case State::NOEXIT:
			std::cout << name() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << name() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOK). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired room to look around is not adjacent (more than one cell away), 
//		at this room the player must backtrack (State::BACKTRACK) to a room 
//		that is adjacent to the next room the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
void Player::update() {
	/* TODO */
	// Set by the settings file, if BACKTRACKENABLED is false, then
	// your program should behave exactly as seen in the slides or
	// example executables (with teleporting).
	// if(BACKTRACKENABLED) { ... code relating to backtracking 
	if (BACKTRACKENABLED) { //NOT WORKING PROPERLY
		if (m_lookingPaper.empty())
		{
			state(State::NOEXIT);
			return;
		}

		Room currRoom = room(); //get current room in entity.cpp
		Room nextRoom = m_lookingPaper.front();

		if (currRoom != nextRoom)//avoid starting point being pushed twice onto the stack
			m_btQueue.enqueue(currRoom); //save the current room onto the stack for later backtracking

		//calculate the distance from current room to next room
		double distance = sqrDist(currRoom, nextRoom); //using function sqrDist in room.h
		if (!m_btQueue.empty() && distance > 1.0) // two rooms are not adjacent or at the same point, do backtracking
		{
			//if (state() == State::LOOK) //before doing backtracking
				//m_btQueue.enqueue(currRoom);
			state(State::BACKTRACK);
			move(m_btQueue.front());
			// m_btList.push_rear(m_btStack.top()); 
			m_btQueue.dequeue();
			return;
			
		}
		else if (distance > 1.0 && m_btStack.empty() && !m_btList.empty())
		{
			state(State::BACKTRACK);
			move(m_btList.front());
			m_btList.pop_front();
			return;
		}
		else //continue looking
		{
			state(State::LOOK);

		}
	
		m_lookingPaper.dequeue();
		move(nextRoom);
		
		if (maze()->foundExit(nextRoom))
		{
			state(State::EXIT);
			return;
		}
		Room look = nextRoom;
		look.write(nextRoom.x() - 1, nextRoom.y()); //look WEST
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(nextRoom.x() + 1, nextRoom.y()); //look EAST
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(nextRoom.x(), nextRoom.y() - 1); //look NORTH
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(nextRoom.x(), nextRoom.y() + 1); //look SOUTH
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}

	}

	//NO BACKTRACKING
	else {
		if (m_lookingPaper.empty())
		{
			state(State::NOEXIT);
			return;
		}
		Room p = getTargetRoom();
		m_lookingPaper.dequeue();
		move(p);
		if (maze()->foundExit(p))
		{
			state(State::EXIT);
			return;
		}
		Room look = getTargetRoom();
		look.write(p.x() - 1, p.y()); //look WEST
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(p.x() + 1, p.y()); //look EAST
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(p.x(), p.y() - 1); //look NORTH
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
		look.write(p.x(), p.y() + 1); //look SOUTH
		if (maze()->open(look) && !discovered(look))
		{
			m_discoveredRooms.push_front(look);
			m_lookingPaper.enqueue(look);
		}
	}
}