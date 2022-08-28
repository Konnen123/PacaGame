#include <iostream>
#include <fstream>
#include<string>

using namespace std;
int width = 40;
int height = 2;


fstream fUsername;
fstream fTokens;

class UserProfile
{
    string name;
    int ID;
    int tokens;

public:
    

   

    UserProfile(string _name)
    {
        name = _name;
        SetID(_name);
        SetTokens();
    }
    void SetID(string _name)
    {   
        ID = 0;
        string line;

        fUsername.open("Username.txt", ios::in);
        if (fUsername.is_open())
        {
            while (getline(fUsername, line))
            {
                ID++;
                if (line == _name)
                    break;
            }
        }
        fUsername.close();  
    }
    void SetTokens()
    {
        
        string line;

       fTokens.open("Tokens.txt", ios::in);
       if (fTokens.is_open())
           for (int i = 0;i < ID;i++)
               getline(fTokens, line);

       tokens = stoi(line);

        fTokens.close();
    }
    int GetID()
    {
        return ID;
    }
    int GetTokens()
    {
        return tokens;
    }
    string GetName()
    {
        return name;
    }
    void ModifyTokens(int bet)
    {
        tokens += bet;
        SaveTokens(bet);
    }
    void SaveTokens(int bet)
    {
       
        fTokens.open("Tokens.txt", ios::in);
        string line;

        int counter = 0;

        if (fTokens.is_open())
        {
            while (getline(fTokens, line))
            {
                counter++;
            }
        }
        fTokens.close();
        fTokens.open("Tokens.txt", ios::in);

        int* tokenList = new int[counter];

        if (fTokens.is_open())
        {
            for (int i = 0;i < counter;i++)
            {
                getline(fTokens, line);
                tokenList[i] = stoi(line);
            }
        }
        fTokens.close();


        fTokens.open("Tokens.txt", ios::out);
        if (fTokens.is_open())
        {
            for (int i = 0;i < counter;i++)
            {
                if (i == GetID() - 1)
                {
                    fTokens << GetTokens() << '\n';
                }
                else
                    fTokens << tokenList[i] << '\n';
            }


            fTokens.close();
        }

        delete[] tokenList;
        tokenList = NULL;
    }
 
};



void FirstPage();
void UserPage(string name);

void LineSpacing(int height)
{
    for (int i = 0;i < height;i++)
        cout << '\n';
}
void Indent(int width,char c)
{
    for (int i = 0;i < width;i++)
        cout << c;
}

int SlotMachineRule(string line, int bet,string* symbols,int totalSymbols)
{
    int prize=0;
    for (int i = 0;i < totalSymbols;i++)
    {
    
        if (line == symbols[i])
        {
            prize += (i + 5) * bet;
            return prize;
        }
    }
}
int PlaceBet(UserProfile userProfile)
{
    int bet;
    system("cls");
    LineSpacing(height);
    cout << "Place your bets(" << userProfile.GetTokens() << "):";
    cin >> bet;
    while(bet>userProfile.GetTokens() || bet<0)
        {
            system("cls");
            LineSpacing(height);
            cout << "You do not have enough tokens.Try again(" << userProfile.GetTokens() << "): ";
            cin >> bet;
        }
  
   
    return bet;
}

void SlotMachine(UserProfile userProfile)
{

    int bet = PlaceBet(userProfile);
    system("cls");
    LineSpacing(height + 2);

    int totalSymbols = 6;

    string* symbols = new string[totalSymbols]{ " Bar "," Jack","Queen"," King"," Ace ","  7  " };


    srand(time(0));
    string* line = new string[3];
    int prize = 0;

    for (int i = 0;i < 3;i++)
    {
        Indent(width, ' ');
        for (int j = 0;j < 3;j++)
        {
            line[j] = symbols[rand() % totalSymbols];
            cout << line[j] << ' ';
        }
        if (line[0] == line[1] && line[1] == line[2])
            prize += SlotMachineRule(line[0], bet, symbols, totalSymbols);

        cout << '\n';
    }
    delete[] line;
    delete[] symbols;

    

    

    line = NULL;
    symbols = NULL;

    LineSpacing(height);

    
    if (prize != 0)
    {
        Indent(width-10, ' ');
        cout << "Congratulations! You won " << prize << " tokens.";
        userProfile.ModifyTokens(prize);
    }
    else
    {
        Indent(width, ' '); 
        cout << "   Try again!";
        userProfile.ModifyTokens(-bet);
    }

    int numberOption=0;
    LineSpacing(height);
    cout << "Press 0 to exit to menu.\n";
    cout << "Press 1 to place your bets.\n";
    
    cin >> numberOption;
    if (numberOption == 1)
        SlotMachine(userProfile);
    else UserPage(userProfile.GetName());



}
void Blackjack(UserProfile userProfile)
{

}
void Roulette(UserProfile userProfile)
{

}

void ChooseGame(UserProfile userProfile)
{
    system("cls");
    Indent(width, '#');
    LineSpacing(height);
    
    string title = "Choose game";

    Indent((width - title.length()) / 2, ' ');
    cout << title;

    LineSpacing(height + 2);
    Indent((width - title.length()) / 2, ' ');
    cout << "1.Slot Machine";

    LineSpacing(height);
    Indent((width - title.length()) / 2, ' ');
    cout << "2.Blackjack";

    LineSpacing(height);
    Indent((width - title.length()) / 2, ' ');
    cout << "3.Roulette";

    LineSpacing(height + 2);
    Indent(width, '#');
    LineSpacing(height + 2);

    int numberOption;
   
    cout << "Enter your option: ";
    cin >> numberOption;

    switch (numberOption)
    {
    case 1: SlotMachine(userProfile);  break;
    case 2: Blackjack(userProfile);  break;
    case 3: Roulette(userProfile);  break;


    default: cout << "\n        WRONG OPTION. TRY AGAIN\n";
        break;
    }
}

void UserPage(string name)
{
    system("cls");
    Indent(width, '#');
    LineSpacing(height);

    UserProfile userProfile(name);
    

    string greetings = "Hello " + name;
  

    Indent((width - greetings.length()) / 2, ' ');
    cout << greetings;

    LineSpacing(height + 2);
    Indent((width - greetings.length()) / 2+3, ' ');
    cout <<"1.Play";
    
    LineSpacing(height);
    Indent((width - greetings.length()) / 2+3, ' ');
    cout << "2.Log out ";

    LineSpacing(height + 2);
    Indent(width - greetings.length()+3, ' ');
    cout << "Tokens: " << userProfile.GetTokens();

    LineSpacing(height);
    Indent(width, '#');
    LineSpacing(height + 2);

    int numberOption;
    cout << "Enter your option: ";
    cin >> numberOption;

    switch (numberOption)
    {
    case 1: ChooseGame(userProfile);  break;
    case 2: FirstPage();  break;


    default: cout << "\n        WRONG OPTION. TRY AGAIN\n";
        break;
    }


   


} 

void Register()
{
    string name;

    LineSpacing(height);
    cout << "Username: ";
    getline(cin>>ws, name);
    fUsername.open("Username.txt", ios::in);
    //check if user is already registered
    if (fUsername.is_open())
    {
        string line;
        while (getline(fUsername,line))
        {
            if (line == name)
            {
                system("cls");
                LineSpacing(height);
                cout << "This username is already registered.";
                LineSpacing(height);
                fUsername.close();
                return;
            }
        }
    
    }
    fUsername.close();
    fUsername.open("Username.txt", ios::app);
    if (fUsername.is_open())
    {
    fUsername << name<<'\n';
    fUsername.close();
     }
    //Give player tokens
    int tokenAmount = 100;
    fTokens.open("Tokens.txt", ios::app);
    if (fTokens.is_open())
    {
        fTokens << tokenAmount << '\n';
        fTokens.close();
    }

    LineSpacing(height+2);
    cout << "CONGRATULATIONS!";
    LineSpacing(height);
    cout << "Your registration has been successful.You will start with 100 tokens, good luck!";
    LineSpacing(height);
    cout << "Press 1 to return to main menu.\n";
    string check;
    cin >> check;
    if (check != "1")
        return;

    FirstPage();

}
void LogIn()
{
    string name;
    LineSpacing(height);
    cout << "Username: ";
    getline(cin >> ws, name);
    fUsername.open("Username.txt", ios::in);
    //check if user is already registered
    if (fUsername.is_open())
    {
        string line;
        while (getline(fUsername, line))
        {
            if (line == name)
            {
                fUsername.close();
                UserPage(name);   
                return;
            }
        }

    }
    fUsername.close();
    system("cls");
    cout << "This username does not exist.Press 1 to try again.";

    string check;
    cin >> check;
    if (check != "1")
        return;

    FirstPage();
}
void FirstPage()
{   
    system("cls");

    Indent(width, '#');
    LineSpacing(height);
    
    string greetings = "WELCOME TO CASINO!";
    Indent((width - greetings.length()) / 2, ' ');
    cout << greetings;

    LineSpacing(height + 2);
    Indent((width - greetings.length()) / 2, ' ');

    cout << "   1.Register";

    LineSpacing(height);

    Indent((width - greetings.length()) / 2, ' ');

    cout << "   2.Login";

    LineSpacing(height + 2);
    Indent(width, '#');
    LineSpacing(height + 2);

    int numberOption;
    cout << "Enter your option: ";
    cin >> numberOption;

    system("cls");
    
  


    switch (numberOption)
    {
    case 1: Register();  break;
    case 2: LogIn();  break;
 

    default: cout << "\n        WRONG OPTION. TRY AGAIN\n";
        break;
    }
    
    
}



int main()
{
   
    FirstPage();
   
    return 0;

}

