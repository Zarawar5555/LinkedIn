#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h> // for sleep()
using namespace std;

struct User {
    string username;
    string password;
};

struct File {
    string name;
    string content;
    string owner;
};

struct Directory {
    string name;
    map<string, File> files;
    map<string, Directory> subdirs;
};

struct Process {
    string name;
    int pid;
    int memory;
    string status;
};

class OS {
    map<string, User> users;
    string currentUser;
    Directory root;
    vector<string> currentPath;
    map<int, Process> processes;
    int nextPid = 1;
    int totalMemory = 1024;
    int usedMemory = 0;

    void printBanner() {
        cout << "\033[1;34m=====================================\n";
        cout << "        Welcome to MiniSimOS\n";
        cout << "=====================================\033[0m\n";
    }

    void loadingAnimation() {
        system("clear");
        cout << "\033[1;34mStarting MiniSimOS...\033[0m\n";
        sleep(1);
        cout << "Loading modules...\n";
        sleep(1);
        cout << "Initializing system...\n";
        sleep(1);
        system("clear");
    }

    string getCurrentPath() {
        stringstream ss;
        ss << "/";
        for (string& p : currentPath) ss << p << "/";
        return ss.str();
    }

    Directory* getCurrentDirectory() {
        Directory* dir = &root;
        for (string& p : currentPath) dir = &dir->subdirs[p];
        return dir;
    }

    void prompt() {
        cout << "\033[1;32m[" << currentUser << "]\033[0m " << getCurrentPath() << "> ";
    }

    void saveUsers() {
        ofstream out("users.db");
        for (auto& [username, user] : users)
            out << username << " " << user.password << endl;
        out.close();
    }

    void loadUsers() {
        ifstream in("users.db");
        string u, p;
        while (in >> u >> p) users[u] = {u, p};
        in.close();
    }

    void registerUser() {
        string u, p;
        cout << "\033[1;33mEnter new username: \033[0m"; cin >> u;
        if (users.count(u)) { cout << "\033[1;31mUser exists.\033[0m\n"; return; }
        cout << "\033[1;33mEnter password: \033[0m"; cin >> p;
        users[u] = {u, p};
        saveUsers();
        cout << "\033[1;32mRegistered successfully.\033[0m\n";
    }

    bool login() {
        string u, p;
        cout << "\033[1;33mUsername: \033[0m"; cin >> u;
        cout << "\033[1;33mPassword: \033[0m"; cin >> p;
        if (users.count(u) && users[u].password == p) {
            currentUser = u;
            cout << "\033[1;32mLogin successful.\033[0m\n";
            return true;
        }
        cout << "\033[1;31mLogin failed.\033[0m\n";
        return false;
    }

    void mkdir(string name) {
        getCurrentDirectory()->subdirs[name].name = name;
        cout << "\033[1;32mDirectory created: " << name << "\033[0m\n";
    }

    void touch(string name) {
        getCurrentDirectory()->files[name] = {name, "", currentUser};
        cout << "\033[1;32mFile created: " << name << "\033[0m\n";
    }

    void write(string name, string content) {
        getCurrentDirectory()->files[name].content = content;
        cout << "\033[1;32mFile written: " << name << "\033[0m\n";
    }

    void cat(string name) {
        auto& f = getCurrentDirectory()->files[name];
        cout << "\033[1;36m" << f.content << "\033[0m\n";
    }

    void ls() {
        cout << "\033[1;33mListing contents of " << getCurrentPath() << ":\033[0m\n";
        auto* dir = getCurrentDirectory();
        for (auto& [n, d] : dir->subdirs) cout << "[DIR] " << n << endl;
        for (auto& [n, f] : dir->files) cout << "[FILE] " << n << endl;
    }

    void cd(string name) {
        if (getCurrentDirectory()->subdirs.count(name))
            currentPath.push_back(name);
    }

    void back() {
        if (!currentPath.empty()) currentPath.pop_back();
    }

    void pwd() {
        cout << getCurrentPath() << endl;
    }

    void runProcess(string name, int mem) {
        if (usedMemory + mem > totalMemory) {
            cout << "\033[1;31mNot enough memory.\033[0m\n"; return;
        }
        processes[nextPid] = {name, nextPid, mem, "running"};
        usedMemory += mem;
        cout << "\033[1;32mProcess " << name << " started with PID " << nextPid << ".\033[0m\n";
        nextPid++;
    }

    void killProcess(int pid) {
        if (processes.count(pid)) {
            usedMemory -= processes[pid].memory;
            processes.erase(pid);
            cout << "\033[1;32mProcess killed.\033[0m\n";
        } else cout << "\033[1;31mInvalid PID.\033[0m\n";
    }

    void ps() {
        cout << "\033[1;33mPID\tName\tMemory\tStatus\033[0m\n";
        for (auto& [pid, p] : processes)
            cout << setw(5) << pid << setw(15) << p.name << setw(10) << p.memory << setw(10) << p.status << endl;
    }

    void mem() {
        cout << "\033[1;33mTotal: \033[1;34m" << totalMemory << " KB\033[0m\n";
        cout << "\033[1;33mUsed: \033[1;34m" << usedMemory << " KB\033[0m\n";
        cout << "\033[1;33mFree: \033[1;34m" << totalMemory - usedMemory << " KB\033[0m\n";
    }

    void help() {
        cout << "\033[1;32mAvailable Commands:\033[0m\n"
             << "mkdir <name>        - Create a new directory\n"
             << "touch <name>        - Create a new file\n"
             << "write <file> <text> - Write text to a file\n"
             << "cat <file>          - Show file content\n"
             << "ls                  - List contents\n"
             << "cd <dir>            - Change directory\n"
             << "back                - Go back to parent directory\n"
             << "pwd                 - Print current directory path\n"
             << "run <name> <mem>    - Run a process\n"
             << "kill <pid>          - Kill a process\n"
             << "ps                  - Show all running processes\n"
             << "mem                 - Show memory usage\n"
             << "logout              - Log out\n"
             << "exit                - Exit MiniSimOS\n";
    }

    void smartInputFeedback(string cmd) {
        if (cmd == "Is") cout << "\033[1;31mDid you mean 'ls'?\033[0m Type 'help'.\n";
        else if (cmd == "rn") cout << "\033[1;31mDid you mean 'run'?\033[0m Type 'help'.\n";
        else cout << "\033[1;31mUnknown command.\033[0m Type 'help'.\n";
    }

public:
    OS() { root.name = "/"; loadUsers(); }

    void start() {
        loadingAnimation();
        printBanner();
        cout << "1. Register\n2. Login\nChoice: ";
        int ch; cin >> ch;
        if (ch == 1) registerUser();
        if (!login()) return;

        cout << "\nWelcome " << currentUser << "!\nType 'help' to get started.\n";

        string cmd;
        while (true) {
            prompt();
            cin >> cmd;
            if (cmd == "mkdir") { string name; cin >> name; mkdir(name); }
            else if (cmd == "touch") { string name; cin >> name; touch(name); }
            else if (cmd == "write") { string f, c; cin >> f; getline(cin, c); write(f, c.substr(1)); }
            else if (cmd == "cat") { string name; cin >> name; cat(name); }
            else if (cmd == "ls") ls();
            else if (cmd == "cd") { string d; cin >> d; cd(d); }
            else if (cmd == "back") back();
            else if (cmd == "pwd") pwd();
            else if (cmd == "run") { string n; int m; cin >> n >> m; runProcess(n, m); }
            else if (cmd == "kill") { int pid; cin >> pid; killProcess(pid); }
            else if (cmd == "ps") ps();
            else if (cmd == "mem") mem();
            else if (cmd == "help") help();
            else if (cmd == "logout") { currentUser = ""; break; }
            else if (cmd == "exit") break;
            else smartInputFeedback(cmd);
        }
    }
};

int main() {
    OS os;
    os.start();
    return 0;
}
