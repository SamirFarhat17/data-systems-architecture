#include "zonemaps.h"
#include <assert.h>

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint32_t _num_elements_per_zone)
{
    // constructor
    elements = _elements;
    num_elements_per_zone =_num_elements_per_zone;
    num_zones = sizeof(_elements)/_num_elements_per_zone;
}

template<typename T>
void zonemap<T>::build()
{
    std::vector<zone<T>> _zones;

    for(uint32_t i = 0; i  <num_zones; i++) {
        std::vector<T> _temp_zone_elements;
        for(uint32_t j = 0; j < num_elements_per_zone; j++) {
            if(i*num_elements_per_zone + j < sizeof(elements)){
                _temp_zone_elements.push_back(elements.at(i*num_elements_per_zone + j));
            }
        }

        zone<T> _temp_zone = {
            _temp_zone_elements,
            i*num_elements_per_zone,
            i*num_elements_per_zone + num_elements_per_zone,
            num_elements_per_zone
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
    quicksort(elements, 0, sizeof(elements));
}


template<typename T>
bool zonemap<T>::query(T _key)
{

}


template<typename T>
std::vector<T> zonemap<T>::query(T _low, T _high)
{
    return false;
}

// Implemented quicksort for sorting elements
template<typename T>
int partition(std::vector<T> &_elements, int start, int end){

	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
		if(_elements[i]<_elements[pivot]){
			std::swap(_elements[i],_elements[j]);
			++j;
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
