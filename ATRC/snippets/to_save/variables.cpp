#include "../../include/ATRC.hpp"
#include "../../include/filer.h"
#include <string>
/*
    ! TEST EVERYTHING
*/
void _W_AddVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(_W_DoesExistVariable_(filedata, varname)){
        errormsg(ERR_VAR_EXISTS, -1, var.Name, filedata->Filename);
        return;
    }
    var.Value = value;
    filedata->Variables->push_back(var);
    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::ADD_VAR, -1, "%"+var.Name+"%="+ParseLineSTRINGtoATRC(var.Value));
    }
}
void _W_RemoveVariable_(PATRC_FD filedata, const std::string& varname){
    Variable var;
    var.Name = varname;
    if(!_W_DoesExistVariable_(filedata, varname)){
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
        _W_Save_(filedata, ATRC_SAVE::REMOVE_VAR, i, varname);
    }
}
void _W_ModifyVariable_(PATRC_FD filedata, const std::string& varname, const std::string& value){
    Variable var;
    var.Name = varname;
    if(!_W_DoesExistVariable_(filedata, varname)){
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
    // std::cout << "1." << filedata->Variables->at(i).Name +"="+ filedata->Variables->at(i).Value + "\n";
    // std::cout << "2." << varname +"="+ value + "\n";
    if(filedata->AutoSave){
        _W_Save_(filedata, ATRC_SAVE::MODIFY_VAR, i, ParseLineSTRINGtoATRC(value)); // Values taken from filedata->Variables->at(xtra_info).[type]
    }
}

void AddVariable(std::shared_ptr<ATRC_FD> filedata, const std::string &varname, const std::string &value){
    return _W_AddVariable_(filedata.get(), varname, value);
}

void RemoveVariable(std::shared_ptr<ATRC_FD> filedata, const std::string &varname){
    return _W_RemoveVariable_(filedata.get(), varname);
}

void ModifyVariable(std::shared_ptr<ATRC_FD> filedata, const std::string &varname, const std::string &value){
    return _W_ModifyVariable_(filedata.get(), varname, value);
}