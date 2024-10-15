#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>
#include <chrono>

using namespace std;

const int N = 5; // Number of processes
atomic<bool> choosing[N]; // Choosing array to indicate if a process is choosing a ticket
atomic<int> ticket[N];    // Ticket array to store the ticket numbers

void bakery_algorithm(int process_id) {
    // Step 1: Entering the bakery (choose a ticket)
    choosing[process_id] = true;
    
    // Assign ticket
    int max_ticket = 0;
    for (int i = 0; i < N; ++i) {
        max_ticket = max(max_ticket, ticket[i].load());
    }
    ticket[process_id] = max_ticket + 1;
    choosing[process_id] = false;

    // Step 2: Check other processes and wait if necessary
    for (int i = 0; i < N; ++i) {
        if (i == process_id) continue;

        // Wait while the other process is choosing a ticket
        while (choosing[i]) {}

        // Wait if the other process has a lower ticket or has the same ticket but lower process ID
        while (ticket[i] != 0 && (ticket[i] < ticket[process_id] || 
               (ticket[i] == ticket[process_id] && i < process_id))) {}
    }

    // Step 3: Critical section
    cout << "Process " << process_id << " is in the critical section.\n";
    this_thread::sleep_for(chrono::milliseconds(1000)); // Simulate critical section work

    // Step 4: Exit critical section and reset ticket
    cout << "Process " << process_id << " is leaving the critical section.\n";
    ticket[process_id] = 0;
}

int main() {
    vector<thread> processes;

    // Initialize the arrays
    for (int i = 0; i < N; ++i) {
        choosing[i] = false;
        ticket[i] = 0;
    }

    // Launch processes
    for (int i = 0; i < N; ++i) {
        processes.push_back(thread(bakery_algorithm, i));
    }

    // Join processes
    for (auto& t : processes) {
        t.join();
    }

    return 0;
}
