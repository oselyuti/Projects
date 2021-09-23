// list all headers needed 
#include <algorithm> //contains std::stable_sort()
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct fruit {
	//overload operators <, <=, ==, and +=
	bool operator< ( const fruit & f_2) const  {
		if (name < f_2.name) {return true;}
		return false;
	}


	bool operator<= ( const fruit & f_2) const  {
		if (name < f_2.name && name == f_2.name) {return true;}
		return false;
	}
	bool operator > (const fruit & f_2) const {
		if (name > f_2.name) return true;
		return false;
	}
	bool operator == (const fruit & f_2)const {
		if (name == f_2.name) return true;
		return false;
	}
	void operator += (const fruit & f_2)  {
		quan = quan + f_2.quan;
	}
	//data declarations
	string name;
	float quan;//quantity
	float price;//per pound
	float total_item_price;
};//END OF FRUIT

struct node {
	node (fruit new_fruit) {
		data = new_fruit;
		next=NULL;
	}
	node (){
		next = NULL;}

	fruit data;
	node * next;
};//end of NODE


void print_list ( node * first_node){

	float total_order_price=0.0;
	node * cur = first_node;
	for (node *it = first_node; it; it = it->next){// pretty-print array list content to stdout
		total_order_price += cur->data.total_item_price;

		cout<<fixed<<left<<setprecision(2)<<setfill('.')<< setw(20)<<cur->data.name<<"  ";
		cout<<setfill(' ')<<setw(5)<<cur->data.quan
			<<"x "<<setw(4)<< cur->data.price<<" = "
			<<setw(7)<<right<<cur->data.total_item_price
			<< "  : "
			<<setw(8)<<right<<total_order_price<<endl;

		if (cur->next) {cur = cur->next;}
	}
}

void compute_total_item_price (fruit & f) { f.total_item_price = f.quan * f.price;}

void list_cleanup(node * head){
	node* it = head;
	node * next_node;
	while (it){ //it != NULL
		next_node  = it->next;
		delete it;
		it = next_node;
	}
}

int main(int argc, char *argv[])
{
	string prog_name = string (argv[0]);
	//prog -inorder|sortall|sortone file.txt
	if (argc != 3) {
		cerr <<"usage: "<<prog_name<< "-inorder|sortall|sortone "<<"file.txt"<<endl;
		return 0;}
	string mode = string (argv[1]);
	if (mode != "-inorder" && mode != "-sortall" && mode != "-sortone"){ //|| mode != "-sortall" || mode != "-sortone"){ 
		cerr <<"usage: "<<prog_name<< " -inorder|-sortall|-sortone "<<"file.txt"<<endl;
		return 0;}
	// set program mode from command line arg


	node *head=new node;// declare linked_list<fruit>
	node * cur = head;
	int size=0, init=0;

	ifstream fin; istringstream sin;
	fin.open (argv[2]);//open file

	string n,line; float q,p;
	while (getline (fin, line) ){//while (reading more data
		sin.str(line);
		sin >> n >> q >> p ;
		fruit f;
		f.name = n; f.quan = q; f.price = p;
		f.total_item_price = q * p;

		node *new_fruit = new node (f);

		if (mode == "-inorder"){
			cur->next = new_fruit;//insert the elements as they are in a file
			cur = cur->next;}

		if (mode == "-sortall" || mode == "-sortone"){
			node * current = head;
			node * prev = head;
			if (size == 0) {//process the first node
				head->next = new_fruit;
			}

			else {//size >=1
				while (current -> next != NULL) {//while not the last element
					prev = current;
					current = current->next;
					//CASE1
					if (n < current->data.name){ //n can be the first one or somewhere in between
						if (prev == head) {//n must become the first element
							new_fruit -> next = current;//head->next
							head->next = new_fruit;
							break;
						}

						else if (n > prev->data.name) {//if prev->data.name < n < current->data.name
							new_fruit->next = current;
							prev->next = new_fruit;
						}
					}
					//CASE 2
					else if (n == current->data.name){					
						if ( current->next && !(n == current->next->data.name)  ){//repeat more than once
							if (mode =="-sortone") {//SORTONE
								current->data.quan += new_fruit->data.quan ;
								compute_total_item_price (current->data);//calculate the new price
								break;//leave it alone; do nOT insert another node
							}
							new_fruit->next = current->next;
							current->next = new_fruit;
							break;
						}

						else if ( current->next && current->data.name == current->next->data.name){//if the element after the next(current) element 
							//is the same as the next(current) element, we continue looping until that condition is false
							//...nothing
						}
						else {
							if (mode == "-sortone") {
								current->data.quan += new_fruit->data.quan ;
								compute_total_item_price (current->data);//calculate the new price
								break;
							}
							current->next = new_fruit;
								break;
						}

					}
					//BASIC CASE ->end of the list
					else if ( n > current->data.name) {//if n is the largest element 
						if (current->next == NULL){//the very last element
							current->next = new_fruit;
							break;
						}
					}

				}
			}
		}

		++size;//for checking if all elements are processed
		sin.clear();
	}
	fin.close();//finished file read

	print_list (head->next);//pretty-print linked list content to stdout
	list_cleanup(head);//free linked list memory
	//cout<< '\n'<<"number processed: "<<size<<'\n';
}
