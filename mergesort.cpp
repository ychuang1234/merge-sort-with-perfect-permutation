#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iomanip>
using namespace std;
template<class T> class twoarray{
	private:
		T *array3;
		int size;
	public:
		twoarray(int n,T* array1, T* array2):size(n){
			array3 = new T [2*size];
			int i;
			for (i=0;i<size/2;i++){
				swap(array2[i],array2[size-1-i]);
			}
			memcpy(array3,array1,size*sizeof(T));
			memcpy(&array3[size],array2,size*sizeof(T));
		}
		void perfect_shuffle(){
			T *result = new T[2*size];
			int i;
			#pragma omp parallel for
			for(i=0;i<size;i++){
				result[2*i] = array3[i];
				result[2*i+1] = array3[i+size];
			}
			#pragma omp parallel for
			for(i=0;i<2*size;i++){
				array3[i] = result[i];
			}
		}
		void compare_and_swap(){
			int i;
			#pragma omp parallel for
			for(i=0;i<size;i++){
				if (array3[2*i+1]>array3[2*i]){
					T tmp;
					tmp = array3[2*i];
					array3[2*i] = array3[2*i+1];
					array3[2*i+1] = tmp;
				}
			}
		}
		void show_twoarray(){
			int i;
			for (i=0;i<2*size;i++){
				cout<<array3[i]<<" ";
			}
			cout<<endl;
		}
		T* merge_twoarray(){
			int n = size;
			while(n){
				perfect_shuffle();
				compare_and_swap();
				//show_twoarray();
				n = n>>1;
			}
			return array3;
		}

};
template<class T> 
void partition(T* array,int l, int r,int num){
	if (num==1){
		if (array[l]<array[r]){
			swap(array[l],array[r]);
		}
	}else{
		int i;
		partition(array, l, l+num/2,num/2);

		partition(array, r, r+num/2,num/2);
		twoarray<T>a(num,&array[l],&array[r]);
		T*b = a.merge_twoarray();
		memcpy(&array[l],b,2*num*sizeof(T));
	}
}
int main(){
	srand(time(NULL));
	int min = 0, max = 500;
	int size = 1<<15;
	int* array = new int[size];
	int i;
	for (i=0;i<size;i++) array[i] = rand()%(max-min+1) + min;
	time_t start,stop;
	start = clock();
	partition(array,0,size/2,size/2);
	stop = clock();
	cout<<std::fixed<<std::setprecision(10)<<double(stop-start)/CLOCKS_PER_SEC<<" sec"<<endl;
	//for (i=0;i<size;i++) cout<<array[i]<<" ";
	//cout<<endl;

	
	/*
    int id;
    #pragma omp parallel private(id)
        {
        id = omp_get_thread_num();
        printf("%d: Hello World!\n", id);
        }
	*/
    return 0;
}