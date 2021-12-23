#include <iostream>
#include <vector>
#include <future>
#include <ctime>
using namespace std;
#define SIZE 1000000

bool multi_thread = true;
 
void merge(vector<int> &arr, int l, int m, int r)
{
    //вычисляем размер левой и правой части массивов
    int nl = m - l + 1;
    int nr = r - m;   

    vector<int>left;
    vector<int> right;

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)       
        left.push_back(arr[l + i]);
    for (int j = 0; j < nr; j++)       
        right.push_back(arr[m + 1 + j]);

    int i = 0, j = 0;
    int k = l;  // начало левой части
    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int l, int r)
{
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;
    if (multi_thread && r - l > 10000)
    {
        auto sort_in_thread = async(launch::async, [&]() {
            mergeSort(arr, l, m);
            });
        mergeSort(arr, m + 1, r);
    }
    else
    {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }

    merge(arr, l, m, r);

}

int main()
{   
    vector<int> arr(SIZE);   
    srand(555);
    for (size_t i = 0; i < SIZE; i++)
    {        
        arr.push_back(rand() % 1000);
    }
    unsigned int start_time = clock(); 
    mergeSort(arr, 0, SIZE-1);
    unsigned int end_time = clock();  
    double seconds = difftime(end_time, start_time)/1000;
    cout << "runtime using multithreading = " << seconds << endl;

    bool sorted_well = true;
    for (size_t i = 0; i < SIZE - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            sorted_well = false;
            break;
        }        
    }   
    if (sorted_well)
        cout << "SORTED ok\n";
    else cout << "Error of sorting\n";
    arr.clear();
    for (size_t i = 0; i < SIZE; i++)
    {
        arr.push_back(rand() % 1000);
    }
    multi_thread = false;
    start_time = clock(); 
    mergeSort(arr, 0, SIZE - 1);
    end_time = clock();  
    seconds = difftime(end_time, start_time) / 1000;
    cout << "runtime without multithreading = " << seconds << endl;

    return 0;
}