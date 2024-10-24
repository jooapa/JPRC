#include "../include/ATRC.hpp"

std::string ReadKey(std::shared_ptr<ATRC_FD> filedata, 
                        const std::string& block, 
                        const std::string& key
                    )
{
    std::string res = "";
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block){
            for(Key k : blk.Keys){
                if(k.Name == key){
                    res = k.Value;
                    return res;
                }
            }
        }
    }
    res = "";
    return res;
}