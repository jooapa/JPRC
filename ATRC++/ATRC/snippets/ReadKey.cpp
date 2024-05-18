#include "../include/ATRC.h"

extern "C" void ReadKey(ATRCFiledata *filedata, 
                        const std::string& block, 
                        const std::string& key, 
                        std::string& contents)
{
    for(Block blk : *filedata->Blocks){
        if(blk.Name == block){
            for(Key k : blk.Keys){
                if(k.Name == key){
                    contents = k.Value;
                    return;
                }
            }
        }
    }
    contents = "";
}