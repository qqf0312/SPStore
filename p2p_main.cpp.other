#include <string>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <fstream>
#include <functional>

using namespace std;

int main()
{
    std::ifstream infiles("./mpt.log");
    assert(infiles.is_open());

    std::string line;
    int count = 1;
    std::cout << "start compute" << std::endl;
    std::vector<double> ec;
    while (infiles >> line)
    {
        //    std::vector<std::string> sub_field;
        // boost::split(sub_field, line, boost::is_any_of(";"));
        // ec.push_back(stod(sub_field[1]));
        // std::cout << line << std::endl;
        ec.push_back(stod(line));
    }
    long double all =0;
    std::map<double,long long> m;
    for(int i =0;i<ec.size();++i)
    {
        // std::cout << "ec[i] = " << ec[i] << std::endl;
        if(m.find(ec[i]) == m.end())
        {
            m[ec[i]] = 1;
        }
        else
        ++m[ec[i]];

        all += ec[i];
    }
    for(auto it= m.begin();it != m.end();++it)
    {
        std::cout << "key = " << it->first << " num = " << it->second << std::endl;
    }
    std::cout << " size = " << all / 1024.0/1024.0 << std::endl;
    std::cout  << "  main tps = " << all / ec.size() << std::endl;
    return 0;
}