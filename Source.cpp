namespace offsets
{
	BYTE Transparency = 0xC4;
	namespace part {
		BYTE partinfo = 0xA0;
		BYTE x = 0x12C;
		BYTE y = 0x130;
		BYTE z = 0x134;
	}
	namespace classdef {
		BYTE classdef_start = 0xC;
		BYTE type = 0x4;
	}
	namespace children {
		BYTE children_start = 0x2C;
		BYTE children_end = 0x4;
	}
	BYTE name = 0x28;
	BYTE parent = 0x34;
}

const char* get_name(DWORD instance)
{
	char* curr_name = *(char**)(instance + offsets::name);
	for (int i = 0; i < strlen(curr_name); i++) {
		if (curr_name[i] > 150 || curr_name[i] < 1)
			return *(const char**)(*(int*)(instance + offsets::name));
	}
	return curr_name;
}

std::vector<uintptr_t> get_children(DWORD instance)
{
	std::vector<uintptr_t> children;
	int start = *(int*)(instance + offsets::children::children_start);
	int end = *(int*)(start + offsets::children::children_end);
	for (int i = *(int*)start; i < end; i += 0x8)
		children.push_back(*(int*)i);
	return children;
}

uintptr_t getChildFromName(uintptr_t ins, std::string childName) 
{
	std::vector<uintptr_t> shit = get_children(ins);
	for (int i = 0; i < shit.size(); i++)
	{
		if (!strcmp(get_name(shit.at(i)), childName.c_str()))
			return (shit.at(i));
	}
	return 0;
}

void print_children(DWORD instance)
{
	std::vector<uintptr_t> children = get_children(instance);
	printf("%s : 0x%p\n", get_name(instance), instance);
	for (int i = 0; i < children.size(); i++)
		printf("%s : 0x%p\n", get_name(children.at(i)),children.at(i));
}
uintptr_t get_parent(uintptr_t instance)
{
	return *(uintptr_t*)(instance + offsets::parent);
}

//0xC + 0x4 = classname
const char* get_type(DWORD instance)
{
	DWORD p = *(DWORD *)(instance + offsets::classdef::classdef_start);
	return *(char**)(p + offsets::classdef::type);
}

float get_transparency(DWORD instance)
{
	return *(float*)(instance + offsets::Transparency);
}

uintptr_t get_obfuscated_walkspeed(DWORD datamodel) 
{
  //...
}
unsigned long obfuscateWalkSpeed(DWORD datamodel, int WS) {
	//...
}

void set_walkspeed(DWORD datamodel,DWORD instance, int ws)
{
	int w_s = obfuscateWalkSpeed(datamodel, ws);
	DWORD loc = *(DWORD*)(instance + 0x1D8);
	*(int*)loc = w_s;
}
uintptr_t FindFirstChild(uintptr_t instance, const char*obj)
{
	std::vector<uintptr_t>children = get_children(instance);
	for (uintptr_t i : children)
	{
		if (!strcmp(get_name(i), obj))
			return i;
		else if (strcmp(get_name(i), obj))
			return 0;
	}
}

uintptr_t WaitForChild(uintptr_t instance, const char*obj, int timeout = 5000)
{
	for (int i = 0; i <= 5000; i++)
	{
		if (FindFirstChild(instance, obj) != 0) { return FindFirstChild(instance, obj); }
		Sleep(1);
	}
}

void getfullname(uintptr_t instance)
{
	std::vector<const char*> child;
	while (strcmp(get_name(instance), "Game")) //while instance != game
	{
		child.push_back(get_name(instance));
		instance = get_parent(instance);
	}
	child.push_back("game");
	for (int i = child.size() - 1; i > -1; i--) //myplayer workspace game 0 1 2
	{
		if (child.at(i) == child.at(0))
			printf("%s", child.at(i));
		else
			printf("%s.", child.at(i));
	}
}

uintptr_t return_first_child(uintptr_t instance)
{
	std::vector<uintptr_t>children = get_children(instance);
	return children.at(0);
}

const char* get_player(uintptr_t instance)
{
	//with instance being datamodel

	uintptr_t players = getChildFromName(instance, "Players");
	const char* my_player = get_name(return_first_child(players));
	return my_player;
}

uintptr_t FindFirstChildOfClass(uintptr_t instance, const char*classtype)
{
	std::vector<uintptr_t>child = get_children(instance);
	for (uintptr_t i : child)
	{
		if (strcmp(get_type(i), classtype) == 0)
		{
			return i;
		}
	}
}

bool IsA(uintptr_t instance, const char*classname)
{
	if (strcmp(get_type(instance), classname) == 0)  return true;
	else return false;
}
bool IsAncestorOf(uintptr_t instance, uintptr_t child)
{
	if (strcmp(get_name(instance), get_name(get_parent(child))) == 0) return true;
	else return false;
}
bool IsDescendantOf(uintptr_t parent, uintptr_t curr_instance)
{
	uintptr_t curr_parent = get_parent(curr_instance);
	if (curr_parent == parent) return true;
	else return false;
}




