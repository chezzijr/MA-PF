#include "knight.h"

#include <vector>
#include <algorithm>

#define LOG(x) cout << #x << ": " << x << endl;
#define LOGVEC(x) cout << #x << ": "; for (auto &i : (x)) cout << i << ' '; cout << endl;
#define endl '\n'
#ifndef __tcpath__
#define __tcpath__
#endif

int MaxHP;

// utilities functions
vector<int> splitStringToInt(string s, int alloc = 0, char delim = ' ') {
    vector<int> tokens;
    if (alloc) {
        tokens.reserve(alloc);
    }

    for (char& c : s) {
        if (c == delim && c != ' ') c = ' ';
    }

    istringstream ss(s);
    int x;
    while (ss >> x) {
        tokens.push_back(x);
    }

    return tokens;
}

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int nearestSmallerFib(int n) {
    int a = 1, b = 1;
    while (b < n) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

bool isMountain(const vector<int>& arr, int maxIndex) {
    for (int i = 0; i < maxIndex; i++) {
        if (arr[i] >= arr[i + 1]) return false;
    }

    for (int i = maxIndex + 1; i < arr.size(); i++) {
        if (arr[i] >= arr[i - 1]) return false;
    }

    return true;
}

void processDataMerlin(const string& fileName, int& HP) {
    ifstream f_m(fileName);
    int n9;
    f_m >> n9;
    f_m.ignore(32767, '\n');
    for (int i = 0; i < n9; i++) {
        string line;
        getline(f_m, line);

        if (min(line.find("Merlin"), line.find("merlin")) != string::npos)
            HP = min(HP + 3, MaxHP);
        else {
            bool hasChar[26] = {0};
            // lowercase
            for (int j = 0; j < line.size(); j++) {
                if ('A' <= line[j] && line[j] <= 'Z')
                    line[j] += 32;
                hasChar[line[j] - 'a'] = true;
            }

            if (hasChar[12] && hasChar[4] && hasChar[17] && hasChar[11] && hasChar[8] && hasChar[13])
                HP = min(HP + 2, MaxHP);
        }
    }
}

void processDataAsclepius(const string& fileName, int& remedy, int& maidenkiss, int& phoenixdown) {
    ifstream f_a(fileName);
    int r1, c1;
    f_a >> r1 >> c1;
    f_a.ignore(32767, '\n');
    for (int i = 0; i < r1; i++) {
        string line;
        getline(f_a, line);
        vector<int> potions = splitStringToInt(line, c1);
        int count = 0;
        for (const int& potion : potions) {
            if (potion == 16) {
                remedy = min(remedy + 1, 99);
                count++;
            }
            if (potion == 17) {
                maidenkiss = min(maidenkiss + 1, 99);
                count++;
            }
            if (potion == 18) {
                phoenixdown = min(phoenixdown + 1, 99);
                count++;
            }
            if (count == 3) break;
        }
    }
}

void initDataMG(const string& fileName, int data[5]) {
    ifstream f_mg(fileName);
    int n2;
    f_mg >> n2;
    f_mg.ignore(32767, '\n');
    string line;
    getline(f_mg, line);
    vector<int> arr = splitStringToInt(line, n2, ',');
    f_mg.close();

    // type 1
    int mini = min_element(arr.begin(), arr.end(), [](int a, int b) { return a <= b; }) - arr.begin();
    int maxi = max_element(arr.begin(), arr.end(), [](int a, int b) { return a <= b; }) - arr.begin();

    // type 2
    int mti = (isMountain(arr, maxi)) ? maxi : -3;
    int mtx = (mti < 0) ? -2 : arr[mti];

    // type 3
    for (int &elem : arr) {
        if (elem < 0)
            elem = -elem;
        elem = (17 * elem + 9) % 257;
    }

    int mini2 = min_element(arr.begin(), arr.end()) - arr.begin();
    int maxi2 = max_element(arr.begin(), arr.end()) - arr.begin();

    // type 4
    // array is not guaranteed to have 3 elements and above
    
    int max2_3i = -7; 
    int max2_3x = -5;

    if (arr.size() >= 3) {
        int largestIndex, secondIndex;

        if (arr[0] < arr[1]) { 
            largestIndex = 1;
            secondIndex = 0;
        } else { 
            largestIndex = 0;
            secondIndex = 1;
        }
        if (arr[2] > arr[largestIndex]) {
            secondIndex = largestIndex;
            largestIndex = 2;
        } else if (arr[2] > arr[secondIndex] && arr[2] != arr[largestIndex]) {
            secondIndex = 2;
        }

        max2_3i = (arr[largestIndex] == arr[secondIndex]) ? -7 : secondIndex;
        max2_3x = (max2_3i < 0) ? -5 : arr[secondIndex];
    }

    data[1] = mini + maxi;
    data[2] = mti + mtx;
    data[3] = mini2 + maxi2;
    data[4] = max2_3i + max2_3x;
}

// main source code
void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue) {
    cout << "HP=" << HP
        << ", level=" << level
        << ", remedy=" << remedy
        << ", maidenkiss=" << maidenkiss
        << ", phoenixdown=" << phoenixdown
        << ", rescue=" << rescue << endl;
}

void eventsHandling(
    const vector<int>& events, 
    int& HP, 
    int& level, 
    int& remedy, 
    int& maidenkiss, 
    int& phoenixdown, 
    int& rescue, 
    const string& f_mush_ghost, 
    const string& f_asclepius_pack, 
    const string& f_merlin_pack) 
{
    const bool isKingArthur = (MaxHP == 999);
    const bool isLancelot = isPrime(MaxHP);

    bool fainted = false;

    // Event 1 - 5
    const float baseDamage[] = {1, 1.5, 4.5, 7.5, 9.5};

    // Event 6
    bool isMini = false;
    int miniCountdown = 0;

    // Event 7
    bool isFrog = false;
    int frogCountdown = 0;
    int initialLevel = 0;

    // Event 13<ms>
    // First element to check if array is initialized
    int dataMushGhost[5] = {0, 0, 0, 0, 0};

    // Event 18
    bool metMerlin = false;

    // Event 19
    bool metAsclepius = false;

    for (int i = 0; i < events.size(); i++) {
        int ev = events[i], evi = i + 1;

        if (ev == 0) {
            rescue = 1;
            break;
        } else if (1 <= ev && ev <= 5) {
            int b = evi % 10;
            int levelO = (evi > 6) ? ( (b > 5) ? b : 5 ) : b;
            if (level > levelO || isLancelot || isKingArthur) level = min(level + 1, 10);
            else if (level < levelO) {
                int damage = (int)(baseDamage[ev - 1] * 10.0) * levelO;
                HP = HP - damage;
            }
        } else if (ev == 6 && !isMini) {
            int b = evi % 10;
            int levelO = (evi > 6) ? ( (b > 5) ? b : 5 ) : b;
            if (level > levelO || isLancelot || isKingArthur) level = min(level + 2, 10);
            else if (level < levelO) {
                if (remedy > 0)
                    remedy--;
                else {
                    HP = max(HP / 5, 1);
                    isMini = true;
                    miniCountdown = 4;
                }
            }
        } else if (ev == 7 && !isFrog) {
            int b = evi % 10;
            int levelO = (evi > 6) ? ( (b > 5) ? b : 5 ) : b;
            if (level > levelO || isLancelot || isKingArthur) level = min(level + 2, 10);
            else if (level < levelO) {
                if (maidenkiss > 0) maidenkiss--;
                else {
                    initialLevel = level;
                    level = 1;
                    isFrog = true;
                    frogCountdown = 4;
                }
            }
        } else if (ev == 11) {
            int n1 = ((level + phoenixdown) % 5 + 1) * 3;
            int s1 = (198 - (n1 - 1) * 2) * n1 / 2;
            HP = HP + (s1 % 100);
            int i = (HP % 2 == 1) ? ((HP < 2) ? 2 : HP + 2) : HP + 1;
            for (;; i += 2) {
                if (isPrime(i)) {
                    HP = i;
                    break;
                }
            }
            HP = min(HP, MaxHP);
        } else if (ev == 12) {
            if (HP > 1) HP = nearestSmallerFib(HP);
        } else if (ev == 15) {
            if (isMini) {
                isMini = false;
                HP = min(HP * 5, MaxHP);
            }
            else
                remedy = min(remedy + 1, 99);
        } else if (ev == 16) {
            if (isFrog) {
                isFrog = false;
                level = initialLevel;
            }
            else
                maidenkiss = min(maidenkiss + 1, 99);
        } else if (ev == 17) {
            phoenixdown = min(phoenixdown + 1, 99);
        } else if (ev == 18 && !metMerlin) {
            metMerlin = true;
            processDataMerlin(f_merlin_pack, HP);
        } else if (ev == 19 && !metAsclepius) {
            metAsclepius = true;
            processDataAsclepius(f_asclepius_pack, remedy, maidenkiss, phoenixdown);

            if (isMini && remedy > 0) {
                isMini = false;
                remedy--;
                HP = min(HP * 5, MaxHP);
            }

            if (isFrog && maidenkiss > 0) {
                isFrog = false;
                maidenkiss--;
                level = initialLevel;
            }

        } else if (ev == 99) {
            if (((isKingArthur || isLancelot) && level >= 8)
                || level == 10) {
                level = 10;
            } else {
                fainted = true;
                break;
            }

        } else if (ev > 100) { // event 13<ms>
            if (!dataMushGhost[0]) {
                dataMushGhost[0] = 1;
                initDataMG(f_mush_ghost, dataMushGhost);
            }    

            string evString = to_string(ev);
            for (int i = 2; i < evString.size(); i++) {
                int type = evString[i] - '0';
                HP = min(HP - dataMushGhost[type], MaxHP);

                if (HP <= 0) {
                    if (phoenixdown > 0) {
                        HP = MaxHP;
                        phoenixdown--;
                        isMini = false;
                        isFrog = false;
                    } else {
                        break;
                    }
                }

            }
        }

        if (HP <= 0) {
            if (phoenixdown > 0) {
                HP = MaxHP;
                phoenixdown--;
                isMini = false;
                isFrog = false;
            } else {
                fainted = true;
                break;
            }
        }

        if (isMini && (--miniCountdown) == 0) {
            isMini = false;
            HP = min(HP * 5, MaxHP);
        }

        if (isFrog && (--frogCountdown) == 0) {
            isFrog = false;
            level = initialLevel;
        }
        if (evi < events.size()) 
            display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
    }
    if (rescue == -1 && !fainted) rescue = 1;
    if (fainted) rescue = 0;
    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
}

void adventureToKoopa(string file_input, int& HP, int& level, int& remedy, int& maidenkiss, int& phoenixdown, int& rescue) {
    ifstream f_handler(file_input);
    string line;

    // 1st line
    f_handler >> MaxHP >> level >> remedy >> maidenkiss >> phoenixdown;
    HP = MaxHP;
    rescue = -1;
    // display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
    f_handler.ignore(32767, '\n');

    // 2nd line
    getline(f_handler, line);
    vector<int> events = splitStringToInt(line);
    
    // 3rd line
    string tcpath = __tcpath__;
    getline(f_handler, line);
    string f_mush_ghost, f_asclepius_pack, f_merlin_pack;
    int index = 0, begin = 0, end;
    end = line.find(',');
    f_mush_ghost = tcpath + line.substr(begin, end - begin);
    begin = end + 1;
    end = line.find(',', begin);
    f_asclepius_pack = tcpath + line.substr(begin, end - begin);
    begin = end + 1;
    f_merlin_pack = tcpath + line.substr(begin);

    f_handler.close();

    eventsHandling(events, HP, level, remedy, maidenkiss, phoenixdown, rescue, f_mush_ghost, f_asclepius_pack, f_merlin_pack);
}