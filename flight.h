#include <iostream>
#include <ctime>
#include <string>
#include <list>
#include <fstream>
#include <cstdlib>

class flight;

class application{
	int id;
	std::string FirstName;
	std::string LastName;
	std::string Destination;
	time_t AirportArrivalTime;
	time_t PrefferedArrivalTime;
	int TicketType;	//1=A 0=B
	
	public:
		int matches(flight&);
		int is_luxury_class();		//1 luxury, 0 normal
		int get_id();
		std::string get_name();
		time_t arrive_by();
		application(int ident, std::string FN,std::string LN,std::string DST,time_t AAT,time_t PT,int Ttype):id(ident),FirstName(FN),LastName(LN),Destination(DST),AirportArrivalTime(AAT),PrefferedArrivalTime(PT),TicketType(Ttype)
		{
			std::cout << "application created" << std::endl;
		};
		~application();
};

class flight{
	std::string Destination;
	time_t DepartureTime;
	time_t FlightTime;
	int MaxFirstClass;
	int MaxNormalClass;
	int NumOfPassengers1;
	int NumOfPassengers0;
	std::list<application> passengers;
	
	public:
		flight(std::string Dst, time_t DTime, time_t FTime, int MFirstClass, int MNormalClass):
		Destination(Dst),DepartureTime(DTime),FlightTime(FTime),MaxFirstClass(MFirstClass),MaxNormalClass(MNormalClass),NumOfPassengers1(0),NumOfPassengers0(0)
		{
			std::cout << "flight was created" << std::endl;
		};	
		int add_passenger(application&);
		int cancel_reservations(int);
		time_t departs_at();
		time_t arrives_at();
		std::string get_destination();
		int get_available_2(int);
		int get_available();
		std::list<application> get_bookings();
		
};

class airport{
	time_t Current_time;
	int terminals;
	int fails;
	flight** terminal_array;
	//int flight_counter;
	std::list<application> application_list;
	
	public:
		airport(time_t curtime, int ter):fails(0),Current_time(curtime),terminals(ter)
		{
			terminal_array=new flight*[terminals];
			for(int i=0;i<terminals;i++){
				terminal_array[i]=NULL;
			}
			std::cout << "an airport was created" << std::endl;
		};
		time_t get_time();
		void add_application(int,std::string,std::string,std::string,time_t,time_t,int);
		void cancel_applications(int);
		void add_flight(std::string, time_t, time_t, int, int);
		void cancel_flight(int);
		int failed_applications();
		void show_timetable();
		void show_people_waiting();
		void flow_time(time_t);
		void add_application_list();
	
};

