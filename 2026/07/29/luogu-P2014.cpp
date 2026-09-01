#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

typedef unsigned long long ull;
typedef long long ll;

    
vector<bool> visited;
void solve(int idx, vector<vector<int>> &graph, vector<vector<int>> &dp)
{
	if(visited[idx])return;
	visited[idx] = true;
	vector<int> &ret = dp[idx];

	auto tmp = vector<int>(dp[0].size(), 0);
	for(auto &i : graph[idx])
	{
		solve(i, graph, dp);
		auto &tmp_a = dp[i];
		auto tmp_b = tmp;
		for(int i = 0; i < tmp_a.size(); i++)
		{
			for(int j = 0; j <= i; j++)
			{
				tmp[i] = max(tmp[i], tmp_a[i-j]+tmp_b[j]);
			}
		}
	}
	for(int i = 1; i < ret.size(); i++)
	{
		ret[i] += tmp[i-1];
	}
}

int main()
{
    cin.tie(0)->sync_with_stdio(false);

	int n, m;
	cin >> n >> m;
	n++;
	m++;
	vector<vector<int>> graph(n);
	vector<vector<int>> dp(n);
	visited.resize(n);
	dp[0] = vector<int>(m+1, 0);
	for(int k, s, idx = 1; idx < n; idx++)
	{
		cin >> k >> s;
		graph[k].push_back(idx);
		dp[idx] = vector<int>(m+1, s);
		dp[idx][0] = 0;
	}

	solve(0, graph, dp);
	cout << dp[0][m];
}