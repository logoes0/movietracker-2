#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>
using namespace std;

// ANSI escape code for text
string blue = "\033[92m";  // Blue color
string green = "\033[34m"; // Green color
string red = "\033[33m";   // Red color
string reset = "\033[0m";  // Reset to default terminal color

// trim from front (in place)
inline void ltrim(string &s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch)
                               { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                    { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
inline void trim(string &s)
{
    rtrim(s);
    ltrim(s);
}

// trim from both ends and round off to given decimal points
inline void trim(string &s, int n)
{
    rtrim(s);
    ltrim(s);
    int i = find(s.begin(), s.end(), '.') - s.begin();
    s = s.substr(0, i + 2);
}

// to check if a given string is a number
bool isnumber(string s)
{
    if (s == "")
        return false;
    int c = 0;
    for (auto i : s)
    {
        if (i == '.')
        {
            c++;
            continue;
        }
        if (c > 1)
            return false;
        if (!isdigit(i))
            return false;
    }
    return true;
}

// to check valid year
bool isyear(string s)
{
    for (auto i : s)
    {
        if (!isdigit(i))
            return false;
    }
    return (s.length() == 4);
}

// to check almost similarity of 2 strings
bool similar(string s1, string s2)
{
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    int n1 = s1.length();
    int n2 = s2.length();
    for (int i = 0; i <= n2 - n1; i++)
    {
        if (s1 == s2.substr(i, n1))
            return true;
    }
    return false;
}

bool same(string s1, string s2)
{
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    return (s1 == s2);
}

void set(string &genre)
{
    string temp = "";
    for (int i = 0; i < genre.length(); i++)
    {
        if (genre[i] >= '1' && genre[i] <= '4')
            temp += genre[i];
    }
    genre = temp;
    sort(genre.begin(), genre.end());
    genre.erase(unique(genre.begin(), genre.end()), genre.end());
}

bool isPassword(const string &str)
{
    if (str.length() < 8 || str.length() > 16)
    {
        return false;
    }
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    for (char c : str)
    {
        if (isupper(c))
        {
            hasUpperCase = true;
        }
        else if (islower(c))
        {
            hasLowerCase = true;
        }
    }
    return hasUpperCase && hasLowerCase;
}
/*____________________________________________________________________________________________________________________________________
____________________________________________________________________________________________________________________________________*/
class Profiles
{
    string name, username, password;
    int total0;
protected:


public:
    static string current;
    vector<vector<string>> prof;
    Profiles()
    {
        name = "";
        username = "";
        password = "";
        total0 = 0;
    }
    void control_Panel(); // Display options for sign-in or creating new account
    void make_new_acc();  // Interface for creating your new account
    void verify();        // Checks wether the username and password match or not
    void load_profiles(); // Loading profile into array
    void unload_profiles();
};
string Profiles::current = "0";
class MovieTracker : public Profiles
{
    int total;
    vector<vector<string>> list;

public:
    void load_movie();     // loading list to array
    void control_panel();  // display options
    void add_movie();      // insert a movie
    void display_movie();  // display all movies
    void search_movie();   // search for a movie
    void update_movie();   // update details of a movie
    void delete_movie();   // remove movie from list
    void filter_movie();     // filter movie according to input
    void unload_movie();   // update list before exiting
    void compare_movies(); // compare movies with other list
    void update_details(); // update profiles details
};
void Profiles ::control_Panel()
{
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++";
    cout << "\n\t Welcome to your personalized\n\t\t" << blue << "Movie Tracker" << reset;
    cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "\n [1]--Make a New Account";
    cout << "\n [2]--Sign-In";
    cout << "\n [3]--Exit Application";
}

void Profiles::load_profiles()
{
    prof.clear();
    fstream file;
    file.open("profiles.csv", ios::in);
    vector<string> row;
    string temp;
    while (file >> temp)
    {
        row.clear();
        string line, word;
        getline(file, line);
        stringstream s(line);
        while (getline(s, word, ','))
        {
            trim(word);
            row.push_back(word); // vector of each row of data
        }
        prof.push_back(row); // inserting into main prof
    }
    total0 = prof.size();
    file.close();
}

void Profiles::unload_profiles()
{
    ofstream file("profiles.csv", ofstream::trunc);
    for (auto i : prof)
    {
        file << ", " << i[0] << ", " << i[1] << ", " << i[2] << ", " << i[3] << "\n";
    }
    file.close();
}

void Profiles::make_new_acc()
{
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "\n\n";
    cout << "Enter your username: ";
    getline(cin, username);
    cout << "\n\n";
    cout << "Enter your password: ";
    getline(cin, password);
    while (!isPassword(password))
    {
        cout << red << "Invalid password. Password must be between 8 to 16 characters with atleast 1 Uppercase and 1 Lowercase letter.\n"
             << reset;
        cout << "Enter your password: ";
        getline(cin, password);
    }
    cout << "\n\n";
    total0++;
    prof.push_back({to_string(total0), name, username, password});
    cout << green << "Account Created Successfully!\n\n"
         << reset;
    unload_profiles();
}

void Profiles::verify()
{
    current = "-1";
    int c = 0;
    do
    {
        c++;
        cout << "Enter your username: ";
        getline(cin, username);
        cout << "\n\n";
        cout << "Enter your password: ";
        getline(cin, password);
        cout << "\n\n";
        for (auto i : prof)
        {
            if (i.size() >= 4)
            {
                if (i[2] == username && i[3] == password)
                {
                    current = i[0];
                    return;
                }
            }
        }
        cout << red << "Entered credentials don't match.\n\n"
             << reset;
        cout << "______________________________________________\n";
    } while (c < 3);
    unload_profiles();
    cout << red << "Input limit exceeded. Shutting down the application.\n" << reset;
    exit(0);
}

//_____________________________________________________________________________________________________
// loading current list into array
void MovieTracker::load_movie()
{
    list.clear();
    fstream file;
    string file_name = "list" + current + ".csv";
    file.open(file_name, ios::in);
    vector<string> row;
    string temp;
    while (file >> temp)
    {
        row.clear();
        string line, word;
        getline(file, line);
        stringstream s(line);
        while (getline(s, word, ','))
        {
            trim(word);
            row.push_back(word); // vector of each row of data
        }
        list.push_back(row); // inserting into main list
    }
    total = list.size();
    file.close();
    file;
    file.open("profiles.csv", ios::in);
    row.clear();
    while (file >> temp)
    {
        row.clear();
        string line, word;
        getline(file, line);
        stringstream s(line);
        while (getline(s, word, ','))
        {
            trim(word);
            row.push_back(word); // vector of each row of data
        }
        prof.push_back(row); // inserting into main prof
    }
    file.close();
    cout << "Welcome " << blue << prof[stoi(current) - 1][1] << reset << endl;
}

// overwriting file with the new data
void MovieTracker::unload_movie()
{
    string file_name = "list" + current + ".csv";
    ofstream file(file_name, ofstream::trunc);
    for (auto i : list)
    {
        file << ", " << i[0] << ", " << i[1] << ", " << i[2] << ", " << i[3] << ", " << i[4] << ", " << i[5] << "\n";
    }
    file.close();
}

// display all the options available
void MovieTracker ::control_panel()
{
    cout << "******************************************************************";
    cout << "\n\t\t\t " << blue << " Movie Tracker" << reset;
    cout << "\n******************************************************************\n";
    cout << "\n [1]--Add New Movie";
    cout << "\n [2]--Display Your List";
    cout << "\n [3]--Search A Movie";
    cout << "\n [4]--Update Entry";
    cout << "\n [5]--Delete Movie";
    cout << "\n [6]--Filter Movie";
    cout << "\n [7]--Compare Movie List";
    cout << "\n [8]--Change UserName or Password";
    cout << "\n [9]--Save and Exit";
}

// add to list
void MovieTracker ::add_movie()
{
    string movie_name, rating, year, status, genre;
    cout << "\n\n\t\t\t\t Add New Movie: \n";
    cout << "--------------------------------------------------------------------------\n";
    // inputting movie name
    cout << "\n\n\t\t Movie Name : ";
    getline(cin, movie_name);
    trim(movie_name);
    while (movie_name.length() == 0)
    {
        cout << red << "You need to input the Movie Title. Re-Enter Movie Name:\n" << reset;
        cout << "\n\n\t\t Movie Name : ";
        getline(cin, movie_name);
        trim(movie_name);
    }
    // inputting year of release
    cout << "\n\n\t\t Year of Release : ";
    getline(cin, year);
    trim(year);
    while (!isyear(year))
    {
        cout << red << "Invalid year. Re-enter:\n" << reset;
        cout << "\n\n\t\t Year of Release : ";
        getline(cin, year);
        trim(year);
    }
    while (stoi(year) > 2026 || stoi(year) < 1900)
    {
        cout << red << "Invalid year. Re-enter:\n" << reset;
        cout << "\n\n\t\t Year of Release : ";
        getline(cin, year);
        trim(year);
    }
    // inputting rating
    cout << "\n\n\t\t Rating(0-10): ";
    getline(cin, rating);
    trim(rating, 2);
    if (rating.length() == 0)
        rating = "-";
    while (!isnumber(rating) && rating != "-")
    {
        cout << red << "Invalid Rating\n" << reset;
        cout << "\n\n\t\t Rating(0-10): ";
        getline(cin, rating);
        trim(rating);
        if (rating.length() == 0)
            rating = "-";
    }
    while (isnumber(rating) && (stof(rating) < 0 || stof(rating) > 10))
    {
        cout << red << "Invalid Rating\n" << reset;
        cout << "\n\n\t\t Rating(0-10): ";
        getline(cin, rating);
        trim(rating);
    }
    // inputting status
    cout << "\n\n\t\t Status(watched/ plan to watch/ dropped): ";
    getline(cin, status);
    trim(status);
    transform(status.begin(), status.end(), status.begin(), ::tolower);
    while (status != "watched" && status != "plan to watch" && status != "dropped")
    {
        cout << red << "Invalid status. Re-enter status:\n" << reset;
        cout << "\n\n\t\t Status(watched/ plan to watch/ dropped): ";
        getline(cin, status);
        trim(status);
        transform(status.begin(), status.end(), status.begin(), ::tolower);
    }
    // inputing genre
    cout << "\n\n\t\t Genre([1]-Action / [2]-Comedy / [3]-Romance / [4]-Horror) : ";
    getline(cin, genre);
    set(genre);
    while (genre.length() == 0)
    {
         cout << red << "Invalid status. Re-enter status:\n" << reset;
        cout << red << "\n\n\t\t Invalid Genre; Re-enter value: " << reset;
        getline(cin, genre);
        set(genre);
    }
    int f = 0;
    for (auto &i : list)
    {
        string check = i[1];
        if (same(movie_name, check))
        {
            f = 1;
            i[2] = year;
            i[3] = rating;
            i[4] = status;
            i[5] = genre;
            break;
        }
    }
    if (f == 0)
    {
        total++;
        list.push_back({to_string(total), movie_name, year, rating, status, genre});
    }
    cout << "==========================================================================" << endl;
}

// display list
void MovieTracker ::display_movie()
{
    cout << "--------------------------------------------------------------------------------------------------\n";
    cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
    cout << "--------------------------------------------------------------------------------------------------\n";
    for (auto i : list)
    {
        cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
        for (auto j : i[5])
        {
            if (j == '1')
                cout << "Action, ";
            else if (j == '2')
                cout << "Comedy, ";
            else if (j == '3')
                cout << "Romance, ";
            else if (j == '4')
                cout << "Horror, ";
        }
        cout << endl;
    }
    cout << "==================================================================================================\n";
}

// check for a specific movie in list
void MovieTracker ::search_movie()
{
    cout << "\n\n\t\t\t\t Search Movie\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << "You need to input the Movie Title.\n\n\n\t\tMovie Name:";
    string search;
    getline(cin, search);
    trim(search);
    vector<vector<string>> possible;
    while (search.length() <= 0)
    {
        cout << red << "You need to input atleast 1 or more letters of the Movie Title. Re-Enter Movie Name:\n" << reset;
        cout << "\n\n\t\t Movie Name : ";
        getline(cin, search);
    }
    string check;
    for (auto i : list)
    {
        check = i[1];
        if (similar(search, check))
            possible.push_back(i);
    }
    if (possible.size() == 0)
        cout << red << "\nNo entries match your search.\n" << reset;
    else
    {
        cout << "\nEntries that match your search:\n";
        cout << "--------------------------------------------------------------------------------------------------\n";
        cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
        cout << "--------------------------------------------------------------------------------------------------\n";
        for (auto i : possible)
        {
            cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
            for (auto j : i[5])
            {
                if (j == '1')
                    cout << "Action, ";
                else if (j == '2')
                    cout << "Comedy, ";
                else if (j == '3')
                    cout << "Romance, ";
                else
                    cout << "Horror, ";
            }
            cout << endl;
        }
        cout << "==================================================================================================\n";
    }
    cout << "\n--------------------------------------------------------------------------\n";
}
// update movie in list
void MovieTracker ::update_movie()
{
    cout << "\n\n\t\t\t\tUpdate Movie Record\n";
    cout << "--------------------------------------------------------------------------\n";
    string id_movie;
    string change;
    string id;
    while (true)
    {
        cout << "Enter serial number of movie you want to make changes in: ";
        getline(cin, id_movie);
        if (isnumber(id_movie))
        {
            if (stoi(id_movie) >= 1 && stoi(id_movie) <= total)
            {
                break;
            }
        }
        cout << red << "Invalid Serial Number.\n" << reset;
    }
    while (true)
    {
        cout << "Enter the field you want to update->\n[1]--Movie Name\n[2]--Year of Release\n[3]--Rating\n[4]--Status\n[5]--Genre\n[6]--Exit\n";
        cout << "Enter choice: ";
        getline(cin, id);
        if (id == "1")
        {
            cout << "\n\n\t\t Enter New Movie Name : ";
            getline(cin, change);
            trim(change);
            while (change.length() == 0)
            {
                cout << red << "You need to input the Movie Title. Re-Enter Movie Name:\n" << reset;
                cout << "\n\n\t\t New Movie Name : ";
                getline(cin, change);
                trim(change);
            }
        }
        else if (id == "2")
        {
            cout << "\n\n\t\t Year of Release : ";
            getline(cin, change);
            trim(change);
            while (!isyear(change))
            {
                cout << red << "Invalid year. Re-enter:\n" << reset;
                cout << "\n\n\t\t Year of Release : ";
                getline(cin, change);
                trim(change);
            }
            while (stoi(change) > 2024)
            {
                 cout << red << "Invalid year. Re-enter:\n" << reset;
                cout << "\n\n\t\t Year of Release : ";
                getline(cin, change);
                trim(change);
            }
        }
        else if (id == "3")
        {
            cout << "\n\n\t\t New Rating(0-10): ";
            getline(cin, change);
            trim(change);
            if (change.length() == 0)
                change = "-";
            while (!isnumber(change) && change != "-")
            {
                cout << red << "Invalid Rating\n" << reset;
                cout << "\n\n\t\t New Rating(0-10): ";
                getline(cin, change);
                trim(change);
                if (change.length() == 0)
                    change = "-";
            }
            while (isnumber(change) && (stof(change) < 0 || stof(change) > 10))
            {
                cout << red << "Invalid Rating\n" << reset;
                cout << "\n\n\t\t New Rating(0-10): ";
                getline(cin, change);
                trim(change);
            }
        }
        else if (id == "4")
        {
            cout << "\n\n\t\t Status(watched/ plan to watch/ dropped): ";
            getline(cin, change);
            trim(change);
            transform(change.begin(), change.end(), change.begin(), ::tolower);
            while (change != "watched" && change != "plan to watch" && change != "dropped")
            {
                cout << red << "Invalid status. Re-enter status:\n" << reset;
                cout << "\n\n\t\t Status(watched/ plan to watch/ dropped): ";
                getline(cin, change);
                trim(change);
                transform(change.begin(), change.end(), change.begin(), ::tolower);
            }
        }
        else if (id == "5")
        {
            cout << "\n\n\t\tEnter genre ([1]-Action, [2]-Comedy, [3]-Romance, [4]-Horror): \n";
            getline(cin, change);
            trim(change);
            set(change);
            while (change.size() == 0)
            {
                cout << red << "Invalid genre. Re-enter status:\n" << reset;
                cout << "\n\n\t\tEnter genre ([1]-Action, [2]-Comedy, [3]-Romance, [4]-Horror): \n";
                getline(cin, change);
                set(change);
            }
        }
        else if (id == "6")
        {
            return;
        }
        else
        {
            cout << red << "Invalid choice. Re-enter:\n" << reset;
            continue;
        }
        list[stoi(id_movie) - 1][stoi(id)] = change;
    }
    cout << "\n--------------------------------------------------------------------------\n";
}

// delete movie from list
void MovieTracker ::delete_movie()
{
    cout << "\n\n\t\t\t\tRemove Movie\n";
    cout << "--------------------------------------------------------------------------\n";
    string id;
    while (true)
    {
        cout << "Enter serial number of movie you want to delete: ";
        getline(cin, id);
        if (isnumber(id))
        {
            if (stoi(id) >= 1 && stoi(id) <= total)
            {
                break;
            }
        }
        cout << red << "\nInvalid Serial Number.\n" << reset;
    }
    if (stoi(id) == total)
        list.pop_back();
    else
    {
        list.erase(list.begin() + stoi(id) - 1);
        for (int i = stoi(id) - 1; i < list.size(); i++)
        {
            int temp = stoi(list[i][0]);
            temp--;
            list[i][0] = to_string(temp);
        }
    }
    total--;
    cout << green << "\nEntry deleted successfully.\n" << reset;
    cout << "\n--------------------------------------------------------------------------\n";
}

void MovieTracker ::filter_movie()
{
    string choice;
    cout << "\n\n\t\t\t\tFilter Movie\n";
    cout << "--------------------------------------------------------------------------\n";
start:
    cout << "Enter the parameter by which you want to filter the list: \n";
    cout << "[1]--Filter by Rating\n[2]--Filter by Year\n[3]--Filter by Status\n[4]--Filter by Genre\n[5]--Exit\n";
    cout << "Enter choice: ";
    getline(cin, choice);
    trim(choice);
    if (choice.length() == 0 || choice == "")
    {
        cout << red << "Invalid Input" << reset <<endl;
        goto start;
    }
    if (choice == "1")
    {
        string l1, l2;
        while (true)
        {
            cout << "Enter range of rating: \n";
            cout << "Starting= ";
            getline(cin, l1);
            trim(l1);
            cout << "Ending= ";
            getline(cin, l2);
            trim(l2);
            if (isnumber(l1) && isnumber(l2) && stof(l1)>=0 && stof(l1)<=10 && stof(l2)>=0 && stof(l2)<=10)
                break;
            else
                cout << red << "\nOut of Range Values. Re-enter values.\n" << reset;
        }
        cout << "\n\n";
        vector<vector<string>> possible;
        float low = stof(l1) > stof(l2) ? stof(l2) : stof(l1);
        float up = stof(l2) + stof(l1) - low;
        for (auto i : list)
        {
            if (i[3] == "-")
                continue;
            float temp = stof(i[3]);
            if (temp >= low && temp <= up)
                possible.push_back(i);
        }
        if (possible.size() == 0)
            cout << "\nNo Entries in the given range\n";
        else
        {
            cout << "\nEntries that match your search:\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            for (auto i : possible)
            {
                cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
                for (auto j : i[5])
                {
                    if (j == '1')
                        cout << "Action, ";
                    else if (j == '2')
                        cout << "Comedy, ";
                    else if (j == '3')
                        cout << "Romance, ";
                    else
                        cout << "Horror, ";
                }
                cout << endl;
            }
            cout << "==================================================================================================\n";
        }
    }
    else if (choice == "2")
    {
        string l1, l2;
        while (true)
        {
            cout << "Enter range of year: \n";
            cout << "Starting= ";
            getline(cin, l1);
            trim(l1);
            cout << "Ending= ";
            getline(cin, l2);
            trim(l2);
            if (isyear(l1) && isyear(l2))
                break;
            else
                cout << red << "\nInvalid Input. Re-enter values.\n\n" << reset;
        }
        cout << "\n\n";
        vector<vector<string>> possible;
        float low = stof(l1) > stof(l2) ? stof(l2) : stof(l1);
        float up = stof(l2) + stof(l1) - low;
        for (auto i : list)
        {
            float temp = stof(i[2]);
            if (temp >= low && temp <= up)
                possible.push_back(i);
        }
        if (possible.size() == 0)
            cout << "No Entries in the given range\n";
        else
        {
            cout << "Entries that match your search:\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            for (auto i : possible)
            {
                cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
                for (auto j : i[5])
                {
                    if (j == '1')
                        cout << "Action, ";
                    else if (j == '2')
                        cout << "Comedy, ";
                    else if (j == '3')
                        cout << "Romance, ";
                    else
                        cout << "Horror, ";
                }
                cout << endl;
            }
            cout << "==================================================================================================\n";
        }
    }
    else if (choice == "3")
    {
        string s;
        while (true)
        {
            cout << "Enter status (watched/ plan to watch/ dropped): \n";
            getline(cin, s);
            if (s == "watched" || s == "plan to watch" || s == "dropped")
                break;
            else
                cout << red << "Invalid Input. Re-enter values.\n\n" << reset;
        }
        cout << "\n\n";
        vector<vector<string>> possible;
        for (auto i : list)
        {
            if (s == i[4])
                possible.push_back(i);
        }
        if (possible.size() == 0)
            cout << "No Entries of given status.\n";
        else
        {
            cout << "Entries that match your search:\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            for (auto i : possible)
            {
                cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
                for (auto j : i[5])
                {
                    if (j == '1')
                        cout << "Action, ";
                    else if (j == '2')
                        cout << "Comedy, ";
                    else if (j == '3')
                        cout << "Romance, ";
                    else
                        cout << "Horror, ";
                }
                cout << endl;
            }
            cout << "==================================================================================================\n";
        }
    }
    else if (choice == "4")
    {
        string s;
        while (true)
        {
            cout << "Enter genre ([1]-Action, [2]-Comedy, [3]-Romance, [4]-Horror): \n";
            cout << "Genre: ";
            getline(cin, s);
            set(s);
            if (s.length() != 0)
                break;
            else
                cout << red << "Invalid Input. Re-enter genre. \n\n"<<reset;
        }
        cout << "\n\n";
        vector<vector<string>> possible;
        for (auto i : list)
        {
            int f = 0;
            for (auto j : s)
            {
                if (find(i[5].begin(), i[5].end(), j) == i[5].end())
                    f = 1;
            }
            if (f == 0)
                possible.push_back(i);
        }
        if (possible.size() == 0)
            cout << "No Entries of given status.\n";
        else
        {
            cout << "Entries that match your search:\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            cout << "Sl. No \t Movie Name \t\t\t Year \tRating \tStatus \t\tGenre\n";
            cout << "--------------------------------------------------------------------------------------------------\n";
            for (auto i : possible)
            {
                cout << left << setw(3) << " " << setw(6) << i[0] << setw(32) << i[1] << setw(10) << i[2] << setw(5) << i[3] << setw(14) << i[4];
                for (auto j : i[5])
                {
                    if (j == '1')
                        cout << "Action, ";
                    else if (j == '2')
                        cout << "Comedy, ";
                    else if (j == '3')
                        cout << "Romance, ";
                    else
                        cout << "Horror, ";
                }
                cout << endl;
            }
            cout << "==================================================================================================\n";
        }
    }
    else if (choice == "5")
    {
        return;
    }
    else
    {
        cout << red << "\nInvalid Input.\n\n" << reset;
        goto start;
    }
    cout << "\n--------------------------------------------------------------------------\n";
}

void MovieTracker ::compare_movies()
{
    cout << "\n\n\t\t\t\tCompare Movies\n";
    cout << "--------------------------------------------------------------------------\n";
    string usr;
    string check;
    int f = 0;
    do
    {
        cout << "Enter username of account you want to compare with: ";
        getline(cin, usr);
        for (auto i : prof)
        {
            if (i[2] == usr)
            {
                f = 1;
                check = i[0];
            }
        }
        if (f == 0)
        {
            cout << red << "Username not found!!\n" << reset;
        }
    } while (f == 0);
    vector<vector<string>> v2;
    fstream file;
    string file_name = "list" + check + ".csv";
    file.open(file_name, ios::in);
    vector<string> row;
    string temp;
    while (file >> temp)
    {
        row.clear();
        string line, word;
        getline(file, line);
        stringstream s(line);
        while (getline(s, word, ','))
        {
            trim(word);
            row.push_back(word); // vector of each row of data
        }
        v2.push_back(row); // inserting into main list
    }
    file.close();
    vector<string> common;
    for (auto i : list)
    {
        for (auto j : v2)
        {
            if (i[1] == j[1])
            {
                common.push_back(i[1]);
            }
        }
    }
    if (common.size() == 0)
    {
        cout << "There were no matches.\n";
    }
    else
    {
        cout << "Matches found: \n";
        cout << "Movies\n";
        cout << "_________\n";
        for (auto i : common)
        {
            cout << i << endl;
        }
        cout << "\n\n";
        cout << "Match Percent = ";
        double percent = (2.0 * common.size()) / (list.size() + v2.size()) * 100.0;
        printf("%.2f\n", percent);
    }
    cout << "--------------------------------------------------------------------------\n";
}

void MovieTracker ::update_details()
{
    cout << "\n\n\t\t\t\tUpdate account details\n";
    cout << "--------------------------------------------------------------------------\n";
inp:
    cout << "[1]--Change UserName\n";
    cout << "[2]--Change Password\n";
    string choice;
    string change;
    getline(cin, choice);
    trim(choice);
    if (choice.length() != 1)
    {
        cout << red << "Invalid input! Please Try again" << reset << endl;
        goto inp;
    }
    if (!(choice == "1" || choice == "2"))
    {
        cout << red << "Invalid input! Please Try again" << reset << endl;
        goto inp;
    }
    if (choice == "1")
    {
        cout << "Enter new username: ";
        getline(cin, change);
        prof[stoi(current) - 1][2] = change;
    }
    else if (choice == "2")
    {
    pass:
        cout << "Enter new password: ";
        getline(cin, change);
        if (isPassword(change))
            prof[stoi(current) - 1][3] = change;
        else
        {
            cout << red << "Invalid Password!!\nYour Password must contain 8-16 characters and must contain atleast one uppercase and one lowercase character\n" << reset;
            goto pass;
        }
    }
    cout << green << "Details updated successfully!" << reset << "\n\n";
    cout << "--------------------------------------------------------------------------\n";
    Profiles::unload_profiles();
    Profiles::load_profiles();
}

int main()
{
    Profiles P;
home:
    P.load_profiles();
    P.control_Panel();
    string choice;
    cout << "\n\n Your Choice : ";
    getline(cin, choice);
    if (choice.length() == 0)
    {
        cout << red << "\n\n Invalid Value....Please Try again\n" << reset;
        goto home;
    }
    if (!isnumber(choice))
    {
         cout << red << "\n\n Invalid Value....Please Try again\n" << reset;
        goto home;
    }
    int c = stoi(choice);
    switch (c)
    {
    case 1:
        P.make_new_acc();
        break;
    case 2:
        P.verify();
        if (P.current != "0")
        {
            MovieTracker L;
            L.load_movie();
        label:
            L.control_panel();
            string choice;

            string subchoice;
            cout << "\n\n Your Choice : ";
            getline(cin, choice);
            if (!isnumber(choice))
            {
                 cout << red << "\n\n Invalid Value....Please Try again\n" << reset;
                goto label;
            }
            int c = stoi(choice);
            switch (c)
            {
            case 1:
                do
                {
                    L.add_movie();
                    cout << "Do You Want To Add Another Item (y/n) "
                            ": ";
                    getline(cin, subchoice);
                } while (subchoice == "Y" || subchoice == "y");
                break;
            case 2:
                L.display_movie();
                break;
            case 3:
                L.search_movie();
                break;
            case 4:
                L.update_movie();
                break;
            case 5:
                L.delete_movie();
                break;
            case 6:
                L.filter_movie();
                break;
            case 7:
                L.compare_movies();
                break;
            case 8:
                L.update_details();
                break;
            case 9:
                L.unload_movie();
                cout << endl;
                cout << red << "This session Ended. Thank you for your Time." << reset << endl;
                break;

            default:
                 cout << red << "\n\n Invalid Value....Please Try again\n" << reset;
            }

            cout << "\n";
            if (c != 9)
                goto label;
        }
        break;
    case 3:
        P.unload_profiles();
        cout << red << "You exited the application" << reset << endl;
        exit(0);
        break;
    default:
         cout << red << "\n\n Invalid Value....Please Try again\n" << reset;
    }
    cout << "\n";
    goto home;
    return 0;
}