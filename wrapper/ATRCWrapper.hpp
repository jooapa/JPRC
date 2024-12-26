#pragma once
#ifndef ATRCWrapper_HPP
#define ATRCWrapper_HPP

#include <msclr/marshal_cppstd.h>
#include "../ATRC/include/ATRC.hpp"

using namespace System;
using namespace System::Collections::Generic;

namespace ATRCWrapper {

    public ref class Variable {
    public:
        String^ Name;
        String^ Value;
        bool IsPublic;

        Variable(const ::Variable& var) {
            Name = gcnew String(var.Name.c_str());
            Value = gcnew String(var.Value.c_str());
            IsPublic = var.IsPublic;
        }
    };

    public ref class Key {
    public:
        String^ Name;
        String^ Value;

        Key(const ::Key& key) {
            Name = gcnew String(key.Name.c_str());
            Value = gcnew String(key.Value.c_str());
        }
    };

    public ref class Block {
    public:
        String^ Name;
        List<Key^>^ Keys;

        Block(const ::Block& block) {
            Name = gcnew String(block.Name.c_str());
            Keys = gcnew List<Key^>();
            for (const auto& key : block.Keys) {
                Keys->Add(gcnew Key(key));
            }
        }
    };

    public ref class ATRCFiledata {
    private:
        std::shared_ptr<::ATRC_FD> nativeFiledata;

    public:
        ATRCFiledata(String^ path) {
            nativeFiledata = std::make_shared<::ATRC_FD>(msclr::interop::marshal_as<std::string>(path));
        }

        bool Read() {
            return nativeFiledata->IRead();
        }

        String^ ReadVariable(String^ varname) {
            return gcnew String(nativeFiledata->IReadVariable(msclr::interop::marshal_as<std::string>(varname)).c_str());
        }

        String^ ReadKey(String^ block, String^ key) {
            return gcnew String(nativeFiledata->IReadKey(
                msclr::interop::marshal_as<std::string>(block),
                msclr::interop::marshal_as<std::string>(key)).c_str());
        }

        void AddBlock(String^ blockname) {
            nativeFiledata->IAddBlock(msclr::interop::marshal_as<std::string>(blockname));
        }

        void RemoveBlock(String^ blockname) {
            nativeFiledata->IRemoveBlock(msclr::interop::marshal_as<std::string>(blockname));
        }

        void AddVariable(String^ varname, String^ value) {
            nativeFiledata->IAddVariable(
                msclr::interop::marshal_as<std::string>(varname),
                msclr::interop::marshal_as<std::string>(value));
        }

        void RemoveVariable(String^ varname) {
            nativeFiledata->IRemoveVariable(msclr::interop::marshal_as<std::string>(varname));
        }

        void AddKey(String^ block, String^ key, String^ value) {
            nativeFiledata->IAddKey(
                msclr::interop::marshal_as<std::string>(block),
                msclr::interop::marshal_as<std::string>(key),
                msclr::interop::marshal_as<std::string>(value));
        }

        void RemoveKey(String^ block, String^ key) {
            nativeFiledata->IRemoveKey(
                msclr::interop::marshal_as<std::string>(block),
                msclr::interop::marshal_as<std::string>(key));
        }
    };
}

#endif // ATRCWrapper_HPP