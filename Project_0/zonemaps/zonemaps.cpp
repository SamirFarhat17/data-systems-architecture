#include "zonemaps.h"
#include <assert.h>

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint32_t _num_elements_per_zone)
{
    // constructor
    elements = _elements;
    zones = new std::vector<zone<T>>;
    num_elements_per_zone =_num_elements_per_zone;
    num_zones = sizeof(_elements)/_num_elements_per_zone;
}

template<typename T>
void zonemap<T>::build()
{
    std::vector<zone<T>> _zones;

    T temp_min = INT32_MAX;
    T temp_max = 0;
    T temp_count = 0;

    for(uint32_t i = 0; i  <num_zones; i++) {
        std::vector<T> _temp_zone_elements;
        for(uint32_t j = 0; j < num_elements_per_zone; j++) {
            temp_count = j;
            if(i*num_elements_per_zone + j < sizeof(elements)){
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

    for(std::vector<T> zone: zones ) {
        assert(sizeof(zone) <= num_elements_per_zone);
    }
}

template<typename T>
void zonemap<T>::sort_elements()
{
    for(zone<T> z: zones ) {
        quicksort(z);
    }
}

template<typename T>
bool zonemap<T>::query(T _key)
{
    for(zone<T> z: zones ) {
        if(z.min >= _key && z.max<= _key) {
            quicksort(z);
            if(binary_search(0, sizeof(z), _key)) return true;
            else continue;
        }
    }
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

// Implemented quicksort for sorting elements
template<typename T>
int partition(std::vector<T> &_elements, int start, int end){

	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
		if(_elements[i]<_elements[pivot]){
			std::swap(_elements[i],_elements[j]);
		}
	}
	std::swap(_elements[j],_elements[pivot]);
	return j;

}
template<typename T>
void quicksort(std::vector<T> &_elements, int start, int end ){

	if(start<end){
		int p = partition(_elements, start, end);
		quicksort(_elements,start,p-1);
		quicksort(_elements,p+1,end);
	}

}
// Binary search for key-based queries
template<typename T>
bool binary_search(T start, T end, T key) {
    T lo = start;
    T up = end;

    while (lo < up)
    {
        T mid = lo + (up-lo)/2;
        if(key == *mid)
        {
            return true;
        }
        if(key < *mid) {
            up = mid;
        }
        else {
            lo = mid;
        }
    }

    return false;
}