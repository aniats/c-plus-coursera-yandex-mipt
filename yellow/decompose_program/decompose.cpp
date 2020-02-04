#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string typeStr;
    is >> typeStr;
    if (typeStr == "NEW_BUS")
    {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count = 0;
        is >> stop_count;
        q.stops.clear();
        for (auto i = 0; i < stop_count; ++i)
        {
            string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
    }
    else if (typeStr == "BUSES_FOR_STOP")
    {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }
    else if (typeStr == "STOPS_FOR_BUS")
    {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }
    else if (typeStr == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    string stop_info;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    os << r.stop_info;
    return os;
}

struct StopsForBusResponse {
    vector<string> bus_info;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    for (size_t i = 0; i + 1 < r.bus_info.size(); i++) {
        os << r.bus_info[i] << endl;
    }
    int last_el_idx = (int)r.bus_info.size() - 1;
    os << r.bus_info[last_el_idx];
    return os;
}

struct AllBusesResponse {
    vector<string> info;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    for (size_t i = 0; i + 1 < r.info.size(); i++) {
        os << r.info[i] << endl;
    }
    int last_el_idx = (int)r.info.size() - 1;
    os << r.info[last_el_idx];
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        route_for_bus[bus] = stops;
        for (const auto& station_name : stops) {
            buses_for_stop[station_name].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse res;
        if (buses_for_stop.count(stop) == 0) {
            res.stop_info = "No stop";
        }
        else {
            for (auto el : buses_for_stop.at(stop)) {
                res.stop_info += el + " ";
            }
            res.stop_info.pop_back();
        }
        return res;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse res;
        if (route_for_bus.count(bus) == 0) {
            res.bus_info.push_back("No bus");
        }
        else {
            for (auto stop : route_for_bus.at(bus)) {
                string tmp = "Stop " + stop + ": ";
                if (buses_for_stop.at(stop).size() == 1) {
                    tmp += "no interchange";
                } else {
                    for (auto other_bus : buses_for_stop.at(stop)) {
                        if (bus != other_bus) {
                            tmp += other_bus + " ";
                        }
                    }
                    tmp.pop_back();
                }
                res.bus_info.push_back(tmp);
            }
        }
        return res;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse res;
        if (route_for_bus.empty()) {
            res.info.push_back("No buses");
        }
        else {
            for (auto bus_item : route_for_bus) {
                string tmp = "Bus " + bus_item.first + ": ";
                for (auto stop : bus_item.second) {
                    tmp += stop + " ";
                }
                tmp.pop_back();
                res.info.push_back(tmp);
            }
        }
        return res;
    }

private:
    map<string, vector<string>> route_for_bus;
    map<string, vector<string>> buses_for_stop;

};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}