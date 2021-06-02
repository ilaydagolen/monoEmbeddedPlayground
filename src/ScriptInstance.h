#pragma once
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/object.h>

#include <string>
#include <vector>
#include <unordered_map>

class ScriptInstance {
public:
    template <class T>
    bool SetValue(T* value, const std::string& name)
    {
        if (MonoClassField* field = mono_class_get_field_from_name(klass, name.c_str())) {
            mono_field_set_value(object, field, value);
            return true;
        }

        printf("Failed to set value for field %s", name.c_str());
        return false;
    }

    bool SetStringValue(MonoDomain* domain, std::string* value, const std::string& name)
    {
        if (MonoClassField* field = mono_class_get_field_from_name(klass, name.c_str())) {
            MonoString * str = mono_string_new(domain, value->c_str());
            mono_field_set_value(object, field, str);
            return true;
        }

        printf("Failed to set value for field %s", name.c_str());
        return false;
    }

    static ScriptInstance load(MonoDomain* domain, const std::string& file_path);
    void unload(MonoDomain* domain);
    void serializeData(const std::string& json_path) const;
    void deserializeData(MonoDomain* domain, const std::string& json_path);

    void printFields();
    void output_methods();
    void output_properties();

    void runMethod(const char* name);

    size_t getFieldCount() const { return fields.size(); }
    MonoClassField* getField(size_t index) { return fields.at(index); }
    const char* getClassName() const;

public:
    MonoAssembly* assembly = nullptr;
    MonoImage* image = nullptr;
    MonoClass* klass = nullptr;
    MonoObject* object = nullptr;
    std::unordered_map<std::string, MonoMethod*> methods;
    std::vector<MonoClassField*> fields;
};