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


// NEED TO CHECK WITH TA!!!!!!!!!!
// "Order does not matter if same name" but does NOT pass test locally and on CodeGrade
// Out out order because using unoredered_map for the containers
std::vector<StopID> Datastructures::stops_alphabetically()
{

    std::vector<StopID> all = all_stops();

    std::sort(all.begin(), all.end(),
        [this](StopID id1, StopID id2) {
            const Name& name1 = stops.at(id1).name;
            const Name& name2 = stops.at(id2).name;

            if (name1 != name2) {
                return name1 < name2;
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

std::vector<StopID> Datastructures::stops_closest_to(StopID /*id*/)
{
    // replace with your implementation
    throw NotImplemented();
}

bool Datastructures::remove_stop(StopID /*id*/)
{
    // replace with your implementation
    throw NotImplemented();
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID /*id*/)
{
    // replace with your implementation
    throw NotImplemented();
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


