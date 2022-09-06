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
void Blackjack(UserProfile userProfile);

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

int PlaceBet(UserProfile userProfile)
{
    int bet;
    system("cls");
    LineSpacing(height);
    cout << "Place your bets(" << userProfile.GetTokens() << "):";
    cin >> bet;
    while (bet > userProfile.GetTokens() || bet <= 0)
    {
        system("cls");
        LineSpacing(height);
        cout << "You do not have enough tokens.Try again(" << userProfile.GetTokens() << "): ";
        cin >> bet;
    }


    return bet;
}

//SlotMachine functions
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



//Blackjack functions
void RemoveCardsFromDeck(string* deckCards, int* pNrOfCardsInADeck,int pivot)
{
    string aux = deckCards[*pNrOfCardsInADeck - 1];
    string aux2;


    for (int j = *pNrOfCardsInADeck - 1;j > pivot;j--)
    {
        aux2 = deckCards[j - 1];
        deckCards[j - 1] = aux;
        aux = aux2;
    }
    (*pNrOfCardsInADeck)--;
}
void GiveBlackjackCards(int* pNrOfCardsInADeck,string* deckCards,string* playerCards,int* pNrOfPlayerCards,int cardsGiven)
{
    int randomness=0;
    for (int i = 0;i < 5;i++)
    {
        randomness += rand() + time(0);
        srand(randomness);
    }
  

    for (int i = 0;i < cardsGiven;i++)
    {
        int card = rand() % *pNrOfCardsInADeck;
       
        playerCards[i+(*pNrOfPlayerCards)] = deckCards[card];

        //removing the cards from deck
        RemoveCardsFromDeck(deckCards, pNrOfCardsInADeck, card);
       
    }
    (*pNrOfPlayerCards)+=cardsGiven;
}
int SumOfCards(string* setOfCards,int nrOfCards)
{
    int sum=0;
    int acesInDeck=0;
    for (int i = 0;i < nrOfCards;i++)
    {
        if (setOfCards[i] == "J" || setOfCards[i] == "Q" || setOfCards[i] == "K")
            sum += 10;
        else if (setOfCards[i] == "A")
            acesInDeck++;
        else
        {
            sum += stoi(setOfCards[i]);
        }
    }
    if (!acesInDeck)
        return sum;
    
    for (int i = 0;i < acesInDeck;i++)
    {
        if (sum + 11 < 21)
            sum += 11;
        else if (sum + 11 == 21 && i == acesInDeck - 1)
            sum += 11;
        else sum += 1;
    }
    return sum;
}
void OptionAfterGame(UserProfile userProfile)
{

    int numberOption = 0;
    LineSpacing(height);
    cout << "Press 0 to exit to menu.\n";
    cout << "Press 1 to place your bets.\n";

    cin >> numberOption;
    if (numberOption == 1)
        Blackjack(userProfile);
    else UserPage(userProfile.GetName());
}
void DisplayCards(int nrOfDealerCards, string* dealerCards, int nrOfPlayerCards, string* playerCards,bool revealSecondCard)
{
    system("cls");

    LineSpacing(height + 2);

    int dealerSum = SumOfCards(dealerCards, nrOfDealerCards);
    int playerSum = SumOfCards(playerCards, nrOfPlayerCards);

    Indent(width, ' ');
    cout << "Dealer`s cards: ";
    if (!revealSecondCard ) //This means we want to reveal the second card when it`s the dealer`s turn
    {
        cout << dealerCards[0] << ' ' << "?\n";
    }
    else
    {
        for (int i = 0;i < nrOfDealerCards;i++)
            cout << dealerCards[i] << ' ';
        cout << "(Total: " << dealerSum << ")\n";
    }
    

    Indent(width + 4, ' ');
    cout << "Your cards: ";
    for (int i = 0;i < nrOfPlayerCards;i++)
        cout << playerCards[i] << ' ';
    cout << "(Total: " << playerSum << ")\n";

    LineSpacing(height + 2);
}

void ComparingSums(int dealerSum, int playerSum, int bet, UserProfile userProfile)
{
    if (playerSum > 21)
    {
        cout << "Busted!";
        userProfile.ModifyTokens(-bet);
    }
    else if (dealerSum > 21)
    {
        cout << "Dealer busted! You won " << bet << " tokens.";
        userProfile.ModifyTokens(bet);
    }
    else if (dealerSum > playerSum)
    {
        cout << "Dealer won!";
        userProfile.ModifyTokens(-bet);

    }
    else if (playerSum > dealerSum)
    {
        cout << "You won " << bet << " tokens.";
        userProfile.ModifyTokens(bet);
    }
    else  cout << "Draw";


    cout << '\n';
    OptionAfterGame(userProfile);
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
    int bet = PlaceBet(userProfile);
   
    LineSpacing(height + 2);

    string cards[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
    int nrOfCardsInADeck = 52;
   
    int* pNrOfCardsInADeck = &nrOfCardsInADeck;
   
   string* deckCards = new string[nrOfCardsInADeck];

    for (int i = 0;i < *pNrOfCardsInADeck;i++)
        deckCards[i] = cards[i / 4];

  
    int maxCards = 11;

  
    int nrOfPlayerCards=0;
    int* pNrOfPlayerCards = &nrOfPlayerCards;

    string* playerCards = new string[maxCards];
    //giving the player the cards 
    GiveBlackjackCards(pNrOfCardsInADeck,deckCards,playerCards,pNrOfPlayerCards,2);

    int nrOfDealerCards =0;
    int* pNrOfDealerCards = &nrOfDealerCards;

    string* dealerCards = new string[maxCards];
    //giving the dealer the cards 
    GiveBlackjackCards(pNrOfCardsInADeck, deckCards, dealerCards,pNrOfDealerCards,2);
   
    int dealerSum = SumOfCards(dealerCards, nrOfDealerCards);
    int playerSum = SumOfCards(playerCards, nrOfPlayerCards);

    DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards,false);

   

    if (playerSum == 21 || dealerSum == 21)
    {
        if (playerSum == 21 && dealerSum != 21)
        {
            DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards, true);
            cout << "Blackjack! You won " << bet * 1.5f << " tokens.\n";
            
            userProfile.ModifyTokens(bet * 1.5f);
        }
        
        else if (playerSum != 21 && dealerSum == 21)
        {
            DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards, true);
            cout << "Dealer has blackjack!\n";
            
            userProfile.ModifyTokens(-bet);
        }

        else if (playerSum == 21 && dealerSum == 21)
        {
            DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards, true);
            cout << "Draw.\n";
        }
        OptionAfterGame(userProfile);
        return;
    }
    bool ok = true;
    while (SumOfCards(playerCards, nrOfPlayerCards) <= 21 && ok)
    {

        cout << "Press 1 to take a card\n";
        cout << "Press 2 to stop \n";
        
        int option = 0;
       
            cin >> option;

            if (option == 1)
            {
                GiveBlackjackCards(pNrOfCardsInADeck, deckCards, playerCards, pNrOfPlayerCards, 1);
                DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards,false);
            }
            else
            {
                ok = false;
                while (SumOfCards(dealerCards, nrOfDealerCards) < 17)
                {
                    GiveBlackjackCards(pNrOfCardsInADeck, deckCards, dealerCards, pNrOfDealerCards, 1);
                    
                   
                }
                
                break;
            }

        
    }

    DisplayCards(nrOfDealerCards, dealerCards, nrOfPlayerCards, playerCards, true);
    dealerSum = SumOfCards(dealerCards, nrOfDealerCards);
    playerSum = SumOfCards(playerCards, nrOfPlayerCards);

    ComparingSums(dealerSum,playerSum,bet,userProfile);

    
    
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

