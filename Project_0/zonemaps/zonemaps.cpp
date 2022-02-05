#include "zonemaps.h"
#include <assert.h>
#include <iostream>
#include <math.h>

// Implemented quicksort for sorting elements
template<typename T>
T partition(std::vector<T> &vec,  size_t start, size_t end) {
    T pivot = vec.at(start);
    auto l = start + 1;
    auto r = end;

    while (true) {
        while (l < r && vec.at(r) >= pivot) r--;
        while (l < r && vec.at(l) < pivot) l++;
        // found edgecase
        if (l == r) break;

        // replace
        T tmp = vec.at(l);
        vec.at(l) = vec.at(r);
        vec.at(r) = tmp;
    }

    if (vec.at(l) >= pivot) return start;
    vec.at(start) = vec.at(l);
    vec.at(l) = pivot;
    return l;
}
template<typename T>
void sort(std::vector<T> &vec, size_t start, size_t end) {
    // edgecase done
    if (start >= end)
        return;

    // continue 
    auto boundary = partition(vec, start, end);

    // recursion on separated
    sort(vec, start, boundary);
    sort(vec, boundary + 1, end);
}
template<typename T>
void quicksort(std::vector<T> &vec) {
    sort(vec, 0, vec.size() - 1);
}

// Binary search for key-based queries
template<typename T>
bool binary_search(int start, int end, int key, std::vector<T> _elements) {
	while (start < end)
	{
        // split search interval in halfs and disregard half not in range
		int mid = (start + end)/2;
		if(key == _elements.at(mid))
		{
			return true;
		}
		if(key < _elements.at(mid)) {
			end = mid;
		}
		else {
			start = mid+1;
		}
	}
	return false;
}

// Initialize zonemap with basic components
template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone)
{
    // constructor
    elements = _elements;
    std::vector<zone<T>> _zones;
    zones = _zones;
    num_elements_per_zone =_num_elements_per_zone;
    // Use ceil to ensure remainder elements have their own zone as well(minimize overhead)
    num_zones = ceil((float)_elements.size()/(float)_num_elements_per_zone);

}

template<typename T>
void zonemap<T>::build()
{
    std::vector<zone<T>> _zones;

    // Temporary holders for zone initialization
    T temp_min = INT32_MAX;
    T temp_max = 0;
    T temp_count = 0;

    for(uint i = 0; i  < num_zones; i++) {
        // reset counters
        temp_min = INT32_MAX;
        temp_max = 0;
        std::vector<T> _temp_zone_elements;
        for(uint j = 0; j < num_elements_per_zone; j++) {
            temp_count = j;
            // So long as zone sizes and elements sizes not broken add to zone
            if(i*num_elements_per_zone + j < elements.size()){
                _temp_zone_elements.push_back(elements.at(i * num_elements_per_zone + j));
                // Adjust mins and maxes
                if(elements.at(i * num_elements_per_zone + j) < temp_min) {
                    temp_min = elements.at(i * num_elements_per_zone + j);
                }
                if(elements.at(i*num_elements_per_zone + j) > temp_max) {
                    temp_max = elements.at(i * num_elements_per_zone + j);
                }
            }
            else break;
        }
        // Initialize zone
        zone<T> _temp_zone = {
                _temp_zone_elements,
                temp_min,
                temp_max,
                temp_count + 1
        };

        // Sort zone prior to prevent overhead while querying(online stage)
        sort_elements(_temp_zone);
        _zones.push_back(_temp_zone);
    }

    zones = _zones;

    // Perform invariant maintenance check which was accounted for earlier 
    for(zone<T> zone: zones ) {
        assert(zone.elements.size() <= num_elements_per_zone);
    }
}

// Sort function which just uses zone for simplicity(memory spaceon stack issue though)
template<typename T>
void zonemap<T>::sort_elements(zone<T> _zone)
{
    quicksort(_zone.elements);
}

template<typename T>
bool zonemap<T>::query(T _key)
{
    // Big O (zone count) for queries(millions occur so not great but not horrible)
    for(zone<T> z: zones ) {
        // key is one of the edges of the zone
        if(_key == z.min || _key == z.max) {
            return true;
        }
        // do binary search for efficient location
        if(_key > z.min && _key < z.max) {
            if(binary_search(0, z.elements.size()-1, _key, z.elements)) {
                return true;
            }
            else continue;
        }
    }
    // std::cout << "not found" << '\n';
    return false;
}

template<typename T>
std::vector<T> zonemap<T>::query(T _low, T _high)
{
    // Initialize vector that contains all elements in range
    std::vector<T> found_values;
    // Edge case for range queries that simplify to single point query
    if(_low == _high) {
        if(query(_low)) found_values.push_back(_low);
        return found_values;
    }
    
    for(zone<T> z: zones) {
        // Entire zone contains elements in the range(quick)
        if(_low >= z.min && _high <= z.max) {
            found_values.insert(found_values.end(), z.elements.begin(), z.elements.end());
            continue;
        }
        // Portion of zone contains elements in range
        if((_low >= z.min && _low <= z.max) || (_high <= z.max && _high >= z.min) ) {
            for(T element: z.elements) {
                if(element >= _low && element <= _high) found_values.push_back(element);
            }
        }
    }
    return found_values;
}