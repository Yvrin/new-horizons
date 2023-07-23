#include "module_loader/storyline_loader.c"

native void LoadConfig(ref target, string path);
native void ListFiles(ref target, string root);
#libriary "config"

object Modules;

void LoadModules() {
	Trace("Initializing module loader...");
	DeleteAttribute(&Modules, "");

	object ModuleFinder;
	ListFiles(&ModuleFinder, "modules");

	aref module_list;
	makearef(module_list, ModuleFinder.files);
	int directory_count = GetAttributesNum(module_list);
	int module_count = 0;
	for (int i = 0; i < GetAttributesNum(module_list); i++) {
		string module_directory = GetAttributeN(module_list, i);
		if (FindFile(module_directory, "*.toml", "module.toml") != "")
		{
			RegisterModule(module_directory);
			module_count++;
		}
	}
	Trace("Loaded " + module_count + " modules");
}

void RegisterModule(string folder) {
	object ModuleConfig;
	LoadConfig(&ModuleConfig, folder + "/module.toml");
	string name = ModuleConfig.name;
	aref version_array;
	makearef(version_array, ModuleConfig.version);
	string version = GetModuleVersion(version_array);

	Trace("Registering module '" + name + "' version '" + version + "'");
	aref module;
	makearef(module, Modules.(name));
	module.name = name;
	module.version = version;
	module.root = folder;

	LoadStorylineConfigs(folder);
}

string GetModuleVersion(aref version_array) {
	string version = "";
	for (int i = 0; i < GetAttributesNum(version_array); i++) {
		if (i != 0) {
			if (i == 3) {
				version += "-";
			}
			else {
				version += ".";
			}
		}
		version += GetAttributeN(version_array, i);
	}
	return version;
}
