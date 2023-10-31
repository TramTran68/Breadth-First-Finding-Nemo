#ifndef QUEUE_H_
#define QUEUE_H_

#include"list.h"

// Queue ADT using linked list as primary data structure.
// 
template<typename Item>
class Queue {
public:

	Queue();

	// Basic queue operations
	void	enqueue(Item item);
	void	dequeue();
	Item	front() const;

	// Check to see if there is anything on the queue
	bool	empty() const;

	// Use to see what is in the queue for debugging
	void	print() const;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	List<Item> m_list;
};


// Queue Implementation
//

// Queue() Default constrcutor:
//		Call the defualt constructor for its List.
//		Already implemented.
template<typename Item>
Queue<Item>::Queue() :m_list() {}


template<typename Item>
void Queue<Item>::enqueue(Item item) {

	/*  TODO  */
	m_list.push_rear(item);
}


template<typename Item>
void Queue<Item>::dequeue() {
	
	/*  TODO  */
	m_list.pop_front();
}


template<typename Item>
bool Queue<Item>::empty() const {

	/*  TODO  */
	return m_list.empty();
}


template<typename Item>
Item Queue<Item>::front() const {

	/*  TODO  */
	return m_list.front();
}


template<typename Item>
void Queue<Item>::print() const {

	/*  TODO  */
	m_list.print();
}




























#endif //QUEUE_H_X_XMMXX 