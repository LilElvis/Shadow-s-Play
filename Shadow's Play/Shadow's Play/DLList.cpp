#include "DLList.h"

namespace ENG
{

	node::node(SceneObject* dataPtr)
	{
		data = dataPtr;
	}

	DLList::DLList()
	{
		first = NULL;
		last = NULL;
		count = 0;
	}

	void DLList::addNode(SceneObject* data, int priority)
	{
		node *newNode = new node(data);
		newNode->priority = priority;
		newNode->next = NULL;
		newNode->prev = NULL;

		if (first == NULL) //this will be the first node in the list
		{
			first = newNode;
			last = newNode;

			count += 1;
		}
		else
		{
			node *temp = first;
			if (temp == last)
			{
				if (newNode->priority > temp->priority)
				{
					temp->next = newNode;
					newNode->prev = last;
					last = newNode;

					count += 1;
				}
				else
				{
					temp->prev = newNode;
					newNode->next = first;
					first = newNode;

					count += 1;
				}
			}
			else if (newNode->priority < temp->priority) //this will replace the current first node in the list
			{
				first->prev = newNode;
				newNode->next = first;
				first = newNode;

				count += 1;
			}
			else
			{
				while (newNode->priority >= temp->priority)
				{
					if (temp->next == NULL)
						break;
					temp = temp->next;
				}
				if (temp->next == NULL && newNode->priority > temp->priority) //this will replace the current last node in the list
				{
					last->next = newNode;
					newNode->prev = last;
					last = newNode;

					count += 1;
				}
				else //this will go in between existing nodes
				{
					temp->prev->next = newNode;
					newNode->prev = temp->prev;
					newNode->next = temp;
					temp->prev = newNode;

					count += 1;
				}
			}
		}
	}

	void DLList::removeMaxNode()
	{
		node *temp = last;

		if (last != NULL)
		{
			if (last->prev != NULL)
			{
				last = last->prev;
				last->next = NULL;
			}
			else //when there's only one thing in the list
			{
				first = NULL;
				last = NULL;
			}
			delete temp;
			count -= 1;
		}
	}

	void DLList::removeMinNode()
	{
		node *temp = first;

		if (first != NULL)
		{
			if (first->next != NULL)
			{
				first = first->next;
				first->prev = NULL;
			}
			else //when there's only one thing in the list
			{
				first = NULL;
				last = NULL;
			}
			delete temp;
			count -= 1;
		}
	}

	void DLList::drawList(Mesh* meshPtr, Shader* shaderPtr)
	{
		node *temp = first;

		while (temp != NULL) //keep rendering until temp is nullified by the "next" pointer of the last node
		{
			temp->data->render(meshPtr, shaderPtr);
			temp = temp->next;
		}
	}

	void DLList::clearList()
	{
		while (first != NULL)
		{
			removeMinNode();
		}
	}

}