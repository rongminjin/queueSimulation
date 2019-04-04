
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <queue>

using namespace std;

const int simulationTime = 43200; // 12 hrs * 60 min * 60 sec

struct Customer{
    
    int firingTime; // when they enqueue
    
    int duration; // number of seconds it will take to serve them
    
    int beginningToBeHelped; // time at which they begin to work with cashier
    
    Customer() { // default constructor to make compiler happy
        firingTime = -1;
        duration = -1;
        beginningToBeHelped = -1;
    }
    
    Customer(int f, int d) { // constructor
        firingTime = f;
        duration = d;
    }
    
};

struct Employee{
    
    Customer currentCustomer;
    
    bool available; // true if not helping anyone, false if busy
    
    int firingTime; // when they start helping the current customer
    
    int finishTime; // when they stop helping the customer
    
    Employee() {
        available = true;
    }
    
    void startServingCustomer(int currentTime, Customer &customer) {
        currentCustomer = customer;
        available = false;
        firingTime = currentTime;
        finishTime = currentTime + customer.duration;
        customer.beginningToBeHelped = currentTime;
    }
    
    void finishServingCustomer() {
        available = true;
    }
};

// overloading < so that Customers can be compared (required for priority_queue)
bool operator<(const Customer& lhs, const Customer& rhs) {
    return lhs.firingTime > rhs.firingTime;
}

// Helper function to calculate percentile
// Math source: https://web.stanford.edu/class/archive/anthsci/anthsci192/anthsci192.1064/handouts/calculating%20percentiles.pdf
float calculatePercentile(vector<float> data, int percentile) {
    sort(data.begin(), data.end());
    float i = ((data.size() * percentile) / 100.0) + 0.5;
    int k = (int) i;
    float f = i - k;
    return ((1 - f) * (data[k])) + (0.25 * data[k + 1]);
}

void simulateBank(vector<struct Employee> &allTellers, vector<struct Customer> &allCustomers) {
    priority_queue<Customer> bankQueue;
    vector<float> allWaitTimes;
    float numberOfCustomersServed = 0;
    float totalServiceTimeAllCustomers = 0;
    for (int currentTime = 0; currentTime < simulationTime; currentTime++) {
        for (int i = 0; i < allCustomers.size(); i++) {
            assert(allCustomers[i].firingTime != -1); // everyone should be assigned a time
            if (allCustomers[i].firingTime == currentTime) { // if they arrive now, enqueue
                bankQueue.push(allCustomers[i]);
            }
        }
        for (int i = 0; i < allTellers.size(); i++) {
            Customer customer;
            if (allTellers[i].available && !bankQueue.empty()) { // if someone is waiting in line and a teller is available, serve
                customer = bankQueue.top();
                bankQueue.pop(); // from queue so no other teller tries to help
                allTellers[i].startServingCustomer(currentTime, customer);
            }
            if (allTellers[i].finishTime == currentTime && !allTellers[i].available) { // if a teller finished serving this round
                allTellers[i].finishServingCustomer(); // stop serving them, reset availability
                allWaitTimes.push_back(currentTime - allTellers[i].currentCustomer.firingTime); // track total queue/service time
                totalServiceTimeAllCustomers += (currentTime - allTellers[i].currentCustomer.firingTime);
                numberOfCustomersServed++;
            }
        }
    }
    float tenthPercentile = (calculatePercentile(allWaitTimes, 10)) / 60;
    float fiftiethPercentile = (calculatePercentile(allWaitTimes, 50)) / 60;
    float ninetiethPercentile = (calculatePercentile(allWaitTimes, 90)) / 60;
    cout << "Bank service times in minutes: 10th %ile " << tenthPercentile << ", 50th %ile " << fiftiethPercentile << ", 90th %ile " << ninetiethPercentile << endl;
//    cout << "Bank customers served: " << numberOfCustomersServed << endl << endl;
}

// Helper method that converts priority_queue to vector to allow for simple iteration.
// Passes queue by value so original is unchanged.
vector<Customer> convertToVector(priority_queue<Customer> lines) {
    vector<Customer> ret;
    int originalSize = (int) lines.size();
    while (lines.size() != 0) {
        ret.push_back(lines.top());
        lines.pop();
    }
    assert(ret.size() == originalSize);
    return ret;
}

void getInShortestLane(const Customer &customer, vector<priority_queue<Customer> > &lines, int currentTime) {
    int shortestLaneIndex = 0;
    int timeLeftInShortestLane = 1000000; // higher than possible
    for (int i = 0; i < lines.size(); i++) {
        vector<Customer> customersInLane = convertToVector(lines[i]);
        if (customersInLane.size() == 0) { // if lane is empty, pick it
            shortestLaneIndex = i;
            break;
        }
        int timeLeftInCurrentLane = 0;
        for (int j = 0; j < customersInLane.size(); j++) {
            if (j == 0) { // prorate currently-being-helped customer's duration
                timeLeftInCurrentLane += ((customersInLane[j].beginningToBeHelped + customersInLane[j].duration) - currentTime);
            } else { // add the full duration
                timeLeftInCurrentLane += customersInLane[j].duration;
            }
        }
        if (timeLeftInCurrentLane < timeLeftInShortestLane) {
            timeLeftInShortestLane = timeLeftInCurrentLane;
            shortestLaneIndex = i;
        }
    }
    lines[shortestLaneIndex].push(customer); // add the customer to the lane with the least remaining time
}

void simulateSupermarket(vector<struct Employee> &allCashiers, vector<struct Customer> &allCustomers) {
    vector<priority_queue<Customer> > allLanes(6);
    vector<float> allWaitTimes;
    
    float numberOfCustomersServed = 0;
    float totalServiceTimeAllCustomers = 0;
    for (int currentTime = 0; currentTime < simulationTime; currentTime++) {
        for (int i = 0; i < allCustomers.size(); i++) {
            assert(allCustomers[i].firingTime != -1); // everyone should be assigned a time
            if (allCustomers[i].firingTime == currentTime) { // if they arrive now, enqueue in shortest lane
                getInShortestLane(allCustomers[i], allLanes, currentTime);
            }
        }
        for (int i = 0; i < allCashiers.size(); i++) {
            Customer customer;
            if (allCashiers[i].available && !allLanes[i].empty()) { // cashier/lane indices correspond; if cashier is available and customer is in their line, serve them
                customer = allLanes[i].top();
                allLanes[i].pop(); // remove them from queue
                allCashiers[i].startServingCustomer(currentTime, customer);
            }
            if (allCashiers[i].finishTime == currentTime && !allCashiers[i].available) { // if a cashier is finishing with someone, move on and track data
                allCashiers[i].finishServingCustomer();
                allWaitTimes.push_back(currentTime - allCashiers[i].currentCustomer.firingTime);
                totalServiceTimeAllCustomers += (currentTime - allCashiers[i].currentCustomer.firingTime);
                numberOfCustomersServed++;
            }
        }
    }
    assert(allWaitTimes.size() != 0);
    float tenthPercentile = (calculatePercentile(allWaitTimes, 10)) / 60;
    float fiftiethPercentile = (calculatePercentile(allWaitTimes, 50)) / 60;
    float ninetiethPercentile = (calculatePercentile(allWaitTimes, 90)) / 60;
    cout << "Supermarket service times in minutes: 10th %ile " << tenthPercentile << ", 50th %ile " << fiftiethPercentile << ", 90th %ile " << ninetiethPercentile << endl;
//    cout << "Supermarket customers served: " << numberOfCustomersServed << endl << endl;
}

int main(int argc, const char * argv[]) {
    
    // Commandline arguments
    float arrivalRate = stof(argv[1]); // number of customers that arrive per minute
    float serviceTime = stof(argv[2]); // max service time in minutes
    float randomSeed = stof(argv[3]); // seeeds the random number generator
    
    // Initialization
    srand (randomSeed);
    
    // Create employees (tellers / cashiers)
    vector<struct Employee> allBankTellers;
    for (int i = 0; i < 6; i++) {
        allBankTellers.push_back(Employee()); // create/store each employee
    }
    
    // Plan bank customer arrivals
    vector<struct Customer> allBankCustomers;
    for (int time = 0; time < 43200; time++) { // 12 hours -> 43200 seconds
        // on average, the probability of someone arriving each second = (1 min / arrivalRate person:min) * (60 seconds / 1 min):
        int arrivalRandomizer = rand() % (int)(60 / arrivalRate) + 1;
        if (arrivalRandomizer == 2) { // someone arrives (2 is arbitrary)
            int randomServiceDuration = rand() % (int)(serviceTime * 60) + 1;
            allBankCustomers.push_back(Customer(time, randomServiceDuration));
        }
    }
    
    // Plan supermarket customer arrivals
    vector<struct Customer> allSupermarketCustomers;
    for (int time = 0; time < 43200; time++) { // 12 hours -> 43200 seconds
        // on average, the probability of someone arriving each second = (1 min / arrivalRate person:min) * (60 seconds / 1 min):
        int arrivalRandomizer = rand() % (int)(60 / arrivalRate) + 1;
        if (arrivalRandomizer == 2) { // someone arrives (2 is arbitrary)
            int randomServiceDuration = rand() % (int)(serviceTime * 60) + 1;
            allSupermarketCustomers.push_back(Customer(time, randomServiceDuration));
        }
    }
    
    // Create employees (tellers / cashiers)
    vector<struct Employee> allSupermarketCashiers;
    for (int i = 0; i < 6; i++) {
        allSupermarketCashiers.push_back(Employee()); // create/store each employee
    }
    
    // Run simulations
    simulateBank(allBankTellers, allBankCustomers);
    simulateSupermarket(allSupermarketCashiers, allSupermarketCustomers);
    
    return 0;
}
