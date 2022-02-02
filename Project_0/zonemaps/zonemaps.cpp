#include "zonemaps.h"
#include <assert.h>
#include <iostream>
using std::cout;

// Implemented quicksort for sorting elements
template<typename T>
T partition(std::vector<T> &vec,  size_t start, size_t end) {
    T pivot = vec.at(start);
    auto lh = start + 1;
    auto rh = end;

    while (true) {
        while (lh < rh && vec.at(rh) >= pivot) rh--;
        while (lh < rh && vec.at(lh) < pivot) lh++;

        if (lh == rh) break;

        T tmp = vec.at(lh);
        vec.at(lh) = vec.at(rh);
        vec.at(rh) = tmp;
    }

    if (vec.at(lh) >= pivot) return start;
    vec.at(start) = vec.at(lh);
    vec.at(lh) = pivot;
    return lh;
}
template<typename T>
void sort(std::vector<T> &vec, size_t start, size_t end) {
    if (start >= end)
        return;

    auto boundary = partition(vec, start, end);

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
		int mid = (start + end)/2;
        cout << "key: " << key << '\n';
        cout << _elements.at(start) << '\n';
        cout << _elements.at(mid) << '\n';
        cout << _elements.at(end) << '\n';
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

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone)
{
    // constructor
    elements = _elements;
    std::vector<zone<T>> _zones;
    zones = _zones;
    num_elements_per_zone =_num_elements_per_zone;
    num_zones = _elements.size()/_num_elements_per_zone;
    cout << "zonemap initial breakdown:\n";
    cout << "num elements: " << elements.size() << '\n';
    cout << "elements: ";
    for(T element: _elements) {
        cout << element << " ";
    }
    cout << '\n';
    cout << "num per zone: " << num_elements_per_zone << '\n';
    cout << "num zones: " << num_zones << "\n\n";

}

template<typename T>
void zonemap<T>::build()
{
    std::vector<zone<T>> _zones;

    T temp_min = INT32_MAX;
    T temp_max = 0;
    T temp_count = 0;

    for(uint i = 0; i  < num_zones; i++) {
        temp_min = INT32_MAX;
        temp_max = 0;
        std::vector<T> _temp_zone_elements;
        for(uint j = 0; j < num_elements_per_zone; j++) {
            temp_count = j;
            if(i*num_elements_per_zone + j < elements.size()){
                _temp_zone_elements.push_back(elements.at(i*num_elements_per_zone + j));
                if(elements.at(i*num_elements_per_zone + j)< temp_min) {
                    temp_min = elements.at(i*num_elements_per_zone + j);
                }
                if(elements.at(i*num_elements_per_zone + j) > temp_max) {
                    temp_max = elements.at(i * num_elements_per_zone + j);
                }
            }
            else break;
        }

        zone<T> _temp_zone = {
                _temp_zone_elements,
                temp_min,
                temp_max,
                temp_count + 1
        };

        _zones.push_back(_temp_zone);
    }

    zones = _zones;

    for(zone<T> zone: zones ) {
        assert(zone.elements.size() <= num_elements_per_zone);
        cout << "zone breakdown:\n";
        cout << "num elements: " << zone.elements.size() << '\n';
        cout << "elements: ";
        for(T element: zone.elements) {
            cout << element << " ";
        }
        cout << "\n\n";
    }
}

template<typename T>
void zonemap<T>::sort_elements(zone<T> _zone)
{
    cout << "sorting zone from original: ";
    for(T element: _zone.elements) {
        cout << element << " ";
    }
    cout << '\n';
    quicksort(_zone.elements);
    cout << "results in: ";
    for(T element: _zone.elements) {
        cout << element << " ";
    }
    cout << '\n';
}

template<typename T>
bool zonemap<T>::query(T _key)
{
    for(zone<T> z: zones ) {
        cout << "key: " << _key << " zone min: " << z.min << " zone max: " << z.max << '\n';
        if(_key >= z.min && _key <= z.max) {
            cout << "searching in zone" << '\n';
            sort_elements(z);
            if(binary_search(0, z.elements.size()-1, _key, z.elements)) {
                cout << "found" << '\n';
                return true;
            }
            else continue;
        }
    }
    cout << "not found" << '\n';
    return false;
}

template<typename T>
std::vector<T> zonemap<T>::query(T _low, T _high)
{
    std::vector<T> found_values;
    if(_low == _high) return found_values;
    if(_low == _high) {
        if(query(_low)) found_values.push_back(_low);
        return found_values;
    }

    for(int i = _low; i <= _high; i++) {
        if(query(i)) found_values.push_back(i);
    }
    return found_values;
}