#include<iostream>
using namespace std;
const char *exception_title = "Exception occurred: "; //Exception information

class IndexOutOfRange{
public:
    IndexOutOfRange(): message("Index out of range") {}
    const char *what() const {return message;}
    void print_exception() const
    {
        cout<<exception_title<<message<<'\n';
    }
private:
    const char *message;
};//exception of index out of range

class StartAndEndPointError{
public:
    StartAndEndPointError(): message("The end point specified is less than the start point") {}
    const char *what() const {return message;}
    void print_exception() const
    {
        cout<<exception_title<<message<<'\n';
    }
private:
    const char *message;
};//exception of that start point is greater than end point

template<class T> class MyList;// statement of class MyList

template<class T> MyList<T> operator + (const MyList<T> &list_1, const MyList<T> &list_2);
template<class T> MyList<T> operator + (const MyList<T> &list_1, const T &item);
template<class T> ostream & operator<<(ostream &os, MyList<T> &obj);
//statements of friend functions

template<class T>
class MyList{
    friend MyList<T> operator + (const MyList<T> &list_1, const MyList<T> &list_2)
	//add the list_2 to the end of list_1 to create a new list and return it
	{
	    int list_1_size = list_1.get_size();
	    int list_2_size = list_2.get_size();
	    int tmplist_size = list_1_size+list_2_size;// the size of the new one
	    MyList<T> tmplist(tmplist_size,0);
	    for (int i=0; i < list_1_size; ++i)
            tmplist[i] = list_1[i];
        for (int i=0; i < list_2_size; ++i)
            tmplist[i+list_1_size] = list_2[i];
        return tmplist;
	}

    friend MyList<T> operator + (const MyList<T> &list_1, const T &item)
    //add an item to the end of list_1 to create a new list and return it
    {
        MyList tmplist = list_1;
        tmplist.push_back(item);
        return tmplist;
    }

    friend ostream & operator<<(ostream &os, MyList<T> &obj)
	//overload the operator <<
	//if T is a class that can be directly print,print it
	{
	    int obj_size = obj.get_size();
	    if (obj_size == 0) // an empty list
        {
            os<<"[]";
            return os;
        }
	    os<<'[';
	    for (int i=0; i<obj_size-1; ++i)
            os<<obj[i]<<", ";
        os<<obj[obj_size-1]<<']';
        return os;
	}
private:
	T *data;  //the point to the array
	int size; //the number of elements in list

	void check_index(const int &index) const
	//function for checking whether the index is legal
	{
	    if (index<0 || index>=size)
            throw IndexOutOfRange();
	}

    int check_start_and_end(const int &start, const int &end) const
    //function for checking whether the start and end are legal
    {
        int difference = end - start + 1;
        if (difference <= 0)
            throw StartAndEndPointError();
        else return difference;
    }

	void check_and_expand_room(const int &room_needed)
	//check whether the room is enough
	{
        int room_allocated = sizeof(data)/sizeof(data[0]);
        if (room_needed <= room_allocated)
            return;
        room_allocated = room_needed * 2 + 1; // if the room will be run out, ask for more
        try
        {
            T *tmp = NULL;
            tmp=new T [room_allocated];
            for (int i=0; i < size; ++i)
                tmp[i] = data[i];
            delete []data;
            data = tmp;
        }
        catch(...) {throw;} // if failed to allocate, throw the exception
	}

	void quick_sort(const int left, const int right, bool less=true)
	// function of quick sort
    {
        int i,j;
        T mid,t;
        i=left;j=right;mid=data[(i+j)/2];
        while(i<=j)
        {
            while(!((data[j]>mid) ^ less)) j--;
            while(!((data[i]<mid) ^ less)) i++;
            if(i<=j)
            {
                t=data[i];data[i]=data[j];data[j]=t;
                i++;j--;
            }
        }
        if(left<j) quick_sort(left,j,less);
        if(i<right) quick_sort(i,right,less);
    }

public:
	MyList ():size(0), data(NULL)//constructor
	{
		try
		{
		    data = new T [100];
		}
		catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
	}

	MyList (const int &num, const T &item):size(0), data(NULL)
	//push item in list for num times
	{
	    try
	    {
	        data = new T [num];
            size = num;
            for (int i=0; i < size; ++i)
                data[i] = item;
	    }
	    catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
	}

	MyList (const MyList<T> &list_1):size(0), data(NULL)
	//copy another list
	{
	    int list_1_size = list_1.get_size();
	    try
	    {
	        data = new T [list_1_size];
            size = list_1_size;
            for (int i=0; i < size; ++i)
                data[i] = list_1[i];
	    }
	    catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
	}

	MyList (const T *arr):size(0), data(NULL)
	//build a list from an array
	{
	    try
	    {
	        int arr_size = sizeof(arr)/sizeof(arr[0]);
            data = new T [arr_size];
            size = arr_size;
            for (int i=0; i < size; ++i)
                data[i] = arr[i];
	    }
	    catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
	}

    MyList (const T *arr, const int &len):size(0), data(NULL)
    //make up a list with the first len elements of arr
    {
        try
        {
            size = min(sizeof(arr)/sizeof(arr[0]),len);
            //if len is greater than the elements of arr, automatically get all elements of arr
            data = new T [size];
            for (int i=0; i < size; ++i)
                data[i] = arr[i];
        }
        catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
    }

	void push_back(const T &item)
	//add an item to the end of list
	{
	    try
	    {
	        check_and_expand_room(size+1);
	        data[size++] = item;
	    }
        catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
	}

	T pop_back()
	//remove the last element of list and return it
	{
	    try
	    {
	        check_index(size-1);
	        --size;
            return data[size];
	    }
	    catch(IndexOutOfRange ex) {ex.print_exception(); return NULL;}
	}

	void insert(const int index, const T &item)
	//insert item to the place with index
	//only for index < size -1
	{
	    try
	    {
	        if (index == size)
            {
                push_back(item);
                return;
            }
            else
            {
                check_index(index);
                check_and_expand_room(size+1);
                for (int i=size-1; i >= index; --i)
                    data[i+1] = data[i];
                data[index] = item;
                ++size;
            }
	    }
        catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}
        catch(IndexOutOfRange ex) {ex.print_exception();}
	}

	void clean() {memset(data,0,sizeof(data)); size = 0;}
	//clean the list

	int get_size() const {return size;}
	//return the number of elements

	void erase(const int &start, const int &end)
	//delete the elements from start to end
	{
	    try
	    {
            check_index(start);
            check_index(end);
            int difference = check_start_and_end(start, end);
            for (int i=end+1; i < size; ++i)
                data[i-difference] = data[i];
            size -= difference;
	    }
        catch(IndexOutOfRange ex) {ex.print_exception();}
        catch(StartAndEndPointError ex) {ex.print_exception();}

	}

	T get_item(const int &index) const
	{
	    try
	    {
	        check_index(index);
	        return data[index];
	    }
        catch(IndexOutOfRange ex) {ex.print_exception(); return NULL;}
    }
	//return the element with index

	MyList<T> get_item(int start, int end) const
	//return elements from start to end.
	{
	    if (end < 0)
            end = size + end;
        if (start < 0)
            start = size + start;
        try
	    {
            check_index(start);
            check_index(end);
            int difference = check_start_and_end(start, end);
            MyList<T> tmplist;
            for (int i=0; i < difference; ++i)
                tmplist.push_back(data[i+start]);
            return tmplist;
	    }
        catch(IndexOutOfRange ex) {ex.print_exception(); MyList empty_list; return empty_list;}
        catch(StartAndEndPointError ex) {MyList empty_list; return empty_list;}
	}

	int count(const T &item) const
	//return the number of elements that equal to item
	{
	    int count_item = 0;
	    for (int i=0; i < size; ++i)
        {
            if (data[i] == item)
                ++count_item;
        }
        return count_item;
	}

	void remove(const T &item)
	//remove the first element that equals to item
	{
	    int remove_index = -1;
	    for (int i=0; i < size; ++i)
        {
            if (data[i] == item)
            {
                remove_index = i;
                break;
            }
        }
        if (remove_index == -1) {return;}
        erase(remove_index, remove_index);
	}

	void sort(bool less=true)
	//sort less==true: up
    {
        int l=0, r=size-1;
        quick_sort(l,r,less);
    }

    void sort(const int &left, const int &right, bool less=true)
    //sort with start point and end point
    {
        try
        {
            check_index(left);
            check_index(right);
            check_start_and_end(left, right);
            quick_sort(left,right,less);
        }
        catch(IndexOutOfRange ex) {ex.print_exception();}
        catch(StartAndEndPointError ex) {ex.print_exception();}
    }

	void reverse()
	//reverse all elements in list
	{
	    MyList tmplist;
	    tmplist = *this;
        for (int i=0; i < size ;++i)
            data[i] = tmplist[size-i-1];
	}

	MyList<T> &operator = (const MyList<T> &list_1)
	//assignment
	{
	    if (this == &list_1)// avoiding assignment to itself. If not, my cause memory leaks
            return *this;
	    int list_1_size = list_1.get_size();
	    if (list_1_size == 0) {size = 0; return *this;}
	    // if the list_1 is an empty list, clean up this list
	    try
	    {
	        check_and_expand_room(list_1_size);
	        size = list_1_size;
            for (int i=0; i < size; ++i)
                data[i] = list_1[i];
            return *this;
	    }
	    catch(bad_alloc &memExp) {cerr<<exception_title<<memExp.what()<<endl; exit(1);}

	}

	MyList<T> &operator += (const T &item)
	//add an item to the end of list
	{
	    *this = *this + item;
	    return *this;
	}

	MyList<T> &operator += (const MyList<T> list_1)
	//add another list to the end of this list
	{
        *this = *this + list_1;
        return *this;
	}

	T &operator [](int index) const
	{
	    try
	    {
	        check_index(index);
	        return data[index];
	    }
        catch(IndexOutOfRange ex) {ex.print_exception();T a = NULL; return a;}
	}
	//return the element with the index

	~MyList(){delete [] data;}
};

int main(){
	MyList<int> a, b;
	int i;
	for (i=0; i<5; ++i)
		a.push_back(i);   //checked;
    // a = [0, 1, 2, 3, 4]
	a[3] = 15; // a = [0, 1, 2, 15, 4]  //checked
	a.sort(); // a = [0, 1, 2, 4, 15]//checked
	a.reverse(); // a = [15, 4, 2, 1, 0] //checked
	a += 12; // a = [15, 4, 2, 1, 0, 12] //checked
	for (i=0; i<a.get_size(); ++i)
		cout<<a[i]<<endl;               //checked
    b = a.get_item(4, -3); // b = []  //checked
	b = a.get_item(3, -1); // b = [1, 0, 12]  //checked
	a += b; // a = [15, 4, 2, 1, 0, 12, 1, 0, 12] //checked
	for (i=0; i<a.get_size(); ++i)
		cout<<a.get_item(i)<<endl;  //checked
	cout<<a.count(5)<<endl;  //checked
	b.clean(); // b = []    //checked
	cout<<b.get_size()<<endl;   //checked
	a.erase(2, 5); // a = [15, 4, 1, 0, 12] //checked
	b = a + a; // b = [15, 4, 1, 0, 12, 15, 4, 1, 0, 12] //checked
	b.insert(3, 116); // b = [15, 4, 1, 116, 0, 12, 15, 4, 1, 0, 12]    //checked
	b.remove(4); // b = [15, 1, 116, ...]   //checked
	cout<<b<<endl;  //checked
	MyList<double> c(10, 3.14); //checked
	for (i=0; i<100; ++i)       //checked
		c.push_back(1.1*i);     //checked
	cout<<c.get_item(100, 105)<<endl;   //checked
    cout<<c[270]<<endl; //checked
    c.erase(105,100);  //checked
    MyList<int> q(1000000000,0);  //checked
	return 0;
}
