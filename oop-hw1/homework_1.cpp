#include<iostream>
#include<string>
#include<iomanip>
#include <fstream>
#include <sstream>

using namespace std;

class student
{
    public:
        string name;
        int score[3];
        float average;
        void calculate() 
        {
            average = (score[0] + score[1] + score[2])*1.0/ 3;
        }
} ;

class course
{
    public:
        float average = 0;
        int max = 1;
        int min = 5;
};

bool Isint(float s)
{
    if(s - (int)s == 0)
    {return true;}
    else{return false;}
}

void CinFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    cin.rdbuf(buffer.rdbuf());
}
int main()
{   
    CinFromFile("test.txt");
    course cou[3];
    student stu[10];
    for(int i=0;i<10;i++)
    {
        cin >> stu[i].name >> stu[i].score[0] >> stu[i].score[1] >> stu[i].score[2];
        stu[i].calculate();
        for(int j=0;j<3;j++)
        {
            
            cou[j].max = max(cou[j].max,stu[i].score[j]);
            cou[j].min = min(cou[j].min,stu[i].score[j]);
            cou[j].average += stu[i].score[j];
        }
    }
    for(int i=0;i<3;i++)
    {
        cou[i].average = cou[i].average*1.0/10;
    }
cout << left << setw(10) << "no" << setw(8) << "name" << setw(8) << "score1"
     << setw(8) << "score2" << setw(8) << "score3" << setw(7) << "average" << endl;
     for (int i = 0; i < 10; i++) {
        if(Isint(stu[i].average))
        {
        cout << setw(10) << i+1
             << setw(8) << stu[i].name
             << setw(8) << stu[i].score[0]
             << setw(8) << stu[i].score[1]
             << setw(8) << stu[i].score[2]
             << fixed << setprecision(0) << setw(7) << stu[i].average << endl;
        }
        else
        {
        cout << setw(10) << i+1
             << setw(8) << stu[i].name
             << setw(8) << stu[i].score[0]
             << setw(8) << stu[i].score[1]
             << setw(8) << stu[i].score[2]
             << fixed << setprecision(5) << setw(7) << stu[i].average << endl;
        }
    }
cout << right << setw(17) << "average"<< setw(4) << fixed << setprecision(1)
     << cou[0].average << setw(8) << cou[1].average << setw(8) << cou[2].average << endl;
cout << right << setw(13) << "min" << setw(6) 
     << cou[0].min << setw(8) << cou[1].min << setw(8) << cou[2].min << endl;
cout << right << setw(13) << "max" << setw(6) 
     << cou[0].max << setw(8) << cou[1].max << setw(8) << cou[2].max << endl;
    return 0;
}