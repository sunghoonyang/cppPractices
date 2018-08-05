#include <iostream> // for cin, cout, cerr
#include <boost/lexical_cast.hpp>
#include <list>

using namespace std;

class list;

char square[10] = {'o', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int checkwin(int choice);

void board();

int getAbove(int choice);

int getBelow(int choice);

int getLeft(int choice);

int getRight(int choice);

int getRightDiagonalUp(int choice); // 2 o'clock

int getRightDiagonalDown(int choice); // 5 o'clock

int getLeftDiagonalUp(int choice); // 10 o'clock

int getLeftDiagonalDown(int choice); // 8 o'clock

list<int> vertical_discovered; // must not forget to reset this after the search
list<int> horizontal_discovered; // must not forget to reset this after the search
list<int> left_diagonal_discovered; // must not forget to reset this after the search
list<int> right_diagonal_discovered; // must not forget to reset this after the search

int main() {
    int player = 1, i, choice;  //  i and choice are both initialized to the int value of 0
    do {
        board();
        player = (player % 2) ? 1 : 2;

        cout << "Player " << player << ", enter a number:  ";
        cin >> choice;
        if (choice < 0 || choice > 9) {
            cout << "invalid input" << endl;
            continue;
        }
        if (square[choice] == 'X' || square[choice] == 'O') {
            cout << "you cannot choose a tile that is already selected" << endl;
            continue;
        }
        square[choice] = (player == 1) ? 'X' : 'O';
        player++;
        i = checkwin(choice);
        vertical_discovered.clear();
        horizontal_discovered.clear();
        left_diagonal_discovered.clear();
        right_diagonal_discovered.clear();
    } while (i == -1);
    board();
    if (i == 1)
        cout << "==>\aPlayer " << --player << " win ";
    else
        cout << "==>\aGame draw";

    cin.ignore();
    cin.get();
    return 0;
}

/*********************************************
    DFS to determine if the latest choice
    has won the game for the player
	FUNCTION TO RETURN GAME STATUS
	1 FOR GAME IS OVER WITH RESULT
	-1 FOR GAME IS IN PROGRESS
	O GAME IS OVER AND NO RESULT
**********************************************/
int getAbove(int choice) {
    int theoreticalAbove = choice - 3;
    if (theoreticalAbove <= 0) {
        return -1;
    } else {
        return theoreticalAbove;
    }
}

int getBelow(int choice) {
    int theoreticalBelow = choice + 3;
    if (theoreticalBelow >= 10) {
        return -1;
    } else {
        return theoreticalBelow;
    }
}

int getLeft(int choice) {
    int leftest[3] = {1, 4, 7};
    bool is_leftest = std::find(std::begin(leftest), std::end(leftest), choice) != std::end(leftest);
    if (is_leftest) { return -1; }
    return choice - 1;
}

int getRight(int choice) {
    int rightest[3] = {3, 6, 9};
    bool is_rightest = std::find(std::begin(rightest), std::end(rightest), choice) != std::end(rightest);
    if (is_rightest) { return -1; }
    return choice + 1;
}

int getRightDiagonalUp(int choice) { // 2 o'clock
    int boundaries[5] = {1, 2, 3, 6, 9};
    bool bound = std::find(std::begin(boundaries), std::end(boundaries), choice) != std::end(boundaries);
    if (bound) { return -1; }
    return choice - 2;
}

int getLeftDiagonalUp(int choice) { // 10 o'clock
    int boundaries[5] = {1, 2, 3, 4, 7};
    bool bound = std::find(std::begin(boundaries), std::end(boundaries), choice) != std::end(boundaries);
    if (bound) { return -1; }
    return choice - 4;
}

int getLeftDiagonalDown(int choice) { // 5 o'clock
    int boundaries[5] = {3, 6, 7, 8, 9};
    bool bound = std::find(std::begin(boundaries), std::end(boundaries), choice) != std::end(boundaries);
    if (bound) { return -1; }
    return choice + 4;
}

int getRightDiagonalDown(int choice) { // 8 o'clock
    int boundaries[5] = {1, 4, 7, 8, 9};
    bool bound = std::find(std::begin(boundaries), std::end(boundaries), choice) != std::end(boundaries);
    if (bound) { return -1; }
    return choice + 2;
}

int getVerticalSet(int choice, char mark) {
    if (choice < 0 || choice > 9) {
        // reset vertical_discovered list for the next search
        return 0;
    } else if (std::find(std::begin(vertical_discovered), std::end(vertical_discovered), choice) !=
               std::end(vertical_discovered)) { // if this choice already discovered, return 0
        return 0;
    }
    vertical_discovered.push_back(choice);
    if (square[choice] != mark) {
        return getVerticalSet(getBelow(choice), mark) +
               getVerticalSet(getAbove(choice),
                              mark); // unnecessary because it will not find 3 in a row - might as well return 0 but this works, too.
    } else {
        return 1 + getVerticalSet(getBelow(choice), mark) +
               getVerticalSet(getAbove(choice), mark);
    }
}

int getLeftDiagonalSet(int choice, char mark) {
    if (choice < 0 || choice > 9) {
        // reset vertical_discovered list for the next search
        return 0;
    } else if (std::find(std::begin(left_diagonal_discovered), std::end(left_diagonal_discovered), choice) !=
               std::end(left_diagonal_discovered)) { // if this choice already discovered, return 0
        return 0;
    }
    left_diagonal_discovered.push_back(choice);
    if (square[choice] != mark) {
        return getLeftDiagonalSet(getLeftDiagonalDown(choice), mark) +
               getLeftDiagonalSet(getLeftDiagonalUp(choice),
                                  mark); // unnecessary because it will not find 3 in a row - might as well return 0 but this works, too.
    } else {
        return 1 + getLeftDiagonalSet(getLeftDiagonalDown(choice), mark) +
               getLeftDiagonalSet(getLeftDiagonalUp(choice), mark);
    }
}

int getRightDiagonalSet(int choice, char mark) {
    if (choice < 0 || choice > 9) {
        // reset vertical_discovered list for the next search
        return 0;
    } else if (std::find(std::begin(right_diagonal_discovered), std::end(right_diagonal_discovered), choice) !=
               std::end(right_diagonal_discovered)) { // if this choice already discovered, return 0
        return 0;
    }
    right_diagonal_discovered.push_back(choice);
    if (square[choice] != mark) {
        return getRightDiagonalSet(getRightDiagonalDown(choice), mark) +
                getRightDiagonalSet(getRightDiagonalUp(choice),
                                  mark); // unnecessary because it will not find 3 in a row - might as well return 0 but this works, too.
    } else {
        return 1 + getRightDiagonalSet(getRightDiagonalDown(choice), mark) +
               getRightDiagonalSet(getRightDiagonalUp(choice), mark);
    }
}

int getHorizontalSet(int choice, char mark) {
    if (choice < 0 || choice > 9) {
        // reset horizontal_discovered list for the next search
        return 0;
    } else if (std::find(std::begin(horizontal_discovered), std::end(horizontal_discovered), choice) !=
               std::end(horizontal_discovered)) { // if this choice already discovered, return 0
        return 0;
    }
    horizontal_discovered.push_back(choice);
    if (square[choice] != mark) {
        return getHorizontalSet(getLeft(choice), mark) +
               getHorizontalSet(getRight(choice),
                                mark); // unnecessary because it will not find 3 in a row - might as well return 0 but this works, too.
    } else {
        return 1 + getHorizontalSet(getLeft(choice), mark) +
               getHorizontalSet(getRight(choice), mark);
    }
}

int checkwin(int choice) {
    char player_mark = square[choice];
    char any_exists_arr[10] = {'x', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int i = 0;
    bool exists = false;
    while (i < sizeof(any_exists_arr)) {
        if (square[i] == any_exists_arr[i]) {
            exists = true;
            break;
        } else {
            i++;
        }
    }
    int hor_pt = getHorizontalSet(choice, player_mark);
    int ver_pt = getVerticalSet(choice, player_mark);
    int ld_pt = getLeftDiagonalSet(choice, player_mark);
    int rd_pt = getRightDiagonalSet(choice, player_mark);
    if (hor_pt == 3 || ver_pt == 3 || ld_pt == 3 || rd_pt == 3) {
        return 1;
    } else if (exists) {
        return -1;
    } else {
        return 0;
    }

}


/*******************************************************************
     FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK
********************************************************************/


void board() {
    cout << "\n\n\tTic Tac Toe\n\n";

    cout << "Player 1 (X)  -  Player 2 (O)" << endl << endl;
    cout << endl;

    cout << "     |     |     " << endl;
    cout << "  " << square[1] << "  |  " << square[2] << "  |  " << square[3] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[4] << "  |  " << square[5] << "  |  " << square[6] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << square[7] << "  |  " << square[8] << "  |  " << square[9] << endl;

    cout << "     |     |     " << endl << endl;
}

/*******************************************************************
				END OF PROJECT
********************************************************************/