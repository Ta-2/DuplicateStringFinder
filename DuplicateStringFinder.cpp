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

class StatisticsCalculator{
private:
	double sum = 0.0, square_sum = 0.0;
	double average = 0.0, square_average = 0.0;
	double variance = 0.0, standard_deviation = 1.0;
	int size = 0;
public:
	void AddData(double data){
		sum += data;
		square_sum += (data*data);
		size++;
	}
	double Sum(){ return sum; }
	double Ave(){ return sum/size; }
	double SquareAve(){ return square_sum/size; }
	double Variance(){ return square_average - pow(average, 2.0); }
	double StandardDeviation(){ return sqrt(variance); }
	double Standardize(double data){ return (data - average)/standard_deviation; }
	void AllCalc(){
		average = Ave();
		square_average = SquareAve();
		variance = Variance();
		standard_deviation = StandardDeviation();
	}
};

class DuplicateStringFinder{
	public:
	int size = 0;
	string plain;
	vector<PSV>Where;
	map<string, vector<int>>CMap;
	map<int, vector<refPSV>> AppearMap;
	map<int, vector<refPSV>*> PeriodMap;
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
	
	set<int> SearchDuplicateString(double limen){
	
		//-------------------------------------------------
		//search strings that appeared 	more than twice
		//-------------------------------------------------
		for(auto c = CMap.begin(); c != CMap.end(); c++)
			MoreSearchDuplicateString(c);
		//sort Where
		/*
		sort(Where.begin(), Where.end(), [](PSV const &a, PSV const &b){
				return a.first.length() < b.first.length();
			}
		);
		*/
		
		//-------------------------------------------------
		//print Where
		//-------------------------------------------------
		cout << "<< Print Where >>" << endl;
		for(auto w: Where){
			cout << "\"" << w.first << "\" is appeared at ";
			for(auto v: w.second)cout << v << " ";
			cout << endl;

		}
		cout << endl;
		
		//-------------------------------------------------
		//calculate scores and statistic
		//-------------------------------------------------
		cout << "<< Print number of appearance and score about each string >>" << endl;
		long double ave = 0.0, square_ave = 0.0;
		vector<double>ScoreTable;
		for(auto a: Where){
			int WordCount = a.first.length(), NumAppearance = a.second.size();
			double score = log2(WordCount) * NumAppearance;
			ave += score;
			square_ave  += (score*score);
			cout << "\""<< a.first << "\"= Number of Appearance: " << NumAppearance << ", Score: "  << score << endl;
		}
		cout << endl;
		
		ave /= Where.size();
		square_ave /= Where.size();
		double variance = square_ave - ave*ave;
		double standard_deviation = sqrt(variance);
		cout << "<<statictics of score>> : average = " << ave << ", variance = " << variance << ", standard deviation = " << standard_deviation << endl;
		cout << endl;
		
		//-------------------------------------------------
		//extract strings most appeared in each group that has same number of length of string
		//-------------------------------------------------
		for(const refPSV& w: Where){
			if(!AppearMap.count(w.first.length())){
				AppearMap[w.first.length()].push_back(w);
				continue;
			} else if (AppearMap[w.first.length()][0].second.size() > w.second.size())
				continue;
			else if(AppearMap[w.first.length()][0].second.size() < w.second.size())
				AppearMap[w.first.length()].clear();
			AppearMap[w.first.length()].push_back(w);
		}
		//-------------------------------------------------
		//print AppearMap
		//-------------------------------------------------
		cout << "<< Print AppearMap >>" << endl;
		for(auto a: AppearMap){
			for(auto s: a.second){
				cout << "--\"" << s.first << "\": ";
				for(auto p: s.second) cout << p << " ";
				cout << endl;
			}
			cout << endl;
		}
		cout << endl;
		
		//-------------------------------------------------
		//change AppearMap to PeriodMap
		//-------------------------------------------------
		cout << "<< Craeting PeriodMap >>" << endl;
		for(auto a: AppearMap){
			int WordCount = a.first, NumAppearance = a.second[0].second.size();
			double score = log2(WordCount) * NumAppearance;
			cout << "--length of string: " << a.first << ", score: " << score << ", " << (score - ave)/standard_deviation << endl;
			if((score - ave)/standard_deviation < limen)continue;
			for(auto s: a.second)cout << s.first << ", ";
			cout << "are passed the limen test!" << endl;
			
			PeriodMap[a.first] = new vector<refPSV>;
			vector<refPSV>* AddrefPSV = PeriodMap[a.first];
			for(auto s: a.second){
				AddrefPSV->push_back(make_pair<const string, vector<int>>((const string)s.first, vector<int>()));
				vector<int>* AddVector = &(AddrefPSV->back().second);
				int Old = s.second[0];
				
				for(auto p: s.second){
					if((p - Old) > 0)AddVector->push_back(p - Old);
					Old = p;
				}
			}
		}
		
		//-------------------------------------------------
		//print all PeriodMap
		//-------------------------------------------------
		cout << "<< Print PeriodMap >>" << endl;
		for(auto p: PeriodMap){
			cout << "--length of string: " << p.first << endl;
			for(auto q: *(p.second)){
				cout << "----string: " << q.first << " = ";
				for(auto r: q.second) cout << r << ", ";
				cout << endl;
			}
		}
		cout << endl;
		
		//-------------------------------------------------
		//calculate common divisors
		//-------------------------------------------------
		cout << endl << "<< PRIORITY MAP by PeriodMap >>" << endl;
		for(auto p: PeriodMap){
			cout << "--length of string: " <<  p.first << endl;
			for(auto q: *(p.second)){
				int min = *min_element(q.second.begin(), q.second.end());
				cout << "----string: \"" << q.first << "\" min: " << min << endl;
				cout << "------priority: ";
				for(int i = 1; i*i <= min; i++){
					int num_front = 0, num_back = 0;
					if(!(min % i)){
						for(auto now: q.second) if(!(now%i))num_front++;
						if(i*i-min) for(auto now: q.second) if(!(now%(min/i)))num_back++;
					}
					if(q.second.size() == num_front){
						Candidates.insert(i);
						cout << i << ", ";
					}
					if(q.second.size() == num_back){
						Candidates.insert(min/i);
						cout << min/i << ", ";
					}
				}
				cout << endl;
			}
		}
		
		//-------------------------------------------------
		//print cnadidates
		//-------------------------------------------------
		cout << "Candidates: ";
		for(auto c: Candidates) cout << c << ", ";
		cout << endl;
		
		return Candidates;
	}
	void MoreSearchDuplicateString(map<string, vector<int>>::iterator &c){
			map<string, vector<int>>NextS;
			
			for(auto l: c->second){
				string AddStr = c->first + plain[l + (c->first).length()];
				if(l < size-(c->first).length())
					NextS[AddStr].push_back(l);
			}
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
	set<int> result;
	if(argc > 1) dsf = new DuplicateStringFinder(argv[1]);
	else dsf = new DuplicateStringFinder("abccabaabcccababcb");
	//dsf->PrintPlain();
	dsf->StringAnalyzer();
	if(argc > 2) result = dsf->SearchDuplicateString(atoi(argv[2]));
	else result = dsf->SearchDuplicateString(2.57);
	
	
	return 0;
}