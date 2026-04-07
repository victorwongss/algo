#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include <algorithm>
#include <cstring>

class TimeCalc{
std::chrono::time_point<std::chrono::high_resolution_clock> start_;
public:
    TimeCalc(){
        start_ = std::chrono::high_resolution_clock::now();
    };

    ~TimeCalc(){
        std::chrono::time_point<std::chrono::high_resolution_clock> end_ = std::chrono::high_resolution_clock::now();
        printf("Time taken: %fs\n", std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count() * 1. / 1e6);
    };
};

void generateRandomArray(int *arr, int size, int min, int max){
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        arr[i] = rand() % (max - min + 1) + min;
    }
}

void generateNearlySortedArray(int *arr, int size, int times){
    for(int i = 0; i < size; i++){
        arr[i] = i;
    }
    srand(time(NULL));
    for(int i = 0; i < times; i++){
        int index1 = rand() % size;
        int index2 = rand() % size;
        std::swap(arr[index1], arr[index2]);
    }
}

void generateSortedArray(int* arr, int size){
    for(int i = 0; i < size; i++){
        arr[i] = i;
    }
}

void quickSortTradntion(int* arr, int left, int right){
    if(left >= right){
        return;
    }
    
    int i = left, j = right, pivot = arr[left];
    while (i <= j){
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if(i <= j){
            std::swap(arr[i++], arr[j--]);
        }
    }
    quickSortTradntion(arr, left, j);
    quickSortTradntion(arr, i, right);
}



void quickSort(int* arr, int left, int right){
    if(left >= right){
        return;
    }
    int i = left, j = right,  pivot = arr[left + (right - left) / 2];
    while (i <= j){
        while (arr[i] < pivot) i++;
        while(arr[j] > pivot) j--;
        if(i <= j){
            std::swap(arr[i++], arr[j--]);
        }
    }

    quickSort(arr, left, j);
    quickSort(arr, i, right);
}

void quickSort3way(int* arr, int left, int right){
    if(left >= right){
        return;
    }

    int i = left, lt = left, gt = right, pivot = arr[left + (right - left) / 2];
    while (i <= gt){
        if(arr[i] < pivot){
            std::swap(arr[i++], arr[lt++]);
        } else if(arr[i] > pivot){
            std::swap(arr[i], arr[gt--]);
        } else{
            i++;
        }
    }
    quickSort3way(arr, left, lt - 1);
    quickSort3way(arr, gt + 1, right);
}

template <typename T>
void mySwap(T& x, T& y){
    x ^= y, y ^= x, x ^= y;
}

void quickSort2Pivot(int* arr, int left, int right){
    if (left >= right){
        return;
    }

    if(arr[left] > arr[right]){
        // 确保left元素一定小于right元素
        std::swap(arr[left], arr[right]);
    }

    const int pivot1 = arr[left]; // 基准值1
    const int pivot2 = arr[right]; //基准值2

    int lt = left + 1; // 小于pivot1的右边界
    int gt = right - 1; // 大于pivot2的左边界, lt ~ gt 为 pivot1 ~ pivot2之间
    int i = left + 1;

    while (i <= gt){
        if (arr[i] < pivot1){
            std::swap(arr[i++], arr[lt++]);
        } else if(arr[i] > pivot2){
            std::swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    std::swap(arr[left], arr[--lt]);
    std::swap(arr[++gt], arr[right]);

    quickSort2Pivot(arr, left, lt - 1);
    quickSort2Pivot(arr, lt + 1, gt - 1);
    quickSort2Pivot(arr, gt + 1, right);
}

void bubbleSort(int* arr, int left, int right){
    if(left >= right){
        return;
    }

    for(int i=left; i < right; i++){
        bool swapped = false;
        for(int j = left; j < right - i;j++){
            if(arr[j] > arr[j+1]){
                std::swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }

        if(!swapped){
            break;
        }
    }   
}

void selectSort(int* arr, int left, int right){
    if (left >= right){
        return;
    }

    for(int i = left; i < right; i++){
        int index = i;
        for(int j= i + 1; j < right; j++){
            if(arr[j] < arr[index]){
                index = j;
            }
        }
        std::swap(arr[i], arr[index]);
    }
}

void insertSort(int* arr, int left, int right){
    if(left >= right){
        return;
    }

    for(int i = left + 1; i < right; i++){
        int pivot = arr[i];
        int j = i - 1;
        while(j >= left && arr[j] > pivot){
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = pivot;
    }
}

// #define SORT_ALGO(x, y, z) quickSortTradntion(x, y, z)
// #define SORT_ALGO(x, y, z) quickSort(x, y, z)
// #define SORT_ALGO(x, y, z) quickSort3way(x, y, z)
// #define SORT_ALGO(x, y, z) quickSort2Pivot(x, y, z)
// #define SORT_ALGO(x, y, z) bubbleSort(x, y, z)
// #define SORT_ALGO(x, y, z) selectSort(x, y, z)
#define SORT_ALGO(x, y, z) insertSort(x, y, z)

int main(int argc, char const *argv[])
{
    int size = 100000;
    int* arr = new int[size];
    generateRandomArray(arr, size, 0, size);
    {
        TimeCalc calc_;
        SORT_ALGO(arr, 0, size -1);
    };  
    assert(std::is_sorted(arr, arr + size - 1));

    generateNearlySortedArray(arr, size, 100);
    {
        TimeCalc calc_;
        SORT_ALGO(arr, 0, size -1);
    };  
    assert(std::is_sorted(arr, arr + size - 1));

    generateSortedArray(arr, size);
    {
        TimeCalc calc_;
        SORT_ALGO(arr, 0, size -1);
    };  
    assert(std::is_sorted(arr, arr + size - 1));

    delete arr;
    arr = nullptr;
    return 0;
}
