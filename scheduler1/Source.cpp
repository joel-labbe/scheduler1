//COP 4610 - Programming Assignment
//CPU Scheduler
//Joel Labbe
//November 11, 2018 
//Professor Borko Furht

#include <iostream>
#include <ctime>  
#include <iomanip>
#include <string>
using namespace std;

//Process Nodes for Queue 
class Process
{
public:
	//Values - waiting;Tw , turnaround;Ttr , response;Tr
	int sizeOfArr, arrayCounter, processNumber;
	double starttime, waiting, responsetime, turnaround;
	string queue = "";

	//Pointers
	int * bursts; //dynamic array
	Process *next, *pre;

	Process() //Default Constructor
	{
		//Values
		processNumber = 0;
		waiting = 0;
		responsetime = 0;
		turnaround = 0;
		starttime = 0;
		sizeOfArr = 20;
		arrayCounter = 0;
		queue = "";

		//Pointers
		bursts = new int[sizeOfArr];
		for (int i = 0; i < sizeOfArr; i++)
		{
			bursts[i] = i; // Dummy values 1, 2, 3, 4, 5...
		}
		next = 0;
		pre = 0;
	}

	Process(const int * arrBursts, const int size, const int procNum) //Copy Constructor
	{
		//Values
		processNumber = procNum;
		waiting = 0;
		responsetime = 0;
		turnaround = 0;
		starttime = 0;
		sizeOfArr = size;
		arrayCounter = 0;
		queue = "";

		//Pointers
		bursts = new int[size];
		for (int i = 0; i < size; i++)
		{
			bursts[i] = arrBursts[i]; // Copy Values
		}
		next = 0;
		pre = 0;
	}

	~Process() //Deconstructor
	{
		//Values
		processNumber = 0;
		waiting = 0;
		responsetime = 0;
		turnaround = 0;
		starttime = 0;
		sizeOfArr = 0;
		arrayCounter = 0;
		queue = "";

		//Pointers
		bursts = 0;
		next = 0;
		pre = 0;

		delete bursts;
		delete next;
		delete pre;
	}

};

//Doubly Linked List Functionality 
class DLinkedList 
{

	int size;
	Process *front; //Head
	Process *back;	//Tail

public:

	DLinkedList(); //Constructor
	DLinkedList(const DLinkedList &list); //Copy Constructor

	void push_back(Process *a);
	void push_front(Process *a);

	Process * pop_front();
	Process * pop_back();

	void insert(Process *a, int t); // insert the item a after the t-th element (head is the first element)
	void insertlist(const DLinkedList &list, int t); // insert the whole list a after the t-th element.
													 // you should make a copy of list first and then insert the copied version of the list. Do not modify or remove the list from the input
	void display(ostream &out);

	int getSize();
	Process * getfront();
	Process * getback();

	Process * extractmin(Process * start); // return the pointer of the min element after  "start",
									 // here you can assume user will always input a valid pointer start that points to an item in the list
	Process * extractmax(Process * start);  // return the pointer of the max element after "start"
};

//***Definitions
DLinkedList::DLinkedList() //Constructor
{
	//Previous 
	this->size = 0;
	this->front = 0;
	this->back = 0;
}

DLinkedList::DLinkedList(const DLinkedList &list)	//Copy Constructor a(list);
{
	Process * listfront = list.front;
	Process * frontcopy = new Process(listfront->bursts, listfront->sizeOfArr, listfront->processNumber);
	frontcopy->pre = 0;
	front = frontcopy;

	size = list.size;

	listfront = listfront->next;

	while (listfront != 0)
	{
		frontcopy->next = new Process(listfront->bursts, listfront->sizeOfArr, listfront->processNumber);
		frontcopy->next->pre = frontcopy;

		//Increment
		listfront = listfront->next;
		frontcopy = frontcopy->next;
	}

	frontcopy->next = 0;
	back = frontcopy;
}

void DLinkedList::push_back(Process *a) //pass by value ~ make a temp variable 
{
	Process * temp = a; //values in 'a' copied

	if (size == 0)
	{
		//In this instance front, back, and temp are all pointing to the same thing
		this->front = temp;
		this->back = temp;

		temp->pre = 0;
		temp->next = 0;
	}

	else//<-[]->
	{
		this->back->next = temp;
		temp->pre = back;
		temp->next = 0; //this->back->next = 0;
		this->back = temp;
	}

	this->size++;
}

void DLinkedList::push_front(Process *a) //Don't pass same pointer twice.
{
	Process * temp = a; //values in 'a' copied

	if (size == 0)
	{
		front = temp;
		back = temp;

		temp->next = 0;
		temp->pre = 0;
	}

	else
	{
		temp->next = this->front;
		temp->pre = 0;
		this->front->pre = temp;
		this->front = temp;
	}

	this->size++;
}

Process * DLinkedList::pop_front()
{
	Process * temp = this->front;
	Process * tempEmpty = 0;

	if (size == 0)
	{
		cout << "The Doubly Linked List is empty.";
		return tempEmpty;
	}

	if (size == 1)
	{
		front = 0;
		back = 0;

		size--;
		return temp;
	}

	if (size != 0)
	{
		this->front = this->front->next;
		this->front->pre = 0;

		temp->next = 0;
		temp->pre = 0; // Not Necessary

		size--;
		return temp;
	}
}

Process * DLinkedList::pop_back()
{
	Process * temp = back;
	Process * tempEmpty = 0;

	if (size == 0)
	{
		cout << "The Doubly Linked List is empty.";
		return tempEmpty;
	}

	if (size == 1)
	{
		front = 0;
		back = 0;

		size--;
		return temp;
	}

	if (size != 0)
	{
		back = back->pre;
		back->next = 0;

		temp->next = 0;
		temp->pre = 0;

		size--;
		return temp;
	}

}

Process * DLinkedList::getfront()
{
	return front;
}

Process * DLinkedList::getback()
{
	return back;
}

//out << left << setw(6) << setfill(8) << "Bob";
void DLinkedList::display(ostream &out)
{
	Process * temp = front;

	if (size == 0)
	{
		cout << "The the Doubly Linked List was empty.";
	}

	else
	{
		out << "Front";
		for (int i = 0; i < size; i++)
		{
			out << "->" << temp->bursts[0];
			temp = temp->next;
		}
		out << "->Back";
	}
}

void DLinkedList::insert(Process *a, int t) // insert the item a after the t-th element (head is the first element)
{
	Process * temp = front;
	int i = 1;

	while (i != t) //t = 1 means front
	{
		temp = front->next;
		i++;
	}

	a->next = temp->next;
	temp->next->pre = a;
	temp->next = a;
	a->pre = temp;

	size++;
}

// Insert the whole list a after the t-th element.
// You should make a copy of list first and then insert the copied version of the list. 
// Do not modify or remove the list from the input
void DLinkedList::insertlist(const DLinkedList &list, int t)
{
	DLinkedList temp(list);
	Process * temp2 = front, *temp3 = front;
	int i = 1;

	while (i != (t))
	{
		temp2 = front->next;
		i++;
	}

	temp2->next->pre = temp.back;
	temp.back->next = temp2->next;
	temp2->next = temp.front;
	temp.front->pre = temp2;

	while (temp3->next != 0)
	{
		temp3 = temp3->next;
	}

	back = temp3;
	size += list.size;
}

// Return the pointer of the min element after  "start",
// here you can assume user will always input a valid pointer start that points to an item in the list
Process * DLinkedList::extractmin(Process * start)
{
	Process * end = 0, *tempstart = start;
	int min = start->bursts[tempstart->arrayCounter];
	end = tempstart;

	while (tempstart != 0)
	{
		if ((tempstart->bursts[tempstart->arrayCounter]) < min)
		{
			min = tempstart->bursts[tempstart->arrayCounter];
			end = tempstart;
		}
		tempstart = tempstart->next;
	}

	return end;
}

// return the pointer of the max element after "start"
Process * DLinkedList::extractmax(Process * start)  // return the pointer of the max element after "start"
{
	Process * end = 0, *tempstart = start;
	int max = start->bursts[tempstart->arrayCounter];
	end = tempstart;

	while (tempstart->next != 0)
	{
		if ((tempstart->bursts[tempstart->arrayCounter]) > max)
		{
			max = tempstart->bursts[tempstart->arrayCounter];
			end = tempstart;
		}
		tempstart = tempstart->next;
	}

return end;
}

int DLinkedList::getSize()
{
	return size;
}

//***Timer functions
void timeWaster(int burst)
{
	int i = 0, p = 0, j = 0;
	for (p; p < burst; p++)
		for (j; j < 1000000; j++)
			i++;
}

double startClock()
{
	//clock_t start;
	return clock();
}

double getClockTicks(double start)
{
	return (clock() - start);
}

double getSeconds(double start)
{
	return (clock() - start) / (double)CLOCKS_PER_SEC;
}

void testClock() {
	double start = startClock();

	timeWaster(100);

	cout << "Seconds: " << getSeconds(start)
		<< " Clock-Ticks: " << clock() - start
		<< " Start: " << start
		<< " Current Clock: " << clock()
		<< '\n';
}

//***Scheduler tasks
void starttime(DLinkedList & readyList_copy)
{
	Process * top = readyList_copy.getfront();

	//set starttime
	for (int i = readyList_copy.getSize(); i > 0; i--)
	{
		top->starttime = startClock();
		top = top->next;
	}
}

void readyToTerm(Process * running, DLinkedList & terminatedList)
{
	running->turnaround = running->turnaround + getClockTicks(running->starttime); //Add waiting in ready
	running->waiting = running->waiting + getClockTicks(running->starttime);//Add waiting in ready
	terminatedList.push_back(running);
}

//Updates values and shifts to ready
void waitToReady(Process * running, DLinkedList & ready)
{
	running->starttime = startClock();
	ready.push_back(running);
}

//Updates values and shifts to waiting
void readyToWait(Process * running, DLinkedList & wait)
{
	running->turnaround = running->turnaround + getClockTicks(running->starttime); //***
	wait.push_back(running);
}

Process * pickprocess(DLinkedList &readyList_copy, DLinkedList &terminatedList, DLinkedList &waitingList, int & rcounter)
{
	Process * running;

	if ((readyList_copy.getSize() != 0) && (rcounter != 1))  
	{
		running = readyList_copy.pop_front();
		if (running->arrayCounter >= running->sizeOfArr)
		{
			readyToTerm(running, terminatedList);
			return pickprocess(readyList_copy, terminatedList, waitingList, rcounter);
		}
	}
	else if (waitingList.getSize() != 0)
	{
		rcounter = 1;
		running = waitingList.pop_front();

		if (waitingList.getSize() == 0)
		{
			rcounter = 0;
		}

		if (running->arrayCounter >= running->sizeOfArr)
		{
			terminatedList.push_back(running); //Don't Update values
			return pickprocess(readyList_copy, terminatedList, waitingList, rcounter);
		}

	}
	else {
		return 0;
	}

	return running;
}

void execute(Process * running, DLinkedList & ready, DLinkedList &wait, DLinkedList & terminated)
{
	//Set values
	//First time running - and only readylist processes
	if ((running->arrayCounter % 2) == 0) {
		if (running->arrayCounter == 0)
			{
				running->responsetime = getClockTicks(running->starttime);	//Clock ticks since start time
				running->waiting = getClockTicks(running->starttime);		//Added Time in readylist
				running->turnaround = getClockTicks(running->starttime);					//Time in readylist
			}
		else //everything else
			{
				running->waiting = running->waiting + getClockTicks(running->starttime);		//Added Time in readylist
				running->turnaround = running->turnaround + getClockTicks(running->starttime);	//Added Time in readylist
			}
	}
	//"run"
	timeWaster(running->bursts[running->arrayCounter]);

	//Update Values
	running->arrayCounter = running->arrayCounter + 1;

	//Shift - Figure out where to put new burst
	//Check for bursts
	if (running->arrayCounter < running->sizeOfArr) {
		if ((running->arrayCounter % 2) == 1) //CPU burst is always first so (even integers - index starts at 0) so...
		{
			readyToWait(running, wait);
		}
		else
		{
			waitToReady(running, ready);
		}
	}
	else {
		if ((running->arrayCounter % 2) == 1) 
		{
			readyToTerm(running, terminated);
		}
		else //I/O doesn't change values
		{
			terminated.push_back(running);
		}
	}

}

void printSampleProcList(DLinkedList & que)
{
	Process * front = que.getfront();	
	cout << "Process " << "\tBurst " << "\tCurrent Execution Time " << endl; //<< "\tWaiting (Tw) " << "\t" << "Turnaround (Ttr) " << "\t" << "Response (Tr) " << endl;

	while (front != 0)
	{
		cout << front->processNumber << "\t\t" << front->bursts[front->arrayCounter] << "\t" << startClock() << endl; // "\t\t" << front->waiting << "\t\t" << front->turnaround << "\t\t\t" << front->responsetime << endl;
		front = front->next;
	}
}

void printRunning(Process * running)
{
	cout << "Process " << "\tBurst " << "\tCurrent-Execution-Time "<< endl; //<< "Waiting-(Tw) " << "Turnaround-(Ttr) " << "Response-(Tr)" << endl;
	cout << running->processNumber << "\t\t" << running->bursts[running->arrayCounter - 1] << "\t" << startClock()<< endl; //<< "\t\t" << running->waiting << "\t\t" << running->turnaround << "\t\t" << running->responsetime << endl;
}

void printResults(DLinkedList & terminatedList, double totalCPUtime, double idle, double stime)
{
	Process * term = terminatedList.getfront();
	int sizeofterm = terminatedList.getSize();
	double cputime = 0, wait = 0, turn = 0, response = 0;

	cout << endl << "***Results: " << endl << "Note: Final results from Terminated List." << endl;

	while (term != 0)
	{
		cout << "Process " << "\tWaiting-(Tw) " << "\tTurnaround-(Ttr) " << "\tResponse-(Tr)" << endl;
		cout << term->processNumber << "\t\t" << term->waiting << "\t\t" << term->turnaround << "\t\t\t" << term->responsetime << endl;

		wait = wait + term->waiting;
		turn = turn + term->turnaround;
		response = response + term->responsetime;
		term = term->next;
	}

	cout << "CPU Utilization: " << (totalCPUtime - idle)/totalCPUtime * 100 << "%"<< endl;
	cout << "Avg Waiting time (Tw): " << wait / sizeofterm << endl;
	cout << "Avg Turnaround time (Ttr): " << turn / sizeofterm << endl;
	cout << "Avg Response time (Tr): " << response / sizeofterm << endl;
	cout << "Start Time: " << stime << endl;
	cout << "End Time: " << startClock() << endl;
	cout << "Total Time Needed to Complete all 8 Processes: " << getClockTicks(stime) << endl;
}

Process * pickprocessSJF(DLinkedList &readyList_copy, DLinkedList &terminatedList, DLinkedList &waitingList, int & rcounter)
{
	//
	Process * running, * smallest;
	DLinkedList hold;

	if ((readyList_copy.getSize() != 0) && (rcounter != 1))
	{
		//Pick shortest job first
		smallest = readyList_copy.extractmin(readyList_copy.getfront());
		while (readyList_copy.getfront() != smallest)
		{
			readyList_copy.push_back(readyList_copy.pop_front());
		}

		//same
		running = readyList_copy.pop_front();

		if (running->arrayCounter >= running->sizeOfArr)
		{
			readyToTerm(running, terminatedList);
			return pickprocess(readyList_copy, terminatedList, waitingList, rcounter);
		}
	}
	else if (waitingList.getSize() != 0)
	{
		rcounter = 1;
		running = waitingList.pop_front();

		if (waitingList.getSize() == 0)
		{
			rcounter = 0;
		}

		if (running->arrayCounter >= running->sizeOfArr)
		{
			terminatedList.push_back(running); //Don't Update values
			return pickprocess(readyList_copy, terminatedList, waitingList, rcounter);
		}

	}
	else {
		return 0;
	}

	return running;
}

Process * pickprocessMLFQ(DLinkedList &rrRL, DLinkedList &rrRL2, DLinkedList & fcfsRL, DLinkedList &terminatedList, DLinkedList &waitingList, int & rcounter)
{
	Process * running;

	if ((rrRL.getSize() != 0) && (rcounter != 1))
	{
		running = rrRL.pop_front();
		running->queue = "rr1";
		if (running->arrayCounter >= running->sizeOfArr)
		{
			readyToTerm(running, terminatedList);
			return pickprocessMLFQ(rrRL, rrRL2, fcfsRL, terminatedList, waitingList, rcounter);
		}
	}
	else if ((rrRL2.getSize() != 0) && (rcounter != 1))
	{
		running = rrRL2.pop_front();
		running->queue = "rr2";
		if (running->arrayCounter >= running->sizeOfArr)
		{
			readyToTerm(running, terminatedList);
			return pickprocessMLFQ(rrRL, rrRL2, fcfsRL, terminatedList, waitingList, rcounter);
		}
	}
	else if ((fcfsRL.getSize() != 0) && (rcounter != 1))
	{
		running = fcfsRL.pop_front();
		running->queue = "fcfs";
		if (running->arrayCounter >= running->sizeOfArr)
		{
			readyToTerm(running, terminatedList);
			return pickprocessMLFQ(rrRL, rrRL2, fcfsRL, terminatedList, waitingList, rcounter);
		}
	}
	else if (waitingList.getSize() != 0)
	{
		rcounter = 1;
		running = waitingList.pop_front();

		if (waitingList.getSize() == 0)
		{
			rcounter = 0;
		}

		if (running->arrayCounter >= running->sizeOfArr)
		{
			terminatedList.push_back(running); //Don't Update values
			return pickprocessMLFQ(rrRL, rrRL2, fcfsRL, terminatedList, waitingList, rcounter);
		}

	}
	else {
		return 0;
	}

	return running;
}

void executeMLFQ(Process * running, DLinkedList &rrRL, DLinkedList &rrRL2, DLinkedList & fcfsRL, DLinkedList &terminatedList, DLinkedList &waitingList)
{
	int finished = 0;

	//Set values
	//First time running - and only readylist processes
	if ((running->arrayCounter % 2) == 0) {
		if (running->arrayCounter == 0)
		{
			running->responsetime = getClockTicks(running->starttime);	//Clock ticks since start time
			running->waiting = getClockTicks(running->starttime);		//Added Time in readylist
			running->turnaround = getClockTicks(running->starttime);	//Time in readylist - running->starttime
		}
		else //everything else
		{
			running->waiting = running->waiting + getClockTicks(running->starttime);		//Added Time in readylist
			running->turnaround = running->turnaround + getClockTicks(running->starttime);	//Added Time in readylist
		}
	}

	//"run"
	if ((running->arrayCounter % 2) == 1) //I/o
	{
		timeWaster(running->bursts[running->arrayCounter]);
		//Update Values
		running->arrayCounter = running->arrayCounter + 1;
		finished = 1;
	}
	else if(running->queue == "rr1")
	{
		timeWaster(6);
		running->bursts[running->arrayCounter] = running->bursts[running->arrayCounter] - 6;
		
		//Update Values
		if(running->bursts[running->arrayCounter] <= 0)
		{
			running->arrayCounter = running->arrayCounter + 1;
			finished = 1;
		}
		else
		{
			running->queue = "rr2";
		}
	}
	else if (running->queue == "rr2") 
	{
		timeWaster(11);
		running->bursts[running->arrayCounter] = running->bursts[running->arrayCounter] - 11;
		
		//Update Values
		if (running->bursts[running->arrayCounter] <= 0)
		{
			running->arrayCounter = running->arrayCounter + 1;
			finished = 1;
		}
		else
		{
			running->queue = "fcfs";
		}
	}
	else if (running->queue == "fcfs") 
	{
		timeWaster(running->bursts[running->arrayCounter]);
		//Update Values
		running->arrayCounter = running->arrayCounter + 1;
		finished = 1;
	}

	//Shift - Figure out where to put new burst
	//Check for bursts
	if (running->arrayCounter < running->sizeOfArr) {
		if ((running->arrayCounter % 2) == 1) //CPU burst is always first so (even integers - index starts at 0) so...
		{
			readyToWait(running, waitingList);
		}
		else//even - cpu
		{
			if ((running->queue == "rr2") && (finished != 1)) {
				//go to rr2
				rrRL2.push_back(running);
			}
			else if((running->queue == "fcfs") && (finished != 1)){
				//go to fcfs
				fcfsRL.push_back(running);
			}
			else {
				//which ready list? (Finished I/O)
				if (running->queue == "rr1"){
					waitToReady(running, rrRL);
				}
				if (running->queue == "rr2") {
					waitToReady(running, rrRL2);
				}
				if (running->queue == "fcfs") {
					waitToReady(running, fcfsRL);
				}
				
			}
		}
	}
	else {
		if ((running->arrayCounter % 2) == 1) 
		{
			readyToTerm(running, terminatedList);
		}
		else //I/O doesn't change values
		{
			terminatedList.push_back(running);
		}
	}

}

//***Schedulers
void FCFS(DLinkedList & readyList)
{
	//Initialize
	DLinkedList readyList_copy(readyList), waitingList, terminatedList;
	Process * running;
	int rlsize = readyList_copy.getSize(), printcount = 30, rcounter = 0;
	double stime = startClock(), etime = 0, notidle1 = 0, notidle2 = 0; 
	starttime(readyList_copy); //Set startimes of all processes

	//Run until terminated list is full.
	while (terminatedList.getSize() < rlsize)
	{
		running = pickprocess(readyList_copy, terminatedList, waitingList, rcounter);
		if (running == 0)
		{
			break;
		}
		else 
		{
			//Only add cpu time if a ready process will run
			if (running->arrayCounter % 2 == 0) {
				notidle1 = startClock();
			}
			else
			{
				notidle1 = 0;
			}

			//run
			execute(running, readyList_copy, waitingList, terminatedList);

			if (notidle1 != 0) {
				notidle2 = notidle2 + getClockTicks(notidle1);
			}

			//display
			if (printcount >= 1)
			{
				cout << "\n\t\tCurrently Running\n";
				printRunning(running);
				cout << "\n\t\tReady List\n";
				printSampleProcList(readyList_copy);
				cout << "\n\t\tWaiting List\n";
				printSampleProcList(waitingList);
				cout << "\n\t\tTerminated List\n";
				printSampleProcList(terminatedList);

				printcount = printcount--;
			}

		}

	}

	etime = getClockTicks(stime);
	printResults(terminatedList, etime, notidle2, stime);
}

void SJF(DLinkedList readyList)
{
	//Initialize
	DLinkedList readyList_copy(readyList), waitingList, terminatedList;
	Process * running;
	int rlsize = readyList_copy.getSize(), printcount = 30, rcounter = 0;
	double stime = startClock(), etime = 0, notidle1 = 0, notidle2 = 0;
	starttime(readyList_copy); //Set startimes of all processes

							   //Run until terminated list is full.
	while (terminatedList.getSize() < rlsize)
	{
		running = pickprocessSJF(readyList_copy, terminatedList, waitingList, rcounter);
		if (running == 0)
		{
			break;
		}
		else
		{
			//Only add cpu time if a ready process will run
			if (running->arrayCounter % 2 == 0) {
				notidle1 = startClock();
			}
			else
			{
				notidle1 = 0;
			}

			//run
			execute(running, readyList_copy, waitingList, terminatedList);

			if (notidle1 != 0) {
				notidle2 = notidle2 + getClockTicks(notidle1);
			}

			//display
			if (printcount >= 1)
			{
				cout << "\n\t\tCurrently Running\n";
				printRunning(running);
				cout << "\n\t\tReady List\n";
				printSampleProcList(readyList_copy);
				cout << "\n\t\tWaiting List\n";
				printSampleProcList(waitingList);
				cout << "\n\t\tTerminated List\n";
				printSampleProcList(terminatedList);

				printcount = printcount--;
			}

		}

	}

	etime = getClockTicks(stime);
	printResults(terminatedList, etime, notidle2, stime);
}

void MLFQ(DLinkedList readyList){
	//Initialize 
	DLinkedList rrRL(readyList) , rrRL2, fcfsRL, waitingList, terminatedList;
	Process * running;
	int rlsize = rrRL.getSize(), printcount = 30, rcounter = 0;
	double stime = startClock(), etime = 0, notidle1 = 0, notidle2 = 0;
	starttime(rrRL); //Set startimes of all processes

	//Run until terminated list is full.
	while (terminatedList.getSize() < rlsize)
	{
		running = pickprocessMLFQ(rrRL, rrRL2, fcfsRL, terminatedList, waitingList, rcounter);
		
		if (running == 0)
		{
			break;
		}
		else
		{
			//Only add cpu time if a ready process will run
			if (running->arrayCounter % 2 == 0) { //(rrRL.getSize() != 0) || (rrRL2.getSize() != 0) || (fcfsRL.getSize() != 0)
				notidle1 = startClock();
			}
			else
			{
				notidle1 = 0;
			}

			//run
			executeMLFQ(running, rrRL, rrRL2, fcfsRL, terminatedList, waitingList);

			if (notidle1 != 0) {
				notidle2 = notidle2 + getClockTicks(notidle1);
			}

			//display
			if (printcount >= 1)
			{
				cout << "\n\t\tCurrently Running\n";
				printRunning(running);
				cout << "\n\t\tRound Robin (tq = 6) Ready List\n";
				printSampleProcList(rrRL);
				cout << "\n\t\tRound Robin Ready (tq = 11) List\n";
				printSampleProcList(rrRL2);
				cout << "\n\t\tFCFS Ready List\n";
				printSampleProcList(fcfsRL);
				cout << "\n\t\tWaiting List\n";
				printSampleProcList(waitingList);
				cout << "\n\t\tTerminated List\n";
				printSampleProcList(terminatedList);
				cout << endl << endl << endl;

				printcount = printcount--;
			}

		}

	}

	etime = getClockTicks(stime);
	printResults(terminatedList, etime, notidle2, stime);
}

//***Test
void test()
{
	int * p1 = new int[17]{ 4, 24, 5, 73, 3, 31, 5, 27, 4, 33, 6, 43, 4, 64, 5, 19, 2 };
	int * p2 = new int[17]{ 18, 31, 19, 35, 11, 42, 18, 43, 19, 47, 18, 43, 17, 51, 19, 32, 10 };
	int * p3 = new int[17]{ 6, 18, 4, 21, 7, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	int * p4 = new int[15]{ 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	int * p5 = new int[19]{ 5, 81, 4, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 };
	int * p6 = new int[13]{ 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	int * p7 = new int[11]{ 21, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20 };
	int * p8 = new int[15]{ 11, 52, 14, 42, 15, 31, 17, 21, 16, 43, 12, 31, 13, 32, 15 };

	Process * P1 = new Process(p1, 17, 1);
	Process * P2 = new Process(p2, 17, 2);
	Process * P3 = new Process(p3, 17, 3);
	Process * P4 = new Process(p4, 15, 4);
	Process * P5 = new Process(p5, 19, 5);
	Process * P6 = new Process(p6, 13, 6);
	Process * P7 = new Process(p7, 11, 7);
	Process * P8 = new Process(p8, 15, 8);

	DLinkedList ready;
	ready.push_back(P1);
	ready.push_back(P2);
	ready.push_back(P3);
	ready.push_back(P4);
	ready.push_back(P5);
	ready.push_back(P6);
	ready.push_back(P7);
	ready.push_back(P8);

	cout << "First Come First Serve (FCFS): " << endl;
	FCFS(ready);
	cout << endl  <<"Shortest Job First (SJF): " << endl;
	SJF(ready);
	cout << endl <<"Multilevel Feedback Queue (3 Queues -> RR:Tq = 6, RR:Tq = 11, FCFS): " << endl;
	MLFQ(ready);
}

int main()
{
	test();
	return 0;
};