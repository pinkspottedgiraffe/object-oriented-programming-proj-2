#include <iostream>
#include <ctime>
#include <string>
#include <list>
#include <fstream>
#include <cstdlib>
#include "flight.h"

using namespace std;

application::~application(){			//application destructor
	cout << "application destroyed" << endl;
}

int application::get_id(){				
	return id;
}

time_t application::arrive_by(){
	return PrefferedArrivalTime;
}

int application::is_luxury_class(){		//returns 1 if the ticket is luxury class, or 0 if it isn't
	if (TicketType==1){
		return 1;
	}
	else{
		return 0;
	}
}

int application::matches(flight& f){		//checks if application matches flight
	string str1=f.get_destination();
	
	if(str1.compare(Destination)==0 && (f.arrives_at()<=PrefferedArrivalTime) && (f.departs_at()>=AirportArrivalTime)){
		return 1;
	}
	else 
		return 0;
}


string application::get_name(){
	return FirstName+' '+LastName;
}

time_t flight::departs_at(){
	return DepartureTime;
}

time_t flight::arrives_at(){
	return (DepartureTime+FlightTime);
}

string flight::get_destination(){
	return Destination;
}

int flight::get_available_2(int t){					//returns number of available places in first class or number of available places in normal class
	if(t==1){
		return(MaxFirstClass-NumOfPassengers1);
	}
	else if(t==0){
		return(MaxNormalClass-NumOfPassengers0);
	}
}

int flight::get_available(){
	return(MaxFirstClass+MaxNormalClass-NumOfPassengers1-NumOfPassengers0);		//returns number of available places in flight for all ticket types
}

int flight::add_passenger(application& p){
	int k=p.is_luxury_class();
	int t=get_available_2(k);		//checks if there is space for the passenger 
	if(t!=0){
		passengers.push_back(p);
	
		if(k==1){
			NumOfPassengers1++;
		}
		else{
			NumOfPassengers0++;
		}
		cout << "passenger added" << endl;
	}	
}

int flight::cancel_reservations(int n){						//cancels reservations in flight with certain id 
	list<application>::iterator it=passengers.begin();
	while(it!=passengers.end()){
	int k=it->get_id();
		if(n==k){
			it=passengers.erase(it);
		}
		else{
			++it;
		}		
	}	
}

list<application> flight::get_bookings(){
	return passengers;
}

time_t airport::get_time(){
	return Current_time;
}

int airport::failed_applications(){
	return fails;
}

void airport::add_application(int num,const string fname,string lname,string dest,time_t aatime,time_t ptime,int ttype){
	int temp=0;
	application p(num,fname,lname,dest,aatime,ptime,ttype);
	
	for(int i=0; i<terminals; i++){
		if(terminal_array[i]!=NULL && temp==0){
			int k=p.matches(*terminal_array[i]);
			
			if(k==1){
				terminal_array[i]->add_passenger(p);		//if application matches with a flight it adds the passenger to the flight
				cout <<"flight " << i << endl;
				temp=1;
			}
		}
	}
	if(temp==0){											//if the application doesn't match with a flight it adds the passenger to the application list
		application_list.push_back(p);
		cout << "passenger added to application list" << endl;
	}
	
}

void airport::add_application_list(){
	list<application>::iterator it;
	for(int i=0;i<terminals;i++){
		if(terminal_array[i]!=NULL){
			it=application_list.begin();
			while(it!=application_list.end()){
				int k=it->matches(*terminal_array[i]);
				if(k==1){										//adds passengers from the application list to flights that they match
					terminal_array[i]->add_passenger(*it);
					
					cout << "application added from application list " << it->get_name() << endl;
					it=application_list.erase(it);
				}
				else{
					it++;
				}
			}
		}
	}
}

void airport::cancel_applications(int k){
	for(int i=0;i<terminals;i++){					//cancels applications from all the flights that have a certain id
		if(terminal_array[i]!=NULL){
			terminal_array[i]->cancel_reservations(k);
		}
	}
	list<application>::iterator it=application_list.begin();	//cancels applications from the application list that have a certain id
	while(it!=application_list.end()){
	int	n=it->get_id();
		if(n==k){
			it=application_list.erase(it);
		}
		else{
			++it;
		}
	}
	fails++;	
}

void airport::add_flight(string dst, time_t dep, time_t fl, int fclass, int nclass){
	int counter=0;
	for(int i=0;i<terminals;i++){					//adds flight to terminal
		if(counter==0){
		if(terminal_array[i]==NULL && dep>=Current_time){
			terminal_array[i]=new flight(dst, dep, fl, fclass, nclass);
			counter=1;
			cout << "flight added to terminal " << i << endl;
		}
		}
	}
	if(counter==0){
		cout << "add flight failed " << endl;
	}
}

void airport::cancel_flight(int k){					//cancels flight 
	if(terminal_array[k]!=NULL){
		list<application> li=terminal_array[k]->get_bookings();
		application_list.insert(application_list.begin(),li.begin(),li.end()); //and adds the bookings from the flight to the start of 
		delete terminal_array[k];												//the application list
		terminal_array[k]=NULL;
		cout << "flight cancelled" << endl;
	}	
}

void airport::show_people_waiting(){
	list<application>::iterator it=application_list.begin();
	while(it!=application_list.end()){
		string n=it->get_name();			//prints all the people in the application list
		cout << n << endl;
		++it;
	}
}

void airport::show_timetable(){				
	
	flight** temp_array;
	flight* temp;
	temp_array=new flight*[terminals];
	for(int k=0;k<terminals;k++){				//copies the terminal_array onto a different array
		temp_array[k]=terminal_array[k];
	}
	for(int i=0;i<terminals;i++){				//sorts the flights by ascending time of departure
		for(int j=i+1;j<terminals;j++){
			if(temp_array[j]!=NULL){
				if(temp_array[i]!=NULL){
					if(temp_array[j]->departs_at() < temp_array[i]->departs_at()){
						temp=temp_array[i];
						temp_array[i]=temp_array[j];
						temp_array[j]=temp;
					}
				}
			}
		}
	}
	for(int h=0;h<terminals;h++){					//prints the destination, the departure time and the arrival time
		if(temp_array[h]!=NULL){
			cout << "destination " << temp_array[h]->get_destination();
			cout << " departs at " << temp_array[h]->departs_at();
			cout << " arrives at " << temp_array[h]->arrives_at() << endl;	
		}
	}
}

void airport::flow_time(time_t t){
	time_t x;
	x=Current_time;
	Current_time=Current_time+t;			//increments the current time by t
	for(int i=0; i<terminals ; i++){
		if(terminal_array[i]!=NULL){
		if(terminal_array[i]->departs_at()>x && terminal_array[i]->departs_at()<Current_time){
			
			string k=terminal_array[i]->get_destination();				//prints the destination of the flights that departed
			cout << k <<" departed" << endl;
			list<application> li=terminal_array[i]->get_bookings();
			list<application>::iterator it=li.begin();					//prints the names of the passengers of the plane that departed
			while(it!=li.end()){
				cout << it->get_name() << endl;
				it++;
			}
			delete terminal_array[i];
			terminal_array[i]=NULL;
		}
	}	
	}
	list<application>::iterator it=application_list.begin();		//removes applications whose preffered arrival time has passed 
	while(it!=application_list.end()){
		time_t k=it->arrive_by();
		if(k<Current_time){
			it=application_list.erase(it);
		}
		else{
			++it;
		}
	}
}
