#include<iostream>
using namespace std;
template<class T> class MyList;
template<class T> MyList<T> operator + (const MyList<T> &list_1, const MyList<T> &list_2);
template<class T> MyList<T> operator + (const MyList<T> &list_1, const T &item);
template<class T> ostream & operator<<(ostream &os, MyList<T> &obj);
template<class T>
class MyList{
    friend MyList<T> operator + (const MyList<T> &list_1, const MyList<T> &list_2)
	//add the list_2 to the end of list_1 to create a new list and return it
	{
	    int list_1_size = list_1.get_size();
	    int list_2_size = list_2.get_size();
	    int tmplist_size = list_1_size+list_2_size;
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
	    if (obj_size == 0)
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
	T *data;
	int size; //the number of elements in list
	bool check_and_expand_room(const int &room_needed)//check whether the room is enough
	{
        int room_allocated = sizeof(data)/sizeof(data[0]);
        if (room_needed <= room_allocated)
            return true;
        room_allocated = room_needed * 2 + 1; // if the room will be run out, ask for more
        T *tmp = NULL;
        tmp=new T [room_allocated];
        if (tmp == NULL)
        {
            cout<<"FAILED to allocate room"<<endl; // Error of failed to allocate room
            return false;
        }
        for (int i=0; i < size; ++i)
            tmp[i] = data[i];
        delete []data;
        data = tmp;
	}
	/*void qs(int l,int r)
    {

    }*/
public:
	MyList ()
	{
		size = 0;
		data = new T [100];
	}

	MyList (int num, const T &item)
	//push item in list for num times
	{
	    size = num;
	    data = new T [size];
	    for (int i=0; i < size; ++i)
            data[i] = item;
	}

	MyList (const MyList<T> &list_1)
	//copy another list
	{
	    int list_1_size = list_1.get_size();
	    data = new T [list_1_size];
        size = list_1_size;
        for (int i=0; i < size; ++i)
            data[i] = list_1[i];
	}

	MyList (const T *arr)
	//build a list from an array
	{
	    int arr_size = sizeof(arr)/sizeof(arr[0]);
	    data = new T [arr_size];
	    size = arr_size;
	    for (int i=0; i < size; ++i)
            data[i] = arr[i];

	}

    MyList (const T *arr, int len)
    //make up a list with the first len elements of arr
    {
        size = min(sizeof(arr)/sizeof(arr[0]),len)
        data = new T [size];
        for (int i=0; i < size; ++i)
            data[i] = arr[i];
    }

	void push_back(const T &item)
	//add an item to the end of list
	{
	    check_and_expand_room(size+1);
	    data[size++] = item;
	}

	T pop_back()
	//remove the last element of list and return it
	{
	    --size;
	    return data[size];
	}

	void insert(int index, const T &item)
	//insert item to the place with index
	{
	    check_and_expand_room(size+1);
	    for (int i=size-1; i >= index; --i)
            data[i+1] = data[i];
        data[index] = item;
        ++size;
	}

	void clean()
	//clean the list
	{
        memset(data,0,sizeof(data));
        size = 0;
	}

	int get_size() const
	//return the number of elements
	{
	    return size;
	}

	void erase(int start, int end)
	//delete the elements from start to end
	{
	    int move_distance = end - start + 1;
	    if (move_distance <= 0)
        //if the index is illegal, throw the error and do nothing
        {
            cout<<"Error! Illegal index!"<<endl;
            return;
        }
        for (int i=end+1; i < size; ++i)
            data[i-move_distance] = data[i];
        size -= move_distance;

	}

	T get_item(int index) const { return data[index]; }
	//return the element with index

	MyList<T> get_item(int start, int end) const
	//return elements from start to end.
	{
	    if (end < 0)
            end = size + end;
	    int length = end - start + 1;
	    if (length <= 0)
        {
            MyList empty_list;
            return empty_list;
        }
	    MyList<T> tmplist;
	    for (int i=0; i < length; ++i)
            tmplist.push_back(data[i+start]);
        return tmplist;

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
        if (remove_index == -1)
        {
            cout<<"Error! The item does not exist!"<<endl;
            return;
        }
        erase(remove_index, remove_index);
	}



	MyList<T> &operator = (const MyList<T> &list_1)
	//assignment
	{
	    if (this == &list_1)// avoiding assignment to itself. If not, my cause memory leaks
            return *this;
	    int list_1_size = list_1.get_size();
	    if (list_1_size == 0) // if the list_1 is an empty list, clean up this list
        {
            size = 0;
            return *this;
        }
	    check_and_expand_room(list_1_size);
	    for (int i=0; i < list_1_size; ++i)
            data[i] = list_1[i];
        size = list_1_size;
        return *this;
	}

	MyList &operator += (const T &item)
	//add an item to the end of list
	{
	    *this = *this + item;
	    return *this;
	}

	MyList &operator += (const MyList<T> list_1)
	//add another list to the end of this list
	{
        *this = *this + list_1;
        return *this;
	}

	T &operator [](int index) const { return data[index]; }
	//return the element with the index



	void sort(int l, int r, bool less=true)
	//sort less==true: up
    {
        int i,j;
        T mid,t;
        i=l;j=r;mid=data[(i+j)/2];
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
        if(l<j) sort(l,j,less);
        if(i<r) sort(i,r,less);
    }

	void reverse()
	//reverse all elements in list
	{
	    MyList tmplist;
	    tmplist = *this;
        for (int i=0; i < size ;++i)
            data[i] = tmplist[size-i-1];
	}

	~MyList(){delete [] data;}
};

int main(){
	MyList<int> a, b;
	int i;
	for (i=0; i<5; ++i)
		a.push_back(i);   //checked;
    // a = [0, 1, 2, 3, 4]
	a[3] = 15; // a = [0, 1, 2, 15, 4]  //checked
	a.sort(0,4); // a = [0, 1, 2, 4, 15]//checked
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

	return 0;
}
