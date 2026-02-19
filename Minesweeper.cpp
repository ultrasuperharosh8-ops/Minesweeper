#include <iostream>
#include <cstdlib>
using namespace std;        //in percents
const int FIELD_SIZE = 8, CHANCE_OF_MINE = 16;
class game {
private:
    void get_seed(int i, int j) {
        srand(i * j + i + j);
    }
    
    bool chance_randomiser() {
        int a = rand() % 101;
        if (a <= CHANCE_OF_MINE) {
            return true;
        }
        else {
            return false;
        }
    }
    void randomise_mines() {
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                if (field[i][j] != '-') {
                    is_there_mine[i][j] = chance_randomiser();
                }
            }
        }
    }
    char scan_mines(int row, int col, int radius) {
        int counter = 0;
        for (int i = row - radius; i <= row + radius; i++) {
            if (i < 0 or i >= FIELD_SIZE) {
                continue;
            }
            for (int j = col - radius; j <= col + radius; j++) {
                if (j < 0 or j >= FIELD_SIZE) {
                    continue;
                }
                if (is_there_mine[i][j] == true) {
                    counter++;
                }
            }
        }
        if (counter == 0) { return '-'; }
        return counter + '0';
    }
    void skip_empty_cells(int row, int col) {
        if ((field[row][col] == '?' or field[row][col] == 'f') && is_there_mine[row][col] == false) {
            field[row][col] = scan_mines(row, col, 1);
            if (field[row][col] == '-') {
                for (int i = row - 1; i <= row + 1; i++) {
                    if (i < 0 or i >= FIELD_SIZE) {
                        continue;
                    }
                    for (int j = col - 1; j <= col + 1; j++) {
                        if (j < 0 or j >= FIELD_SIZE) {
                            continue;
                        }
                        skip_empty_cells(i, j);
                    }
                }
            }
        }
    }
    bool mine_peek(int i, int j) {
        if (is_there_mine[i][j] == true) {
            field[i][j] = '*';
            return true;
        }
        else {
            field[i][j] = scan_mines(i, j, 1);
            return false;
        }
    }
    void finish(char status[8]) {
        cout << status << endl;
        cout << "Game over!" << endl;
        system("pause>nul");
        exit(0);
    }
    int are_there_unopened_clear_cells_left() {
        int counter_unopened_clears = 0;
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                if (field[i][j] == '?' && is_there_mine[i][j] == false) {
                    counter_unopened_clears++;
                }
            }
        }
        return counter_unopened_clears;
    }
    void put_a_flag(int inp_i, int inp_j) {
        if (inp_i >= FIELD_SIZE or inp_i < -1 or inp_j >= FIELD_SIZE or inp_j < 0) {
            cout << "Invalid input!" << endl;
        }
        field[inp_i][inp_j] = 'f';
    }
public:
    char field[FIELD_SIZE][FIELD_SIZE]; 
    bool is_there_mine[FIELD_SIZE][FIELD_SIZE];
    void init() {
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                field[i][j] = '?';
                is_there_mine[i][j] = false;
            }
        }
        //getting random seed by player first move
        int i = input_i() - 1, j = input_j() - 1;
        field[i][j] = '-';
        get_seed(i, j);
        randomise_mines();
        field[i][j] = '?';
        make_turn(i, j);
    }
    int input_i() {
        int i;
        cin >> i;
        return i;
    }
    int input_j() {
        int j;
        cin >> j;
        return j;
    }
    void make_turn(int inp_i, int inp_j) {
        if (inp_i == -1) {
            cout << "Input flag row and column " << endl;
            int i = input_i() - 1, j = input_j() - 1;
            put_a_flag(i, j);
            for (int i = 0; i < FIELD_SIZE; i++) {
                cout << i + 1 << "     ";
                for (int j = 0; j < FIELD_SIZE; j++) {
                    cout << field[i][j] << " ";
                }
                cout << endl;
            }
        }
        else if (inp_i >= FIELD_SIZE or inp_i < -1 or inp_j >= FIELD_SIZE or inp_j < -1) {
            cout << "Invalid input!" << endl;
        }
        else {
            if (scan_mines(inp_i, inp_j, 1) == '-') { //skip empty cells
                skip_empty_cells(inp_i, inp_j);
            }
            bool turn = mine_peek(inp_i, inp_j);
            for (int i = 0; i < FIELD_SIZE; i++) {
                cout << i + 1 << "     ";
                for (int j = 0; j < FIELD_SIZE; j++) {
                    cout << field[i][j] << " ";
                }
                cout << endl;
            }
            if (turn == true) {
                char defeat[7] = "Defeat";
                finish(defeat);
            }
            int mines_left = are_there_unopened_clear_cells_left();
            if (mines_left == 0) {
                char victory[8] = "Victory";
                finish(victory);
            }
        }
    }
    
    void showall() {
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                make_turn(i, j);
                cout << "\n\n\n\n\n\n\n\n";
            }
            cout << endl;
        }
    }
};
int main()
{
    game my_game;
    cout << "Input row and column " << endl;
    my_game.init();
    while (true) {
        cout << "Input row and column (put 0 to place a flag) " << endl;
        int i = my_game.input_i() - 1;
        int j = my_game.input_j() - 1;
        my_game.make_turn(i, j);
    }
    system("pause>nul");
}
