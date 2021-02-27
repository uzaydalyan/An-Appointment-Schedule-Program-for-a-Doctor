/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

/* @Author
Student Name: Uzay Dalyan
Student ID : 150170041
Date: 13.11.2019 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL;
	tail = NULL;
}

void WorkPlan::close()
{
	Task *traverse, *traverseback;
	
	traverse = head;
	while(traverse){
		
		traverseback = traverse;
		traverse = traverse->next;
		remove(traverseback);
	
		if(traverse->next == traverse){
			
			delete traverse;
			return;
		}
	}
}

void WorkPlan::add(Task *task)
{
	Task *traverse, *traverseback, *fortime, *fortimeback, *toadd;
	toadd = new Task;
	toadd->name = new char[strlen(task->name) + 1];
	strcpy(toadd->name, task->name);
	toadd->priority = task->priority;
	toadd->day = task->day;
	toadd->time = task->time;
	
	if(head == NULL){  //adding first task
		
		toadd->counterpart = NULL;
		toadd->next = toadd;
		toadd->previous = toadd;
		head = toadd;
		tail = toadd;
		return;
	}
		
	if(toadd->day < head->day){ // adding day before the first day
		
		toadd->next = head;
		head->previous = toadd;
		toadd->counterpart = NULL;
		head = toadd;
		tail->next = toadd;
		toadd->previous = tail;
		return;
	}
	
	traverse = head;    // deciding if that day exists in list
	while(traverse){
		
		if(traverse->day == toadd->day){
			
			break;
		}
		
		traverse = traverse->next;
		
		if(traverse == head){
			
			break;
		}
	}
	
	fortime = traverse;
	
	if(traverse->day == toadd->day){ // day exists
		
		while(traverse){// deciding if time exists
			
			if(traverse->time == toadd->time){
				
				break;
			}
			
			traverseback = traverse;
			traverse = traverse->counterpart;
		}
		
		if(traverse){ //time exists
			
			if(toadd->priority > traverse->priority){ // task has priority
				
				checkAvailableNextTimesFor(traverse);
				Task *newnode;
				newnode = new Task;
				newnode->day = usable_day;
				newnode->time = usable_time;
				newnode->priority = traverse->priority;
				newnode->name = new char[strlen(traverse->name) + 1];
				strcpy(newnode->name, traverse->name);
				remove(traverse);
				add(toadd);
				add(newnode);
				return;
			}
			
			else{ // traverse has priority
				
				checkAvailableNextTimesFor(toadd);
				toadd->day = usable_day;
				toadd->time = usable_time;
				add(toadd);
				return;
			}
		}
		
		else{ // time doesn't exist
			
			if(toadd->time < fortime->time){
			
				if(fortime->next != fortime){
					toadd->counterpart = fortime;
					toadd->next = fortime->next;
					toadd->previous = fortime->previous;
					toadd->next->previous = toadd;
					toadd->previous->next = toadd;
					if(head == fortime){
						
						head = toadd;
					}
					if(tail == fortime){
						tail = toadd;
					}
					fortime->next = NULL;
					fortime->previous = NULL;
					return;
				}
				
				else{
					
					toadd->next = toadd;
					toadd->previous = toadd;
					toadd->counterpart = fortime;
					head = toadd;
					if(tail == fortime){
						
						tail = toadd;
					}
					fortime->next = NULL;
					fortime->previous = NULL;
					return;
				}
			}
			
			while(fortime){
				
				if(fortime->time > toadd->time){
		
					break;
				}
				
				fortimeback = fortime;
				fortime = fortime->counterpart;
			}
			
			if(fortime){   // adding between hours
				
				toadd->next = NULL;
				toadd->previous = NULL;
				toadd->counterpart = fortime;
				fortimeback->counterpart = toadd;
				return;
			}
			
			else{
				
				toadd->next = NULL;
				toadd->previous = NULL;
				fortimeback->counterpart = toadd;
				toadd->counterpart = NULL;
				return;
				
			}
		}
	}
	
	else { // day doesn't exist
		
		traverse = head;  
		while(traverse){
			
			if(traverse->day > toadd->day){
				
				break;
			}
			
			traverseback = traverse;
			traverse = traverse->next;
			
			if(traverse == head){
				
				break;
			}
		}
		
		if (traverse == head){  // adding day to the end
			
			tail->next = toadd;
			toadd->previous = tail;
			toadd->next = head;
			head->previous = toadd;
			toadd->counterpart = NULL;
			tail = toadd;
			return;
			
		}
		
		else{    //adding day between days
			
			toadd->counterpart = NULL;
			toadd->next = traverse;
			traverse->previous = toadd;
			toadd->previous = traverseback;
			traverseback->next = toadd;
			return;
		}
	}
} 

Task * WorkPlan::getTask(int day, int time) 
{
	Task *traverse;
	traverse = head;
	
	while(traverse){
		
		if(traverse->day == day){
			
			break;
		}
		
		traverse = traverse->next;
		
		if(traverse == head){
			
			return NULL;
		}
		
	}
	
	while(traverse){
		
		if(traverse->time == time){
			
			break;
		}
		
		traverse = traverse->counterpart;
		
	}
	
	if(traverse){
		
		return traverse;
	}
	
	else{
		
		return NULL;
	}
	
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	
	Task *traverse, *traverseback;
	int delayday, delaytime;
	delayday = delayed->day;
	delaytime = delayed->time + 1;
	
	while(delayed){
		
		traverse = NULL;
		traverseback = head;
		while(traverseback){ // check if day exists
			
			if(traverseback->day == delayday){
				
				break;
			}
			
			traverseback = traverseback->next;
			
			if(traverseback == head){
				
				break;
			}
		}
		
		if(traverseback->day == delayday){// if day exists finding last time in day
		
			traverse = head;
			while(traverse){ 
				
				if(traverse->day == delayday){
					
					break;
				}
				
				traverse = traverse->next;
			}
			
			while(traverse->counterpart){
				
				traverse = traverse->counterpart;
				
			}
		}
	
	
		while(delaytime < 16){
			
			if(traverse){ // if day exists
				if(traverse->time > delaytime){// if it's before last appointment
					
					if(!(getTask(delayday, delaytime))){
						
						
						usable_day = delayday;
						usable_time = delaytime;
						return;
					}
				}
				
				delaytime++;
			}
			
			else{ // day doesn't exist
			
						usable_day = delayday;
						usable_time = 8;
						return;
					
			}
		}
		
		delayday++;
		delaytime = 8;
	}
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *traverse, *traverseback, *fortime, *delayertask;
	traverse = head;
	
	while(traverse){ // finding day in list
		
		if(traverse->day == day){
			
			break;
		}
		
		traverse = traverse->next;
	}
	
	traverseback = traverse;
	while(traverseback->counterpart){ // finding last appointment in that day
		
		traverseback = traverseback->counterpart;
		
	}
	
	delayertask = new Task; // creating a new last appointment to use it for delaying others
	delayertask->day = traverseback->day;
	delayertask->time = traverseback->time + 1;
	delayertask->name = new char[strlen(traverse->name) + 1];
	strcpy(delayertask->name, traverse->name);
	delayertask->priority = 10;
	traverseback->counterpart = delayertask;
	delayertask->next = NULL;
	delayertask->previous = NULL;
	delayertask->counterpart = NULL;
	
	while(traverse != delayertask){
		
		Task *newnode = new Task;
		newnode->day = traverse->day;
		newnode->priority = traverse->priority;
		newnode->time = traverseback->time + 1;
		newnode->name = new char[strlen(traverse->name) + 1];
		strcpy(newnode->name, traverse->name);
		checkAvailableNextTimesFor(newnode);
		newnode->day = usable_day;
		newnode->time = usable_time;
		add(newnode);
		fortime = traverse;
		traverse = traverse->counterpart;
		remove(fortime);
	}
	
	remove(delayertask);
	
	return;
	
}

void WorkPlan::remove(Task *target)
{
	Task *traverse, *traverseback;
	
	if(target->next){ //if target is first hour of a day
		
		if(target->next == target){ // if target is the only day in schedule
			
			if(target->counterpart){ // if target is not the only hour in its day
				
				traverse = target->counterpart;
				traverse->next = traverse;
				traverse->previous = traverse;
				head = traverse;
				tail = traverse;
				delete target;
				return;
			}
			
			else{// if target is only hour in its day
				
				traverse = target->next;
				traverseback = target->previous;
				if(target == head){
					
					head = traverse;
				}
				
				if(target == tail){
					
					tail = traverseback;
				}
				traverse->previous = traverseback;
				traverseback->next = traverse;
				delete target;
				return;
			}
			
		}
		
		else{//if target is not the only day in schedule
			
			if(target->counterpart){//if target is NOT only hour in its day
				
				traverse = target->counterpart;
				traverse->next = target->next;
				traverse->previous = target->previous;
				target->previous->next = traverse;
				target->next->previous = traverse;
				if(target == head){
					
					head = traverse;
				}
				
				if(target == tail){
					
					tail = traverse;
				}
				delete target;
				return;
			}
			
			else{//if target is the only hour in its day
				
				traverse = target->next;
				traverseback = target->previous;
				if(target == head){
					
					head = traverse;
				}
				
				if(target == tail){
					
					tail = traverseback;
				}
				traverse->previous = traverseback;
				traverseback->next = traverse;
				delete target;
				return;
			}
		}
	}
	
	else{//if target is not the first hour in its day
		
		traverse = head;
		while(traverse){ // choosing that day for starting operations from first hour
			
			if(traverse->day == target->day){
				
				break;
			}
			
			traverse= traverse->next;
		}
		
		while(traverse){ // choosing time
			
			traverseback = traverse;
			traverse = traverse->counterpart;
			if(traverse->time == target->time){
				
				break;
			}
		}
		
		if(traverse->counterpart){ // if it is NOT the last hour in its day
			
			traverseback->counterpart = traverse->counterpart;
			delete traverse;
			return;
		}
		
		else{//if it is the last hour in its day
			
			delete traverse;
			traverseback->counterpart = NULL;
			return;
		}
	}
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
