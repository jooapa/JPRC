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
        Save(filedata, AUTOSAVE_ADD_VAR, -1, varname);
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
        Save(filedata, AUTOSAVE_REMOVE_VAR, i, varname);
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
    for(Variable var : *filedata->Variables){
        if(var.Name == varname){
            break;
        }
        i++;
    }
    filedata->Variables->at(i).Value = value;
    if(filedata->AutoSave){
        Save(filedata, AUTOSAVE_MODIFY_VAR, i, varname);
    }
}