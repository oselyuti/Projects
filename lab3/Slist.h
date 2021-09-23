#ifndef SLIST_H
#define SLIST_H
#include <iostream>
#include <vector>

using namespace std;
// include header file(s) needed

// qsort_r3: randomly chosen pivot, 3-way partition {<},{=},{>}

// template <typename T>
template <class T>
class slist {
	private:
		struct node {
			node() { data = T(); next = NULL; }
			node (const T &key) {// add node(const T &key) { write this }
				data = key;
				next = NULL;
			}  

			bool operator < (const node n) const{// add overloaded operator< code
				return (data<n.data);
			
			}

			T data;
			node *next;
		};

		// add class sptr { write this for node data }
		class sptr {
			public:
				sptr (node *_ptr = NULL) {ptr = _ptr;}
				bool operator < (const sptr &rhs)const {
					return *ptr < *rhs.ptr;}//returns smaller data the node points to.
				operator node* () const {return ptr;}

			private :
				node * ptr;
				
		};


	public:
		class iterator {
			public:
				iterator() { p = NULL; }
				T & operator*() { return p->data; }
				iterator & operator++() { p = p->next; return *this; }
				bool operator!=(const iterator & rhs) const { return p != rhs.p; }

			private:
				iterator(node *n_p) { p = n_p; }
				node *p;

				friend class slist<T>;
		};

	public:
		slist();
		~slist();

		void push_back(const T &);
		void sort(const string &);

		iterator begin() { return iterator(head->next); }
		iterator end() { return iterator(NULL); }

	private:
		node *head;
		node *tail;
};

template <typename T>
slist<T>::slist() {
	head = new node();
	tail = head;
}

template <typename T>
slist<T>::~slist() {
	while (head->next != NULL) {
		node *p = head->next;
		head->next = p->next;
		delete p;
	}
	delete head;

	head = NULL;
	tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
	tail->next = new node(din);
	tail = tail->next;
}

template <typename T>
void slist<T>::sort(const string &algname) {
	int size=0;
	node *p = head;
	while (p->next){size++;p= p->next;}// determine number of list elements CORRECT
	vector <sptr> Ap;// set up smart pointer array called Ap
	Ap.resize(size);
	
	p = head;
	for (int i=0; i<size; i++){
		Ap[i] = p->next;
		p = p->next;
		//cout<<p->data<<endl;
	}
	if (algname == "-quicksort"){std::sort (Ap.begin(), Ap.end() );}// if quicksort, apply std::sort(...)
	if (algname == "-mergesort"){std::stable_sort (Ap.begin(), Ap.end() );}// if mergesort, apply std::stable_sort(...)
	// if qsort_r3, apply qsort_r3(...)
	// use sorted Ap array to relink list 
	
	p = head;
	for (int i=0; i< size; i++){
		p->next = Ap[i];//rearrange the old list with order from Ap (sorted)
		p = p->next;
		p->next = NULL;
	}
	tail = p;

}

#endif // SLIST_H
