#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

const int MOVIE_COUNT = 3;

class Movie
{
    public:

    int i;
    std::string movieName;
    int availableSeats[3];
    double prices[3];
    std::string showTimes;

    Movie() {}

    Movie(std::string n, int seats[], double price[], std::string st)
    {
        movieName = n;
        for(i=0;i<3;i++)
        {
            availableSeats[i] = seats[i];
            prices[i] = price[i];
        }
        showTimes = st;
    }
};

class BookingSystem
{
    private:

    Movie movies[MOVIE_COUNT];
    std::string filename = "booking.txt";

    public:

    BookingSystem() 
    {
        loadMovies();
    }

    void loadMovies()
    {
        int seats1[3] = {10, 30, 50};
        double prices1[3] = {10.0, 12.0, 14.0};

        int seats2[3] = {20, 40, 60};
        double prices2[3] = {11.0, 13.0, 15.0};

        int seats3[3] = {5, 15, 25};
        double prices3[3] = {20.0, 25.0, 30.0};

        movies[0] = Movie("Chhava", seats1, prices1, "10:00 AM");
        movies[1] = Movie("Oppenheimer", seats2, prices2, "2:00 PM");
        movies[2] = Movie("Hurry Up Tommorrow", seats3, prices3, "5:00 PM");
    }

    void displayMovies()
    {
        std::cout<<"Showing Movies:"<<"\n";
        std::cout<<"------------------------------------------------"<<"\n";
        for(int i=0;i<MOVIE_COUNT;i++)
        {
            std::cout<<i+1<<". "<<movies[i].movieName<<" ("<<movies[i].showTimes<<")"<<"\n";
            std::cout<<"Regular:"<<movies[i].availableSeats[0]<<" seats | Price:$"<<movies[i].prices[0]<<"\n";
            std::cout<<"Super:"<<movies[i].availableSeats[1]<<" seats | Price:$"<<movies[i].prices[1]<<"\n";
            std::cout<<"Premium:"<<movies[i].availableSeats[2]<<" seats | Price:$"<<movies[i].prices[2]<<"\n";
            std::cout<<"------------------------------------------------"<<"\n";
        }
    }

    void bookTickets()
    {
        displayMovies();

        int choice, category, tickets;

        std::cout<<"Enter the movie you are watching (1- "<<MOVIE_COUNT<<" ):";
        std::cin>>choice;

        if(choice < 1 || choice > MOVIE_COUNT)
        {
            std::cout<<"Invalid Choice!"<<"\n";
            return;
        }

        std::cout<<"Enter the seat categroy as your preference (0 - Regular, 1 - Super, 2 - Premium):";
        std::cin>>category;

        if(category < 0 || category > 2)
        {
            std::cout<<"Invalid Seat Category Selection!"<<"\n";
            return;
        }

        std::cout<<"Enter the number of tickets:";
        std::cin>>tickets;

        if(tickets < 0 || tickets > movies[choice - 1].availableSeats[category])
        {
            std::cout<<"Not too much tickets are available!"<<"\n";
            return;
        }

        double totalCost = tickets * movies[choice - 1].prices[category];

        if(tickets > 5)
        {
            std::cout<<"Got 10% Discount on your first booking!"<<"\n";
            totalCost *= 0.9;
        }

        movies[choice - 1].availableSeats[category] -= tickets;
        saveBooking(movies[choice - 1].movieName, tickets, category, totalCost);
        std::cout<<"Booking Successful! Total Cost:$"<<totalCost<<"\n";
    }

    void saveBooking(std::string movie, int tickets, int category, double cost)
    {
        std::ofstream file("bookings.txt", std::ios::app);

        if(!file)
        {
            std::cerr<<"Couldn't open the file for saving booking!"<<"\n";
        }

        file<<movie<<" "<<tickets<<" "<<category<<" "<<cost<<"\n";

        file.close();
    }

    void cancelBooking()
    {
        std::ifstream file("booking.txt");

        if(!file)
        {
            std::cout<<"No Booking Found!"<<"\n";
            return;
        }

        std::string movie, tempMovie;
        int tickets, category;
        double cost;

        std::string tempFile = "temp.txt";
        std::ofstream temp(tempFile);
        bool found = false;

        std::cout<<"Enter movie name to cancel booking:";
        std::cin.ignore();
        std::getline(std::cin, tempMovie);

        while (file>>movie>>tickets>>category>>cost)
        {
            if(!found && movie == tempMovie)
            {
                movies[category].availableSeats[category] += tickets;
                found = true;
            }
            else
            {
                temp<<movie<<" "<<tickets<<" "<<category<<" "<<cost<<"\n";
            }
        }
        
        file.close();
        temp.close();

        if(!found)
        {
            std::cout<<"No Booking Found!"<<"\n";
            remove(tempFile.c_str());
            return;
        }

        remove("booking.txt");
        rename(tempFile.c_str(), "booking.txt");

        std::cout<<"Book Cancelled Successfully."<<"\n";
    }

    void generateReport()
    {
        std::ifstream file(filename);

        if(!file)
        {
            std::cout<<"No Booking Found!"<<"\n";
            return;
        }

        std::string movie, tempMovie;
        int tickets, category;
        double cost, totalRevenue = 0;
        int totalBookings = 0;

        std::cout<<"Booking Summary Report:"<<"\n";
        std::cout<<"------------------------------------------------"<<"\n";

        while (file>>movie>>tickets>>category>>cost)
        {
            std::cout<<"Movie Name: "<<movie<<" , Tickets:"<<tickets<<", Category:"<<category<<", Cost:$"<<cost<<"\n";
            totalBookings += tickets;
            totalRevenue += cost;
        }
        
        file.close();

        std::cout<<"------------------------------------------------"<<"\n";
        std::cout<<"Total Bookings:"<<totalBookings<<"\n";
        std::cout<<"Total Cost:$"<<totalRevenue<<"\n";
    }
};

int main()
{
    BookingSystem system;

    int choice;

    do
    {
        std::cout<<"Movie Ticket Booking System"<<"\n";
        std::cout<<"1. View Showing Movies."<<"\n";
        std::cout<<"2. Book Tickets."<<"\n";
        std::cout<<"3. Cancel Booking."<<"\n";
        std::cout<<"4. Generate Report."<<"\n";
        std::cout<<"5. Exit..."<<"\n";
        std::cout<<"------------------------------------------------"<<"\n";
        
        std::cout<<"Enter your choice from (1 - 4):";
        std::cin>>choice;

        switch (choice)
        {
            case 1:
            system.displayMovies();
            break;

            case 2:
            system.bookTickets();
            break;

            case 3:
            system.cancelBooking();
            break;

            case 4:
            system.generateReport();
            break;

            case 5:
            std::cout<<"Thank you for using the Movie Ticket Booking System!"<<"\n";
            break;

            default:
            std::cout<<"Invalid Choice!"<<"\n";
        }
    } 
    
    while (choice != 5);
    
    return 0;
}