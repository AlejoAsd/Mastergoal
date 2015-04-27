#ifndef SAMPLE_H
#define SAMPLE_H

#include <GA/GAList.h>

void TestGAList();

void TestGAList()
{
	GAList<float> list;
	list.insert(0.032f);
	cout << "Element 0.032f inserted" << endl;

	cout << "list.head(): " << list.head() << endl;
	cout << "list.current(): " << list.current() << endl;
	cout << "list.next(): " << list.next() << endl;
	//cout << "list.remove(): " << list.remove() << endl;
	cout << "list.next(): " << list.next() << endl;
	cout << "list.next(): " << list.next() << endl << endl;

	list.insert(3.232f);
	cout << "Element 3.232f inserted" << endl;

	// Place the list's iterator at the head
	cout << "list.head(): " << list.head() << endl;

	cout << "list.next(): " << list.next() << endl;
	cout << "list.next(): " << list.next() << endl << endl;

	float *r = list.remove(); 
	cout << "Removing second element\nlist.remove()" << endl << endl;
	delete r;

	//Now list.current() == list.head()
	//Shouldn't list.next() return NULL? (since list.current() is pointing to the last element)
	cout << "list.head(): " << list.head() << endl;
	cout << "list.current(): " << list.current() << endl;
	cout << "list.next(): " << list.next() << " <--- shouldn't it be NULL? " << endl;

	list.remove();
}


#endif
