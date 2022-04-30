# include <iostream>
# include <list>
# include <algorithm>

using namespace std;

void print(string s) {
	cout << s << endl;
}

int get(list<int> _list, int _i) {
	list<int>::iterator it = _list.begin();
	for (int i = 0; i < _i; i++) {
		++it;
	}
	return *it;
}

void listPrint(list<int> l) {
	for (int i = 0; i < l.size(); i++) {
		cout << "index: " << i << endl;
		int v = get(l, i);
		cout << "value: " << v << endl << endl;
	}
	cout << endl;
}


int main() {
	list<int> l = {1, 2, 3, 4, 5, 6};

	listPrint(l);

	return 0;
}
