#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(string domain) 
    : domain_(move(domain)) {}

    bool operator==(const Domain& other) const { return domain_ == other.domain_; }

    bool operator<(const Domain& domain) const { return domain_ < domain.domain_; }    
    
    bool IsSubdomain (const Domain& domain) const {
        for (size_t i = 0; i < domain.domain_.size(); ++i) {
            if (domain_[i] != domain.domain_[i])
                return false;
        }
        return true;
    }
    string GetDomain() const{
        return domain_;
    }
private:
    string domain_;
};

class DomainChecker {
public:
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
    : forbidden_domains_(first, last) {
        SortAndDelSubdomens();    
    }

    bool IsForbidden(const Domain& domain) const {
        return ranges::find_if(forbidden_domains_, [&](const Domain& other_domain) { 
                       return domain == other_domain || domain.IsSubdomain(other_domain); }) != forbidden_domains_.end();
    }
private:
    vector<Domain> forbidden_domains_;
    
    void SortAndDelSubdomens() {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        const auto sub_domains = ranges::unique(forbidden_domains_, [](const Domain& lhs, const Domain& rhs) {
                                                                     return rhs.IsSubdomain(lhs); });
        forbidden_domains_.erase(sub_domains.begin(), sub_domains.end());
    }
};

template <typename Number>
vector<Domain> ReadDomains(istream& in, Number number) {
    vector<Domain> result;
    for (Number i = 0; i < number; ++i) {
        string str;
        getline(in, str);
        str.push_back('.');
        ranges::reverse(str);
        str.push_back('.');
        result.emplace_back(str);
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    Number num;
    std::istringstream(line) >> num;
    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

/*
4
gdz.ru
maps.me
m.gdz.ru
com
7
gdz.ru
gdz.com
m.maps.me
alg.m.gdz.ru
maps.com
maps.ru
gdz.ua

Bad
Bad
Bad
Bad
Bad
Good
Good
*/