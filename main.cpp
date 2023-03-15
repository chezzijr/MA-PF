#include "knight.h"

string trim(const string& s) {
    int i = 0, j = s.size() - 1;
    while (isspace(s[i])) i++;
    while (isspace(s[j])) j--;
    return s.substr(i, j - i + 1);
}

int main(int argc, char ** argv) {
    // string file_input(argv[1]);
    const char* testcases[] = {
        "tc1",
        "tc2",
        "tc3",
        "tc4",
        "tc5",
        "tc7",
        "tc12",
        "tc14",
        "tc17",
        "tc1077",
        "tc1088",
        "tc1117",
        "tc1122",
        "tc1124"
    };
    for (int i = 0; i < sizeof(testcases) / sizeof(char*); i++) {
        string file = testcases[i];
        stringstream output;
        auto *old = cout.rdbuf(output.rdbuf());

        int HP, level, remedy, maidenkiss, phoenixdown, rescue;
        adventureToKoopa(__tcpath__ + file, HP, level, remedy, maidenkiss, phoenixdown, rescue);
        cout.rdbuf(old);
        
        ifstream expectedFile(__tcpath__ + file + "_expected");
        stringstream expected;
        expected << expectedFile.rdbuf();

        string o = trim(output.str());
        string e = trim(expected.str());

        if (o.compare(e) == 0)
            clog << "[PASSED] " << file << endl;
        else {
            clog << "[FAILED] " << file << endl;
            clog << "\tExpected result:\n";
            clog << e << '\n';
            clog << "\tOutput:\n";
            clog << o << '\n';
        }
    }

    return 0;
}