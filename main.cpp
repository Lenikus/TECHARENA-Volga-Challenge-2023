#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;
const int N = 100;
static int a[N][N]{};
bool isB = false;
int n = 100, m = 100;
int cnt = 0;

enum OP{SAME, MUL, DIV, SUB, NONE};

OP rowCompare(int i, int ii);
int almostTheSame(int i, int ii);
stringstream calcB();

int main(){
cin >> n >> m;
vector<bool>rowIsCalc(n, false);
vector<int> onesRowInd;
for(int i = 0; i < n; i++){
	int zCnt = 0, oCnt = 0;
    for(int j = 0; j < m; j++){
        cin >> a[i][j];
		if(a[i][j] == 0) zCnt++;
		if(a[i][j] == 1) oCnt++;
		}
	if (zCnt == m)
		rowIsCalc[i] = true;
	if (oCnt == m)
		onesRowInd.push_back(i);
	}
map<int, int> mulRow;//INDEX OF FIRST
map<int, int> divRow;
map<int, int> subRow;
map<int, int> sameRow;
map<int, int> likeRow;
stringstream ans, prevname;
vector <map<int, int>> vCoef(m);
map<int, bool> bCoef;
for(int i = 0; i < n; i++){
    if(rowIsCalc[i]) continue;
	if(binary_search(onesRowInd.begin(), onesRowInd.end(), i)){
		rowIsCalc[i] = true;
		if(!isB){
            ans << (calcB()).str();
            bCoef[1] = true;
            }
        cnt++;
        ans << "r[" << i << "] = bsum1pos * 1\n";
        continue;
		}
	}
for(int i = 0; i < n; i++){
	if(rowIsCalc[i]) continue;
	int like = -1;
    for(int ii = 0; ii < i; ii++){
        if(likeRow.count(i) == 0){
            like = almostTheSame(i, ii);
            if(like != -1)
                likeRow[i] = ii;
            }
        OP duplKind = rowCompare(i, ii);
        switch(duplKind){
            case SAME:
            if(sameRow.count(i) == 0)
                sameRow[i] = ii;
            break;
            case MUL:
            if(mulRow.count(i) == 0)
                mulRow[i] = ii;
            break;
            case DIV:
            if(divRow.count(i) == 0)
                divRow[i] = ii;
            break;
            case SUB:
            if(subRow.count(i) == 0)
                subRow[i] = ii;
            break;
            default: break;
            }
        }
    int zCnt = 0;
    int nonZInd = -1;
    for(int j = 0; j < m; j++){
        if(a[i][j] == 0) zCnt++;
        else nonZInd = j;
        }
    if(zCnt == m - 1){
        if(vCoef[nonZInd].count(a[i][nonZInd]) == 0)
            vCoef[nonZInd][a[i][nonZInd]] = i;
        cnt++;
        ans << "r[" << i << "] = " << "v[" << nonZInd << "] * ";
        if(a[i][nonZInd] != 1)
            ans << a[i][nonZInd] << "\n";
        else
            ans << "1\n";
        continue;
        }
    if (sameRow.count(i) != 0){
        cnt++;
        ans << "r[" << i << "] = r[" << sameRow[i] << "] * 1\n";
        continue;
        }
    if (mulRow.count(i) != 0){//i=ii/k
        cnt++;
        int k = a[mulRow[i]][0] / a[i][0];
        ans << "r[" << i << "] = r[" << mulRow[i] << "] / " << k << "\n";
        continue;
        }
    if (divRow.count(i) != 0){//i=ii*k
        cnt++;
        int k = a[i][0] / a[divRow[i]][0] ;
        ans << "r[" << i << "] = r[" << divRow[i] << "] * " << k << "\n";
        continue;
        }
    if (subRow.count(i) != 0){ //i=ii-k
        int k = a[subRow[i]][0] - a[i][0];
        stringstream bkname;
        bkname << "bsum" << abs(k);
        if(k > 0) bkname << "neg";
        else bkname << "pos";
        if(bCoef.count(k) == 0){
            cnt++;
            bCoef[k] = true;
            if(!isB){
                ans << (calcB()).str();
                bCoef[1] = true;
                }
            ans << bkname.str() << " = bsum1pos * " << -k << "\n";
            }
        cnt++;
        ans << "r[" << i << "] = r[" << subRow[i] << "] + " << bkname.str() << "\n";
        continue;
        }
    if(likeRow.count(i) != 0){
        int iInd = likeRow[i];
        cnt += 2;
        int jInd = like;
        int k = a[i][jInd] - a[iInd][jInd];
        stringstream name;
        name << "b" << iInd << "_" << jInd;
        ans << name.str() << " = " << k << " * v[" << jInd << "]\n";
        ans << "r[" << i << "] = r[" << iInd << "] + " << name.str() << "\n";
        continue;
        }
    int addCnt = 0;
    for(int j = 0; j < m; j++){
        if(a[i][j] == 0) continue;
        stringstream curname;
        addCnt++;
        if(a[i][j] != 1) {
            if(vCoef[j].count(a[i][j]) == 0){
                cnt++;
                vCoef[j][a[i][j]] = i;//like multiply
                curname << "a" << i << "_" << j;
                ans << curname.str() << " = " << a[i][j] << " * v[" << j << "]\n";
                }
            else
                curname << "a" << vCoef[j][a[i][j]] << "_" << j;
            }
        else
            curname << "v[" << j << "]";
        if (addCnt == 2){ // like summing
            cnt++;
            ans << "r[" << i << "] = " << curname.str() << " + " << prevname.str() << "\n";
            }
        else
            if(addCnt > 2){
                cnt++;
                ans << "r[" << i << "] = r[" << i << "] + " << curname.str() << "\n";
                }
        prevname.str(curname.str());
        }
    if (addCnt == 1){
        cnt++;
        ans << "r[" << i << "] = " << prevname.str() << " * 1\n";
        }
    }
cout << cnt << "\n" << ans.str();
}

stringstream calcB(){
isB = true;
stringstream bans;
cnt += 2;
bans << "bsum1pos = v[0] + v[1]\n" << "bsum1pos = bsum1pos + v[2]\n";
for(int j = 3; j < m; j++){
    cnt++;
    bans << "bsum1pos = bsum1pos + v[" << j << "]\n";
    }
return bans;
}

int almostTheSame(int i, int ii){
int sameCnt = 0;
int ind = -1;
for(int j = 0; j < m; j++)
    if(a[i][j] - a[ii][j] == 0) sameCnt++;
    else ind = j;
if(sameCnt == m - 1) return ind;
else return -1;
}

OP rowCompare(int i, int ii){
int mulCnt = 0;
int divCnt = 0;
int sameCnt = 0;
for(int j = 0; j < m; j++)
    if(a[i][j] == a[ii][j])
        sameCnt++;
if(sameCnt == m) return SAME;
int divI = 1;
int divII = 1;
if(a[ii][0])
    divII = a[i][0] / a[ii][0];
if(a[i][0])
    divI = a[ii][0] / a[i][0];
for(int j = 0; j < m; j++){
    if(divII && divII * a[ii][j] == a[i][j])
        divCnt++;
    if(divI && divI * a[i][j] == a[ii][j])
        mulCnt++;
    }
if(mulCnt == m) return MUL;
if(divCnt == m) return DIV;
int sub = a[i][0] - a[ii][0];
int subCnt = 1;
for(int j = 1; j < m; j++)
    if(a[i][j] - a[ii][j] == sub)
        subCnt++;
if(subCnt == m) return SUB;
return NONE;
}
/*

*/
