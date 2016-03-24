#include <bits/stdc++.h>

using namespace std;

const int INF = (int)1e9;
enum ColorType {WHITE, BLACK, NONE};
typedef vector<vector<ColorType>> BoardType;
const int maxDepth = 5;
ColorType color;
BoardType board(8, vector<ColorType>(8, NONE));
vector<vector<int>> boardCost;

void initBoard() {
    board['d' - 'a'][4 - 1] = WHITE;
    board['d' - 'a'][5 - 1] = BLACK;
    board['e' - 'a'][4 - 1] = BLACK;
    board['e' - 'a'][5 - 1] = WHITE;

    boardCost.resize(8);
    for (int i = 0; i < 8; ++i) {
        boardCost[i].resize(8);
    }
    boardCost[0][0] = 200;
    boardCost[0][1] = -15;
    boardCost[0][2] = 15;
    boardCost[0][3] = 12;
    boardCost[1][1] = -40;
    boardCost[1][2] = -8;
    boardCost[1][3] = -5;
    boardCost[2][2] = 15;
    boardCost[2][3] = 8;
    boardCost[3][3] = 5;

    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            boardCost[j][i] = boardCost[i][j];
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            boardCost[7 - i][j] = boardCost[i][j];
        }
    }
    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 8; ++i) {
            boardCost[i][7 - j] = boardCost[i][j];
        }
    }
}

ColorType notColor(ColorType a) {
    if (a == WHITE) {
        return BLACK;
    } else {
        return  WHITE;
    }
}

bool onBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

int getMark() {
    int ans = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == color) {
                ans += boardCost[i][j];
            }
            if (board[i][j] == notColor(color)) {
                ans -= boardCost[i][j];
            }
        }
    }

    bool corner = true;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (board[i][j] != board[0][0]) {
                corner = false;
            }
        }
    }
    if (corner) {
        if (board[0][0] == color) {
            ans += boardCost[0][0] * 3;
        }
        if (board[0][0] == notColor(color)) {
            ans -= boardCost[0][0] * 3;
        }
    }
    corner = true;
    for (int i = 0; i < 2; ++i) {
        for (int j = 6; j < 8; ++j) {
            if (board[i][j] != board[0][7]) {
                corner = false;
            }
        }
    }
    if (corner) {
        if (board[0][7] == color) {
            ans += boardCost[0][7] * 3;
        }
        if (board[0][7] == notColor(color)) {
            ans -= boardCost[0][7] * 3;
        }
    }
    corner = true;
    for (int i = 6; i < 8; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (board[i][j] != board[7][0]) {
                corner = false;
            }
        }
    }
    if (corner) {
        if (board[7][0] == color) {
            ans += boardCost[7][0] * 3;
        }
        if (board[7][0] == notColor(color)) {
            ans -= boardCost[7][0] * 3;
        }
    }
    corner = true;
    for (int i = 6; i < 8; ++i) {
        for (int j = 6; j < 8; ++j) {
            if (board[i][j] != board[7][7]) {
                corner = false;
            }
        }
    }
    if (corner) {
        if (board[7][7] == color) {
            ans += boardCost[7][7] * 3;
        }
        if (board[7][7] == notColor(color)) {
            ans -= boardCost[7][7] * 3;
        }
    }
    return ans;
}

bool putOnBoard(int x, int y, ColorType color) {
    bool answer = false;
    int i, j, i1, j1;
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) {
                continue;
            }
            i = x + di;
            j = y + dj;
            while (onBoard(i, j)) {
                if (board[i][j] != notColor(color)) {
                    if (board[i][j] == NONE) {
                        i -= 100;
                        j -= 100;
                    }
                    break;
                }
                i += di;
                j += dj;
            }
            if (onBoard(i, j)) {
                i1 = x + di;
                j1 = y + dj;
                while (i1 != i || j1 != j) {
                    board[i1][j1] = color;
                    answer = true;
                    i1 += di;
                    j1 += dj;
                }
            }
        }
    }
    if (answer) {
        board[x][y] = color;
    }
    return answer;
}

int getMove(ColorType currentMove, int depth, int &x, int &y) {
    if (depth == maxDepth) {
        return getMark();
    }
    int ans;
    if (currentMove == color) {
        ans = -INF + depth;
    } else {
        ans = INF - depth;
    }
    int a, b;
    BoardType boardSave = board;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != NONE) {
                continue;
            }
            if (putOnBoard(i, j, currentMove)) {
                int cur = getMove(notColor(currentMove), depth + 1, a, b);
                if (currentMove == color && cur > ans) {
                    ans = cur;
                    x = i;
                    y = j;
                }
                if (currentMove != color && cur < ans) {
                    ans = cur;
                }
                board = boardSave;
            }
        }
    }
    if (ans == -INF + depth || ans == INF - depth) {
        return getMove(notColor(currentMove), depth + 1, a, b);
    }
    return ans;
}

int main() {
	ios_base::sync_with_stdio(false);
	initBoard();
	string initString;
	getline(cin, initString);
	if (initString == "init white") {
        color = WHITE;
	} else {
        color = BLACK;
	}

	string command;
	int x, y;
	while (true) {
		getline(cin, command);
		if (command == "win" || command == "lose" || command == "draw" || command == "bad") {
			exit(0);
		}
		if (command == "turn") {
			getMove(color, 0, x, y);
			cout << "move " << char(x + 'a') << " " << y + 1 << endl;
			putOnBoard(x, y, color);
			continue;
		}
		x = command[5] - 'a';
		y = command[7] - '1';
		putOnBoard(x, y, notColor(color));
	}
}