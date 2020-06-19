#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
  cout << "please input the iterations" << endl;
  int iterations;
  cin >> iterations;

  cout << "please input the template string below" << endl;
  cout << "use # to replace the number, use Ctrl+D to enter EOF to stop" << endl;

  vector<string> t;
  string tmp;
  while(getline(cin, tmp, '#')) {
    t.push_back(tmp);
  }
  int v;
  for (int i = 0; i < iterations; i++) {
    cout << t[0];
    for (int j = 1; j < t.size(); j++) {
      if (t[j].substr(0, 1) == "-") {
        v = stoi(t[j].substr(1,1), nullptr, 10);
        cout << i-v << t[j].substr(2);
      } 
      else if (t[j].substr(0, 1) == "+") {
        v = stoi(t[j].substr(1,1), nullptr, 10);
        cout << i+v << t[j].substr(2);
      } else {
        cout << i << t[j];
      }

    }
  } 
  
  return 0;
}