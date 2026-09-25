//This project is done by Muhammad Huzaifa (25F-0798) , Muhammad Uzair (25F-0683) , Muhammad Ammar (25F-0686)
//This is a fully functional console based quiz game
//User can choose 3 type of difficulties, 5 Categories and 4 Ingame Cheats(LifeLine)

#include <iostream>
#include <fstream>      //File Handling
#include <cstdlib>      //Random
#include <ctime>        //Time
#include <string>       //Strings
#include <conio.h>      //kbit && getch() funcions - used to read input without clicking enter
#include <windows.h>    //sleep() - used to add timer
using namespace std;

//Game Settings 
int timeLeft = 10; //10 sec for each question
int noofQuestions = 10; //no of questions ask for each round
int lifeline = 1; //no. of life line can be used per quiz
string textColorCode = "07"; //Color

//Global Declarations
const int length = 60;
const int availableOption = 4;
string playerName;
string questions[length];
string option[length][availableOption];
int answers[length];
int difficulties[length];
bool used[length];
string reviewQuestions[10000];
string reviewCorrectOption[10000];
string reviewUserOption[10000];


//Functions Prototype/Decleration
void loadSettings();
void saveSettings();
void settingsMenu();
int startmenu();
int difficultymenu();
char exitmenu();
void loadingQuestions(string category, int difficulty,int categoryChoce);
void displayLifelines();
void displayquestions(int difficulty, int categoryChoice);
int lifeline5050(int correctAnswer, int questionIndex, int& count);
bool lifelineSkip(int& count);
int lifelineReplace(int currentAns, int difficulties[], bool used[], int difficulty, int& count, int index,int &t);
int lifelineExtraTime(int currentTime, int& count);
int updateScore(int currentScore, int userAnswer, int correctAnswer, int difficulty, int& streak, string correct);
void endscreen(int score, int categoryChoice, int difficulty);
void logsSaver(int score);
void HighscoreSaver(int score);
void reviewing(int score);
void showHighScores();
bool postGame();

int main()
{   
    loadSettings();
    int menuChoice = 1, categoryChoice, difficulty;
    char exitChoice;

    while (menuChoice != -1)
    {
        system("cls");
        cout << "\n\n\t\t\t\t\t===================================\n";
        cout << "\t\t\t\t\t           MAIN MENU              \n";
        cout << "\t\t\t\t\t===================================\n\n";
        cout << "\t\t\t\t\t1. Start Quiz\n";
        cout << "\t\t\t\t\t2. View High Scores\n";
        cout << "\t\t\t\t\t3. Settings\n";  // Add this in the main menu
        cout << "\t\t\t\t\t4. Exit\n\n";
        cout << "\t\t\t\t\t-----------------------------------\n\n";
        cout << "\t\t\t\t\tInput: ";
        cin >> menuChoice;

        //Input Validation 
        while (cin.fail() || menuChoice < 1 || menuChoice > 4)
        {
            if (cin.fail())
            {
                cin.clear(); 
                cin.ignore(1000, '\n');
            }
            cout << "\t\t\t\t\tInvalid input. Re-enter: ";
            cin >> menuChoice;
        }

        switch (menuChoice)
        {
        case 1:        //Start Quiz

            //Calling Start Menu 
            categoryChoice = startmenu();

            //If pressed exit
            if (categoryChoice == 6)
            {
                exitChoice = exitmenu();
                if (exitChoice == 'Y')
                {
                    menuChoice = -1;
                    break;
                }
                else
                {
                    categoryChoice = startmenu();
                }
            }
            //Calling Difficulty menu
            difficulty = difficultymenu();

            //Asking Name Before Starting Quiz (for saving logs)
            system("cls");
            cout << "\n\n\t\t\t\t\t===================================\n";
            cout << "\t\t\t\t\t               NAME      \n";
            cout << "\t\t\t\t\t===================================\n\n";
            cout << "\t\t\t\t\tEnter your name: \n\n";
            cout << "\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\t\tInput: ";

            cin.ignore();
            getline(cin, playerName);

            //Calling function to load Questions from file and save them to array
            switch (categoryChoice)
            {
            case 1:
                loadingQuestions("science.txt", difficulty,categoryChoice);
                break;
            case 2:
                loadingQuestions("computer.txt", difficulty,categoryChoice);
                break;
            case 3:
                loadingQuestions("sports.txt", difficulty,categoryChoice);
                break;
            case 4:
                loadingQuestions("history.txt", difficulty,categoryChoice);
                break;
            case 5:
                loadingQuestions("iq.txt", difficulty,categoryChoice);
                break;
            }
            break;

        case 2:         //High Score
            showHighScores();
            break;
        
        case 3:       // Settings
            settingsMenu();
            break;

        case 4:         //Exit
            exitChoice = exitmenu();
            if (exitChoice == 'Y')
            {
                menuChoice = -1;
            }
            break;
        }
    }
    cout << "\n\t\t\t\t\tPress Enter to continue";
    cin.get();
    cin.ignore();
    return 0;
}

// Save current settings to a file
void saveSettings() 
{
    ofstream out("settings.txt");
    if (out) 
    {
        out << noofQuestions << " " << timeLeft << " " << lifeline << " " << textColorCode << endl;
    }
    out.close();
}

// Load settings from a file
void loadSettings()
{
    ifstream in("settings.txt");
    if (in)
    {
        in >> noofQuestions >> timeLeft >> lifeline >> textColorCode;
        system(("color " + textColorCode).c_str()); // Apply saved text color
    }
    in.close();
}

//Display Settings menu
void settingsMenu()
{
    int choice;
    do
    {
        system("cls");
        cout << "\n\n\t\t\t\t\t===================================\n";
        cout << "\t\t\t\t\t             SETTINGS              \n";
        cout << "\t\t\t\t\t===================================\n\n";
        cout << "\t\t\t\t1. Change Console Color\n";
        cout << "\t\t\t\t2. Change Number of Quiz Questions (Currently: " << noofQuestions << ")\n";
        cout << "\t\t\t\t3. Change Time per Question (Currently: " << timeLeft << " sec)\n";
        cout << "\t\t\t\t4. Change Number of Lifelines (Currently: " << lifeline << " each)\n";
        cout << "\t\t\t\t5. Reset to Default\n";
        cout << "\t\t\t\t6. Back to Main Menu\n\n";
        cout << "\t\t\t\t\t-----------------------------------\n\n";
        cout << "\t\t\t\t\tInput: ";
        cin >> choice;

        while (cin.fail() || choice < 1 || choice > 6)
        {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cout << "\t\t\t\t\tInvalid input. Re-enter: ";
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
        
            int colorChoice;
            cout << "\n\t\t\t\tChoose Console Color:\n";
            cout << "\t\t\t\t1. Blue\n";
            cout << "\t\t\t\t2. Green\n";
            cout << "\t\t\t\t3. Red\n";
            cout << "\t\t\t\t4. Dark Mode\n";
            cout << "\t\t\t\t5. Light Mode\n\n";
            cout << "\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\tInput: ";
            cin >> colorChoice;

            switch (colorChoice)
            {
            case 1:
                textColorCode = "09";
                break; //Blue
            case 2:
                textColorCode = "0A";
                break;   // Green
            case 3:
                textColorCode = "0C";
                break;   // Red
            case 4:
                textColorCode = "07";
                break;  // White on Black
            case 5:
                textColorCode = "F0";
                break;  // light Mode
            default:
                cout << "\t\t\t\t\tInvalid color choice!\n";
            }
            system(("color " + textColorCode).c_str()); // Apply immediately
            saveSettings();
            cout << "\n\t\t\t\t\tPress Enter to continue";
            cin.ignore();
            cin.get();
            break;
        


        case 2:
            cout << "\n\t\t\t\t\tEnter number of questions (Range: 1-20 ): ";
            cin >> noofQuestions;
            while (!(noofQuestions > 0 && noofQuestions <= 20))
            {
                cout << "\t\t\t\t\tInvalid Input or Range !\n";
                cout << "\t\t\t\t\tRe-enter: ";
                cin >> noofQuestions;
            }
            saveSettings();
            cout << "\n\t\t\t\t\tPress Enter to continue";
            cin.get();
            cin.ignore();
            break;

        case 3:
            cout << "\n\t\t\t\t\tEnter time per question (seconds): ";
            cin >> timeLeft;
            saveSettings();
            cout << "\n\t\t\t\t\tPress Enter to continue";
            cin.get();
            cin.ignore();
            break;

        case 4:
            cout << "\n\t\t\t\t\tEnter number of lifelines per quiz: ";
            cin >> lifeline;
            saveSettings();
            cout << "\n\t\t\t\t\tPress Enter to continue";
            cin.get();
            cin.ignore();
            break;

        case 5: // Reset settings
            noofQuestions = 10;
            timeLeft = 10;
            lifeline = 1;
            textColorCode = "07";        // White text on black background
            system("color 07");          // Apply default immediately
            saveSettings();
            cout << "\n\t\t\t\t\tSettings reset to default!";
            cout << "\n\t\t\t\t\tPress Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }
    } 
    while (choice != 6);
}

//Display Start Menu
int startmenu()         
{
    int categoryChoice;
    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t             CATEGORY      \n";
    cout << "\t\t\t\t\t===================================\n\n";
    cout << "\t\t\t\t\t1. Science\n";
    cout << "\t\t\t\t\t2. Computer\n";
    cout << "\t\t\t\t\t3. Sports\n";
    cout << "\t\t\t\t\t4. History\n";
    cout << "\t\t\t\t\t5. IQ\n";
    cout << "\t\t\t\t\t6. Exit\n\n";
    cout << "\t\t\t\t\t--------------------------------\n\n";
    cout << "\t\t\t\t\tInput: ";
    cin >> categoryChoice;

    //Input Validation
    while (cin.fail() || categoryChoice < 1 || categoryChoice > 6)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "\t\t\t\t\tInvalid input. Re-enter: ";
        cin >> categoryChoice;
    }
    return categoryChoice;
}

//Display Difficulty Menu
int difficultymenu()
{
    int difficulty;
    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t            DIFFICULTY     \n";
    cout << "\t\t\t\t\t===================================\n\n";
    cout << "\t\t\t\t\t1. Easy\n";
    cout << "\t\t\t\t\t2. Medium\n";
    cout << "\t\t\t\t\t3. Hard\n\n";
    cout << "\t\t\t\t\t-----------------------------------\n\n";
    cout << "\t\t\t\t\tInput: ";
    cin >> difficulty;

    while (cin.fail() || difficulty < 1 || difficulty > 3)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "\t\t\t\t\tInvalid input. Re-enter: ";
        cin >> difficulty;
    }
    return difficulty;
}

//Display Exit Menu
char exitmenu()
{
    char exit;
    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t              EXIT         \n";
    cout << "\t\t\t\t\t===================================\n\n";
    cout << "\t\t\t\t\tAre you sure you want to exit? (Y/N) \n\n";
    cout << "\t\t\t\t\t-----------------------------------\n\n";
    cout << "\t\t\t\t\tInput: ";
    cin >> exit;
    exit = toupper(exit);

    while (exit != 'Y' && exit != 'N')
    {
        cout << "Invalid! Re-enter: ";
        cin >> exit;
        exit = toupper(exit);
    }

    if (exit == 'Y')
    {
        cout << "\n\t\t\t\t\tThanks for playing!\n";
        cout << "\t\t\t\t\tGoodbye!\n\n";
        cout << "\t\t\t\t\t-----------------------------------\n\n";
    }
    return exit;
}

//Loading Questions from File to arrays according to Difficulty Choosed
void loadingQuestions(string category, int difficulty,int categoryChoice)
{
    //Opening and loading from file.
    ifstream input;
    input.open(category);        

    if (!input.is_open())         
    {
        cout << "Error! Couldn't read file: " << category;
        return;
    }
    else
    {
        for (int i = 0;i < length;i++)
        {
            used[i] = false;
            getline(input, questions[i]);
            for (int j = 0;j < availableOption;j++)
            {
                getline(input, option[i][j]);
            }
            input >> answers[i];
            input >> difficulties[i];
            input.ignore();     //As reading 'int' from last line, so ignoring new lines
        }
        input.close();          //Closing file

        displayLifelines();     //Displaying Lifeline Hint
        displayquestions(difficulty, categoryChoice);
    }
}

//LifeLines Hint
void displayLifelines()
{
    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t              LIFELINES       \n";
    cout << "\t\t\t\t\t===================================\n\n";
    cout << "\t\t\t   These are your special powers! Each can be used once per quiz.\n\n";
    cout << "\t\t\t   1. 50/50         -> Removes two wrong options from the question.\n";
    cout << "\t\t\t   2. Skip Question -> Skip the current question without penalty.\n";
    cout << "\t\t\t   3. Replace Ques. -> Replaces current question with a new one.\n";
    cout << "\t\t\t   4. Extra Time    -> Gives 10 more seconds for the current question.\n";
    cout << "\n\t\t\t\t\t-----------------------------------\n\n";
    cout << "\t\t\t\t\tEach lifeline can be used " << lifeline << " Time(s)\n";
    cout << "\t\t\t\t\tPress Enter to start the quiz...";
    cin.ignore();
    cin.get();    
}

//Display Questions on screen
void displayquestions(int difficulty, int categoryChoice)
{    
    int score = 0;      
    int streak = 0;     
    int count = 0;     

    //Randomizing Quiz Questions
    srand(time(0));

    //Lifeline counter Decleration reflecting game settings
    int lifeline5050Count = lifeline;     // 50/50
    int lifelineSkipCount = lifeline;     // Skip
    int lifelineReplaceCount = lifeline;  // Replace
    int lifelineExtraTimeCount = lifeline;// Extra Time

    system("cls");
    while (count < noofQuestions)
    {
        int userAnswer = -1;
        int random = rand() % length;
        if (difficulties[random] == difficulty && used[random]==false)
        {
            cout << "\n\n\t\t\t\t\t===================================\n";
            cout << "\t\t\t\t\t             QUESTION        \n";
            cout << "\t\t\t\t\t===================================\t";
            //Displaying Questions
            cout << "\n\n\t\t\t\tQuestion " << count + 1 << ": " << questions[random] << endl;
            for (int j = 0; j < availableOption; j++)
            {
                cout <<"\t\t\t\t" << j + 1 << ". " << option[random][j] << endl;
            }

            //Making 'Used Array' true so that same questions don't repeat
            used[random] = true;

            //Displaying LifeLine Cheats
            cout << "\n\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\t5. 50/50\n";
            cout << "\t\t\t\t6. Skip Question\n";
            cout << "\t\t\t\t7. Replace Question\n";
            cout << "\t\t\t\t8. Extra Time\n\n";
            cout << "\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\tInput: Answer (1-4), Lifeline (5,8): \n";

            //Timer Logic
            int currentTime = timeLeft; //Time Declaration reflecting game 
            string input = "";
            for (int t = currentTime; t >= 0; t--)
            {
                cout << "\r\t\t\t\tTime left: " << t << " seconds   ";
                Sleep(1000);  //1000 miliseconds = 1 second

                if (_kbhit()) //Detect Key Pressing
                {
                    int ch = _getch(); //Read pressed key 

                    //For Answers
                    if (ch >= '1' && ch <= '4')
                    {
                        userAnswer = ch - '0';  //Char(ASCII) to int converstion
                        break;
                    }

                    //For lifeline
                    else if (ch >= '5' && ch <= '8')
                    {
                        switch (ch - '0')
                        {
                        case 5: //50-50
                            userAnswer = lifeline5050( answers[random], random, lifeline5050Count);
                            break;

                        case 6: //Skip
                            if (lifelineSkip(lifelineSkipCount)) //If skipped
                            {
                                userAnswer = -2; // Don't trigger 'score' function
                                t = -1;  //Next Iteration
                            }
                            else
                            {
                                userAnswer = -1; // No lifeline left, keep Quiz running
                            }
                            break;

                        case 7: //Replaced
                            answers[random]=lifelineReplace(answers[random], difficulties, used, difficulty, lifelineReplaceCount,count,t);
                            userAnswer = -1;
                            break;

                        case 8: //Extra Time
                            t = lifelineExtraTime(t, lifelineExtraTimeCount);
                            userAnswer = -1;
                        }
                    }
                }
            }
            if (userAnswer != -2)
            {
                //Triggering Score Function
                score = updateScore(score, userAnswer, answers[random], difficulty, streak, option[random][answers[random] - 1]);
                
                //Saving review Questions for logs and View Incorrect Question after quiz end
                reviewQuestions[count] = questions[random];
                reviewCorrectOption[count] = option[random][answers[random] - 1];

                if (userAnswer >= 1 && userAnswer <= 4)
                {
                    reviewUserOption[count] = option[random][userAnswer - 1];
                }
                else
                {
                    reviewUserOption[count] = " Skipped/Timeout";
                }
            }
            count++;
            if (count < noofQuestions )
            {
                cout << "\n\n\t\t\t\tPress Enter to continue";
                cin.get();
                system("cls");
            }
        }
        
    }
    endscreen(score, categoryChoice, difficulty);
}
//Lifeline 50/50
int lifeline5050( int correctAnswer, int questionIndex, int& count)
{
    if (count <= 0)
    {
        cout << "\n\n\t\t\t\tNo 50/50 lifelines left!\n";
        return -1; // user still needs to answer
    }
    count--; //Used one lifeline
    cout << "\n\n\t\t\t\t50/50 applied! Two wrong options removed, Remaining options are: \n";
    int removed = 0;
    for (int j = 0; j < availableOption; j++)
    {
        if (j + 1 != correctAnswer && removed < 2)
        {
            removed++;
            continue;
        }
        //Display remaining options
        cout <<"\t\t\t\t" << j + 1 << ". " << option[questionIndex][j] << endl;
    }
    cout << endl;
    return -1;
}

//Lifeline Skip
bool lifelineSkip(int& count)
{
    if (count <= 0)
    {
        cout << "\n\n\t\t\t\tNo Skip lifelines left!\n";
        return false;
    }
    else
    {
        count--;
        cout << "\n\n\t\t\t\tQuestion skipped!";
        return true;
    }
}

//Lifeline replace
int lifelineReplace(int currentAns, int difficulties[], bool used[], int difficulty,int &count,int index,int &t)
{
    if (count <= 0)
    {
        cout << "\n\n\t\t\t\tNo Replace lifelines left!\n";
        return currentAns;
    }
    count--;
    cout << "\n\n\t\t\t\tQuestion is replaced, new question is:";

    while (true)
    {
        int random = rand() % length;
        if (difficulties[random] == difficulty && used[random] == false)
        {
            //Displaying Questions
            cout << "\n\n\n\t\t\t\tQuestion " << index + 1 << ": " << questions[random] << endl;
            for (int j = 0; j < availableOption; j++)
            {
                cout <<"\t\t\t\t"<< j + 1 << ". " << option[random][j] << endl;
            }
            //Making Used true so that same questions don't repeat
            used[random] = true;

            //Displaying LifeLine Cheats
            cout << "\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\t5. 50/50\n";
            cout << "\t\t\t\t6. Skip Question\n";
            cout << "\t\t\t\t7. Replace Question\n";
            cout << "\t\t\t\t8. Extra Time\n";
            cout << "\t\t\t\t\t-----------------------------------\n\n";
            cout << "\t\t\t\tInput: Answer (1-4), Lifeline (5,8): \n";
            t = 11;
            return answers[random];
        }
    }
}

//Lifeline Extra Time
int lifelineExtraTime(int currentTime, int& count)
{
    if (count <= 0)
    {
        cout << "\n\n\t\t\t\tNo Extra Time lifelines left!\n";
        return currentTime;
    }
    count--;
    cout << "\n\n\t\t\t\tExtra Time applied! +10 seconds\n";
    return currentTime + 10;
}

//Updating score
int updateScore(int currentScore, int userAnswer, int correctAnswer, int difficulty, int& streak,string correct)
{
    cout << endl;

    //If answer is true
    if (userAnswer == correctAnswer)
    {
        cout << "\n\t\t\t\tCorrect Answer +1!\n";
        streak++;
        currentScore++;

        // Streak bonus
        if (streak == 3 )
        {
            cout << "\t\t\t\tStreak Bonus +3!\n";
            currentScore += 3;
        }
        else if (streak == 5)
        {
            cout << "\t\t\t\tStreak Bonus +5!\n";
            currentScore += 5;
        }
        else if (streak == 10)
        {
            cout << "\t\t\t\tStreak Bonus +10!\n";
            currentScore += 10;
        }
    }
    //If answer isn't true
    else
    {
        if(userAnswer==-1) //Unanswered 
        {
            cout << "\n\t\t\t\tTime's up! Question marked wrong.\n";
            cout << "\t\t\t\tCorrect answer: " << correct << endl;
        }
        else //Wrong Answered
        {
            cout << "\n\t\t\t\tIncorrect Answer!\n";
            cout << "\t\t\t\tCorrect answer: " << correct << endl;
        }
        streak = 0;

        // Negative marking
        if (difficulty == 1) 
        {
            currentScore -= 2;
            cout << "\t\t\t\tNegative Marking -2!\n";
        }
        else if (difficulty == 2)
        {
            currentScore -= 3;
            cout << "\t\t\t\tNegative Marking -3!\n";
        }
        else 
        {
            currentScore -= 5;
            cout << "\t\t\t\tNegative Marking -5!\n";
        }
    }
    cout << "\t\t\t\tCurrent Score: " << currentScore;
    return currentScore;
}

// Displaying Endscreen
void endscreen(int score, int categoryChoice, int difficulty)
{
    //Save logs
    logsSaver(score);

    //Save HighScore
    HighscoreSaver(score);

    //Reviewing incorrect questions after quiz end
    reviewing(score);

    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\t\t\t\n";
    cout << "\t\t\t\t\t             GAME OVER       \n";
    cout << "\t\t\t\t\t===================================\t\t\t\n\n";
    cout << "\t\t\t\t\tYour final score: " << score << endl;
    if (score < 1)
        cout << "\t\t\t\t\tYou need more practice! :(" << endl;
    else if (score < 6)
        cout << "\t\t\t\t\tGreat Job !" << endl;
    else
        cout << "\t\t\t\t\tYou Scored very well! :)" << endl;
    cout << "\n\n\t\t\t\t\tPress Enter to continue";
    cin.get();

    bool replay = postGame();

    //True --> Replay quiz
    if (replay)
    {
        // Replay quiz with the same category & difficulty
        switch (categoryChoice)
        {
        case 1: 
            loadingQuestions("science.txt", difficulty,categoryChoice);
            break;
        case 2:
            loadingQuestions("computer.txt", difficulty, categoryChoice);
            break;
        case 3:
            loadingQuestions("sports.txt", difficulty, categoryChoice);
            break;
        case 4:
            loadingQuestions("history.txt", difficulty, categoryChoice);
            break;
        case 5:
            loadingQuestions("iq.txt", difficulty, categoryChoice);
            break;
        }
    }

    //False --> Main Menu
    else
    {
        return;
    }
}

//Log Saver
void logsSaver(int score)
{
    //Save All logs
    ofstream log("quiz_logs.txt", ios::app);

    time_t now = time(0);
    char dt[26];            // ctime_s needs a buffer of size 26
    ctime_s(dt, sizeof(dt), &now);
    log << "\t\t\t\t\tPlayer: " << playerName << endl;
    log << "\t\t\t\t\tDate: " << dt;
    log << "\t\t\t\t\tScore: " << score << endl;
    log << "\t\t\t\t\t------ QUESTIONS ------\n";

    for (int i = 0; i < noofQuestions; i++)
    {
        log << "Q" << i + 1 << ": " << reviewQuestions[i] << endl;
        log << "tCorrect: " << reviewCorrectOption[i] << endl;
        log << "Your Answer : " << reviewUserOption[i] << endl;
        log << endl;
    }

    log << "\t\t\t\t\t===================================\n\n";
    log.close();
}

//Score Saver
void HighscoreSaver(int score)
{
    // Save score to high_scores.txt
    ofstream hs("high_scores.txt", ios::app);

    if (!hs) {
        cout << "\t\t\t\t\tError opening high_scores.txt\n";
    }
    else {
        time_t now = time(0);
        char dt[26];            // ctime_s needs a buffer of size 26
        ctime_s(dt, sizeof(dt), &now);
        hs << playerName << endl;
        hs << score << endl;
        hs << dt;        // saves readable date/time
        hs << "---" << endl;
        hs.close();
    }

}

//Review after completing Quiz
void reviewing(int score)
{
    // Review incorrect questions
    cout << "\n\n\n\t\t\t\tWould you like to review incorrect questions? (Y/N): ";
    char reviewChoice;
    cin >> reviewChoice;

    if (toupper(reviewChoice) == 'Y')
    {
        bool incorrect = false;
        system("cls");
        cout << "\n\n\t\t\t\t\t===================================\n";
        cout << "\t\t\t\t\t REVIEWING INCORRECT QUESTIONS\n";
        cout << "\t\t\t\t\t===================================\n\n";
        for (int i = 0; i < noofQuestions; i++)
        {
            if (reviewCorrectOption[i] != reviewUserOption[i])
            {
                cout << "\t\t\t\tQ" << i + 1 << ": " << reviewQuestions[i] << endl;
                cout << "\t\t\t\tCorrect: " << reviewCorrectOption[i] << endl;
                cout << "\t\t\t\tYour Answer: " << reviewUserOption[i] << endl << endl;
                incorrect = true;
            }
        }
        if (incorrect==false)
        {
            cout<<"\t\t\t\t\tThere is no incorrect questions!\n";
        }
        cout << "\t\t\t\t\t-----------------------------------\n\n";

        cout << "\t\t\t\t\tPress Enter to continue. ";
        cin.ignore();
        cin.get();
    }
    else
    {
        cin.ignore();
    }
}

//Showing Highscore
void showHighScores()
{
    //Creating arrays to read high scores
    string names[100];
    int scores[100];
    string dates[100];
    int count = 0;

    //Opening Highscore File
    ifstream hs("high_scores.txt");

    while (getline(hs, names[count]))
    {
        hs >> scores[count];
        hs.ignore();
        getline(hs, dates[count]);
        string dash;
        getline(hs, dash);
        count++;
    }
    hs.close();

    // Simple bubble sort: descending order
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
                swap(dates[j], dates[j + 1]);
            }
        }
    }

    //Displaying 
    system("cls");
    cout << "\n\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t             HIGH SCORE      \n";
    cout << "\t\t\t\t\t===================================\n\n";

    int limit = (count < 5 ? count : 5);

    for (int i = 0; i < limit; i++)
    {
        cout <<"\t\t\t\t" << i + 1 << ". " << names[i]
            << " - " << scores[i]
            << " (" << dates[i] << ")\n";
    }
    cout << "\n\t\t\t\t\t-----------------------------------\n\n";

    cout << "\t\t\t\t\tPress Enter to return to main menu";
    cin.ignore();
    cin.get();
}

// Returns true if user wants to replay, false if want to return to main menu
bool postGame()
{
    system("cls");
    cout << "\n\t\t\t\t\t===================================\n";
    cout << "\t\t\t\t\t        POST-GAME OPTIONS\n";
    cout << "\t\t\t\t\t===================================\n\n";
    cout << "\t\t\t\t\t1. Replay Quiz\n";
    cout << "\t\t\t\t\t2. Return to Main Menu\n";
    cout << "\n\t\t\t\t\t-----------------------------------\n\n";
    cout << "\t\t\t\t\tInput: ";

    int choice;
    cin >> choice;

    while (cin.fail() || (choice != 1 && choice != 2))
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "\t\t\t\t\tInvalid input. Re-enter: ";
        cin >> choice;
    }

    if (choice == 1)
    {
        cout << "\n\t\t\t\t\tStarting new quiz...\n";
        return true;  // Replay
    }
    else
    {
        cout << "\n\t\t\t\t\tReturning to main menu...\n";
        return false; // Main menu
    }
}
