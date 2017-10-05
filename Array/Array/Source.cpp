#include <iostream>
#include <vector>
#include <exception>

using namespace std;

typedef vector<unsigned int> VUI;

class Invalid_MDArray : public exception{
public:
	virtual const char* what() const throw  (){
		return "Array cannot be constructed ";
	}
};

template <class T>
class MArray{
private:
	VUI dsize;
	VUI cumsize;
	T* p;
	unsigned int stot;

public:

	unsigned int size(){ return stot; }
	unsigned int size(unsigned int i) { return dsize[i]; }


	MArray(const VUI& a){


		stot = 1;
		for (unsigned int i = 0; i<a.size(); i++){

			if (a[i] == 0) {
				Invalid_MDArray o;
				throw o;
			}

			stot = stot*a[i];

			dsize.push_back(a[i]);
			cumsize.push_back(stot);
		}

		dsize.push_back(stot);

		p = new T[stot];

		//	cout<<stot<<endl;
	}

	template <typename ...Args>
	MArray(Args... args){


		unsigned int tot_arg_num = sizeof... (args);
		unsigned int a[] = { args... } ;
		
		stot = 1;
		for (unsigned int i = 0; i< tot_arg_num ; i++){

			if (a[i] == 0) {
				Invalid_MDArray o;
				throw o;
			}

			stot = stot*a[i];

			dsize.push_back(a[i]);
			cumsize.push_back(stot);
		}

		dsize.push_back(stot);

		p = new T[stot];

		//	cout<<stot<<endl;
	}


	~MArray(){

		delete[] p;
	}


	T& operator()(VUI&& a){

		if (a.size() != dsize.size() - 1) {

			out_of_range o("Index is out of bound!");
			throw o;

		}

		unsigned int i = 0;
		while (i<a.size()){
			if (a[i]>dsize[i] - 1) {

				out_of_range o("Index is out of bound!");
				throw o;
			}
			i++;
		}

		unsigned int index = 0;

		//	    index=i+imax*j+imax*jmax*k

		i = 0;
		while (i<a.size()){

			if (i == 0) {
				index = a[i];
			}
			else {
				index = index + a[i] * cumsize[i - 1];

			}


			i++;
		}


		return p[index];
	}



	template <typename ...Args>
	T& operator()(Args... args){

		unsigned int tot_arg_num = sizeof... (args);
		unsigned int a[] = { args... };

		if (tot_arg_num != dsize.size() - 1) {

			out_of_range o("Index is out of bound!");
			throw o;

		}

		unsigned int i = 0;
		while (i< tot_arg_num ){
			if (a[i]>dsize[i] - 1) {

				out_of_range o("Index is out of bound!");
				throw o;
			}
			i++;
		}

		unsigned int index = 0;

		//	    index=i+imax*j+imax*jmax*k

		i = 0;
		while (i<tot_arg_num){

			if (i == 0) {
				index = a[i];
			}
			else {
				index = index + a[i] * cumsize[i - 1];

			}


			i++;
		}


		return p[index];
	}

};


int main(){


	try{
		//MArray<int>  t({ 2, 2, 2 });
		MArray<int>  t(2, 2, 2);

		
		
		
		//t({ 1, 1, 1 }) = 10;
		t( 1, 1, 1 ) = 10;
		cout << t({ 1, 1, 1 }) << endl;
		cout << t( 1, 1, 1 ) << endl;
		
		cout << t.size() << endl;

		//MArray<int>  tt({ 2, 0, 2 }); // OOPS! cannot construct this array!
		//MArray<int>  tt( 2, 0, 2 ); // OOPS! cannot construct this array!
		
		t({ 1, 2, 1 }) = 1000; //OOPS! outofbound exception!
		t( 1, 2, 1 ) = 1000; //OOPS! outofbound exception!
	}
	catch (exception &e){
		cout << e.what() << endl;
	}

	getchar();
}