#include <iostream>
#include <string>
#include "SceneObject.h"

namespace ENG
{

	// Sorted Doubly Linked List
	// The list is sorted based on priority, with the item that has the lowest priority at the front of the list.

	class node {
	public:
		node(SceneObject* dataPtr);
		int priority;
		SceneObject* data;
		node *next;
		node *prev;
	};

	class DLList {
	public:
		DLList();

		void addNode(SceneObject* data, int priority = 0);// add a node to the list. 
												// put it in the correct place 
		void removeMaxNode();		//Remove the node with the maximum priority
		void removeMinNode();       	//Remove the node with the lowest priority 
		void drawList(Mesh* meshPtr, Shader* shaderPtr);		// Print the content of the whole list

		void clearList();

	private:
		node *first;			// pointer to the first node in the list
		node *last;			// pointer to the last node in the list
		unsigned int count; //int tracking exact number of items in the list


	};
}