#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;

const int INFINITY_COST = 1e9;
const char STOP_SYMBOL = '#';

class TravelSystem {
    struct City {
        int original_id;
        string name;
    };

    vector<City> all_cities;
    vector<City> selected_cities;
    map<pair<int, int>, int> routes;
    map<int, int> city_mapping;

public:
    TravelSystem() {
        all_cities = {
            {1, "Dhaka"}, {2, "Chittagong"}, {3, "Rajshahi"},
            {4, "Khulna"}, {5, "Barisal"}, {6, "Sylhet"},
            {7, "Chandpur"}, {8, "Mymensingh"}, {9, "Comilla"},
            {10, "Cox's Bazar"}
        };
    }

    void showAllCities() {
        cout << "\nAvailable Cities:\n";
        for (const auto& city : all_cities) {
            cout << city.original_id << ". " << city.name << "\n";
        }
    }

    void selectCities() {
        cout << "\nSelect cities by entering their numbers (space separated, minimum 3, end with " << STOP_SYMBOL << "): ";
        vector<int> choices;
        int city_id;

        while (true) {
            if (!(cin >> city_id)) {
                cin.clear();
                char c;
                cin >> c;
                if (c == STOP_SYMBOL) break;
                continue;
            }

            if (city_id >= 1 && city_id <= 10) {
                if (find(choices.begin(), choices.end(), city_id) == choices.end()) {
                    choices.push_back(city_id);
                }
            }
        }

        if (choices.size() < 3) {
            cout << "Please select at least 3 cities.\n";
            exit(1);
        }

        sort(choices.begin(), choices.end());
        for (size_t i = 0; i < choices.size(); i++) {
            selected_cities.push_back(all_cities[choices[i] - 1]);
            city_mapping[choices[i]] = i + 1;
        }
    }

    void buildRoutes() {
        for (size_t i = 1; i <= selected_cities.size(); i++) {
            for (int j = 1; j <= min(3, (int)(selected_cities.size() - i)); j++) {
                int cost = j * 200;
                routes[{i, i + j}] = cost;
            }
        }

        cout << "\nAdd aerial routes (enter original city IDs, " << STOP_SYMBOL << " to stop):\n";
        int from, to;
        while (true) {
            cout << "From city ID: ";
            if (!(cin >> from)) {
                cin.clear();
                char c;
                cin >> c;
                if (c == STOP_SYMBOL) break;
                continue;
            }

            cout << "To city ID: ";
            if (!(cin >> to)) {
                cin.clear();
                char c;
                cin >> c;
                if (c == STOP_SYMBOL) break;
                continue;
            }

            if (city_mapping.count(from) && city_mapping.count(to) && from != to) {
                int steps = abs(city_mapping[to] - city_mapping[from]);
                int cost = steps * 800;
                routes[{city_mapping[from], city_mapping[to]}] = cost;
                cout << "Added aerial: " << all_cities[from - 1].name << " -> "
                     << all_cities[to - 1].name << " (cost: " << cost << " BDT)\n";
            } else {
                cout << "Invalid route. Both cities must be selected and different.\n";
            }
        }
    }

    void showTravelPlan() {
        cout << "\n=== YOUR TRAVEL PLAN ===\n";
        cout << "Selected Cities:\n";
        for (size_t i = 0; i < selected_cities.size(); i++) {
            cout << i + 1 << ". " << selected_cities[i].name << " (Original ID: "
                 << selected_cities[i].original_id << ")\n";
        }

        cout << "\nAvailable Routes:\n";
        for (auto& route_pair : routes) {
            int from = route_pair.first.first;
            int to = route_pair.first.second;
            int cost = route_pair.second;
            cout << from << ":" << selected_cities[from - 1].name << " -> "
                 << to << ":" << selected_cities[to - 1].name << " (cost: " << cost << " BDT)\n";
        }
    }

    void calculateRoute() {
        int strategy;
        cout << "\nChoose travel strategy:\n1. Fastest Route (Fewest Stops)\n2. Cheapest Route (Lowest Cost)\n> ";
        cin >> strategy;

        int n = selected_cities.size();
        vector<int> dist(n + 1, INFINITY_COST);
        vector<int> prev(n + 1, -1);
        vector<int> steps(n + 1, INFINITY_COST);
        map<pair<int, int>, int> used_cost;

        dist[1] = 0;
        steps[1] = 0;

        for (int i = 0; i < n - 1; i++) {
            for (auto& route_pair : routes) {
                int u = route_pair.first.first;
                int v = route_pair.first.second;
                int cost = route_pair.second;

                if (strategy == 1) {
                    if (steps[u] + 1 < steps[v]) {
                        steps[v] = steps[u] + 1;
                        dist[v] = dist[u] + cost;
                        prev[v] = u;
                        used_cost[{u, v}] = cost;
                    }
                } else {
                    if (dist[u] + cost < dist[v]) {
                        dist[v] = dist[u] + cost;
                        steps[v] = steps[u] + 1;
                        prev[v] = u;
                        used_cost[{u, v}] = cost;
                    }
                }
            }
        }

        vector<int> path;
        for (int at = n; at != -1; at = prev[at]) {
            if (at != 0) path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (dist[n] != INFINITY_COST && path.size() >= 2) {
            cout << "\n=== RECOMMENDED ROUTE (" << (strategy == 1 ? "Fastest" : "Cheapest") << ") ===\n";
            int totalCost = 0;
            for (size_t i = 1; i < path.size(); ++i) {
                int from = path[i - 1];
                int to = path[i];
                int cost = used_cost[{from, to}];
                cout << i << ". " << selected_cities[from - 1].name << " -> "
                     << selected_cities[to - 1].name << " : " << cost << " BDT\n";
                totalCost += cost;
            }
            cout << "Total Cost: " << totalCost << " BDT\n";
        } else {
            cout << "No valid route found to destination!\n";
        }
    }
};

int main() {
    TravelSystem system;
    system.showAllCities();
    system.selectCities();
    system.buildRoutes();
    system.showTravelPlan();
    system.calculateRoute();
    return 0;
}
