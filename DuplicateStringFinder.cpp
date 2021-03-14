#include <string.h>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
using namespace std;
typedef pair<string, vector<int>> PSV;
typedef pair<const string, vector<int>> refPSV;

class DuplicateStringFinder{
	public:
	int size = 0;
	string plain;
	vector<PSV>Where;
	map<string, vector<int>>CMap;
	map<int, vector<refPSV>> AppearMap;
	vector<vector<int>*> PeriodMap;
	set<int> Candidates;
	
	DuplicateStringFinder(string plain){
		this->plain = plain;
	}
	void StringAnalyzer(){
		size = plain.length();
		for(int i = 0; i < size; i++)
			CMap[string()+plain[i]].push_back(i);
		
		return;
	}
	void PrintPlain(){
		for(auto c: plain)cout << c << " ";
		cout << endl;
		for(int i = 0; i < plain.size(); i++)cout << i%10 << " ";
		cout << endl;
	}
	void SearchDuplicateString(){
		for(auto c = CMap.begin(); c != CMap.end(); c++)
			MoreSearchDuplicateString(c);
		sort(Where.begin(), Where.end(), [](PSV const &a, PSV const &b){
				return a.first.length() < b.first.length();
			}
		);
		for(auto w: Where){
			cout << "\"" << w.first << "\"";
			for(auto v: w.second)cout << v << " ";
			cout << endl;
		}
		
		for(const refPSV& w: Where){
			//cout << w.first << ", " << w.second.size() << endl;
			if(!AppearMap.count(w.first.length())){
				//cout << "add " << w.first << ", " << w.first.length() << endl;
				AppearMap[w.first.length()].push_back(w);
				continue;
			} else if (AppearMap[w.first.length()][0].second.size() > w.second.size())
				continue;
			else if(AppearMap[w.first.length()][0].second.size() < w.second.size())
				AppearMap[w.first.length()].clear();
			AppearMap[w.first.length()].push_back(w);
		}
		
		for(auto a: AppearMap){
			int WordCount = a.first, NumAppearance = a.second[0].second.size();
			cout << "\""<< a.first << "\"= Number of Appearance: " << NumAppearance << ", Score: "  << WordCount + NumAppearance << endl;
			for(auto s: a.second){
				cout << "--\"" << s.first << "\": ";
				for(auto p: s.second) cout << p << " ";
				cout << endl;
			}
			cout << endl;
		}

		for(auto a: AppearMap){
			for(auto s: a.second){
				vector<int> *nowVector = new vector<int>;
				PeriodMap.push_back(nowVector);
				int Old = s.second[0];
				
				for(auto p: s.second){
					if((p - Old) > 0)nowVector->push_back(p - Old);
					Old = p;
				}
			}		
		}
		for(auto p: PeriodMap){
			cout << "PeriodMap: ";
			for(auto now: *p)cout << now << ", ";
			cout << endl;
		}
		//vector<vector<int>*> PeriodMap;
		for(auto p: PeriodMap){
			//int min = (*p)[0];
			int min = *min_element(p->begin(), p->end());
			cout << "min: " << min << endl;
			cout << "--priority: ";
			for(int i = 1; i*i <= min; i++){
				int num_front = 0, num_back = 0;
				//cout << "now is: " << i << endl;
				if(!(min % i)){
					for(auto now: *p) if(!(now%i))num_front++;
					if(i*i-min) for(auto now: *p) if(!(now%(min/i)))num_back++;
				}
				if(p->size() == num_front){
					Candidates.insert(i);
					cout << i << ", ";
				}
				if(p->size() == num_back){
					Candidates.insert(min/i);
					cout << min/i << ", ";
				}
			}
			cout << endl;
		}
		cout << "Candidates: ";
		for(auto c: Candidates) cout << c << ", ";
		cout << endl;
		
		return;
	}
	void MoreSearchDuplicateString(map<string, vector<int>>::iterator &c){
			map<string, vector<int>>NextS;
			
			//cout << "\"" << c->first << "\": ";
			for(auto l: c->second){
				string AddStr = c->first + plain[l + (c->first).length()];
				//cout << l << " ";
				if(l < size-(c->first).length())
					NextS[AddStr].push_back(l);
			}
			//cout << endl;
			map<string, vector<int> >::iterator it = NextS.begin();
			while(it != NextS.end()){
				if(it->second.size() > 1){
					MoreSearchDuplicateString(it);
					Where.push_back(*it);
					++it;
				} else NextS.erase(it++);
			}
			
		return;
	}
};

int main(int argc, char *argv[]){
	DuplicateStringFinder *dsf;
	if(argc > 1) dsf = new DuplicateStringFinder(argv[1]);
	else dsf = new DuplicateStringFinder("abccabaabcccababcb");
	dsf->PrintPlain();
	dsf->StringAnalyzer();
	dsf->SearchDuplicateString();
	
	
	return 0;
}