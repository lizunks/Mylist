/*
 file:Mylist.cpp
 a simulation of list in Python, which is generally done by array in this document.
 */

#include<iostream>
using namespace std;
template<class T>
class MyList{
	template<class t> friend MyList<t> operator + (const MyList<t> &l1, const MyList<t> &l2); //merge two MyLists
    template<class t> friend MyList<t> operator + (const MyList<t> &l1, const t& item); //the same as push(T item)，without modifing l1，return a new array	
    template<class t> friend ostream & operator<< (ostream &os, const MyList<t> &obj);//a type of output, e.g. [1,2,5,4,1]
private:
	T *a;
	int size; //the size of space that are able to contain the elements
	int curlen;// the amounts of element, which is actually the length of the array by definition
	void double_space();//double the space when it's full
public:
	MyList(){
		size = 100;
		a = new int [size];
	}
	MyList(int num, const T &item);//fill item in the array num times
	MyList(const MyList &l);//copy another MyList。
    MyList(T* arr, int len);//construct the list with first len times item of arr

	void push(const T &item);//add item to the bottom of Mylist
	T pop();//delete the element at the bottom
	void insert(int index, const T &item);//insert the item to the the place in index
	void clean();//clean the Mylist
	int get_size() const;//return the amount of elements
	void erase(int start, int times); //delete the element of MyList from start with amounts of times，including the edges
	T get_item(int index) const;//return the element in index
	MyList get_item(int start, int end) const;//return the element of MyList from start to end, including both edges, negative numbers are acceptable
	int count(const T &item) const;//return the amounts of elements that is equal to item
	void remove(const T &item);//delete the first element that is equal to item

 
	MyList<T> &operator = (const MyList &l);//give the value
	MyList<T> &operator += (const T &item);//the same as push(T item)
	MyList<T> &operator += (const MyList &l);//add a MyList to the current MyList
	T &operator [](int index);//return the element in the index
	

	void sort(bool less=true);//a quicksort in this program
    // if less=true，sort it with ascending order
	void reverse();//reverse it

	~MyList(){delete [] a;}
};

class excp{ // an exception class to throw the specific messege
	public:
		excp(const char *ms):message(ms){}
		const char *what() const {return message;}
	private:
		const char *message;
};

template<class T>
void MyList<T>::double_space(){
	size*=2;
	T *b=a;
	a=new T[size];
	for(int i=0;i<curlen;++i) a[i]=b[i];
	delete [] b;
}

template<class T>
MyList<T>::MyList(int num, const T &item){
	try{
		if(num<0) throw excp("the number must not be negative!");
		curlen=size=num;
		a=new T[size];
		for(int i=0;i<size;++i) a[i]=item;
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
MyList<T>::MyList(const MyList &l){
	curlen=size=l.get_size();
	a=new T[size];
	for(int i=0;i<size;++i) a[i]=l.a[i];
}

template<class T>
MyList<T>::MyList(T *arr, int len){
	try{
		if(len<0) throw excp("the number must not be negative!");
		curlen=size=len;
		a=new T[size];
		for(int i=0;i<size;++i) a[i]=arr[i];
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
void MyList<T>::push(const T&item){
	if(size==curlen) double_space();
	a[curlen]=item;
	curlen++;
}

template<class T>
T MyList<T>::pop(){
	try{
		if(curlen==0) throw excp("the list has nothing to pop!");
		curlen--;
		return a[curlen];
		
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}

}

template<class T>
void MyList<T>::insert(int index, const T &item){
	try{
		if(index<0||index>curlen) throw excp("index out of range!");
		if(size==curlen) double_space();
		T *b=new T[curlen-index];
		for(int i=0;i<curlen-index;++i) b[i]=a[index+i];
		a[index]=item;
		curlen++;
		for(int i=index+1;i<curlen;++i) a[i]=b[i-index-1];
		delete [] b; 
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
void MyList<T>::clean(){
	curlen=0;
}

template<class T>
int MyList<T>::get_size() const{
	return curlen;
}

template<class T>
void MyList<T>::erase(int start, int times){
	try{
		if(start<0||times<0||start+times>=curlen) throw excp("range error!");
		T *b=new T[curlen-start-times];
		for(int i=0;i<curlen-start-times;++i) b[i]=a[start+times+i];
		for(int i=start;i<curlen-times;i++) a[i]=b[i-start];
		delete [] b; 
		curlen=curlen-times;
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
T MyList<T>::get_item(int index) const{
	try{
		if(index<-curlen||index>=curlen) throw excp("range error!");
		return a[(index+curlen)%curlen];
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
MyList<T> MyList<T>::get_item(int start, int end) const{
	try{
		if(start<-curlen||end>=curlen||start>=curlen||end<-curlen) throw excp("range error!");
		start=(start+curlen)%curlen;
		end=(end+curlen)%curlen;
		int len;
		if(start>end) len=0;
		else len=end-start+1;
		T *b=new T[end-start+1];
		for(int i=0;i<end-start+1;++i) b[i]=a[start+i];
		MyList result(b,len);
		delete [] b;
		return result;
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
int MyList<T>::count(const T &item) const{
	int counter=0;
	for(int i=0;i<curlen;++i){
		if(a[i]==item) counter++;
	}
	return counter;
}

template<class T>
void MyList<T>::remove(const T &item){
	for(int i=0;i<curlen;++i){
		if(a[i]==item){
			erase(i,i);
			break;
		} 
	}
}

template<class T>
MyList<T> operator + (const MyList<T> &l1, const MyList<T> &l2){
	int len=l1.curlen+l2.curlen;
	T *b=new T[len];
	for(int i=0;i<l1.curlen;++i) b[i]=l1.a[i];
	for(int i=0;i<l2.curlen;++i) b[i+l1.curlen]=l2.a[i];
	MyList<T> result(b,len);
	delete [] b;
	return result;
}

template<class T>
MyList<T> operator + (const MyList<T> &l1, const T &item){
	MyList<T> result(l1);
	result.push(item);
	return result;
}

template<class T>
MyList<T>& MyList<T>::operator = (const MyList<T> &l){
	curlen=l.curlen;
	if(size<curlen) double_space();
	for(int i=0;i<curlen;++i) a[i]=l.a[i];
	return *this;
}

template<class T>
MyList<T>& MyList<T>::operator += (const T& item){
	push(item);
	return *this;
}

template<class T>
MyList<T>& MyList<T>::operator += (const MyList<T>& l){
	for(int i=0;i<l.get_size();++i) push(l.get_item(i));
	return *this;
}

template<class T>
T& MyList<T>::operator[](int index){
	try{
		if(index<-curlen||index>=curlen) throw excp("index out of range!");
		return a[(index+curlen)%curlen];
	}
	catch(excp ex) {cout<<"excption occured:"<<ex.what()<<'\n';}
}

template<class T>
ostream & operator<<(ostream &os, const MyList<T> &obj){
	os<<'[';
	for(int i=0;i<obj.get_size()-1;++i) os<<obj.a[i]<<',';
	if(obj.get_size()!=0) os<<obj.a[obj.get_size()-1];
	os<<']';
	return os;
}


template<class T>
int divide(T *a,int low, int high, bool less){
	T temp=a[low];
	while(low!=high){
		while(low<high&&(a[high]>=temp&&less)) --high;
		if(low<high){
			a[low]=a[high];
			++low;
		}
		while(low<high&&(a[low]<=temp&&less)) ++low;
		if(low<high){
			a[high]=a[low];
			--high;
		}
	}
	a[low]=temp;
	return low;
}

template<class T>
void quicksort(T *a, int low, int high, bool less){
	int mid;
	if(low>=high) return;
	mid=divide(a,low,high,less);
	quicksort(a,low,mid-1,less);
	quicksort(a,mid+1,high,less);
}

template<class T>
void MyList<T>::sort(bool less){
	quicksort(a,0,curlen,less);
} 

template<class T>
void MyList<T>::reverse(){
	T* b=a;
	a=new T[curlen];
	for(int i=0;i<curlen;++i) a[i]=b[curlen-i];
	delete [] b;
}

int main(){
	MyList<int> a, b;
	int i;
	for (i=0; i<5; ++i)
		a.push(i);
    // a = [0, 1, 2, 3, 4]
	a[3] = 15; // a = [0, 1, 2, 15, 4]
	a.sort(); // a = [0, 1, 2, 4, 15]
	a.reverse(); // a = [15, 4, 2, 1, 0]
	a += 12; // a = [15, 4, 2, 1, 0, 12]
	for (i=0; i<a.get_size(); ++i)
		cout<<a[i]<<endl;
    b = a.get_item(4, -3); // b = [] 
	b = a.get_item(3, -1); // b = [1, 0, 12] 
	a += b; // a = [15, 4, 2, 1, 0, 12, 1, 0, 12]
	for (i=0; i<a.get_size(); ++i)
		cout<<a.get_item(i)<<endl;
	cout<<a.count(5)<<endl;
	b.clean(); // b = []
	cout<<b.get_size()<<endl;
	a.erase(2, 5); // a = [15, 4, 0, 12]
	b = a + a; // b = [15, 4, 0, 12, 15, 4, 0, 12]
	b.insert(3, 116); // b = [15, 4, 0, 116, 12, 15, 4, 0, 12]
	b.remove(4); // b = [15, 0, 116, ...]
	cout<<b<<endl;
	MyList<double> c(10, 3.14);
	for (i=0; i<100; ++i)
		c.push(1.1*i);
	cout<<c.get_item(100, 105)<<endl;
	
	return 0;
}
