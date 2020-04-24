#include<iostream>
#include<algorithm>
#include<functional>
#include<vector>
using namespace std;

int main(){
    int ia[6] = {2,34,56,12,35,67};
    vector<int, allocator<int>> vi (ia, ia + 6);
    cout<<count_if(vi.begin(), vi.end(), not1(bind2nd(less<int>(), 40)));
    
    return 0;
}