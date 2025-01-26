
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <string>
#include <vector>

bool atrc::BlockContainsKey(std::vector<atrc::Key>& keys, const atrc::Key& key) {
    for (const atrc::Key& _key : keys) {
        if (_key.Name == key.Name) {
            return true;
        }
    }
    return false;
}
bool atrc::BlockContainsBlock(std::unique_ptr<std::vector<atrc::Block>>& blocks,const atrc::Block& block) {
    for (atrc::Block &_block : *blocks) {
        if (_block.Name == block.Name) {
            return true;
        }
    }
    return false;
}
bool atrc::VariableContainsVariable(std::unique_ptr<std::vector<atrc::Variable>>& variables, const atrc::Variable& variable){
    for (atrc::Variable &var : *variables) {
        if (var.Name == variable.Name) {
            return true;
        }
    }
    return false;
}