#include "../../include/ATRC.h"
#include "../../include/filer.h"
#include <string>
/*
    ! TEST EVERYTHING
*/
void AddVariable(ATRCFiledata *filedata, const std::string &varname, const std::string &value){
    Variable var;
    var.Name = varname;
    if(VariableContainsVariable(filedata->Variables, &var)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, filedata->Filename);
        return;
    }
    var.Value = value;
    filedata->Variables->push_back(var);
    if(filedata->AutoSave){
        Save(filedata, ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+ParseLineSTRINGtoATRC(var.Value));
    }
}

void RemoveVariable(ATRCFiledata *filedata, const std::string &varname){
    Variable var;
    var.Name = varname;
    if(!VariableContainsVariable(filedata->Variables, &var)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, filedata->Filename);
        return;
    }

    int i = 0;    
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    filedata->Variables->erase(filedata->Variables->begin() + i);

    if(filedata->AutoSave){
        Save(filedata, ATRC_SAVE::REMOVE_VAR, i, varname);
    }
}

void ModifyVariable(ATRCFiledata *filedata, const std::string &varname, const std::string &value){
    Variable var;
    var.Name = varname;
    if(!VariableContainsVariable(filedata->Variables, &var)){
        errormsg(ERR_VAR_NOT_FOUND, -1, var.Name, filedata->Filename);
        return;
    }
    int i = 0;    
    for(Variable &var : *filedata->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    filedata->Variables->at(i).Value = value;
    std::cout << filedata->Variables->at(i).Name +"="+ filedata->Variables->at(i).Value + "\n";
    if(filedata->AutoSave){
        Save(filedata, ATRC_SAVE::MODIFY_VAR, i, ParseLineSTRINGtoATRC(value)); // Values taken from filedata->Variables->at(xtra_info).[type]
    }
}