#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <vector>
using namespace std;

#define SIZE 150
#define PLAYER 'C'
#define MONSTER 'X'
#define MONEY '$'

	// ansii color
#define RED "\u001b[31m"
#define BLUE "\u001b[34m"
#define BLACK "\u001b[30m"
#define YELLOW "\u001b[33m"
#define YBackground "\u001b[43m"
#define BGreset "\033[0m"

// the currency in this game is called "diamond"
// default diamond for new player is 10, the player can buy an item or elemental skill in the shop
// default hp is 100
// default level is 1 (max level is 3), once the player reach the level ascencion, they will be rewarded with diomonds(20, 30, 50), extra hp (increased by 20%), and get one new skill (randomly)

							/*  INITIAL DECLARATION*/
struct{
    string name;
    string password;
    int HP;
    string role;
    int diamond;
    vector<string>elementalSkills;
    int level;
}Player1;

struct{
    string name;
    int hp;
    string attack;
    int damage;
}monster1;

int posX = SIZE/2, posY = SIZE /2;


void fill(char pattern[3][3], int row, int col);
bool isNextChar(int row, int col);
void saveGame();
void Shopping();

									/* MAP AND PRINT MAP*/

char map[SIZE][SIZE];

char grass1[3][3] = {{'v',' ', 'v'}, 
                     {' ', 'v', 'v'}, 
                     {' ', 'v', ' '}};

char grass2[3][3] = {{'v', 'v', 'v'}, 
                     {' ', 'v', 'v'}, 
                     {'v', 'v', ' '}};

char empty[3][3] = {{' ', ' ', ' '},
                    {' ', ' ', ' '},
                    {' ', ' ', ' '}};
                    
                    
void AssignMap()
{
    // assigning pattern
    for(int i = 0 ; i < SIZE ; i+=3)
    {
        for(int j = 0 ; j< SIZE ; j+=3)
        {
            if(isNextChar(i, j))
            {
                fill(empty, i, j);
            }
            else
            {
                int random = rand() % 3;
                if(random == 0)
                {
                    fill(grass1, i, j);  
                }
                else if(random == 1)
                {
                    fill(grass2, i, j);
                }
                else
                {
                    fill(empty, i, j);
                }
            }
        }
    }

    // assigning diamond (diamond dropped on the road or in the grasses) and monsters (60% 40%)
    for(int i = 0 ; i<300 ; i++)
    {
        int randomX = rand() % 149 + 1;
        int randomY = rand() % 149 + 1;

        int chance = rand() % 10 +1;
        if(chance <= 6)
        {
            map[randomY][randomX] = MONEY;
		}
    // assigning monster that will chase player
        else
        {
            if(map[randomY][randomX] != 'v')
                map[randomY][randomX] = MONSTER;
        }
    }
}

bool isNextChar(int row, int col)
{
    for(int i = max(0, row-3); i<min(SIZE, row+3); i++)
    {
        for(int j = max(0, col-3); j<min(SIZE, col+3); j++)
        {
            if(map[i][j] == 'v')
                return true;
        }
    }
    return false;
}

void fill(char pattern[3][3], int row, int col)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            map[row+i][col+j] = pattern[i][j];
        }
    }
}

void printMap()
{
    // kita akan print area map nya cukup 15 x 30 dengan camera movement
    cout << YELLOW;
    for(int i = posY - 7; i < posY + 7; i++)
    {
        for(int j = posX - 15; j< posX + 15; j++)
        {
            if(i == posY - 7 || i == posY + 6 )
                cout << '=';
            else if(j == posX - 15 || j == posX + 14)
                cout << '|';
            else if(i == posY && j == posX)
                cout << PLAYER;
            else
                cout << map[i][j];
        }
        cout << endl;
    }
    cout << BLUE;
    cout << Player1.name <<"\t Diamond : " << Player1.diamond << "\t|| " << "HP : " << Player1.HP << endl; 
        cout << "movement : [ W || A || S || D ]" << "\n Q : quit game || M : go to shop || T : save the current progress"<< endl;
        cout << ">>";
}


				
							/* MOVEMENT AND POINTS */

void monsterMove()
{
    const int directionX[] = {1, -1, 0, 0}; // right left up down
    const int directionY[] = {0, 0, -1, 1}; // right left up down

    // collision detection, using 15x15 grid
    
    for(int i = posY-7; i <= posY+7; i++)
    {
        for(int j = posX-7; j <= posX+7; j++)
        {
            if(map[i][j] == MONSTER)
            {
                int bestX = j;
                int bestY = i;

                int minDistance = SIZE + SIZE; // set as large as possible to avoid errors

                // Iterate through all 4 directions to find the best move
                for(int d = 0; d < 4; d++)
                {
                    int newX = j + directionX[d];
                    int newY = i + directionY[d];

                    // Check if the move is within bounds
                    if(newX > 0 && newX < SIZE && newY > 0 && newY < SIZE)
                    {
                        if(map[newY][newX] != 'v') // Make sure the new position is not an obstacle
                        {
                            int distance = abs(posX - newX) + abs(posY - newY);

                            // Check if it is the best move
                            if(distance < minDistance)
                            {
                                minDistance = distance;
                                bestX = newX;
                                bestY = newY;
                            }
                        }
                    }
                }

                // Move the monster in the temporary map
                map[i][j] = ' ';           // Clear old position
                map[bestY][bestX] = MONSTER; // Set new position
            }
        }
    }
}


void PlayerMovement()
{
        char kbhit = 'x';
        kbhit = getch();
        if(kbhit == 'w' || kbhit == 'W')
        {
            if(posY - 7 > 0)
                posY--;
        }
        if(kbhit == 's' || kbhit == 'S')
        {
            if(posY + 7 < SIZE-1)
                posY++;
        }
        if(kbhit == 'a' || kbhit == 'A')
        {
            if(posX -7 > 0)
                posX--;
        }
        if(kbhit == 'd' || kbhit == 'D')
        {
            if(posX + 7 < SIZE-1)
                posX++;
        }
        if(kbhit == 'm' || kbhit == 'M')
       	    Shopping();
        if(kbhit == 't' || kbhit == 'T')
       	    saveGame();
        if(kbhit == 'q' || kbhit == 'Q')
            exit(1);

        fflush(stdin);
}


void getDiamond()
{
	if(map[posY][posX] == MONEY)
	{
		map[posY][posX] = ' ';
		Player1.diamond++;
	}
}


							/*STATUS*/
void PlayerStatus()
{
    cout << Player1.name << " || Role : " << Player1.role << endl;
    cout << "Level " << Player1.level << " || HP : " << Player1.HP << endl;
    cout << "Elemental Skills : \n";
    int i = 1;
    for(auto element : Player1.elementalSkills)
    {
        cout << i << ". " << element << endl;
        i++;
    }
}

void EnemyStatus()
{
    cout << monster1.name;
    cout << " || HP : " << monster1.hp << endl;
    cout << "============================================\n";
}

void LevelAscension()
{
    if(Player1.level <=1 && Player1.diamond >= 30 && Player1.elementalSkills.size() >= 3)
    {
    	system("cls");
    	cout << R"(
+===============================================================================================+
| __       ___________    ____  _______  __       _______  _______      __    __  .______    __ |
||  |     |   ____\   \  /   / |   ____||  |     |   ____||       \    |  |  |  | |   _  \  |  ||
||  |     |  |__   \   \/   /  |  |__   |  |     |  |__   |  .--.  |   |  |  |  | |  |_)  | |  ||
||  |     |   __|   \      /   |   __|  |  |     |   __|  |  |  |  |   |  |  |  | |   ___/  |  ||
||  `----.|  |____   \    /    |  |____ |  `----.|  |____ |  '--'  |   |  `--'  | |  |      |__||
||_______||_______|   \__/     |_______||_______||_______||_______/     \______/  | _|      (__)|
+===============================================================================================+)" << '\n';
        Player1.HP = 130;
        Player1.diamond +=10;
        Player1.level++;
        cout << "Now You are Level 2!\n";
        Sleep(500);
        cout << "\t Max HP increased by 30% [Max HP : 130]\n";
        Sleep(500);
        cout << "\t Recieving reward of 10 Diamonds";
        Sleep(3000);
    }
    else if (Player1.level <=2 && Player1.diamond >= 50 && Player1.elementalSkills.size() >= 5)
    {
    	system("cls");
    	cout << R"(
+===============================================================================================+
| __       ___________    ____  _______  __       _______  _______      __    __  .______    __ |
||  |     |   ____\   \  /   / |   ____||  |     |   ____||       \    |  |  |  | |   _  \  |  ||
||  |     |  |__   \   \/   /  |  |__   |  |     |  |__   |  .--.  |   |  |  |  | |  |_)  | |  ||
||  |     |   __|   \      /   |   __|  |  |     |   __|  |  |  |  |   |  |  |  | |   ___/  |  ||
||  `----.|  |____   \    /    |  |____ |  `----.|  |____ |  '--'  |   |  `--'  | |  |      |__||
||_______||_______|   \__/     |_______||_______||_______||_______/     \______/  | _|      (__)|
+===============================================================================================+)" << '\n';
        Player1.HP = 200;
        Player1.diamond += 25;
        Player1.level++;
        cout << "Now You are Level 3!\n";
        Sleep(500);
        cout << "\t Max HP increased by 54% [Max HP : 200]\n";
        Sleep(500);
        cout << "\t Recieving reward of 25 Diamonds";
        Sleep(3000);
    }
}



							/*FIGHT SCENE*/

bool TimeToFight()
{
    if(map[posY][posX] == MONSTER)
        return true;

    return false;
}

void AssignMonster()
{
    srand(time(0));
    string monsterName;
    int HP;
    string elementalSkill;
    int Damage;

    ifstream ReadMonster("monster.txt");

    int randomOrder = rand() % 100 + 1;
    int limit = -1;
    if(randomOrder <= 30)
    {
        limit = 0;
    }
    else if(randomOrder <= 50)
    {
        limit = 1;
    }
    else if(randomOrder <= 75)
    {
        limit = 2;
    }
    else if (randomOrder <= 90)
    {
        limit = 3;
    }
    else
    {
        limit = 4;
    }
    string skip;
    for(int i = 0; i<limit; i++)
    {
        getline(ReadMonster, skip);
    }
    // read monster name
    getline(ReadMonster, monster1.name, '#');
    // read HP and convert it into integer
    string temp;
    getline(ReadMonster, temp, '#');
    istringstream convert(temp);
    convert >> monster1.hp;
    // read attack
    getline(ReadMonster, monster1.attack, '#');
    // read the damage and store it as an integer
    getline(ReadMonster, temp, '\n');
    istringstream convertdamage(temp);
    convertdamage >> monster1.damage;
    
    ReadMonster.close();
}



void fight()
{
    fflush(stdin);
    int i = 1;
    while(monster1.hp > 0 && Player1.HP > 0)
    {
        system("cls");
        cout << RED;
        cout << R"( 
     _____ _       _     _     _ 
    |  ___(_) __ _| |__ | |_  | |
    | |_  | |/ _` | '_ \| __| | |
    |  _| | | (_| | | | | |_  |_|
    |_|   |_|\__, |_| |_|\__| (_)
            |___/               )" << '\n';

        cout << BLUE << "You meet " << monster1.name << " !!!\n";
        cout << "beat him !!\n";
        
        Sleep(500);

        EnemyStatus();
        PlayerStatus();
        if(i == 1) // player turn
        {
            int attack;
            cout << "Your Turn!!\n";
            cout << "Choose one elemental skill\n" << ">> ";
            cin >> attack;
            string filename = Player1.role + ".txt";
            ifstream ReadSkill(filename);
            string nameTemp;
            int damage = 0;
            while(!ReadSkill.eof())
            {
                getline(ReadSkill, nameTemp, ',');
                if(nameTemp.compare(Player1.elementalSkills.at(attack-1)) == 0)
                {
                    getline(ReadSkill, nameTemp, ',');
                    istringstream temp(nameTemp);
                    temp >> damage;
                }
                else
                {
                    string buffer;
                    getline(ReadSkill, buffer);
                }
            }

            cout << "You're attacking the monster using " << Player1.elementalSkills.at(attack-1) << "\n";
            cout << "damage applied : " << RED << damage << endl;
            monster1.hp -= damage;
            i = 0;
            Sleep(1500);
        }
        else
        {
            cout << "Enemy's Turn....";
            Sleep(500);
            cout << "Enemy is attacking you using " << monster1.attack << endl;
            cout << "damage applied : " << RED << monster1.damage << endl;
            Player1.HP -= monster1.damage;
            i = 1;
            Sleep(1500);
        }
        
    }
    if(monster1.hp < 0 && Player1.HP > 0)
    {
        // annihilating monster in radius 7x7
        for(int i = posY-7; i<posY+7; i++)
        {
            for(int j = posX-7; j<posX+7 ; j++)
            {
                if(map[i][j] != 'v')
                    map[i][j] = ' ';
            }
        }
    	cout << "\nEnemy has Beaten!";
    	cout << BLUE;
    	Sleep(1000);
	}
	
}

bool isGameOver()
{
	if(Player1.HP <= 0)
	{
        if(Player1.level == 1)
            Player1.HP = 100;
        else if(Player1.level == 2)
            Player1.HP = 130;
        else if(Player1.level == 3)
            Player1.HP = 200;

		system("cls");
		cout << R"(  
	  ____                         ___                 
	 / ___| __ _ _ __ ___   ___   / _ \__   _____ _ __ 
	| |  _ / _` | '_ ` _ \ / _ \ | | | \ \ / / _ \ '__|
	| |_| | (_| | | | | | |  __/ | |_| |\ V /  __/ |   
	 \____|\__,_|_| |_| |_|\___|  \___/  \_/ \___|_|   
	          _    _              _                    
	__      _| | _| | ____      _| | ____      __      
	\ \ /\ / / |/ / |/ /\ \ /\ / / |/ /\ \ /\ / /      
	 \ V  V /|   <|   <  \ V  V /|   <  \ V  V /       
	  \_/\_/ |_|\_\_|\_\  \_/\_/ |_|\_\  \_/\_/        )";
    //   cout << "\n press any button to continue......";
      saveGame();
      system("cls");
		return true;
	}
	return false;
}



					/* GAME CONTROL*/

void entergame()
{
        AssignMap();
    while(1)
    {
        system("cls");
        printMap();
        PlayerMovement();
        monsterMove();
        getDiamond();
        if(TimeToFight())
        {
            AssignMonster();
            fight();
        	if(isGameOver())
        		break;
        }
        if(Player1.level <=2)
        	LevelAscension();
        fflush(stdin);
    }
}

						/* ANIMATIONS */
void LoadingPage()
{
	system("cls");
    cout << endl;
        cout << RED;
	    cout << R"( 
	 _   _             _         _   __  __                     _     _ _       
	| | | | ___  _ __ | | ____ _(_) |  \/  | ___   ___  _ __   | |   (_) |_ ___ 
	| |_| |/ _ \| '_ \| |/ / _` | | | |\/| |/ _ \ / _ \| '_ \  | |   | | __/ _ \
	|  _  | (_) | | | |   < (_| | | | |  | | (_) | (_) | | | | | |___| | ||  __/
	|_| |_|\___/|_| |_|_|\_\__,_|_| |_|  |_|\___/ \___/|_| |_| |_____|_|\__\___|)" << "\n";

    cout << BLUE << "\t\t\tCreated By : Moh. Rega Assegaf Nur Habibi\n\n";

	cout << "\n\t\t\tEntering World ";
	int i = 10;
	while(i >0)
	{
		Sleep(500);
		cout << "=";
		i--;
	}
}


   						 /* Shopping*/

void Shopping() // belum jadi ya
{
    system("cls");
    cout << YELLOW;
    cout << R"(
__        __                             __  __           _                 
\ \      / /_ _ _ __ _   _ _ __   __ _  |  \/  | __ _  __| |_   _ _ __ __ _ 
 \ \ /\ / / _` | '__| | | | '_ \ / _` | | |\/| |/ _` |/ _` | | | | '__/ _` |
  \ V  V / (_| | |  | |_| | | | | (_| | | |  | | (_| | (_| | |_| | | | (_| |
 __\_/\_/ \__,_|_|___\__,_|_| |_|\__, | |_|  |_|\__,_|\__,_|\__,_|_|  \__,_|
|___ \| || |   / /___  |         |___/                                      
  __) | || |_ / /   / /                                                     
 / __/|__   _/ /   / /                                                      
|_____|  |_|/_/   /_/                                                       )" << endl;

    cout << BLUE <<"Selamat datang di Warung Madura 24/7 buka sampai kiamat !!\n";
    cout << "Kami menyedikan skill untuk segala role, mohon maaf kami tidak menjual celurit hehe...\n";
    cout << "silakan dipilih : " << endl;
    string filename = Player1.role + ".txt";
    ifstream Read(filename);

    // Skip the first line
    string skip;
    getline(Read, skip);

    string skills, damage, price;
    int i = 1;
    vector<pair<string, int>> availableSkills;  // Store skills and prices int an ordered pairs format
    cout << RED << YBackground;
    // Display the skills
    while (getline(Read, skills, ','))
    {
        getline(Read, damage, ',');
        getline(Read, price);

        int skillPrice;
        istringstream priceStream(price);
        priceStream >> skillPrice;

        cout << i << ". " << skills << "\t|| damage : " << damage << "\t price : " << skillPrice << " diamonds" << endl;
        availableSkills.push_back(make_pair(skills, skillPrice));  // Store skills with their prices
        i++;
    }

    Read.close();

    cout << BGreset << BLUE <<"Choose a skill to purchase: " << RED;
    int choose;
    cin >> choose;

    cout << BLUE;
    if (choose > 0 && choose <= availableSkills.size())
    {
        string selectedSkill = availableSkills[choose - 1].first;
        int selectedPrice = availableSkills[choose - 1].second;

        cout << "Are you sure you want to purchase " << selectedSkill << " for " << selectedPrice << " diamonds? (Y/N): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y')
        {
            if (Player1.diamond >= selectedPrice)
            {
                Player1.elementalSkills.push_back(selectedSkill);
                Player1.diamond -= selectedPrice;
                cout << "You have successfully purchased " << selectedSkill << "!" << endl;
            }
            else
            {
                cout << "Not enough diamonds to make this purchase." << endl;
            }
        }
    }
    else
    {
        cout << "Invalid choice. Returning to the game..." << endl;
    }

    Sleep(2000);  // Pause before returning to the game
}

								/* ACCOUNT AND FILE HANDLING */

void saveGame()
{
    cout << "Loading........................... ";
	// yang update itu hp, skills, diamond, sama level
	std::fstream MyFile("credentials.txt", std::ios::in | std::ios::out); // allow us to read and write
    // tampung dulu data yang bukan line kita
    string Holder;
    string line;
    while(getline(MyFile, line))
    {
        // if the line is not the player's line
        if(line.find(Player1.name) == string::npos) // npos is usually returned -1 (means the position of player name in line is -1 / not found)
        {
            Holder += line + "\n"; // tampung ke holder
        }
    }

    // pastiin file yang tadi kosong dulu
    MyFile.clear();
    // move the pointer to the beginning of fileee
    MyFile.seekp(0, ios::beg);  // move the 'put pointer' to a desired position (sesuai reference point nya, dalam hal ini adalah 0 position from ios::beg yaitu beginning)
    // write data yang tadi ditampung
    MyFile << Holder;
    // save the current player's data
    MyFile << Player1.name << ',' << Player1.password << ',' << Player1.HP << ',' << Player1.role << '#';

    for(const auto& skill : Player1.elementalSkills)
    {
        MyFile << skill << '#';
    }
    MyFile << ',' << Player1.diamond << ',' << Player1.level << '\n';
    
    MyFile.close();

    Sleep(2000);

    cout << "Game Saved Successfully!" << endl;

    Sleep(2000);
}

void CreateAccount()
{
    string username;
    cout << "Create username : ";
    getline(cin, username);
    string password;
    cout << "Create a password [must not contain any blank space] : ";
    getline(cin, password);
    cout << "Choose role :\n" << "1. Fighter\n2. Assasin\n3. Mage\n" << ">> ";
    string role, defaultskill;
    int choose;
    cin >> choose;
    switch (choose)
    {
    case 1:
        cout << "You Choose Fighter as a role.....";
        role = "Fighter";
        defaultskill = "Blazing Furry";
        break;
    case 2 :
        cout << "You Choose Assasin as a role.....";
        role = "Assasin";
        defaultskill = "Inferno Blade";
        break;
    case 3 :
        cout << "You Choose Mage as a role........";
        role = "Mage";
        defaultskill = "Pyroclasm";
        break;
    default:
        break;
    }
    fstream MyFile("credentials.txt", std::ios::app);

    MyFile << username << ',' << password << ',' << "100" << ',' << role << '#' << defaultskill << '#' << ',' << "10" << ',' << "1" << '\n'; 

    MyFile.close();
}


void Login()
{
    string username1;
    cout << "enter username : ";
    getline(cin, username1);
    string password1;
    cout << "enter your password : ";
    getline(cin, password1);
    ifstream Myread("credentials.txt");
    string username2, password2, rolename, skills;
    string HP, diamond, lvl;

    int check;
    while(!Myread.eof())
    {
        getline(Myread, username2, ',');
        getline(Myread, password2, ',');
        getline(Myread, HP, ',');
        getline(Myread, rolename, '#');
        getline(Myread, skills, ',');
        getline(Myread, diamond, ',');
        getline(Myread, lvl);

        // cout << username2 << "||";
        // cout << password2 << "\n";
        
        if(username1.compare(username2) == 0 && password2.compare(password1) == 0)
        {
            check = 1;
            break;
        }
    }

    if(check)
    {
        // name, password, hp, role, skills, diamond
        Player1.name = username2;
        // read pass
        Player1.password = password2;
        // read HP
        istringstream temp(HP);
        temp >> Player1.HP;
        // read role
        Player1.role = rolename;
        // read entire skills
        istringstream skill(skills);
        string hold;
        while(getline(skill, hold, '#'))
        {
            Player1.elementalSkills.push_back(hold);
        }
        // read diamond
        stringstream temp2(diamond);
        temp2 >> Player1.diamond;
        // read
        stringstream temp3(lvl);
        temp3 >> Player1.level;
        LoadingPage();
        entergame();
    }
    else
    {
        cout << "wrong credentials!!\n";
        // main();
    }
}

	


int main()
{
	// entergame();
    while(1)
    {
        fflush(stdin);
        cout << endl;
        cout << RED;
	    cout << R"( 
	 _   _             _         _   __  __                     _     _ _       
	| | | | ___  _ __ | | ____ _(_) |  \/  | ___   ___  _ __   | |   (_) |_ ___ 
	| |_| |/ _ \| '_ \| |/ / _` | | | |\/| |/ _ \ / _ \| '_ \  | |   | | __/ _ \
	|  _  | (_) | | | |   < (_| | | | |  | | (_) | (_) | | | | | |___| | ||  __/
	|_| |_|\___/|_| |_|_|\_\__,_|_| |_|  |_|\___/ \___/|_| |_| |_____|_|\__\___|)" << "\n";

    cout << BLUE << "\t\t\tCreated By : Moh. Rega Assegaf Nur Habibi\n\n";

        srand(static_cast<unsigned int>(time(NULL)));
        cout << "Welcome Adventurer !! Let's Begin Your Journey : \n" << "1. create new account\n" << "2. Login\n" << "3. Exit\n";
        cout << ">>";
        int choose;
        cin >> choose;
        fflush(stdin);
        fflush(stdin);
        switch (choose)
        {
        case 1 :
            CreateAccount();
            break;
        case 2 :
            Login();
            break;
        case 3 :
            return 0;
        default :
            break;
        }
    }
    return 0;
}

/* kurang :
- musuh yang sudah beaten masih ada di map   v
- nambah uang  v
- tempat belanja skill
- save game v
- naik level*/
