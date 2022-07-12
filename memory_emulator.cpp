// memory_emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
using namespace std;



class Command {
public:
    int id;
    string type;
    int priority;
    int data_length;
    const void* data_address;
    vector<string> data;
    bool failed;
    Command();
    Command(int id, string type, int priority, int data_length,
        const void* data_address, vector<string> data);
       

    
    ~Command();
};
Command::Command() {
    failed = false;
}

Command::Command(int id, string type, int priority, int data_length,
    const void* data_address, vector<string> data) {
    id = id;
    type = type;
    priority = priority;
    data_length = data_length;
    data_address = data_address;
    data = data;
    failed = false;
}

Command :: ~Command() {
   // cout << "Command destructor";
}
struct Comparepriority {
    bool operator()(Command const& c1, Command const& c2)
    {
        
        return c1.priority < c2.priority;
    }
};

class Mem {

public:
    priority_queue<Command, vector<Command>, Comparepriority> pq;
    
    Mem();
    ~Mem();

    void Add(Command c);
    void Remove(int id);
    void Execute();
    void Abort();
    void* memory_read(const void* lba, int size);
    void memory_write(const void* lba, string data[]);
    
};

Mem::Mem() {
   // cout << "Mem constructor";
}

Mem::~Mem() {
   // cout << "Mem destructor";
}
void Mem::Add(Command c) {
    pq.push(c);
}

void Mem::Remove(int id) {
    priority_queue<Command, vector<Command>, Comparepriority> ref;
    int s = pq.size();
    int cnt = 0;

    // Finding the command to be removed
    while (pq.top().id != id and !pq.empty()) {
        ref.push(pq.top());
        pq.pop();
        cnt++;
    }

    // If command  not found
    if (pq.empty()) {
        cout << "element not found!!" << endl;
        while (!ref.empty()) {

            // Pushing all the elements back into q
            pq.push(ref.top());
            ref.pop();
        }
    }

    // If command is found
    else {
        pq.pop();
        while (!ref.empty()) {

            // Pushing all the commands back into q
            pq.push(ref.top());
            ref.pop();
        }
        int k = s - cnt - 1;
        while (k--) {

            // Pushing commands from front of q to its back
            Command p = pq.top();
            pq.pop();
            pq.push(p);
        }
    }
}


void Mem::Execute() {
    while (!pq.empty()) {
        if (!pq.top().failed) {
            cout << pq.top().id << endl;
            //execute the command
        }
        pq.pop();
    }
}

void* Mem :: memory_read(const void* lba, int size) {
    void* buffer;
    buffer = malloc(size);
    memcpy(buffer, lba, size);
    return buffer;
}

void Mem::memory_write(const void* lba, string data[]) {
    try {
        memcpy((void*)lba, (void*)data, sizeof(data));
    }
    catch (exception e) {
        cout << "couldn't write to address "<< lba;

    }
}

int main()
{
    int max_priority = 0;
    //const void* lba = "0x2428";
    const void* lba = "0x8000000";
    Mem mem;
    Command c1;
    c1.type = "read";
    c1.priority = 1;
    c1.data_address = lba;
    Command c2;
    c2.type = "write";
    c2.priority = 0;
    c1.data_address = lba;
    Command c3;
    c3.type = "delete";
    c3.priority = 1;
    
    if (c1.priority == 1) {
        max_priority++;
        c1.priority = max_priority;
    }
    mem.pq.push(c1);
    
    if (c2.priority == 1) {
        max_priority++;
        c2.priority = max_priority;
    }
    mem.pq.push(c2);
    if (c3.priority == 1) {
        max_priority++;
        c3.priority = max_priority;
    }
    mem.pq.push(c3);

    while (!mem.pq.empty()) {
        Command comm = mem.pq.top();
        cout << comm.type << "\n";
        if (comm.type == "read") {
            void* b = mem.memory_read(comm.data_address, 512);
            cout << "b = " << b << "\n";
        }
        else if (comm.type == "write") {
            string data[] = { "a" };
            mem.memory_write(lba, data);
            void* b2 = mem.memory_read(lba, 512);
            cout << "b2 = " << b2 << "\n";

        }
        mem.pq.pop();
    }
    
    
    string data[] = { "data"};
    mem.memory_write(lba, data);
    void* b2 = mem.memory_read(lba, 512);
    cout << "b2 = " << b2 << "\n";
}
