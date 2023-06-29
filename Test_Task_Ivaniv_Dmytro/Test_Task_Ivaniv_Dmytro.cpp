#include <stdio.h>
#include <climits>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

const int INF = INT_MAX;

const int N = 6;
vector<int> final_path_time(N + 1);
vector<int> final_path_price(N + 1);
vector<bool> visited_time(N);
vector<bool> visited_price(N);
int final_res_time = INT_MAX;
int final_res_price = INT_MAX;

struct Train {
    std::string number;
    std::string departureStation;
    std::string arrivalStation;
    std::string departureTime;
    std::string arrivalTime;
    double cost;
};

std::vector<Train> parseTrainSchedule(const std::string& filename) {
    std::vector<Train> trains;
    std::ifstream file(filename);

    if (file) {
        std::string line;
        getline(file, line);

        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string cell;
            std::vector<std::string> cells;

            while (getline(ss, cell, ';')) {
                cells.push_back(cell);
            }

            if (cells.size() == 6) {
                Train train;
                train.number = cells[0];
                train.departureStation = cells[1];
                train.arrivalStation = cells[2];
                train.cost = std::stod(cells[3]);
                train.departureTime = cells[4];
                train.arrivalTime = cells[5];

                trains.push_back(train);
            }
        }

        file.close();
    }

    return trains;
}

int getTimeDifference(const std::string& time1, const std::string& time2) {
    int hourTrain1, minuteTrain1, secondTrain1, hourTrain2, minuteTrain2, secondTrain2;

    std::istringstream issTrain1(time1);
    std::string hour1, minute1, second1;
    getline(issTrain1, hour1, ':');
    getline(issTrain1, minute1, ':');
    getline(issTrain1, second1, ':');

    hourTrain1 = stoi(hour1);
    minuteTrain1 = stoi(minute1);
    secondTrain1 = stoi(second1);

    int totalMinutesTrain1 = hourTrain1 * 60 + minuteTrain1 + secondTrain1 / 60;

    std::istringstream issTrain2(time2);
    std::string hour2, minute2, second2;
    getline(issTrain2, hour2, ':');
    getline(issTrain2, minute2, ':');
    getline(issTrain2, second2, ':');

    hourTrain2 = stoi(hour2);
    minuteTrain2 = stoi(minute2);
    secondTrain2 = stoi(second2);

    int totalMinutesTrain2 = hourTrain2 * 60 + minuteTrain2 + secondTrain2 / 60;

    int timeDifference;
    if (totalMinutesTrain2 >= totalMinutesTrain1) {
        timeDifference = totalMinutesTrain2 - totalMinutesTrain1;
    }
    else {
        timeDifference = (24 * 60 - totalMinutesTrain1) + totalMinutesTrain2;
    }

    return timeDifference;
}

std::vector<std::vector<int>> generateAdjacencyMatrix(const std::vector<Train>& trains) {
    std::map<std::string, std::map<std::string, int>> trainsMap;
    std::vector<std::string> stationsArray;

    for (const Train& train : trains) {
        trainsMap[train.departureStation][train.arrivalStation] = 0;
        stationsArray.push_back(train.departureStation);
        stationsArray.push_back(train.arrivalStation);
    }

    std::sort(stationsArray.begin(), stationsArray.end());
    stationsArray.erase(std::unique(stationsArray.begin(), stationsArray.end()), stationsArray.end());

    for (const std::string& departure : stationsArray) {
        for (const std::string& arrival : stationsArray) {
            if (departure != arrival && trainsMap[departure][arrival] == 0) {
                trainsMap[departure][arrival] = 0;
            }
        }
    }

    for (const Train& train : trains) {
        int timeDifference = getTimeDifference(train.departureTime, train.arrivalTime);
        if (timeDifference < trainsMap[train.departureStation][train.arrivalStation] || trainsMap[train.departureStation][train.arrivalStation] == 0) {
            trainsMap[train.departureStation][train.arrivalStation] = timeDifference;
        }
    }

    std::vector<std::vector<int>> adjacencyMatrix(stationsArray.size(), std::vector<int>(stationsArray.size(), 0));

    for (int i = 0; i < stationsArray.size(); ++i) {
        for (int j = 0; j < stationsArray.size(); ++j) {
            if (i != j && trainsMap[stationsArray[i]][stationsArray[j]] != 0) {
                adjacencyMatrix[i][j] = trainsMap[stationsArray[i]][stationsArray[j]];
            }
        }
    }

    return adjacencyMatrix;
}

std::vector<std::vector<double>> generatePriceAdjacencyMatrix(const std::vector<Train>& trains) {
    std::map<std::string, std::map<std::string, double>> trainsMap;
    std::vector<std::string> stationsArray;

    for (const Train& train : trains) {
        trainsMap[train.departureStation][train.arrivalStation] = 0;
        stationsArray.push_back(train.departureStation);
        stationsArray.push_back(train.arrivalStation);
    }

    std::sort(stationsArray.begin(), stationsArray.end());
    stationsArray.erase(std::unique(stationsArray.begin(), stationsArray.end()), stationsArray.end());

    for (const std::string& departure : stationsArray) {
        for (const std::string& arrival : stationsArray) {
            if (departure != arrival && trainsMap[departure][arrival] == 0) {
                trainsMap[departure][arrival] = 0;
            }
        }
    }

    for (const Train& train : trains) {
        if (train.cost < trainsMap[train.departureStation][train.arrivalStation] || trainsMap[train.departureStation][train.arrivalStation] == 0) {
            trainsMap[train.departureStation][train.arrivalStation] = train.cost;
        }
    }

    std::vector<std::vector<double>> priceAdjacencyMatrix(stationsArray.size(), std::vector<double>(stationsArray.size(), 0));

    for (int i = 0; i < stationsArray.size(); ++i) {
        for (int j = 0; j < stationsArray.size(); ++j) {
            if (i != j && trainsMap[stationsArray[i]][stationsArray[j]] != 0) {
                priceAdjacencyMatrix[i][j] = trainsMap[stationsArray[i]][stationsArray[j]];
            }
        }
    }

    return priceAdjacencyMatrix;
}

void copyToFinalTime(const vector<int>& curr_path_time) {
    final_path_time = curr_path_time;
    final_path_time[N] = curr_path_time[0];
}

void copyToFinalPrice(const vector<int>& curr_path_price) {
    final_path_price = curr_path_price;
    final_path_price[N] = curr_path_price[0];
}

int firstMinTime(const vector<vector<int>>& adjTime, int i) {
    int min_val = INT_MAX;
    for (int k = 0; k < N; k++) {
        if (adjTime[i][k] < min_val && i != k)
            min_val = adjTime[i][k];
    }
    return min_val;
}

int secondMinTime(const vector<vector<int>>& adjTime, int i) {
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < N; j++) {
        if (i == j)
            continue;

        if (adjTime[i][j] <= first) {
            second = first;
            first = adjTime[i][j];
        }
        else if (adjTime[i][j] <= second && adjTime[i][j] != first)
            second = adjTime[i][j];
    }
    return second;
}

int firstMinPrice(const vector<vector<double>>& adjPrice, int i) {
    int min_val_price = INT_MAX;
    for (int k = 0; k < N; k++) {
        if (adjPrice[i][k] < min_val_price && i != k)
            min_val_price = adjPrice[i][k];
    }
    return min_val_price;
}

int secondMinPrice(const vector < vector<double>>& adjPrice, int i) {
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < N; j++) {
        if (i == j)
            continue;

        if (adjPrice[i][j] <= first) {
            second = first;
            first = adjPrice[i][j];
        }
        else if (adjPrice[i][j] <= second && adjPrice[i][j] != first)
            second = adjPrice[i][j];
    }
    return second;
}

// time
void TSPRecTime(const vector<vector<int>>& adjTime, int curr_bound, int curr_weight, int level, vector<int>& curr_path_time) {
    if (level == N) {
        if (adjTime[curr_path_time[level - 1]][curr_path_time[0]] != 0) {
            int curr_res = curr_weight + adjTime[curr_path_time[level - 1]][curr_path_time[0]];

            if (curr_res < final_res_time) {
                copyToFinalTime(curr_path_time);
                final_res_time = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (adjTime[curr_path_time[level - 1]][i] != 0 && !visited_time[i]) {
            int temp = curr_bound;
            curr_weight += adjTime[curr_path_time[level - 1]][i];

            if (level == 1)
                curr_bound -= ((firstMinTime(adjTime, curr_path_time[level - 1]) + firstMinTime(adjTime, i)) / 2);
            else
                curr_bound -= ((secondMinTime(adjTime, curr_path_time[level - 1]) + firstMinTime(adjTime, i)) / 2);

            if (curr_bound + curr_weight < final_res_time) {
                curr_path_time[level] = i;
                visited_time[i] = true;

                TSPRecTime(adjTime, curr_bound, curr_weight, level + 1, curr_path_time);
            }

            curr_weight -= adjTime[curr_path_time[level - 1]][i];
            curr_bound = temp;

            fill(visited_time.begin(), visited_time.end(), false);
            for (int j = 0; j <= level - 1; j++)
                visited_time[curr_path_time[j]] = true;
        }
    }
}

void TSPTime(const std::vector<std::string>& stationsArray, const vector<vector<int>>& adjTime) {
    vector<int> curr_path_time(N + 1, -1);
    int curr_bound_time = 0;
    visited_time.assign(N, false);

    for (int i = 0; i < N; i++) {
        curr_bound_time += (firstMinTime(adjTime, i) + secondMinTime(adjTime, i));
    }
    curr_bound_time = (curr_bound_time & 1) ? curr_bound_time / 2 + 1 : curr_bound_time / 2;

    visited_time[0] = true;
    curr_path_time[0] = 0;
    int minTime = INT_MAX;

    TSPRecTime(adjTime, curr_bound_time, 0, 1, curr_path_time);
    cout << "\n";
    cout << "By time: " << endl;
    cout << "Path Taken: ";
    for (int i = 0; i <= N; i++) {
        cout << stationsArray[final_path_time[i]] << "  ";
    }
    cout << endl;
    int hours = final_res_time / 60;
    int minutes = minTime % 60;
    int seconds = 0;

    std::cout << "Time: " << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << std::endl;
    cout << endl;
}

// TSP for Price
void TSPRecPrice(const vector<vector<double>>& adjPrice, int curr_bound, int curr_weight, int level, vector<int>& curr_path_price) {
    if (level == N) {
        if (adjPrice[curr_path_price[level - 1]][curr_path_price[0]] != 0) {
            int curr_res = curr_weight + adjPrice[curr_path_price[level - 1]][curr_path_price[0]];

            if (curr_res < final_res_price) {
                copyToFinalPrice(curr_path_price);
                final_res_price = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (adjPrice[curr_path_price[level - 1]][i] != 0 && !visited_price[i]) {
            int temp = curr_bound;
            curr_weight += adjPrice[curr_path_price[level - 1]][i];

            if (level == 1)
                curr_bound -= ((firstMinPrice(adjPrice, curr_path_price[level - 1]) + firstMinPrice(adjPrice, i)) / 2);
            else
                curr_bound -= ((secondMinPrice(adjPrice, curr_path_price[level - 1]) + firstMinPrice(adjPrice, i)) / 2);

            if (curr_bound + curr_weight < final_res_price) {
                curr_path_price[level] = i;
                visited_price[i] = true;

                TSPRecPrice(adjPrice, curr_bound, curr_weight, level + 1, curr_path_price);
            }

            curr_weight -= adjPrice[curr_path_price[level - 1]][i];
            curr_bound = temp;

            fill(visited_price.begin(), visited_price.end(), false);
            for (int j = 0; j <= level - 1; j++)
                visited_price[curr_path_price[j]] = true;
        }
    }
}

void TSPPrice(const std::vector<std::string>& stationsArray, const vector <vector<double>>& adjPrice) {

    vector<int> curr_path_price(N + 1, -1);
    int curr_bound_price = 0;
    visited_price.assign(N, false);

    for (int i = 0; i < N; i++) {
        curr_bound_price += (firstMinPrice(adjPrice, i) + secondMinPrice(adjPrice, i));
    }
    curr_bound_price = (curr_bound_price & 1) ? curr_bound_price / 2 + 1 : curr_bound_price / 2;

    visited_price[0] = true;
    curr_path_price[0] = 0;

    TSPRecPrice(adjPrice, curr_bound_price, 0, 1, curr_path_price);

    cout << "By price: " << endl;
    cout << "Path Taken: ";
    for (int i = 0; i <= N; i++)
        cout << stationsArray[final_path_price[i]] << "  ";
    cout << endl;
    cout << "Minimum cost: " << final_res_price << "$" << endl;
}


int main() {
    std::string filename = "test_task_data.csv";
    std::vector<Train> trains = parseTrainSchedule(filename);

    std::vector<std::vector<int>> timeAdjMatrix = generateAdjacencyMatrix(trains);
    std::vector<std::vector<double>> priceAdjMatrix = generatePriceAdjacencyMatrix(trains);

    std::vector<std::string> stationsArray;
    for (const Train& train : trains) {
        stationsArray.push_back(train.departureStation);
        stationsArray.push_back(train.arrivalStation);
    }

    std::sort(stationsArray.begin(), stationsArray.end());
    stationsArray.erase(std::unique(stationsArray.begin(), stationsArray.end()), stationsArray.end());

    TSPPrice(stationsArray, priceAdjMatrix);
    TSPTime(stationsArray, timeAdjMatrix);

    return 0;
}