#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

typedef unsigned long long ull;
typedef long long ll;

int solve(int idx, vector<vector<int>> &graph, vector<int> &dp)
{
	pair<int, int> tmp = {0, 0};
	int ret = 0;
	dp[idx] = 1;
	for(auto &i : graph[idx])
	{
		if(dp[i] != 0)continue;
		ret = max(solve(i, graph, dp), ret);
		if(tmp.second < dp[i])tmp.second = dp[i];
		if(tmp.first < tmp.second)swap(tmp.first, tmp.second);
	}
	ret = max(ret, tmp.first+tmp.second);
	dp[idx] += tmp.first;

	return ret;
}

int main()
{
    cin.tie(0)->sync_with_stdio(false);
    
    int n;
	cin >> n;

	vector<vector<int>> graph(n);
	for(int a, b; cin >> a >> b; )
	{
		a--; b--;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	vector<int> dp(n);
	cout << solve(0, graph, dp);
}