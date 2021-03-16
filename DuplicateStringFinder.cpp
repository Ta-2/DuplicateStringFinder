#include <string.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <cmath>
using namespace std;
typedef pair<string, vector<int>> PSV;
typedef pair<const string, vector<int>> refPSV;

class DuplicateStringFinder{
	public:
	int size = 0;
	string plain;
	vector<PSV>Where;
	map<string, vector<int>>CMap;
	map<int, vector<refPSV>> AppearMap/*, StatisticsMap*/;
	vector<vector<int>*> PeriodMap;
	set<int> Candidates;
	
	DuplicateStringFinder(string plain){
		this->plain = plain;
	}
	void StringAnalyzer(){
	
		//-------------------------------------------------
		//initialize CMap to register first location owhere each characters appeared
		//-------------------------------------------------
		size = plain.length();
		for(int i = 0; i < size; i++)
			CMap[string()+plain[i]].push_back(i);
		
		return;
	}
	void PrintPlain(){
	
		//-------------------------------------------------
		//print plain text
		//-------------------------------------------------
		for(auto c: plain)cout << c << " ";
		cout << endl;
		for(int i = 0; i < plain.size(); i++)cout << i%10 << " ";
		cout << endl;
	}
	void SearchDuplicateString(double limen){
	
		//-------------------------------------------------
		//search strings that appeared 	more than twice
		//-------------------------------------------------
		for(auto c = CMap.begin(); c != CMap.end(); c++)
			MoreSearchDuplicateString(c);
		//sort Where
		sort(Where.begin(), Where.end(), [](PSV const &a, PSV const &b){
				return a.first.length() < b.first.length();
			}
		);
		//print Where
		for(auto w: Where){
			cout << "\"" << w.first << "\"";
			for(auto v: w.second)cout << v << " ";
			cout << endl;

		}
		//-------------------------------------------------
		//calculate scores and statistics
		//-------------------------------------------------
		long double ave = 0.0, square_ave = 0.0;
		vector<double>ScoreTable;
		for(auto a: Where){
			int WordCount = a.first.length(), NumAppearance = a.second.size();
			double score = log2(WordCount) * NumAppearance;
			ScoreTable.push_back(score);
			ave += score;
			square_ave  += (score*score);
			cout << "\""<< a.first << "\"= Number of Appearance: " << NumAppearance << ", Score: "  << score << endl;
		}
		ave /= Where.size();
		square_ave /= Where.size();
		double variance = square_ave - ave*ave;
		double standard_deviation = sqrt(variance);
		cout << "<<statictics of score>> : average = " << ave << ", variance = " << variance << ", standard deviation = " << standard_deviation << endl;
		cout << "standartized datas" << endl;
		/*
		for(auto s: ScoreTable){
			cout << setfill('0') << setw(6) << "score: " << s << ", standardzed score: " << (s - ave)/standard_deviation;
			if((s - ave)/standard_deviation >= 2.32)cout << " << this score is in te top of 1 percent >>";
			cout << endl;
		}
		*/
		ScoreTable.clear();
		//ave = 0.0, square_ave = 0.0;
		
		//-------------------------------------------------
		//extract strings most appeared in each group that has same number of length of string
		//-------------------------------------------------
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
		
		//-------------------------------------------------
		//print AppearMao
		//-------------------------------------------------
		for(auto a: AppearMap){
			for(auto s: a.second){
				cout << "--\"" << s.first << "\": ";
				for(auto p: s.second) cout << p << " ";
				cout << endl;
			}
			cout << endl;
		}
		
		//-------------------------------------------------
		//calculate scores and statistics
		//-------------------------------------------------
		/*
		double ave = 0.0, square_ave = 0.0;
		vector<double>ScoreTable;
		*/
		for(auto a: AppearMap){
			int WordCount = a.first, NumAppearance = a.second[0].second.size();
			double score = log2(WordCount) * NumAppearance;
			ScoreTable.push_back(score);
			/*
			ave += score;
			square_ave  += (score*score);
			cout << "\""<< a.first << "\"= Number of Appearance: " << NumAppearance << ", Score: "  << score << endl;
			*/
		}
		/*
		ave /= AppearMap.size();
		square_ave /= AppearMap.size();
		double variance = square_ave - ave*ave;
		double standard_deviation = sqrt(variance);
		*/
		cout << "<<statictics of score>> : average = " << ave << ", variance = " << variance << ", standard deviation = " << standard_deviation << endl;
		cout << "standartized datas" << endl;
		for(auto s: ScoreTable){
			cout << setfill('0') << setw(6) << "score: " << s << ", standardzed score: " << (s - ave)/standard_deviation;
			//if((s - ave)/standard_deviation >= 2.57)cout << " << this score is in te top of 0.5 percent >>";
			if((s - ave)/standard_deviation >= limen)cout << " << this score is more than limen >>";
			cout << endl;
		}
		
		//-------------------------------------------------
		//change AppearMap to PeriodMap
		//-------------------------------------------------
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
		
		//-------------------------------------------------
		//print all PeriodMap
		//-------------------------------------------------
		for(auto p: PeriodMap){
			cout << "PeriodMap: ";
			for(auto now: *p)cout << now << ", ";
			cout << endl;
		}
		
		//-------------------------------------------------
		//calculate common divisors
		//-------------------------------------------------
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
		
		//-------------------------------------------------
		//print cnadidates
		//-------------------------------------------------
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
	if(argc > 2)dsf->SearchDuplicateString(atoi(argv[2]));
	else dsf->SearchDuplicateString(2.57);
	
	
	return 0;
}