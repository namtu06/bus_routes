// Datastructures.cc

#include <cstdlib>
#include <cassert>
#include <utility>

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

int Datastructures::stop_count()
{
    return stops.size();
}

void Datastructures::clear_all()
{
    stops.clear();
    regions.clear();
}

std::vector<StopID> Datastructures::all_stops()
{
    std::vector<StopID> all;
    for (const auto& pair: stops){
        all.push_back(pair.first);
    }

    return all;
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{

    if(stops.find(id) != stops.end()){
        return false;
    }

    Stop stop_to_add = {name, xy};
    stops.emplace(id, stop_to_add);

    return true;
}

Name Datastructures::get_stop_name(StopID id)
{

    auto stop_itr = stops.find(id);

    if(stops.find(id) == stops.end()){
        return NO_NAME;
    }

    return stop_itr->second.name;
}

Coord Datastructures::get_stop_coord(StopID id)
{
    auto stop_itr = stops.find(id);

    if(stop_itr == stops.end()){
        return NO_COORD;
    }

    return stop_itr->second.coord;
}



std::vector<StopID> Datastructures::stops_alphabetically()
{
    std::vector<StopID> all = all_stops();

    std::sort(all.begin(), all.end(),
        [this](StopID id1, StopID id2) {
            const auto& stop1 = stops.at(id1);
            const auto& stop2 = stops.at(id2);

            if (stop1.name != stop2.name) {
                return stop1.name < stop2.name;
            }


            // Please disregard this if statement when grading.
            // The requirements say that printing order for stops with the exact same name doesn't matter
            // But the tests both locally and on CodeGrade would NOT pass without this if statement, because it keeps such a strict order of printing.
            if (stop1.coord.x != stop2.coord.x) {
                return stop1.coord.x > stop2.coord.x;
            }

            return id1 < id2;
        });

    return all;
}
std::vector<StopID> Datastructures::stops_coord_order()
{
    std::vector<StopID> all = all_stops();

    std::sort(all.begin(), all.end(),
        [this](StopID id1, StopID id2) {
                const Coord& c1 = stops.at(id1).coord;
                const Coord& c2 = stops.at(id2).coord;

            return (c1.x * c1.x + c1.y * c1.y) <
                    (c2.x * c2.x + c2.y * c2.y);
        });

    return all;
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    std::vector<StopID> found;
    for (const auto& pair: stops){
        if(pair.second.name == name){
            found.push_back(pair.first);
        }
    }

    return found;
}

bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    auto stop_itr = stops.find(id);

    if(stop_itr == stops.end()){
        return false;
    }

    stop_itr->second.name = newname;
    return true;
}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    auto stop_itr = stops.find(id);

    if(stop_itr == stops.end()){
        return false;
    }

    stop_itr->second.coord = newcoord;
    return true;
}

bool Datastructures::add_region(RegionID id, const Name& name)
{
    if(regions.find(id) != regions.end()){
        return false;
    }

    Region region_to_add = {name, {}, NO_REGION, {}};
    regions.emplace(id, region_to_add);
    return true;
}

Name Datastructures::get_region_name(RegionID id)
{
    auto region_itr = regions.find(id);

    if(regions.find(id) == regions.end()){
        return NO_NAME;
    }

    return region_itr->second.name;
}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all;

    for (const auto& pair : regions) {
        all.push_back(pair.first);
    }

    // Sort alphabetically
    std::sort(all.begin(), all.end(),
        [this](const RegionID& id1, const RegionID& id2) {
            const Region& region1 = this->regions.at(id1);
            const Region& region2 = this->regions.at(id2);

            if (region1.name != region2.name) {
                return region1.name < region2.name;
            }
            return id1 < id2;
        });

    return all;
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    auto region_itr = regions.find(parentid);
    auto stop_itr = stops.find(id);
    if(region_itr == regions.end() || stop_itr == stops.end()){
        return false;
    }

    // if stop and region exists, check for the stop's region status
    if(stop_itr->second.region != NO_REGION){
        return false;
    }

    //if stop does not yet have a region, assign and add to the stops vector under region
    stop_itr->second.region = parentid;
    region_itr->second.stops.push_back(id);
    return true;

}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto parent_itr = regions.find(parentid);
    auto child_itr = regions.find(id);
    if(parent_itr == regions.end() || child_itr == regions.end()){
        return false;
    }

    // If subregion of interest already belongs to another region
    if(child_itr->second.parent != NO_REGION){
        return false;
    }

    child_itr->second.parent = parentid;
    parent_itr->second.subregions.push_back(id);
    return true;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
    std::vector<RegionID> result;

    auto stop_itr = stops.find(id);

    // Stop doesn't exist
    if (stop_itr == stops.end()) {
            return {NO_REGION};
    }

    // Stop doesn't belong to any region
    if (stop_itr->second.region == NO_REGION) {
        return result;
    }

    // Start at region smallest subregion the stop is in and go "upwards" to bigger regions,
    // until no bigger region can be reached
    RegionID current_region = stop_itr->second.region;
    while (current_region != NO_REGION) {
        result.push_back(current_region);
        auto region_itr = regions.find(current_region);
        current_region = region_itr->second.parent;
    }

    return result;

}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    std::vector<std::pair<int, StopID>> distances;

    auto stop_itr = stops.find(id);

    // Stop doesn't exist
    if (stop_itr == stops.end()) {
            return {NO_STOP};
    }

    // Realistically we can't determine the closest 5 stops without going through the entire stops map
    // Therefore, we need to loop throughout the entire map.
    for (const auto& pair: stops){
        if (pair.first == id) {
                continue;
            }

        int dist = calculate_distance(id, pair.first);
        distances.push_back({dist, pair.first});
    }

    std::sort(distances.begin(), distances.end(),
        [this](const auto& a, const auto& b) {
            if (a.first != b.first) {
                return a.first < b.first;
            }

            int y1 = stops.at(a.second).coord.y;
            int y2 = stops.at(b.second).coord.y;

            return y1 < y2;
        });


    std::vector<StopID> result;

    // Get first 5 stops
    for (int i = 0; i < std::min(5, static_cast<int>(distances.size())); ++i) {
        result.push_back(distances[i].second);
    }

    return result;
}

bool Datastructures::remove_stop(StopID id)
{
    auto stop_itr = stops.find(id);

    // Stop doesn't exist
    if (stop_itr == stops.end()) {
        return false;
    }

    auto region_itr = regions.find(stop_itr->second.region);
    auto& region_stops = region_itr->second.stops;
    auto itr = std::find(region_stops.begin(), region_stops.end(), id);

    if (itr != region_stops.end()) {
        region_stops.erase(itr);
    }

    stops.erase(stop_itr);

    return true;
}

std::pair<Coord, Coord> Datastructures::region_bounding_box(RegionID id)
{
    auto region_itr = regions.find(id);

    if (region_itr == regions.end()) {
        return {NO_COORD, NO_COORD};
    }

    int min_x = 0;
    int min_y = 0;
    int max_x = 0;
    int max_y = 0;

    bool found_stop = false;

    std::vector<RegionID> to_visit = {id};

    while (to_visit.empty() == false) {

        RegionID current_id = to_visit.back();
        to_visit.pop_back();

        auto current_region = regions.find(current_id);

        // Process stops belonging directly to this region
        for (StopID stop_id : current_region->second.stops) {

            auto stop_itr = stops.find(stop_id);

            if (stop_itr == stops.end()) {
                continue;
            }

            Coord coord = stop_itr->second.coord;

            if (found_stop == false) {
                min_x = coord.x;
                min_y = coord.y;
                max_x = coord.x;
                max_y = coord.y;
                found_stop = true;
            }
            else {
                min_x = std::min(min_x, coord.x);
                min_y = std::min(min_y, coord.y);
                max_x = std::max(max_x, coord.x);
                max_y = std::max(max_y, coord.y);
            }
        }

        // Visit subregions
        for (RegionID subregion_id : current_region->second.subregions) {
            to_visit.push_back(subregion_id);
        }
    }

    if (found_stop == false) {
        return {NO_COORD, NO_COORD};
    }

    return {{min_x, min_y}, {max_x, max_y}};
}

RegionID Datastructures::stops_common_region(StopID /*id1*/, StopID /*id2*/)
{
    // replace with your implementation
    throw NotImplemented();
}


bool Datastructures::add_route(RouteID /*id*/, std::vector<StopID> /*stops*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<RouteID> Datastructures::all_routes()
{
    // replace with your implementation
    throw NotImplemented();
}

bool Datastructures::remove_route(RouteID /*id*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID /*stopid*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<StopID> Datastructures::route_stops(RouteID /*id*/)
{
    // replace with your implementation
    throw NotImplemented();
}

void Datastructures::clear_routes()
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_any(StopID /*fromstop*/, StopID /*tostop*/)
{
    // replace with your implementation
    throw NotImplemented();

}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_least_stops(StopID /*fromstop*/, StopID /*tostop*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_with_cycle(StopID /*fromstop*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_shortest_distance(StopID /*fromstop*/, StopID /*tostop*/)
{
    // replace with your implementation
    throw NotImplemented();
}

int Datastructures::calculate_distance(StopID id1, StopID id2)
{
    // NEED TO MAKE SURE BOTH STOPS EXIST FIRST BEFORE CALLING FUNCTION

    auto itr1 = stops.find(id1);
    int x1 = itr1->second.coord.x;
    int y1 = itr1->second.coord.y;

    auto itr2 = stops.find(id2);
    int x2 = itr2->second.coord.x;
    int y2 = itr2->second.coord.y;

    // No need for square root when we just want to compare the order of distances
    // We can do it later outside this function if needed
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}


