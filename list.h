#ifndef F22_CS20A_LIST_H
#define F22_CS20A_LIST_H

#include<iostream>
#include<assert.h>
// Linked List object that maintains both m_front and m_rear pointers
// and the m_size of the list.  Note that you have to keep the m_front,
// m_rear and m_size consistent with the intended state of the List 
// otherwise very bad things happen. 
template<typename Type>
class List {
public:

	List();

	List(const List<Type>& other);

	List<Type>& operator=(const List<Type>& other);

	~List();

	void	print() const;
	bool	empty() const;

	void	push_front(const Type& value);
	void	push_rear(const Type& value);
	void	push_at(int idx, const Type& value);

	// Note that the user must m_front ensure the list is not empty
	// prior to calling these functions. 
	Type	front() const;
	Type	rear() const;
	Type	at(int idx) const;

	int		size() const;
	int		count(const Type& value) const;

	int		find(const Type& value) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int idx);
	int		pop_value(const Type& value);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our List.
	// Will be implemented along with list
	// member functions
	class DLNode;

	// We'll have both m_front and m_rear points for 
	// Fast insertion/deletion from both ends.
	DLNode* m_front;
	DLNode* m_rear;

	// Keep track of number of nodes in the list
	int		m_size;
};


/* List Implementation
//
//  Since List is a template class (which is not an actual
//  class yet, not until we actually instantiate the list)
//  we need to keep the implementation together with
//  the definition.  There are ways to simulate having
//  separate "implementation/definition" with templates,
//  but they aren't necessary and can be confusing.
*/

/* DLNode definition
//		Already implemented, nothing to do here but to use it.
//		Make note of the fact the the member variables are private
*/
template<typename Type>
class List<Type>::DLNode {
public:
	DLNode() :nxt(nullptr), prv(nullptr) {}
	DLNode(Type v, DLNode* p, DLNode* n) : val(v), nxt(n), prv(p) {}

	DLNode* next() const { return nxt; } // Access the address of the next node
	void	 next(DLNode* n) { nxt = n; } // Modify the address of the next node

	DLNode* prev() const { return prv; } // Access the address of the previous node
	void	 prev(DLNode* p) { prv = p; } // Modify the address of the previous node

	Type value() const { return val; }		// Access the value stored in the current node
	void value(const Type& v) { val = v; }	// Modify the value stored in the current node

private:
	Type  val;
	DLNode* nxt;
	DLNode* prv;
};



/* List default constructor
//		Set m_front and m_rear pointer to point to nothing, m_size is zero
//		Already implemented, nothing to do.
*/
template<typename Type>
List<Type>::List() :m_front(nullptr), m_rear(nullptr), m_size(0) {
}


/* Copy constructor
*/
template<typename Type>
List<Type>::List(const List<Type>& other) {
	m_front = nullptr;
	m_rear = nullptr;
	m_size = 0;
	DLNode* p = other.m_front; //traverse the other list
	while (p != nullptr) {
		push_rear(p->value());
		p = p->next();
	}
}
/* Overloaded assignment operator
*/
template<typename Type>
List<Type>& List<Type>::operator=(const List<Type>& other) {
	if (this != &other) {

		//Clean up current list
		DLNode* clean = m_front;
		while (clean != nullptr)
		{
			DLNode* next = clean->next();
			delete clean;
			clean = next;
		}
		m_front = nullptr;
		m_rear = nullptr;
		m_size = 0;

		DLNode* p = other.m_front; //traverse the other list
		while (p != nullptr) {
			push_rear(p->value());
			p = p->next();
		}
	}
	return *this;
}


/* List destructor
*/
template<typename Type>
List<Type>::~List() {
	DLNode* p = m_front;
	while (p != nullptr)
	{
		DLNode* n = p->next();
		delete p;
		p = n;
	}
	m_front = nullptr;
	m_rear = nullptr;
	m_size = 0;
}

/* List print
*/
template<typename Type>
void List<Type>::print() const {
	DLNode* p;
	p = m_front;
	while (p != nullptr)
	{
		if (p->next() != nullptr)
			std::cout << p->value() << " ";
		else
			std::cout << p->value();
		p = p->next();
	}
}

/* List empty
*/
template<typename Type>
bool List<Type>::empty() const {

	if (m_front == nullptr)
		return true;
	return false;

}


/* List push_front
*/
template<typename Type>
void List<Type>::push_front(const Type& value) {
	DLNode* p = new DLNode;
	p->value(value);
	p->next(m_front);
	p->prev(nullptr);
	if (m_front != nullptr) //if there are nodes already in the list, update m_front's previous node; m_rear is not affected as the following nodes are not affected
		m_front->prev(p);
	else //if this is the first node added the list, update m_rear
		m_rear = p;
	m_front = p;
	m_size++;
}

/* List push_rear
*/
template<typename Type>
void List<Type>::push_rear(const Type& value) {

	//Empty list
	if (m_front == nullptr)
		push_front(value);
	else {
		DLNode* p = new DLNode;
		p->value(value);
		p->next(nullptr);
		p->prev(m_rear);
		m_rear->next(p);
		m_rear = p;
		m_size++;
	}
}

/* List push_at
*/
template<typename Type>
void List<Type>::push_at(int idx, const Type& value) {
	if (idx <= 0)
		push_front(value);
	else if (idx >= m_size)
		push_rear(value);
	else {
		int i = 0;
		DLNode* p = m_front; //start traversing at the first DLNode
		while (p->next() != nullptr)
		{
			if (i == idx - 1) // traverse to find the DLNODE just above the index to be inserted
				break;
			p = p->next();
			i++; //update the index of the current node traversed
		}
		DLNode* n = new DLNode;
		n->value(value);
		n->next(p->next());
		p->next(n);
		n->next()->prev(n);
		n->prev(p);
		m_size++;
	}
}

/*  List front
*/
template<typename Type>
Type List<Type>::front() const {
	// Force check that the m_front is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_front != nullptr);
	Type frontVal;
	frontVal = m_front->value();
	return frontVal;
}

/* List rear
*/
template<typename Type>
Type List<Type>::rear() const {

	// Force check that the m_rear is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_rear != nullptr);
	Type rearVal = m_rear->value();
	return rearVal;

}

/* List at
*/
template<typename Type>
Type List<Type>::at(int idx) const {

	// Force index to be correct before getting the Type
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(idx >= 0 && idx < m_size);

	/*   TODO   */
	Type retval;
	int i = 0;
	DLNode* p = m_front; //start traversing at the first DLNode
	while (i != idx) // traverse to find the DLNODE at the index
	{
		p = p->next();
		i++;
	}
	retval = p->value();
	return retval;
}

/* List size
//		return the m_size of the list
*/
template<typename Type>
int List<Type>::size() const {
	return m_size;
}

/*List count
*/
template<typename Type>
int List<Type>::count(const Type& value) const {


	int count = 0;
	DLNode* p = m_front;
	while (p != nullptr) // traverse until the end of list
	{
		if (p->value() == value)
			count++;
		p = p->next();
	}
	return count;

}
/* List find
*/
template<typename Type>
int List<Type>::find(const Type& value) const {
	if (!empty()) {
		int i = 0;
		DLNode* p = m_front;
		while (p != nullptr) // traverse until the end of list
		{
			if (p->value() == value)
				return i;
			p = p->next();
			i++;
		}
	}
	return -3;

}

/* List pop_front
*/
template<typename Type>
bool List<Type>::pop_front() {

	if (m_front == nullptr)
		return false;
	else if (m_front->next() == nullptr)
	{
		delete m_front;
		m_size--;
		m_front = nullptr;
		m_rear = nullptr;
		return true;
	}
	else {
		DLNode* killMe = m_front;
		m_front = killMe->next();
		killMe->next()->prev(nullptr);
		delete killMe;
		m_size--;
		return true;
	}
}

/* List pop_rear
*/
template<typename Type>
bool List<Type>::pop_rear() {
	if (m_front == nullptr)
		return false;
	else if (m_front->next() == nullptr) {
		delete m_front;
		m_size--;
		m_front = nullptr;
		m_rear = nullptr;
		return true;
	}
	else {
		DLNode* killMe = m_rear;
		m_rear = killMe->prev();
		m_rear->next(nullptr);
		delete killMe;
		m_size--;
		return true;
	}
}

/* List pop_at
*/
template<typename Type>
bool List<Type>::pop_at(int idx) {
	if (m_front == nullptr || idx < 0 || idx >= m_size)
		return false;
	else if (m_front->next() == nullptr || idx == 0) //delete the only node or the first node in the list
		pop_front();
	else if (idx == m_size - 1) //the last idex of the list
		pop_rear();
	else {
		DLNode* p = m_front;
		int i = 0;
		while (p != nullptr) // traverse to find the DLNODE just above the index to be inserted
		{
			if (p->next() != nullptr && i == idx - 1)
				break;
			p = p->next();
			i++; //update the index of the current node traversed
		}
		DLNode* killMe = p->next();
		p->next(killMe->next());
		killMe->next()->prev(p);
		delete killMe;
		m_size--;
		return true;
	}


}

/* List pop_at on value
*/
template<typename Type>
int List<Type>::pop_value(const Type& value) {
	if (m_front == nullptr)
		return -3;
	if (m_front->value() == value)
	{
		pop_front();
		return 0;
	}
	DLNode* p = m_front;
	int i = 0;
	while (p != nullptr) // traverse to find the DLNODE just above the index to be inserted
	{
		if (p->next() != nullptr && p->next()->value() == value)
			break; //p points to node above the one to be deleted
		p = p->next();
		i++; //update the index of the current node traversed
	}
	if (p == nullptr) //value not found
		return -2;
	if (i == m_size - 2) { //if the node to be deleted is the last node, i is not the last index, but the previous one of the last idex
		pop_rear();
		return i + 1;
	}
	else {
		DLNode* killMe = p->next();
		p->next(killMe->next());
		killMe->next()->prev(p);
		delete killMe;
		m_size--;
		return i + 1;
	}
}


#endif



























































































// _X_XMMXXI