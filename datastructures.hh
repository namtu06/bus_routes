// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <algorithm>
#include <functional>
#include <source_location>
#include <queue>

// Omat includet
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cstddef>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using RouteID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
RouteID const NO_ROUTE = "!!NO_ROUTE!!";
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;


// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    explicit NotImplemented(std::string const msg = "",
                            const std::source_location location = std::source_location::current())
        : msg_{}
    {
        std::string funcname = location.function_name();
        if (auto namestart = funcname.find_last_of(':'); namestart != std::string::npos)
        { funcname.erase(0, namestart+1); }
        if (auto nameend = funcname.find_first_of('('); nameend != std::string::npos)
        { funcname.erase(nameend, std::string::npos); }
        msg_ = (!msg.empty() ? msg+" in " : "")+funcname+"()";
    }
    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// ---------------------------------------------------------------------------------------
// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Constant
    // Short rationale for estimate: .size() method of unordered_map has constant complexity as per cppreference.com
    int stop_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: .clear() method of unordered map has linear complexity
    // in the number of elements in the unordered_map (cppreference.com)
    void clear_all();

    // Estimate of performance: O(N)
    // Short rationale for estimate: The program has to iterate through every single element in the stop list
    // to add to the vector to be returned.
    std::vector<StopID> all_stops();

    // Estimate of performance: Constant on average if stop exists (.find() for unordered_map on cppreference)
    // Amortized constant on average if stop does not exist (.emplace() for unordered_map on cppreference)
    // Short rationale for estimate: The program has to check if the stop already exists, only if it doesn't exist
    // do we add the stop
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance: Constant
    // Short rationale for estimate: Utilizes .find() to both check for stop's existence and get name (cppreference)
    Name get_stop_name(StopID);

    // Estimate of performance: Constant
    // Short rationale for estimate: Utilizes .find() to both check for stop's existence and get name (cppreference)
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(N log(N))
    // Short rationale for estimate: all_stops is O(N), sorting is O(N log(N)), lookup with .at() is O(1) on average (cppreference)
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance: O(N log(N))
    // Short rationale for estimate: all_stops is O(N), sorting is O(N log(N)), lookup with .at() is O(1) on average (cppreference)
    std::vector<StopID> stops_coord_order();

    // Estimate of performance: O(N * L), where N is the number of stops, L is the length of the name string
    // Short rationale for estimate: Iterate through the entire unordered_map to find matching strings O(N), comparing O(L), push_back constant time
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance: Constant on average
    // Short rationale for estimate: .find() takes O(1) in both cases stop does not and does exist, assume replacing the name is constant therefore also O(1)
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance: Constant on average
    // Short rationale for estimate: .find() takes O(1) in both cases stop does not and does exist, assume replacing the coord is constant therefore also O(1)
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: Constant
    // Short rationale for estimate: Similarly to add_stop, .find() is O(1) in both cases of existence, .emplace() is also O(1)
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance: Constant
    // Short rationale for estimate: .find() is O(1), accessing the name is also assumed to be O(1).
    Name get_region_name(RegionID id);

    // Estimate of performance: O(N log(N))
    // Short rationale for estimate: the function iterates through every element which takes O(N), sorting takes O(N log N).
    std::vector<RegionID> all_regions();

    // Estimate of performance: Constant
    // Short rationale for estimate: Finding, checking, accessing and updating the values all O(1)
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance: Constant
    // Short rationale for estimate: Finding, checking, accessing and updating the values all O(1)
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(N), where N is the number of regions containing the stop
    // Short rationale for estimate: finding and checking stop's attributes O(1), travelling upwards starting from the smallest subregion in a while loop O(N).
    std::vector<RegionID> stop_regions(StopID id);

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: O(N) for iterating through the stops + O(N logN) for actually sorting the distances vector
    // = O(N logN)
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: finding stop and associated region O(1), remove from region's stops vector O(N), remove from major stops unordered map O(1)
    bool remove_stop(StopID id);

    // Estimate of performance: O(R + S), where R is the number of regions the stop is in, S is the number of stops within each region
    // Short rationale for estimate: Find initial region O(1) + visit all regions and their stops O(R + S)
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance: O(R1 + R2), where R1 and R2 are the number of regions stop 1 and 2 are in respectively.
    // Short rationale for estimate: The function travels up the region tree for both stops. Best case scenario they are in the same smallest region already.
    RegionID stops_common_region(StopID id1, StopID id2);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Need to iterate through the proposed stops vector to check for validity O(N), best case first stop doesn't exist so function exists immediately.
    bool add_route(RouteID id, std::vector<StopID> stops);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Iterate through the entire unordered map.
    std::vector<RouteID> all_routes();

    // Estimate of performance: Constant
    // Short rationale for estimate: Finding the route by ID O(1) + removing the route by ID O(1), all in unordered map
    bool remove_route(RouteID id);

    // Estimate of performance: O(R x N) where R is the number of routes and average number of stops in a route
    // Short rationale for estimate: Loops through every route, and also loops through every stops in a route
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Estimate of performance: O(N)
    // Short rationale for estimate: O(1) route lookup + O(N) for copying the elements if applicable.
    std::vector<StopID> route_stops(RouteID id);

    // Estimate of performance: Constant
    // Short rationale for estimate: unordered map clear O(1) as per cppreferences.
    void clear_routes();

    // Notation for below functions:
    // V = number of stops
    // R = number of routes
    // S = total number of stops across all routes

    // Estimate of performance: O(V x (R + S))
    // Short rationale for estimate:BFS visits each stop once, but for every stop it has to
    // iterate through all routes and their stops to find neighbouring stops using get_neighbours().
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_any(StopID fromstop, StopID tostop);

//    // Non-compulsory operations

    // Estimate of performance: O(V x (R + S))
    // Short rationale for estimate: Similarly to journey_any, BFS visits each stop once and
    // has to iterate through all routes and their stops to find neighbouring stops.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_least_stops(StopID fromstop, StopID tostop);

    // Estimate of performance: O(V x (R + S))
    // Short rationale for estimate: DFS visits each stop once and for every stop it has to
    // iterate through all routes and their stops to find the next stops to visit.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_with_cycle(StopID fromstop);

    // Estimate of performance: O(V x (R + S) + S log S)
    // Short rationale for estimate: Dijkstra visits stops based on their shortest distance.
    // For every stop it has to iterate through all routes and their stops to find neighbours.
    // The priority queue also requires sorting operations when adding and removing elements.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_shortest_distance(StopID fromstop, StopID tostop);

private:
    // Add stuff needed for your class implementation here
    // Explain below your rationale for choosing the data structures you use in
    // this class.

    // Define struct for a single stop, with name, its coordinate, and which region it belongs to.
    struct Stop {
        Name name;
        Coord coord;
        RegionID region = NO_REGION;
    };

    // Each stop has its own *unique ID*, meaning we don't have to worry about overlapping ID
    // and searching via key with this is asymptoticaly fastest (from the table provided in the lecture videos)
    std::unordered_map<StopID, Stop> stops;



    // Define struct for each region similarly to Stop. This (personally) helps with code-reading,
    // the surrounding functions will be similar to that of Stop's.
    struct Region {
        Name name;
        std::vector<StopID> stops;
        RegionID parent = NO_REGION;
        std::vector<RegionID> subregions;
    };

    // Define container for regions. Similar reasoning with stops.
    // Also has a unique ID and the corresponding information, don't have to worry about overlapping ID
    std::unordered_map<RegionID, Region> regions;



    // Define Route struct
    struct Route{
        std::vector<StopID> stops;
    };

    // Define container for routes. Each route is defined by unique ID so unordered_map works here
    std::unordered_map<RouteID, Route> routes;

    // Helper function for calculating distance
    int calculate_distance(StopID id1, StopID id2);

    // Helper function for getting neighbours of stops to be used in the last few functions (nodes in route graphs)
    std::vector<std::pair<StopID, RouteID>> get_neighbours(StopID stopid);
};

#endif // DATASTRUCTURES_HH
