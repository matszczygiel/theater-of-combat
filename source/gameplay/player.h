#ifndef PLAYER_H    
#define PLAYER_H    

#include <string>
#include <vector>
#include <stack>
#include <memory>



class Player {
    public:
     Player() = default;
     Player(std::string name, const std::vector<std::string>& teams);

     const std::string& name() const;
     const std::vector<std::string> teams() const;

    private:
     std::string _name{};
     std::vector<std::string> _owned_teams{};
};

#endif