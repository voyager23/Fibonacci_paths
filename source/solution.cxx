
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <set>
#include <memory>
#include <vector>

uint32_t sqr(uint32_t x) {return x*x;}

const uint32_t M=1000000007;

template<int S> struct solver {

  typedef std::array<std::array<uint32_t,S+1>,S+1> A;
  
  solver()
    :_a(std::make_unique<A>())
  {
    for (auto row: (*_a)) {std::fill(row.begin(),row.end(),0);}
    (*_a)[0][0]=1;

    std::vector<uint32_t> fibs;
    std::set<uint32_t> fibs2;
    fibs.push_back(1);
    fibs.push_back(1);
    fibs2.insert(1);
    while (sqr(fibs.back())<=2*S*S) {
      fibs.push_back(fibs[fibs.size()-1]+fibs[fibs.size()-2]);
      fibs2.insert(sqr(fibs.back()));
    }

    for (uint32_t y=0;y<=S;++y) {
      for (uint32_t x=y;x<=S;++x) {
        if (fibs2.find(x*x+y*y)!=fibs2.end()) {
          _moves.push_back(std::make_pair(y,x));
          if (x!=y) _moves.push_back(std::make_pair(x,y));
        }
      }
    }
  }

  uint32_t solve() const {

    for (uint32_t d=0;d<S;++d) {
      spray(d,d);
      for (uint32_t x=d+1;x<=S;++x) spray(d,x);
      for (uint32_t y=d+1;y<=S;++y) spray(y,d);
    }
    
    return (*_a)[S][S];
  }

private:
  
  void spray(uint32_t y,uint32_t x) const {
    for (auto m: _moves) {
      const uint32_t ty=y+m.first;
      const uint32_t tx=x+m.second;
      if (ty<=S && tx<=S) (*_a)[ty][tx]=((*_a)[ty][tx]+(*_a)[y][x])%M;
    }
  }
  
  const std::unique_ptr<A> _a;
  std::vector<std::pair<uint32_t,uint32_t>> _moves;
};

int main(int c,char** p) {

  assert(solver<10>().solve()==215846462);
  
  std::cout << solver<46>().solve() << std::endl;
  
  return 0;
}
