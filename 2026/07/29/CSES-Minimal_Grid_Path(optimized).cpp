#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

typedef unsigned long long ull;
typedef long long ll;

int main()
{
    cin.tie(0)->sync_with_stdio(false);

	int n;
	cin >> n;

	vector<string> graph(n);
	for(auto &s : graph)cin >> s;

	vector<vector<bool>> cv(n+1, vector<bool>(n+1, false));
	cv[0][0] = true;
	string ans;
	for(int i = 0; i < 2*n-1; i++)
	{
		char small = 'Z';
		for(int j = max(i-n+1, 0); j <= i && j < n; j++)
			if(cv[i-j][j])small = min(small, graph[i-j][j]);
			
		ans += small;
		for(int j = max(i-n+1, 0); j <= i && j < n; j++)
			if(graph[i-j][j] == small && cv[i-j][j])cv[i-j+1][j] = cv[i-j][j+1] = true;
	}
	cout << ans;
}