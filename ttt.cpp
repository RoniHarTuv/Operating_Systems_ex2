#include <iostream>
#include <string>
using namespace std;

void check_argv(int programdec[9])
{
    bool exp[9] = {0};
    for (size_t i = 0; i < 9; i++)
    {
        exp[programdec[i] - 1] = true;
    }
    for (size_t i = 0; i < 9; i++)
    {
        if (!exp[i])
        {
            cout << "Error!" << endl;
            fflush(stdout);
            exit(1);
        }
    }
}

bool check_win(char matrix[3][3])
{
    char a = matrix[0][0];
    char b = matrix[0][1];
    char c = matrix[0][2];
    char d = matrix[1][0];
    char e = matrix[1][1];
    char f = matrix[1][2];
    char g = matrix[2][0];
    char h = matrix[2][1];
    char i = matrix[2][2];
    if (a == b && b == c && a != 'n')
    {
        return true;
    }
    if (a == d && d == g && a != 'n')
    {
        return true;
    }
    if (a == e && e == i && a != 'n')
    {
        return true;
    }
    if (d == e && e == f && d != 'n')
    {
        return true;
    }
    if (b == e && e == h && b != 'n')
    {
        return true;
    }
    if (c == e && e == g && c != 'n')
    {
        return true;
    }
    if (g == h && h == i && g != 'n')
    {
        return true;
    }
    if (c == f && f == i && c != 'n')
    {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    char matrix[3][3] = {{'n', 'n', 'n'}, {'n', 'n', 'n'}, {'n', 'n', 'n'}};
    int programdec[9];
    int number_of_turn = 0;
    if (!argv[1])
    {
        cout << "Error!" << endl;
        fflush(stdout);
        exit(1);
    }
    string s = argv[1];
    if (s.size() != 9)
    {
        cout << "Error!" << endl;
        fflush(stdout);
        exit(1);
    }
    for (size_t i = 0; i < 9; i++)
    {
        programdec[i] = s[i] - '0';
    }
    check_argv(programdec);
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            int p_row = (programdec[j] - 1) / 3;
            int p_col = (programdec[j] - 1) % 3;
            if (matrix[p_row][p_col] == 'n')
            {
                matrix[p_row][p_col] = 'X';
                cout << "X was added in [" << p_row << "][" << p_col << "]" << endl;
                fflush(stdout);
                number_of_turn++;
                if (number_of_turn == 9)
                {
                    cout << "I lost" << endl;
                    fflush(stdout);
                    exit(1);
                }
                //  print_matrix(matrix);
                if (check_win(matrix))
                {
                    cout << "I win" << endl;
                    fflush(stdout);
                    exit(1);
                }
                break;
            }
        }
        int input;
        //   cout << "enter a number between 1 and 9 that is empty in the board";
        cin >> input;
        int p_row = (input - 1) / 3;
        int p_col = (input - 1) % 3;
        if (matrix[p_row][p_col] != 'n')
        {
            cout<<"Eroor! unvalid cell";
            fflush(stdout);
            exit(1);
        }
        int u_row = (input - 1) / 3;
        int u_col = (input - 1) % 3;
        matrix[u_row][u_col] = 'O';
        cout << "O was added in [" << u_row << "][" << u_col << "]" << endl;
        fflush(stdout);
        number_of_turn++;
        
        if (check_win(matrix))
        {
            cout << "I lost" << endl;
            fflush(stdout);
            exit(1);
        }
    }
}
