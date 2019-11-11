#include <iostream>
#include <ctime>
#include <string>
#include <list>
#include <fstream>
#include <cstdlib>
#include "flight.h"

using namespace std;

int main(int argc,char *argv[]){
	time_t t=atoi(argv[1]);
	int passenger_count=0;
	int flight_count=0;
	int addf=0;
	int addp=0;
	srand(time(NULL));
	const int term=7;
	time_t temp_time=0;
	
	airport a(temp_time,term);

	for(int i=0;i<5;i++){
		addf=0;
		addp=0;
		cout << "loop " << i << endl;
		a.flow_time(t);
		cout << "time " << a.get_time() << endl;
		
		ifstream myfile("FlightInformation.txt");			//reads file with flight information
		int temp1=rand()%term;								//and adds a random number of flights to terminals
		string s;
		for(int j=0;j<flight_count;j++){
			getline(myfile,s);
		}
		while(true && addf<temp1){
			string Dst;
			time_t DepTime;
			time_t FlTime;
			int MFClass;
			int MNClass;
			myfile >> Dst >> DepTime >> FlTime >> MFClass >> MNClass;
			if(myfile.eof()) break;
			//cout << Dst << " "<< DepTime <<" "<< FlTime <<" "<< MFClass <<" "<< MNClass << endl;
			
			a.add_flight(Dst,DepTime,FlTime,MFClass,MNClass);
			flight_count++;
			addf++;
			
		}
		
		ifstream in("PassengerInformation.txt");		//reads file with passenger information
		int temp2=rand()%10;							//and adds a random number of passengers to either the flights or the waiting list
		string s2;
		for(int m=0;m<passenger_count;m++){
			getline(in,s2);
		}
		while(true && addp<temp2){
			int rid;
			string rfname;
			string rlname;
			string rdest;
			time_t raat;
			time_t rpad;
			int rtt;
			in >> rid >> rfname >> rlname >> rdest >> raat >> rpad >> rtt;
			if(in.eof()) break;	
			//cout << rid << " "<< rfname <<" "<< rlname <<" "<< rdest <<" "<< raat <<" "<< rpad <<" "<< rtt << endl;
			
			a.add_application(rid,rfname,rlname,rdest,raat,rpad,rtt);
			
			passenger_count++;
			addp++;
		}
		a.add_application_list();			
		a.show_timetable();				//prints the flights(destination) in the terminals in ascending departure time
		a.show_people_waiting();		//prints the names of the passengers in the waiting list
	}
		a.show_timetable();
}
