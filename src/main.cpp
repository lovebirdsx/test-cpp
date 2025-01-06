#include <iostream>
#include <vector>
#include <string>

int main()
{
    using namespace std;

    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }

    cout << endl;

    return 0;
}
